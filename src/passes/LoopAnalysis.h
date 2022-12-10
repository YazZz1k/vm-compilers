#pragma once

#include <IR/include/BasicBlock.h>
#include <IR/include/Graph.h>
#include <IR/include/GraphTraits.h>
#include <passes/DominatorTree.h>

#include <sstream>

class LoopTreeNode;

using LoopTreeNodePtr = std::shared_ptr<LoopTreeNode>;

class LoopTreeNode : NodeTraits<LoopTreeNode> {
public:
  LoopTreeNode(bool isRoot = false) : isRoot(isRoot){};
  LoopTreeNode(const BBlockPtr head, const BBlockPtr backEdge)
      : head(head), reducible(false), isRoot(false) {
    backEdges.insert(backEdge);
  }

  void MarkReducible() { reducible = true; }

  void MarkIrreducible() { reducible = false; }

  bool IsReducible() { return reducible; }

  BBlockPtr GetHead() const { return head; }

  void SetHead(const BBlockPtr newHead) { head = newHead; }

  const std::set<BBlockPtr> &GetBackEdges() { return backEdges; }

  void RemoveBackEdge(const BBlockPtr be) {
    assert(backEdges.count(be) > 0 &&
           "attempt to erase  non-exsitend back edge");
    backEdges.erase(be);
  }

  void AddBackEdge(const BBlockPtr be) { backEdges.insert(be); }

  void AddSource(const BBlockPtr src) { srcs.push_back(src); }

public:
  // node traits
  using NodePtr = LoopTreeNodePtr;
  void AddSucc(const NodePtr succ) override { childs.push_back(succ); }

  void AddPred(const NodePtr pred) override { parent = pred; }

  void RemoveSucc() override { assert(false && "unimplemented"); }

  void RemovePred() override { assert(false && "unimplemented"); }

  std::vector<NodePtr> GetSuccessors() const override { return childs; }
  std::vector<NodePtr> GetPredessors() const override {
    // no predessors -- return empty vector
    return parent ? std::vector<NodePtr>{parent} : std::vector<NodePtr>();
  }

  std::string GetName() const override {
    if (isRoot) {
      return "Root";
    }
    return head->GetName();
  }

  void DumpDot(std::ostream &os) const override {
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
      }
      os << "\n";
    }

    os << "\"]";
  }

private:
  BBlockPtr bblock;

  std::set<BBlockPtr> backEdges;
  BBlockPtr head;
  std::vector<BBlockPtr> srcs;
  bool reducible;
  bool isRoot;

  std::vector<NodePtr> childs; // inner loops
  NodePtr parent;              // outer loop
};

class LoopTree : public GraphTraits<LoopTreeNode> {
public:
  LoopTree(const Graph &graph) : graph(graph), domTree(graph) { Init(); }

private:
  LoopTreeNodePtr root;
  std::vector<LoopTreeNodePtr> loops;
  std::map<BBlockPtr, LoopTreeNodePtr> block2node;

private:
  // for analysis
  const Graph &graph;
  const DominatorTree domTree;

  enum Marker { WHITE = 0, GREY, BLACK, GREEN };

  std::map<BBlockPtr, Marker> bb2marker;
  void SetMarker(BBlockPtr bb, Marker m) { bb2marker[bb] = m; }
  Marker GetMarker(BBlockPtr bb) {
    if (bb2marker.count(bb) == 0) {
      return WHITE;
    }
    return bb2marker.at(bb);
  }

  void DfsCollectBackEdges(const BBlockPtr curr) {
    SetMarker(curr, GREY);
    for (const auto succ : curr->GetSuccessors()) {
      auto succMarker = GetMarker(succ);
      if (succMarker == WHITE) {
        DfsCollectBackEdges(succ);
      } else if (succMarker == GREY) {
        // succ -- head, curr -- back edge
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

  void CollectBackEdges() {
    DfsCollectBackEdges(graph.GetEntry());
    bb2marker.clear();
  }

  void ReverseDfsPopulateLoop(const BBlockPtr bb, const LoopTreeNodePtr loop) {
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

  void PopulateReducibleLoop(const LoopTreeNodePtr loop) {
    assert(loop->IsReducible() && "expected reducible loop");
    BBlockPtr head = loop->GetHead();
    assert(head);
    SetMarker(head, GREEN);
    for (const auto be : loop->GetBackEdges()) {
      ReverseDfsPopulateLoop(be, loop);
    }
  }

  void PopulateLoops() {
    // post-ordered sequence of head blocks
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

    for (const auto bb : graph.GetBlocks()) {
      if (GetMarker(bb) != GREEN) {
        root->AddSource(bb);
      }
    }
  }

  bool IsBackEdge(const BBlockPtr bb) const {
    if (block2node.count(bb) == 0) {
      return false;
    }

    const auto loopPtr = block2node.at(bb);
    return loopPtr->GetBackEdges().count(bb) > 0;
  }

  bool IsHead(const BBlockPtr bb) const {
    if (block2node.count(bb) == 0) {
      return false;
    }

    const auto loopPtr = block2node.at(bb);
    return loopPtr->GetHead() == bb;
  }

  const LoopTreeNodePtr GetLoopNode(const BBlockPtr bb) const {
    assert(block2node.count(bb) != 0 && "attempt to get non-existend loop");
    return block2node.at(bb);
  }

  void Init() {
    root = std::make_shared<LoopTreeNode>(LoopTreeNode(true /* isRoot */));
    PushBlock(root);
    CollectBackEdges();
    PopulateLoops();
  }
};
