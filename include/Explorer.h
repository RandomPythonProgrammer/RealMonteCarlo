#pragma once

#include <memory>

#include "Node.h"
#include "SearchTree.h"
#include "Board.h"

class Explorer {
   private:
    SearchTree* tree;
    std::shared_ptr<Node> current;
    Board board;
    int rowCount = 0;

   public:
    Explorer(SearchTree* tree);
    Explorer& moveUp();
    Explorer& moveDown(int index);
    Explorer& printRow(std::vector<std::string> items, bool header=false);
    Explorer& display();
    Explorer& run();
};