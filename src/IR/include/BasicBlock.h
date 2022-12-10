#pragma once

#include <IR/include/Inst.h>
#include <IR/include/Opcode.h>
#include <IR/include/Operand.h>

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <vector>

class Inst;
class Label;
class PhiInst;
class Operand;
class InstOperand;
class ImmOperand;
class LabelOperand;

class BBlock;
using BBlockPtr = std::shared_ptr<BBlock>;

class BBlock {
public:
  BBlock();
  BBlock(const std::string &lname);

  // non-movable, non-copyable
  BBlock(BBlock &) = delete;
  BBlock(BBlock &&) = delete;

  using Iterator = std::list<std::shared_ptr<Inst>>::iterator;
  using PhiIterator = std::list<std::shared_ptr<PhiInst>>::iterator;

  Iterator InstBegin() { return instList.begin(); }
  Iterator InstEnd() { return instList.end(); }

  PhiIterator PhiBegin() { return phiList.begin(); }
  PhiIterator PhiEnd() { return phiList.end(); }

  std::list<std::shared_ptr<Inst>> &GetInstList() { return instList; }
  std::list<std::shared_ptr<PhiInst>> &GetPhiList() { return phiList; }

  void AddSucc(std::shared_ptr<BBlock> succ) {
    auto it = std::find(successors.begin(), successors.end(), succ);
    if (it == successors.end()) {
      assert(successors.size() <= 2 && "no more than 2 successors");
      successors.push_back(succ);
    }
  }

  void AddPred(std::shared_ptr<BBlock> pred) {
    auto it = std::find(predessors.begin(), predessors.end(), pred);
    if (it == predessors.end()) {
      predessors.push_back(pred);
    }
  }

  bool RemoveSucc(std::shared_ptr<BBlock> succ) {
    auto it = std::find(successors.begin(), successors.end(), succ);
    return successors.erase(it) != successors.end();
  }

  bool RemovePred(std::shared_ptr<BBlock> pred) {
    auto it = std::find(predessors.begin(), predessors.end(), pred);
    return predessors.erase(it) != predessors.end();
  }

  const std::list<std::shared_ptr<BBlock>> &GetSuccessors() const {
    return successors;
  }

  const std::list<std::shared_ptr<BBlock>> &GetPredessors() const {
    return predessors;
  }

  const std::string &GetName() const { return name; };

  void Dump(std::ostream &os) const;

private:
  std::string name;

  std::list<std::shared_ptr<Inst>> instList;
  std::list<std::shared_ptr<PhiInst>> phiList;
  std::list<std::shared_ptr<BBlock>> predessors;
  std::list<std::shared_ptr<BBlock>> successors;
};

class Inserter {
public:
  Inserter() {
    // FIXME
    assert(false && "unimplemented");
  }

  Inserter(std::shared_ptr<BBlock> block)
      : block(block), pos(block->InstBegin()), posPhi(block->PhiBegin()) {
    instCount = std::distance(block->InstBegin(), block->InstEnd());
  }

  using Iterator = BBlock::Iterator;
  using PhiIterator = BBlock::PhiIterator;

  template <class I> std::shared_ptr<Operand> Insert(std::shared_ptr<I> inst) {
    ++instCount;
    auto instList = block->GetInstList();
    pos = instList.insert(++pos, inst);
    return std::static_pointer_cast<Operand>(
        std::make_shared<InstOperand>(*pos));
  }

  std::shared_ptr<Operand> Insert(std::shared_ptr<PhiInst> phiInst) {
    ++instCount;
    auto phiList = block->GetPhiList();
    phiList.insert(++posPhi, phiInst);

    auto inst = std::static_pointer_cast<Inst>(phiInst);
    auto instList = block->GetInstList();
    pos = instList.insert(++pos, inst);
    return std::static_pointer_cast<Operand>(
        std::make_shared<InstOperand>(*pos));
  }

  std::shared_ptr<BBlock> GetBBlock() const { return block; }

  void SetBBlock(std::shared_ptr<BBlock> newBlock) {
    block = newBlock;
    instCount = std::distance(block->InstBegin(), block->InstEnd());
    pos = block->InstBegin();
    posPhi = block->PhiBegin();
  }

  uint64_t GetInstCount() const { return instCount; }

private:
  uint64_t instCount;
  std::shared_ptr<BBlock> block;
  Iterator pos; // insert before position
  PhiIterator posPhi;
};

class IRBuilder {
public:
  IRBuilder(std::shared_ptr<BBlock> block) : inserter(block) {}
  IRBuilder() {}

  using Iterator = BBlock::Iterator;

  std::shared_ptr<BBlock> GetBBlock() { return inserter.GetBBlock(); }

  void SetBBlock(std::shared_ptr<BBlock> newBlock) {
    inserter.SetBBlock(newBlock);
  }

  std::shared_ptr<Operand> CreateImm(uint64_t val);
  std::shared_ptr<Operand> CreateAssign(std::shared_ptr<Operand> src);
  std::shared_ptr<Operand> CreateAdd(std::shared_ptr<Operand> src0,
                                     std::shared_ptr<Operand> src1);
  std::shared_ptr<Operand> CreateMul(std::shared_ptr<Operand> src0,
                                     std::shared_ptr<Operand> src1);
  std::shared_ptr<Operand> CreateCmp(std::shared_ptr<Operand> src0,
                                     std::shared_ptr<Operand> src1);
  std::shared_ptr<Operand> CreateJeq(std::shared_ptr<Operand> src0,
                                     std::shared_ptr<Operand> src1,
                                     std::shared_ptr<LabelOperand> label);
  std::shared_ptr<Operand> CreateJmp(std::shared_ptr<LabelOperand> label);
  std::shared_ptr<Operand> CreateRet(std::shared_ptr<Operand> ret);
  std::shared_ptr<Operand> CreatePhi(std::shared_ptr<Operand> src0,
                                     std::shared_ptr<LabelOperand> pred0,
                                     std::shared_ptr<Operand> src1,
                                     std::shared_ptr<LabelOperand> pred1);

private:
  std::string CreateInstName() const {
    return inserter.GetBBlock()->GetName() +
           std::to_string(inserter.GetInstCount());
  }

private:
  Inserter inserter;
};
