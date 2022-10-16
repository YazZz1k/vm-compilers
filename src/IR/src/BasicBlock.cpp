#include <IR/include/BasicBlock.h>

BBlock::BBlock(const std::string &name) : name(name) {}

void BBlock::Dump(std::ostream &os) const {
  os << "%" << name << ":" << std::endl;

  for (auto i : instList) {
    os << "  ";
    i->Dump(os);
    os << std::endl;
  }
}

std::shared_ptr<Operand> IRBuilder::CreateImm(uint64_t val) {
  return std::make_shared<ImmOperand>(val);
}

std::shared_ptr<Operand> IRBuilder::CreateAssign(std::shared_ptr<Operand> src) {
  assert((src->IsImm() || src->IsInst()) && "invalid arguments");
  return inserter.Insert(std::make_shared<Inst>(
      Inst(OP_assign, {src}, inserter.GetBBlock(), CreateInstName())));
}

std::shared_ptr<Operand> IRBuilder::CreateAdd(std::shared_ptr<Operand> src0,
                                              std::shared_ptr<Operand> src1) {
  assert((src0->IsImm() || src0->IsInst()) &&
         (src1->IsImm() || src1->IsInst()) && "invalid arguments");
  return inserter.Insert(std::make_shared<Inst>(
      Inst(OP_add, {src0, src1}, inserter.GetBBlock(), CreateInstName())));
}

std::shared_ptr<Operand> IRBuilder::CreateMul(std::shared_ptr<Operand> src0,
                                              std::shared_ptr<Operand> src1) {
  assert((src0->IsImm() || src0->IsInst()) &&
         (src1->IsImm() || src1->IsInst()) && "invalid arguments");
  return inserter.Insert(std::make_shared<Inst>(
      Inst(OP_mul, {src0, src1}, inserter.GetBBlock(), CreateInstName())));
}

std::shared_ptr<Operand> IRBuilder::CreateCmp(std::shared_ptr<Operand> src0,
                                              std::shared_ptr<Operand> src1) {
  assert((src0->IsImm() || src0->IsInst()) &&
         (src1->IsImm() || src1->IsInst()) && "invalid arguments");
  return inserter.Insert(std::make_shared<Inst>(
      Inst(OP_cmp, {src0, src1}, inserter.GetBBlock(), CreateInstName())));
}

std::shared_ptr<Operand>
IRBuilder::CreateJeq(std::shared_ptr<Operand> src0,
                     std::shared_ptr<Operand> src1,
                     std::shared_ptr<LabelOperand> label) {
  assert((src0->IsImm() || src0->IsInst()) &&
         (src1->IsImm() || src1->IsInst()) && "invalid arguments");
  return inserter.Insert(std::make_shared<Inst>(
      Inst(OP_jeq, {src0, src1, std::static_pointer_cast<Operand>(label)},
           inserter.GetBBlock(), CreateInstName())));
}

std::shared_ptr<Operand>
IRBuilder::CreateJmp(std::shared_ptr<LabelOperand> label) {
  return inserter.Insert(std::make_shared<Inst>(
      Inst(OP_jmp, {std::static_pointer_cast<Operand>(label)},
           inserter.GetBBlock(), CreateInstName())));
}

std::shared_ptr<Operand> IRBuilder::CreateRet(std::shared_ptr<Operand> ret) {
  assert((ret->IsImm() || ret->IsInst()) && "invalid arguments");
  return inserter.Insert(std::make_shared<Inst>(
      Inst(OP_ret, {ret}, inserter.GetBBlock(), CreateInstName())));
}

std::shared_ptr<Operand> IRBuilder::CreatePhi(
    std::shared_ptr<Operand> src0, std::shared_ptr<LabelOperand> pred0,
    std::shared_ptr<Operand> src1, std::shared_ptr<LabelOperand> pred1) {
  assert((src0->IsImm() || src0->IsInst()) &&
         (src1->IsImm() || src1->IsInst()) && "invalid arguments");
  return inserter.Insert(std::make_shared<PhiInst>(PhiInst(
      src0, pred0, src1, pred1, inserter.GetBBlock(), CreateInstName())));
}
