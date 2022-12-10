#include "gtest/gtest.h"

#include <IR/include/BasicBlock.h>
#include <IR/include/Graph.h>
#include <passes/DominatorTree.h>
#include <passes/LoopAnalysis.h>

#include <fstream>
#include <iostream>

#define DUMP_DOT

TEST(Required, first) {
  Graph graph;

  BBlockPtr A = std::make_shared<BBlock>("A");
  // first pushed block is the entry
  graph.PushBlock(A);
  BBlockPtr B = std::make_shared<BBlock>("B");
  graph.PushBlock(B);
  BBlockPtr C = std::make_shared<BBlock>("C");
  graph.PushBlock(C);
  BBlockPtr D = std::make_shared<BBlock>("D");
  graph.PushBlock(D);
  BBlockPtr E = std::make_shared<BBlock>("E");
  graph.PushBlock(E);
  BBlockPtr F = std::make_shared<BBlock>("F");
  graph.PushBlock(F);
  BBlockPtr G = std::make_shared<BBlock>("G");
  graph.PushBlock(G);

  graph.CreateEdge(A, B);
  graph.CreateEdge(B, C);
  graph.CreateEdge(B, F);
  graph.CreateEdge(C, D);
  graph.CreateEdge(F, E);
  graph.CreateEdge(F, G);
  graph.CreateEdge(G, D);
  graph.CreateEdge(A, B);
  graph.CreateEdge(E, D);

#ifdef DUMP_DOT
  std::ofstream f("first.dot");
  assert(f.is_open() && "can't open file");
  graph.DumpDot(f);
  f.close();
#endif

  std::vector<BBlockPtr> dfs = graph.GetDfs();
  std::vector<BBlockPtr> dfs_exp = {A, B, C, D, F, E, G};
  ASSERT_EQ(dfs, dfs_exp);

  std::vector<BBlockPtr> po = graph.GetPo();
  std::vector<BBlockPtr> po_exp = {A, B, C, F, D, E, G};
  ASSERT_EQ(po, po_exp);

  DominatorTree domTree(graph);

#ifdef DUMP_DOT
  std::ofstream fdom("first.dt.dot");
  assert(fdom.is_open() && "can't open file");
  domTree.DumpDot(fdom);
  fdom.close();
#endif

  // check some dominators
  ASSERT_TRUE(domTree.IsDominate(A, B));
  ASSERT_TRUE(domTree.IsDominate(B, C));
  ASSERT_TRUE(domTree.IsDominate(B, D));
  ASSERT_TRUE(domTree.IsDominate(B, F));
  ASSERT_TRUE(domTree.IsDominate(F, E));
  ASSERT_TRUE(domTree.IsDominate(F, G));

  ASSERT_FALSE(domTree.IsDominate(F, D));
}

TEST(Required, second) {
  Graph graph;

  BBlockPtr A = std::make_shared<BBlock>("A");
  // first pushed block is the entry
  graph.PushBlock(A);
  BBlockPtr B = std::make_shared<BBlock>("B");
  graph.PushBlock(B);
  BBlockPtr C = std::make_shared<BBlock>("C");
  graph.PushBlock(C);
  BBlockPtr D = std::make_shared<BBlock>("D");
  graph.PushBlock(D);
  BBlockPtr E = std::make_shared<BBlock>("E");
  graph.PushBlock(E);
  BBlockPtr F = std::make_shared<BBlock>("F");
  graph.PushBlock(F);
  BBlockPtr G = std::make_shared<BBlock>("G");
  graph.PushBlock(G);
  BBlockPtr H = std::make_shared<BBlock>("H");
  graph.PushBlock(H);
  BBlockPtr I = std::make_shared<BBlock>("I");
  graph.PushBlock(I);
  BBlockPtr K = std::make_shared<BBlock>("K");
  graph.PushBlock(K);
  BBlockPtr J = std::make_shared<BBlock>("J");
  graph.PushBlock(J);

  graph.CreateEdge(A, B);
  graph.CreateEdge(B, C);
  graph.CreateEdge(C, D);
  graph.CreateEdge(D, E);
  graph.CreateEdge(E, F);
  graph.CreateEdge(F, G);
  graph.CreateEdge(I, K);
  graph.CreateEdge(G, H);
  graph.CreateEdge(G, I);
  graph.CreateEdge(B, J);
  graph.CreateEdge(J, C);
  graph.CreateEdge(D, C);
  graph.CreateEdge(H, B);
  graph.CreateEdge(F, E);

#ifdef DUMP_DOT
  std::ofstream f("second.dot");
  assert(f.is_open() && "can't open file");
  graph.DumpDot(f);
  f.close();
#endif

  std::vector<BBlockPtr> dfs = graph.GetDfs();
  std::vector<BBlockPtr> dfs_exp = {A, B, C, D, E, F, G, H, I, K, J};
  ASSERT_EQ(dfs, dfs_exp);

  std::vector<BBlockPtr> po = graph.GetPo();
  std::vector<BBlockPtr> po_exp = {A, B, C, J, D, E, F, G, H, I, K};
  ASSERT_EQ(po, po_exp);

  DominatorTree domTree(graph);

#ifdef DUMP_DOT
  std::ofstream fdom("second.dt.dot");
  assert(fdom.is_open() && "can't open file");
  domTree.DumpDot(fdom);
  fdom.close();
#endif

  // check some dominators
  ASSERT_TRUE(domTree.IsDominate(A, B));
  ASSERT_TRUE(domTree.IsDominate(B, C));
  ASSERT_TRUE(domTree.IsDominate(B, J));
  ASSERT_TRUE(domTree.IsDominate(C, D));
  ASSERT_TRUE(domTree.IsDominate(D, E));
  ASSERT_TRUE(domTree.IsDominate(E, F));
  ASSERT_TRUE(domTree.IsDominate(F, G));
  ASSERT_TRUE(domTree.IsDominate(G, H));
  ASSERT_TRUE(domTree.IsDominate(G, I));
  ASSERT_TRUE(domTree.IsDominate(I, K));
}

TEST(Required, third) {
  Graph graph;

  BBlockPtr A = std::make_shared<BBlock>("A");
  // first pushed block is the entry
  graph.PushBlock(A);
  BBlockPtr B = std::make_shared<BBlock>("B");
  graph.PushBlock(B);
  BBlockPtr C = std::make_shared<BBlock>("C");
  graph.PushBlock(C);
  BBlockPtr D = std::make_shared<BBlock>("D");
  graph.PushBlock(D);
  BBlockPtr E = std::make_shared<BBlock>("E");
  graph.PushBlock(E);
  BBlockPtr F = std::make_shared<BBlock>("F");
  graph.PushBlock(F);
  BBlockPtr G = std::make_shared<BBlock>("G");
  graph.PushBlock(G);
  BBlockPtr H = std::make_shared<BBlock>("H");
  graph.PushBlock(H);
  BBlockPtr I = std::make_shared<BBlock>("I");
  graph.PushBlock(I);

  graph.CreateEdge(A, B);

  graph.CreateEdge(B, E);
  graph.CreateEdge(E, F);
  graph.CreateEdge(F, H);
  graph.CreateEdge(H, I);
  graph.CreateEdge(H, G);

  graph.CreateEdge(B, C);
  graph.CreateEdge(C, D);
  graph.CreateEdge(D, G);
  graph.CreateEdge(G, I);

  graph.CreateEdge(F, B);
  graph.CreateEdge(G, C);

#ifdef DUMP_DOT
  std::ofstream f("third.dot");
  assert(f.is_open() && "can't open file");
  graph.DumpDot(f);
  f.close();
#endif

  std::vector<BBlockPtr> dfs = graph.GetDfs();
  std::vector<BBlockPtr> dfs_exp = {A, B, E, F, H, I, G, C, D};
  ASSERT_EQ(dfs, dfs_exp);

  std::vector<BBlockPtr> po = graph.GetPo();
  std::vector<BBlockPtr> po_exp = {A, B, E, C, F, D, H, G, I};
  ASSERT_EQ(po, po_exp);

  DominatorTree domTree(graph);

#ifdef DUMP_DOT
  std::ofstream fdom("third.dt.dot");
  assert(fdom.is_open() && "can't open file");
  domTree.DumpDot(fdom);
  fdom.close();
#endif

  // check some dominators
  ASSERT_TRUE(domTree.IsDominate(A, B));
  ASSERT_TRUE(domTree.IsDominate(B, G));
  ASSERT_TRUE(domTree.IsDominate(B, E));
  ASSERT_TRUE(domTree.IsDominate(B, I));
  ASSERT_TRUE(domTree.IsDominate(B, C));
  ASSERT_TRUE(domTree.IsDominate(E, F));
  ASSERT_TRUE(domTree.IsDominate(F, H));
  ASSERT_TRUE(domTree.IsDominate(C, D));

  ASSERT_FALSE(domTree.IsDominate(D, C));
  ASSERT_FALSE(domTree.IsDominate(E, D));

  LoopTree loopTree(graph);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
