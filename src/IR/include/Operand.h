#pragma once

#include <IR/include/Inst.h>

#include <cassert>
#include <iostream>
#include <memory>
#include <string>

class Inst;
class Label;

class Operand {
public:
  // FIXME should be external only
  enum OpKind { kImm, kInst, kLabel };

  Operand(OpKind kind) : kind(kind) {}

  bool IsInst() const { return kind == OpKind::kInst; }
  bool IsImm() const { return kind == OpKind::kImm; }
  bool IsLabel() const { return kind == OpKind::kLabel; }

  virtual void Dump(std::ostream &os) const = 0;

private:
  OpKind kind;
};

class LabelOperand final : public Operand {
public:
  LabelOperand(std::shared_ptr<Label> label)
      : Operand(OpKind::kLabel), label(label) {}
  std::shared_ptr<Label> GetLabel() const { return label; }

  void Dump(std::ostream &os) const override;

private:
  std::shared_ptr<Label> label;
};

class InstOperand final : public Operand {
public:
  InstOperand(std::shared_ptr<Inst> inst)
      : Operand(OpKind::kInst), inst(inst) {}
  std::shared_ptr<Inst> GetInst() { return inst; }

  void Dump(std::ostream &os) const override;

private:
  std::shared_ptr<Inst> inst;
};

class ImmOperand final : public Operand {
public:
  ImmOperand(uint64_t value) : Operand(OpKind::kImm), value(value) {}
  uint64_t GetValue() const { return value; }

  void Dump(std::ostream &os) const override;

private:
  uint64_t value;
};
