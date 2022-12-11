#pragma once

#include <BasicBlock.h>
#include <Graph.h>
#include <GraphTraits.h>
#include <passes/DominatorTree.h>

#include <sstream>

class LoopTreeNode;
using LoopTreeNodePtr = std::shared_ptr<LoopTreeNode>;

class LoopTreeNode : public NodeTraits<LoopTreeNode> {
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
  const std::set<BBlockPtr> &GetBackEdges() { return backEdges; }

  void AddBackEdge(const BBlockPtr be) { backEdges.insert(be); }
  void AddSource(const BBlockPtr src) { srcs.push_back(src); }

  const LoopTreeNodePtr GetOuterLoop() const { return parent; }
  const std::vector<LoopTreeNodePtr> &GetInnerLoops() const { return childs; }

public:
  // node traits
  using NodePtr = LoopTreeNodePtr;

  void AddSucc(const NodePtr succ) override { childs.push_back(succ); }
  void AddPred(const NodePtr pred) override { parent = pred; }

  void RemoveSucc(const NodePtr succ) override { assert(false && "NIY"); }
  void RemovePred(const NodePtr pred) override { assert(false && "NIY"); }

  std::vector<NodePtr> GetSuccessors() const override { return childs; }
  std::vector<NodePtr> GetPredessors() const override {
    return parent ? std::vector<NodePtr>{parent} : std::vector<NodePtr>();
  }

  std::string GetName() const override;
  void DumpDot(std::ostream &os) const override;

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

  const LoopTreeNodePtr GetRoot() const { return root; }

  const LoopTreeNodePtr GetLoopNode(const BBlockPtr bb) const {
    assert(block2node.count(bb) != 0 && "attempt to get non-existend loop");
    return block2node.at(bb);
  }

private:
  const Graph &graph;
  const DominatorTree domTree;

  LoopTreeNodePtr root;
  std::vector<LoopTreeNodePtr> loops;
  std::map<BBlockPtr, LoopTreeNodePtr> block2node;

private:
  // markers
  enum Marker { WHITE = 0, GREY, BLACK, GREEN };

  std::map<BBlockPtr, Marker> bb2marker;

  void SetMarker(BBlockPtr bb, Marker m) { bb2marker[bb] = m; }

  Marker GetMarker(BBlockPtr bb) {
    if (bb2marker.count(bb) == 0) {
      return WHITE;
    }
    return bb2marker.at(bb);
  }

private:
  // FIXME: replace with marker?
  bool IsBackEdge(const BBlockPtr bb) const;
  bool IsHead(const BBlockPtr bb) const;

  void DfsCollectBackEdges(const BBlockPtr curr);

  void CollectBackEdges() {
    DfsCollectBackEdges(graph.GetEntry());
    bb2marker.clear();
  }

  void ReverseDfsPopulateLoop(const BBlockPtr bb, const LoopTreeNodePtr loop);
  void PopulateReducibleLoop(const LoopTreeNodePtr loop);
  void PopulateLoops();

  void Init() {
    root = std::make_shared<LoopTreeNode>(LoopTreeNode(true /* isRoot */));
    // FIXME: rename GraphTraits::PushBlock
    PushBlock(root);
    CollectBackEdges();
    PopulateLoops();
  }
};
