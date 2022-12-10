#pragma once

#include <IR/include/BasicBlock.h>
#include <IR/include/Graph.h>

#include <iostream>
#include <memory>
#include <vector>

class DomTreeNode;

class DomTreeNode : NodeTraits<DomTreeNode> {
 public:
  DomTreeNode(BBlockPtr bblock) : bblock(bblock) {}

  using NodePtr = std::shared_ptr<DomTreeNode>;

  void AddSucc(const NodePtr succ) override {
    childs.push_back(succ);
  }

  void AddPred(const NodePtr pred) override {
    // no predessors -- method is empty
  }

  void RemoveSucc() override {
    assert(false && "unimplemented");
  }

  void RemovePred() override {
    assert(false && "unimplemented");
  }

  std::vector<NodePtr> GetSuccessors() const override {
    return childs;
  }
  std::vector<NodePtr> GetPredessors() const override {
    // no predessors -- return empty vector
    return std::vector<NodePtr>();
  }

  std::string GetName() const override {
    return bblock->GetName();
  }

  void DumpDot(std::ostream &os) const override { os << bblock->GetName(); }

private:
  BBlockPtr bblock;
  std::vector<NodePtr> childs;
};

class DominatorTree : public GraphTraits<DomTreeNode> {
 public:
  DominatorTree(const Graph &graph) { InitFrom(graph); }

  // return true if 'dom' dominate 'block'
  bool IsDominate(const BBlockPtr dom, const BBlockPtr block) const {
    assert(block2node.count(dom) > 0 && block2node.count(block) > 0 &&
           "dominator tree doesn't consist required blocks");
    NodePtr domNode = block2node.at(dom);
    NodePtr domBlock = block2node.at(block);

    // find in subtree
    std::set<NodePtr> s;
    auto dfs = GetDfs(domNode, s);
    return find(dfs.begin(), dfs.end(), domBlock) != dfs.end();
  }

private:
  NodePtr root;

  // map from bblock to node that consist this bblock
  std::map<BBlockPtr, NodePtr> block2node;

  static std::set<BBlockPtr> diff(const std::set<BBlockPtr> &full, const std::set<BBlockPtr> &part) {
    std::set<BBlockPtr> result;
    for (const auto &el : full) {
      if (part.count(el) == 0) {
        result.insert(el);
      }
    }

    return std::move(result);
  }

  // FIXME arguments are awful
  NodePtr CreateBranch(BBlockPtr block, const std::vector<std::set<BBlockPtr>> &domSet,
                       const std::map<BBlockPtr, size_t> block2idx) {
    auto IsDominate = [&domSet, &block2idx](BBlockPtr dom, BBlockPtr node) {
      size_t idx = block2idx.at(dom);
      return domSet.at(idx).count(node) > 0;
    };

    NodePtr newBranch = std::make_shared<DomTreeNode>(block);
    block2node.insert({block, newBranch});

    size_t idx = block2idx.at(block);
    if (!domSet[idx].empty()) {
      for (auto &child : domSet[idx]) {
        // if child are not dominated by any block from set
        // then there is an edge of the dominator tree
        bool notDominated = true;
        for (auto &d : domSet[idx]) {
          if (d != child && IsDominate(d, child)) {
            notDominated = false;
            break;
          }
        }
        if (notDominated) {
          newBranch->AddSucc(CreateBranch(child, domSet, block2idx));
        }
      }
    }
    PushBlock(newBranch);
    return newBranch;
  }

  void InitFrom(const Graph &graph) {
    const BBlockPtr entry = graph.GetEntry();

    // full set of visited blocks
    std::set<BBlockPtr> full;
    std::vector<BBlockPtr> dfs = graph.GetDfs(entry, full);

    // map from blocks to dfs idxs
    std::map<BBlockPtr, size_t> block2idx;

    std::vector<std::set<BBlockPtr>> dominatorSets;
    for (size_t i = 0; i < dfs.size(); ++i) {
      block2idx[dfs[i]] = i;

      std::set<BBlockPtr> part;
      // mark block as visited
      part.insert(dfs[i]);
      (void)graph.GetDfs(entry, part);

      dominatorSets.push_back(diff(full, part));
    }

    root = CreateBranch(graph.GetEntry(), dominatorSets, block2idx);
  }
};
