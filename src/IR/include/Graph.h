#pragma once

#include <list>
#include <memory>
#include <stack>

#include <IR/include/BasicBlock.h>

using BBlockPtr = std::shared_ptr<BBlock>;

// TODO graph traits to avoid copy-past
class Graph {
public:
  using Iter = std::vector<BBlockPtr>::iterator;

  Iter BlockBegin() { return blocks.begin(); }
  Iter BlockEnd() { return blocks.end(); }
  std::vector<BBlockPtr> &GetBlocks() { return blocks; }

  void PushBlock(BBlockPtr block) { blocks.push_back(block); }

  void CreateEdge(BBlockPtr from, BBlockPtr to) {
    assert(Contains(from) && Contains(to) && "blocks outside of graph");
    from->AddSucc(to);
    to->AddPred(from);
  }

  bool RemoveEdge(BBlockPtr from, BBlockPtr to) {
    assert(Contains(from) && Contains(to) && "blocks outside of graph");
    return from->RemoveSucc(to) & to->RemovePred(from);
  }

  void RemoveBlock(BBlockPtr block) {
    assert(Contains(block) && "block outside of graph");
    for (const auto &succ : block->GetSuccessors()) {
      RemoveEdge(block, succ);
    }
    for (const auto &pred : block->GetPredessors()) {
      RemoveEdge(pred, block);
    }
  }

  BBlockPtr GetEntry() const { return blocks.at(0); }

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

  std::vector<BBlockPtr> GetDfs() const {
    std::set<BBlockPtr> visited;
    return std::move(GetDfs(GetEntry(), visited));
  }

  std::vector<BBlockPtr> GetDfs(const BBlockPtr start,
                                std::set<BBlockPtr> &visited) const {
    std::vector<BBlockPtr> dfs;
    std::stack<BBlockPtr> nodesToVisit;

    nodesToVisit.push(start);
    while (!nodesToVisit.empty()) {
      BBlockPtr curr = nodesToVisit.top();
      nodesToVisit.pop();

      if (!visited.count(curr)) {
        dfs.push_back(curr);
        visited.insert(curr);

        const auto &succs = curr->GetSuccessors();
        for_each(succs.crbegin(), succs.crend(), [&visited, &nodesToVisit](auto &succ) {
          if (!visited.count(succ)) {
            nodesToVisit.push(succ);
          }
        });
      }
    }

    return std::move(dfs);
  }

  std::vector<BBlockPtr> GetPo() {
    std::set<BBlockPtr> visited;
    std::vector<BBlockPtr> po;
    std::list<BBlockPtr> nodesToVisit;
    nodesToVisit.push_back(GetEntry());

    while (!nodesToVisit.empty()) {
      BBlockPtr curr = nodesToVisit.back();
      nodesToVisit.pop_back();

      if (!visited.count(curr)) {
        po.push_back(curr);
        visited.insert(curr);
        for (const auto &succ : curr->GetSuccessors()) {
          if (!visited.count(succ)) {
            nodesToVisit.push_front(succ);
          }
        }
      }
    }

    return std::move(po);
  }

private:
  std::vector<BBlockPtr> blocks;
};
