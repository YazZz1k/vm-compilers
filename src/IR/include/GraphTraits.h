#pragma once

#include <ostream>
#include <stack>

template <class Node>
class NodeTraits {
 public:
  using NodePtr = std::shared_ptr<Node>;

  virtual void AddSucc(const NodePtr succ) = 0;
  virtual void AddPred(const NodePtr pred) = 0;
  virtual void RemoveSucc(const NodePtr succ) = 0;
  virtual void RemovePred(const NodePtr pred) = 0;
  virtual std::vector<NodePtr> GetSuccessors() const = 0;
  virtual std::vector<NodePtr> GetPredessors() const = 0;

  // for DumpDot
  virtual std::string GetName() const = 0;
  virtual void DumpDot(std::ostream &) const = 0;
};

template <class Node>
class GraphTraits {
 public:
  using NodePtr = typename Node::NodePtr;
  using Iter = typename std::vector<NodePtr>::iterator;

  Iter BlockBegin() {
    return nodes.begin();
  }
  Iter BlockEnd() {
    return nodes.end();
  }
  const std::vector<NodePtr> &GetBlocks() const { return nodes; }

  void PushBlock(NodePtr block) {
    nodes.push_back(block);
  }

  void CreateEdge(NodePtr from, NodePtr to) {
    assert(Contains(from) && Contains(to) && "nodes outside of graph");
    from->AddSucc(to);
    to->AddPred(from);
  }

  bool RemoveEdge(NodePtr from, NodePtr to) {
    assert(Contains(from) && Contains(to) && "nodes outside of graph");
    return from->RemoveSucc(to) & to->RemovePred(from);
  }

  void RemoveBlock(NodePtr block) {
    assert(Contains(block) && "block outside of graph");
    for (const auto &succ : block->GetSuccessors()) {
      RemoveEdge(block, succ);
    }
    for (const auto &pred : block->GetPredessors()) {
      RemoveEdge(pred, block);
    }
  }

  NodePtr GetEntry() const {
    return nodes.at(0);
  }

  bool Contains(const NodePtr b) const {
    return std::find(nodes.cbegin(), nodes.cend(), b) != nodes.cend();
  }

  void DumpDot(std::ostream &os) const {
    os << "digraph G {" << std::endl;
    for (const auto &b : nodes) {
      b->DumpDot(os);
      os << std::endl;
    }
    for (const auto &b : nodes) {
      for (const auto &succ : b->GetSuccessors()) {
        os << b->GetName() << " -> " << succ->GetName() << std::endl;
      }
    }
    os << "}";
  }

  std::vector<NodePtr> GetDfs() const {
    std::set<NodePtr> visited;
    return std::move(GetDfs(GetEntry(), visited));
  }

  std::vector<NodePtr> GetDfs(const NodePtr start, std::set<NodePtr> &visited) const {
    std::vector<NodePtr> dfs;
    std::stack<NodePtr> nodesToVisit;

    nodesToVisit.push(start);
    while (!nodesToVisit.empty()) {
      NodePtr curr = nodesToVisit.top();
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

  std::vector<NodePtr> GetPo() const {
    std::set<NodePtr> visited;
    std::vector<NodePtr> po;
    std::list<NodePtr> nodesToVisit;
    nodesToVisit.push_back(GetEntry());

    while (!nodesToVisit.empty()) {
      NodePtr curr = nodesToVisit.back();
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
  std::vector<NodePtr> nodes;
};
