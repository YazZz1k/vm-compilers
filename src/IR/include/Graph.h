#pragma once

#include <list>
#include <memory>

#include <IR/include/BasicBlock.h>

class Graph {
public:
  using Iterator = std::list<std::shared_ptr<BBlock>>::iterator;

  Iterator BlockBegin() { return blockList.begin(); }

  Iterator BlockEnd() { return blockList.end(); }

  void PushBlock(std::shared_ptr<BBlock> block) { blockList.push_back(block); }

  void CreateEdge(std::shared_ptr<BBlock> from, std::shared_ptr<BBlock> to) {
    from->SetSucc(to);
    to->SetPred(from);
  }

  std::shared_ptr<BBlock> GetEntryBlock() { return blockList.front(); }

private:
  std::list<std::shared_ptr<BBlock>> blockList;
};
