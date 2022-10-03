#pragma once

#include <map>
#include <string>

enum Opcode {
#define DEF_INST(x) OP_##x,
#include "Op.def"
#undef DEF_INST
  OP_undef
};

static std::string ToString(Opcode op) {
  std::map<Opcode, std::string> op2str = {
#define DEF_INST(x) {OP_##x, #x},
#include "Op.def"
#undef DEF_INST
      {OP_undef, "undef"}};
  return op2str.at(op);
}
