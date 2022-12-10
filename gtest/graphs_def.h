#pragma once
#include <IR/include/BasicBlock.h>
#include <IR/include/Graph.h>

#define INIT_FIRST_GRAPH(NAME)                                                 \
  Graph NAME;                                                                  \
  BBlockPtr A = std::make_shared<BBlock>("A");                                 \
  NAME.PushBlock(A);                                                           \
  BBlockPtr B = std::make_shared<BBlock>("B");                                 \
  NAME.PushBlock(B);                                                           \
  BBlockPtr C = std::make_shared<BBlock>("C");                                 \
  NAME.PushBlock(C);                                                           \
  BBlockPtr D = std::make_shared<BBlock>("D");                                 \
  NAME.PushBlock(D);                                                           \
  BBlockPtr E = std::make_shared<BBlock>("E");                                 \
  NAME.PushBlock(E);                                                           \
  BBlockPtr F = std::make_shared<BBlock>("F");                                 \
  NAME.PushBlock(F);                                                           \
  BBlockPtr G = std::make_shared<BBlock>("G");                                 \
  NAME.PushBlock(G);                                                           \
  NAME.CreateEdge(A, B);                                                       \
  NAME.CreateEdge(B, C);                                                       \
  NAME.CreateEdge(B, F);                                                       \
  NAME.CreateEdge(C, D);                                                       \
  NAME.CreateEdge(F, E);                                                       \
  NAME.CreateEdge(F, G);                                                       \
  NAME.CreateEdge(G, D);                                                       \
  NAME.CreateEdge(A, B);                                                       \
  NAME.CreateEdge(E, D);

#define INIT_SECOND_GRAPH(NAME)                                                \
  Graph NAME;                                                                  \
  BBlockPtr A = std::make_shared<BBlock>("A");                                 \
  NAME.PushBlock(A);                                                           \
  BBlockPtr B = std::make_shared<BBlock>("B");                                 \
  NAME.PushBlock(B);                                                           \
  BBlockPtr C = std::make_shared<BBlock>("C");                                 \
  NAME.PushBlock(C);                                                           \
  BBlockPtr D = std::make_shared<BBlock>("D");                                 \
  NAME.PushBlock(D);                                                           \
  BBlockPtr E = std::make_shared<BBlock>("E");                                 \
  NAME.PushBlock(E);                                                           \
  BBlockPtr F = std::make_shared<BBlock>("F");                                 \
  NAME.PushBlock(F);                                                           \
  BBlockPtr G = std::make_shared<BBlock>("G");                                 \
  NAME.PushBlock(G);                                                           \
  BBlockPtr H = std::make_shared<BBlock>("H");                                 \
  NAME.PushBlock(H);                                                           \
  BBlockPtr I = std::make_shared<BBlock>("I");                                 \
  NAME.PushBlock(I);                                                           \
  BBlockPtr K = std::make_shared<BBlock>("K");                                 \
  NAME.PushBlock(K);                                                           \
  BBlockPtr J = std::make_shared<BBlock>("J");                                 \
  NAME.PushBlock(J);                                                           \
  NAME.CreateEdge(A, B);                                                       \
  NAME.CreateEdge(B, C);                                                       \
  NAME.CreateEdge(C, D);                                                       \
  NAME.CreateEdge(D, E);                                                       \
  NAME.CreateEdge(E, F);                                                       \
  NAME.CreateEdge(F, G);                                                       \
  NAME.CreateEdge(I, K);                                                       \
  NAME.CreateEdge(G, H);                                                       \
  NAME.CreateEdge(G, I);                                                       \
  NAME.CreateEdge(B, J);                                                       \
  NAME.CreateEdge(J, C);                                                       \
  NAME.CreateEdge(D, C);                                                       \
  NAME.CreateEdge(H, B);                                                       \
  NAME.CreateEdge(F, E);

#define INIT_THIRD_GRAPH(NAME)                                                 \
  Graph graph;                                                                 \
  BBlockPtr A = std::make_shared<BBlock>("A");                                 \
  graph.PushBlock(A);                                                          \
  BBlockPtr B = std::make_shared<BBlock>("B");                                 \
  graph.PushBlock(B);                                                          \
  BBlockPtr C = std::make_shared<BBlock>("C");                                 \
  graph.PushBlock(C);                                                          \
  BBlockPtr D = std::make_shared<BBlock>("D");                                 \
  graph.PushBlock(D);                                                          \
  BBlockPtr E = std::make_shared<BBlock>("E");                                 \
  graph.PushBlock(E);                                                          \
  BBlockPtr F = std::make_shared<BBlock>("F");                                 \
  graph.PushBlock(F);                                                          \
  BBlockPtr G = std::make_shared<BBlock>("G");                                 \
  graph.PushBlock(G);                                                          \
  BBlockPtr H = std::make_shared<BBlock>("H");                                 \
  graph.PushBlock(H);                                                          \
  BBlockPtr I = std::make_shared<BBlock>("I");                                 \
  graph.PushBlock(I);                                                          \
  graph.CreateEdge(A, B);                                                      \
  graph.CreateEdge(B, E);                                                      \
  graph.CreateEdge(E, F);                                                      \
  graph.CreateEdge(F, H);                                                      \
  graph.CreateEdge(H, I);                                                      \
  graph.CreateEdge(H, G);                                                      \
  graph.CreateEdge(B, C);                                                      \
  graph.CreateEdge(C, D);                                                      \
  graph.CreateEdge(D, G);                                                      \
  graph.CreateEdge(G, I);                                                      \
  graph.CreateEdge(F, B);                                                      \
  graph.CreateEdge(G, C);
