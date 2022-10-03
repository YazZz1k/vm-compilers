#include <IR/include/Operand.h>

void ImmOperand::Dump(std::ostream &os) const { os << value; }

void InstOperand::Dump(std::ostream &os) const { os << inst->GetName(); }

void LabelOperand::Dump(std::ostream &os) const { os << label->GetName(); }
