#include "gtest/gtest.h"

#include <IR/include/BasicBlock.h>
#include <IR/include/Graph.h>

#include <iostream>

TEST(IR, fact) {
  auto entry = std::make_shared<BBlock>("\%entry");
  auto loop = std::make_shared<BBlock>("\%loop");
  auto calc = std::make_shared<BBlock>("\%calc");
  auto done = std::make_shared<BBlock>("\%done");

  Graph graph;

  graph.PushBlock(entry);
  graph.PushBlock(loop);
  graph.PushBlock(calc);
  graph.PushBlock(done);

  graph.CreateEdge(entry, loop);
  graph.CreateEdge(loop, calc);
  graph.CreateEdge(calc, loop);
  graph.CreateEdge(calc, done);

  auto entryLabel = std::make_shared<LabelOperand>(entry->GetLabel());
  auto loopLabel = std::make_shared<LabelOperand>(loop->GetLabel());
  auto calcLabel = std::make_shared<LabelOperand>(calc->GetLabel());
  auto doneLabel = std::make_shared<LabelOperand>(done->GetLabel());

  constexpr uint64_t inputValue = 10;

  IRBuilder builder(entry);
  auto imm1 = builder.CreateImm(1);
  auto imm2 = builder.CreateImm(2);
  auto imm3 = builder.CreateImm(inputValue);
  auto entry0 = builder.CreateAssign(imm1);
  auto entry1 = builder.CreateAssign(imm2);
  auto entry2 = builder.CreateAssign(imm3);

  builder.SetBBlock(loop);
  auto phi1 = builder.CreatePhi(entry0, entryLabel, entry0, calcLabel);
  auto phi2 = builder.CreatePhi(entry1, entryLabel, entry1, calcLabel);
  auto jeq = builder.CreateJeq(phi1, entry2, doneLabel);

  builder.SetBBlock(calc);
  auto calc0 = builder.CreateMul(phi2, phi1);
  auto calc1 = builder.CreateAdd(phi1, imm1);
  auto jmp = builder.CreateJmp(loopLabel);

  builder.SetBBlock(done);
  builder.CreateRet(phi2);

  std::static_pointer_cast<InstOperand>(phi1)->GetInst()->SetOperand(2, calc1);
  std::static_pointer_cast<InstOperand>(phi2)->GetInst()->SetOperand(2, calc0);

  entry->Dump(std::cout);
  loop->Dump(std::cout);
  calc->Dump(std::cout);
  done->Dump(std::cout);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
