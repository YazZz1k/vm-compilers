#pragma once

#include <IR/include/BasicBlock.h>
#include <IR/include/GraphTraits.h>

#include <list>
#include <memory>
#include <stack>

using BBlockPtr = std::shared_ptr<BBlock>;

class BBlockNode {
 public:
  using NodePtr = BBlockPtr;
};

class Graph : public GraphTraits<BBlockNode> {
 public:
  using NodePtr = BBlockPtr;
};
