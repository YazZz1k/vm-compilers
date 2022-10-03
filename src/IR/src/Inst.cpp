#include <IR/include/Inst.h>

#include <algorithm>

void Inst::Dump(std::ostream &os) const {
  if (!IsLabel() && !IsRet() && !IsJmp() && !IsJeq()) {
    os << name << " = ";
  }

  os << ToString(op);

  if (IsLabel()) {
    os << " " << GetBBlock()->GetLabel()->GetName();
  } else {
    std::for_each(sources.begin(), sources.end(), [&os](const auto &src) {
      os << " ";
      src->Dump(os);
    });
  }
}

void PhiInst::Dump(std::ostream &os) const {
  os << GetName() << " = ";
  os << ToString(GetOpcode());
  os << " [ ";
  GetOperand(0)->Dump(os);
  os << " ";
  GetOperand(1)->Dump(os);
  os << " ]";
  os << " [ ";
  GetOperand(2)->Dump(os);
  os << " ";
  GetOperand(3)->Dump(os);
  os << " ]";
}

void Label::Dump(std::ostream &os) const {
  os << ToString(GetOpcode()) << " " << GetName();
}
