#pragma once

#include <IR/include/BasicBlock.h>
#include <IR/include/Opcode.h>
#include <IR/include/Operand.h>

#include <cassert>
#include <iostream>
#include <list>
#include <vector>

class Operand;
class BBlock;

class Inst {
public:
  Inst(Opcode op, const std::vector<std::shared_ptr<Operand>> &sources,
       std::shared_ptr<BBlock> block, const std::string &name)
      : op(op), block(block), sources(sources), name(name) {}

  Inst(const Inst &rhs) : Inst(rhs.op, rhs.sources, rhs.block, rhs.name) {}

  bool operator==(const Inst &rhs) {
    // FIXME
    return name == rhs.name;
  }

  bool IsPhi() const { return op == OP_phi; }
  bool IsLabel() const { return op == OP_label; }
  bool IsJmp() const { return op == OP_jmp; }
  bool IsJeq() const { return op == OP_jeq; }
  bool IsRet() const { return op == OP_ret; }

  const std::string &GetName() const { return name; }
  void SetName(const std::string &newName) { name = newName; }

  std::shared_ptr<BBlock> GetBBlock() const { return block; }

  Opcode GetOpcode() const { return op; }

  std::shared_ptr<Operand> GetOperand(size_t numOp) const {
    assert(numOp < sources.size() && "invalid numOp");
    return sources.at(numOp);
  }

  // FIXME Phi inst must be handled separately
  virtual void SetOperand(size_t numOp, std::shared_ptr<Operand> opnd) {
    assert(numOp < sources.size() && "invalid numOp");
    sources[numOp] = opnd;
  }
  virtual void Dump(std::ostream &os) const;

private:
  Opcode op;
  std::string name;
  std::vector<std::shared_ptr<Operand>> sources;
  std::shared_ptr<Operand> src0;
  std::shared_ptr<Operand> src1;
  std::shared_ptr<BBlock> block;
};

class Label : public Inst {
public:
  Label(std::shared_ptr<BBlock> block, const std::string &name)
      : Inst(OP_label, std::vector<std::shared_ptr<Operand>>(), block, name) {}

  void Dump(std::ostream &os) const override;
};

class PhiInst : public Inst {
public:
  PhiInst(std::shared_ptr<Operand> src0, std::shared_ptr<LabelOperand> pred0,
          std::shared_ptr<Operand> src1, std::shared_ptr<LabelOperand> pred1,
          std::shared_ptr<BBlock> block, const std::string &name)
      : Inst(OP_phi,
             {src0, std::static_pointer_cast<Operand>(pred0), src1,
              std::static_pointer_cast<Operand>(pred1)},
             block, name) {}

  void Dump(std::ostream &os) const override;
};
