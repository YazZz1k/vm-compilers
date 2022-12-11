#include <passes/LoopAnalysis.h>

#include <sstream>

std::string LoopTreeNode::GetName() const {
  if (isRoot) {
    return "Root";
  }
  return head->GetName();
}

void LoopTreeNode::DumpDot(std::ostream &os) const {
  os << GetName() << "[label = \"";
  if (isRoot) {
    os << "Root node"
       << "\n";
    if (!srcs.empty()) {
      os << "Sources: ";
      for (const auto &src : srcs) {
        os << src->GetName() << " ";
      }
    }
  } else {
    assert(head && !backEdges.empty());
    os << "Head: " << head->GetName() << "\n";
    os << "Back edges sources: ";
    for (const auto beSrc : backEdges) {
      os << beSrc->GetName() << " ";
    }
    os << "\n";
    if (!srcs.empty()) {
      os << "Sources: ";
      for (const auto src : srcs) {
        os << src->GetName() << " ";
      }
      os << "\n";
    }
  }

  os << "\"]";
}

bool LoopTree::IsBackEdge(const BBlockPtr bb) const {
  if (block2node.count(bb) == 0) {
    return false;
  }

  const auto loopPtr = block2node.at(bb);
  return loopPtr->GetBackEdges().count(bb) > 0;
}

bool LoopTree::IsHead(const BBlockPtr bb) const {
  if (block2node.count(bb) == 0) {
    return false;
  }

  const auto loopPtr = block2node.at(bb);
  return loopPtr->GetHead() == bb;
}

void LoopTree::DfsCollectBackEdges(const BBlockPtr curr) {
  SetMarker(curr, GREY);
  for (const auto succ : curr->GetSuccessors()) {
    auto succMarker = GetMarker(succ);
    if (succMarker == WHITE) {
      DfsCollectBackEdges(succ);
    } else if (succMarker == GREY) {
      const BBlockPtr head = succ, backEdge = curr;
      LoopTreeNode loop(head, backEdge);
      domTree.IsDominate(head, backEdge) ? loop.MarkReducible()
                                         : loop.MarkIrreducible();
      auto loopPtr = std::make_shared<LoopTreeNode>(loop);
      PushBlock(loopPtr);
      block2node.insert({head, loopPtr});
      block2node.insert({backEdge, loopPtr});
    }
  }

  SetMarker(curr, BLACK);
}

void LoopTree::ReverseDfsPopulateLoop(const BBlockPtr bb,
                                      const LoopTreeNodePtr loop) {
  if (IsBackEdge(bb) && GetMarker(bb) != GREEN) {
    // populate inner loop
    auto innerLoop = GetLoopNode(bb);
    if (innerLoop != loop) {
      SetMarker(bb, GREEN);
      PopulateReducibleLoop(innerLoop);
      CreateEdge(loop, innerLoop);
      for (const auto &pred : innerLoop->GetHead()->GetPredessors()) {
        ReverseDfsPopulateLoop(pred, loop);
      }
    }
  }

  if (GetMarker(bb) != GREEN) {
    // block without loop, add to sources
    SetMarker(bb, GREEN);
    block2node.insert({bb, loop});
    loop->AddSource(bb);
    for (const auto &pred : bb->GetPredessors()) {
      ReverseDfsPopulateLoop(pred, loop);
    }
  }
}

void LoopTree::PopulateReducibleLoop(const LoopTreeNodePtr loop) {
  assert(loop->IsReducible() && "expected reducible loop");
  BBlockPtr head = loop->GetHead();
  assert(head);
  SetMarker(head, GREEN);
  for (const auto be : loop->GetBackEdges()) {
    ReverseDfsPopulateLoop(be, loop);
  }
}

void LoopTree::PopulateLoops() {
  for (const auto bb : graph.GetPo()) {
    if (IsHead(bb)) {
      auto head = bb;
      auto loop = GetLoopNode(head);
      // not handled loop
      if (GetMarker(head) != GREEN) {
        if (loop->IsReducible()) {
          PopulateReducibleLoop(loop);
        }
        CreateEdge(root, loop);
      }
    }
  }

  // blocks without loop, add to root node
  for (const auto bb : graph.GetBlocks()) {
    if (GetMarker(bb) != GREEN) {
      root->AddSource(bb);
      block2node.insert({bb, root});
    }
  }
}
