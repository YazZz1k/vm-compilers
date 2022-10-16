#pragma once

#include <list>
#include <memory>

#include <IR/include/BasicBlock.h>

using BBlockPtr = std::shared_ptr<BBlock>;

class Graph {
public:
  using Iter = std::vector<BBlockPtr>::iterator;

  Iter BlockBegin() { return blocks.begin(); }
  Iter BlockEnd() { return blocks.end(); }
  std::vector<BBlockPtr> &GetBlocks() { return blocks; }

  void PushBlock(BBlockPtr block) { blocks.push_back(block); }

  void CreateEdge(BBlockPtr from, BBlockPtr to) {
    assert(Contains(from) && Contains(to) && "blocks outside of graph");
    from->SetSucc(to);
    to->SetPred(from);
  }

  BBlockPtr GetEntry() { return blocks.at(0); }

  bool Contains(const BBlockPtr b) const {
    return std::find(blocks.cbegin(), blocks.cend(), b) != blocks.cend();
  }

  void DumpDot(std::ostream &os) const {
    os << "digraph G {" << std::endl;
    for (const auto &b : blocks) {
      const std::string &bName = b->GetName();
      os << bName << std::endl;
      for (const auto &succ : b->GetSuccessors()) {
        os << bName << " -> " << succ->GetName() << std::endl;
      }
    }
    os << "}";
  }

private:
  std::vector<BBlockPtr> blocks;
};
