#include "gtest/gtest.h"

#include <passes/LoopAnalysis.h>

TEST(LoopAnalysis, first) {
  Graph graph;
  BBlockPtr A = std::make_shared<BBlock>("A");
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
  graph.CreateEdge(B, E);
  graph.CreateEdge(E, D);
  graph.CreateEdge(E, G);
  graph.CreateEdge(D, F);
  graph.CreateEdge(G, F);
  graph.CreateEdge(C, F);

  LoopTree loopTree(graph);

  const auto root = loopTree.GetRoot();
  ASSERT_EQ(root->GetInnerLoops().empty(), true);
  ASSERT_EQ(root->GetOuterLoop() == nullptr, true);

  ASSERT_EQ(loopTree.GetLoopNode(A) == root, true);
  ASSERT_EQ(loopTree.GetLoopNode(B) == root, true);
  ASSERT_EQ(loopTree.GetLoopNode(C) == root, true);
  ASSERT_EQ(loopTree.GetLoopNode(D) == root, true);
  ASSERT_EQ(loopTree.GetLoopNode(E) == root, true);
  ASSERT_EQ(loopTree.GetLoopNode(F) == root, true);
  ASSERT_EQ(loopTree.GetLoopNode(G) == root, true);
}

TEST(LoopAnalysis, second) {
  Graph graph;
  BBlockPtr A = std::make_shared<BBlock>("A");
  graph.PushBlock(A);
  BBlockPtr B = std::make_shared<BBlock>("B");
  graph.PushBlock(B);
  BBlockPtr C = std::make_shared<BBlock>("C");
  graph.PushBlock(C);
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
  BBlockPtr L = std::make_shared<BBlock>("L");
  graph.PushBlock(L);
  BBlockPtr M = std::make_shared<BBlock>("M");
  graph.PushBlock(M);

  graph.CreateEdge(A, B);
  graph.CreateEdge(B, C);
  graph.CreateEdge(B, E);
  graph.CreateEdge(C, E);
  graph.CreateEdge(E, F);
  graph.CreateEdge(F, E);
  graph.CreateEdge(F, G);
  graph.CreateEdge(G, H);
  graph.CreateEdge(H, G);

  graph.CreateEdge(H, I);
  graph.CreateEdge(I, K);
  graph.CreateEdge(K, L);
  graph.CreateEdge(I, M);
  graph.CreateEdge(M, B);

  LoopTree loopTree(graph);

  const auto root = loopTree.GetRoot();
  const auto loop0 = loopTree.GetLoopNode(B);
  const auto loop1 = loopTree.GetLoopNode(E);
  const auto loop2 = loopTree.GetLoopNode(G);

  // root: sources: A, K, L
  ASSERT_EQ(root->GetOuterLoop() == nullptr, true);
  ASSERT_EQ(loopTree.GetLoopNode(A) == root, true);
  ASSERT_EQ(loopTree.GetLoopNode(K) == root, true);
  ASSERT_EQ(loopTree.GetLoopNode(L) == root, true);

  // loop0: head B, back edge src M, sources I, C
  ASSERT_EQ(loop0->GetHead() == B, true);
  ASSERT_EQ(loop0->GetBackEdges() == std::set<BBlockPtr>{M}, true);
  ASSERT_EQ(loopTree.GetLoopNode(I) == loop0, true);
  ASSERT_EQ(loopTree.GetLoopNode(C) == loop0, true);
  // root -> loop0
  ASSERT_EQ(loop0->GetOuterLoop() == root, true);

  // loop1: head E, back edge src F
  ASSERT_EQ(loop1->GetHead() == E, true);
  ASSERT_EQ(loop1->GetBackEdges() == std::set<BBlockPtr>{F}, true);
  // loop0 -> loop1
  ASSERT_EQ(loop1->GetOuterLoop() == loop0, true);

  // loop2: head G, back edge src H
  ASSERT_EQ(loop2->GetHead() == G, true);
  ASSERT_EQ(loop2->GetBackEdges() == std::set<BBlockPtr>{H}, true);
  // loop0 -> loop2
  ASSERT_EQ(loop2->GetOuterLoop() == loop0, true);
}

TEST(LoopAnalysis, third) {
  Graph graph;
  BBlockPtr A = std::make_shared<BBlock>("A");
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
  graph.CreateEdge(B, C);
  graph.CreateEdge(B, G);
  graph.CreateEdge(C, D);
  graph.CreateEdge(G, H);
  graph.CreateEdge(G, D);
  graph.CreateEdge(H, B);
  graph.CreateEdge(H, I);
  graph.CreateEdge(D, E);
  graph.CreateEdge(I, E);
  graph.CreateEdge(I, F);
  graph.CreateEdge(E, C);
  graph.CreateEdge(E, F);

  LoopTree loopTree(graph);

  const auto root = loopTree.GetRoot();
  const auto loop0 = loopTree.GetLoopNode(C);
  const auto loop1 = loopTree.GetLoopNode(B);

  // root. sources: A F I
  ASSERT_EQ(root->GetOuterLoop() == nullptr, true);
  ASSERT_EQ(loopTree.GetLoopNode(A) == root, true);
  ASSERT_EQ(loopTree.GetLoopNode(F) == root, true);
  ASSERT_EQ(loopTree.GetLoopNode(I) == root, true);

  // root -> loop0
  ASSERT_EQ(loop0->GetOuterLoop() == root, true);
  ASSERT_EQ(loop0->IsReducible(), false);
  ASSERT_EQ(loop0->GetHead() == C, true);
  ASSERT_EQ(loop0->GetBackEdges() == std::set<BBlockPtr>{E}, true);

  // root -> loop1
  ASSERT_EQ(loop1->GetOuterLoop() == root, true);
  // loop1 : head B, back edge src H, sources G
  ASSERT_EQ(loop1->GetHead() == B, true);
  ASSERT_EQ(loop1->GetBackEdges() == std::set<BBlockPtr>{H}, true);
  ASSERT_EQ(loopTree.GetLoopNode(G) == loop1, true);
}
