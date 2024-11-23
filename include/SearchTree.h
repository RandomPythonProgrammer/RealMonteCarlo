#pragma once

#include <Board.h>

#include <memory>

#include "Node.h"

class SearchTree {
   private:
    double c;

   public:
    std::shared_ptr<Node> root;

    SearchTree(double c);
    double bias(std::shared_ptr<Node> node, int i);

    int treePolicy(std::shared_ptr<Node> start, bool inverse);
    int defaultPolicy(std::shared_ptr<Node> start, bool inverse=false);
    int exploitPolicy(std::shared_ptr<Node> start, bool inverse);

    std::shared_ptr<Node> select(int (SearchTree::*policy)(std::shared_ptr<Node>, bool));
    std::shared_ptr<Node> expand(std::shared_ptr<Node> node, Move move);
    std::stack<Move> traverse(std::shared_ptr<Node> end);
    SearchTree& backPropagate(std::shared_ptr<Node> start, double result, double alpha);
};

int random(int start, int stop);