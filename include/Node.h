#pragma once

#include <memory>
#include <vector>
#include <Board.h>
#include <set>

class Node : public std::enable_shared_from_this<Node> {
   private:
    double value = 0;

   public:
    std::set<Move> exploredSet;
    bool expandable = true;
    std::vector<Move> actions;
    Move lastAction;
        int valueCount = 0;

    std::vector<std::shared_ptr<Node>> children;
    std::shared_ptr<Node> parent;

    std::vector<int> actionCount;
    int visitCount = 0;

    Node(std::shared_ptr<Node> parent={});

    std::shared_ptr<Node> addChild(Move action);
    Node& addValue(double value);
    double getValue();
    std::string toString();
};
