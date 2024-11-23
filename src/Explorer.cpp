#include "Explorer.h"

#include <format>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>

#include "Board.h"
#include "SearchTree.h"

Explorer::Explorer(SearchTree* tree) {
    this->tree = tree;
    current = tree->root;
}

Explorer& Explorer::moveUp() {
    current = current->parent;
    board.undo();
    display();
    return *this;
}

Explorer& Explorer::moveDown(int index) {
    board.step(current->actions[index]);
    current = current->children[index];
    return *this;
}

Explorer& Explorer::printRow(std::vector<std::string> items, bool header) {
    for (int i = 0; i < items.size(); i++) {
        int colorCode = i % 6 + 31;
        std::string weight = "";
        
        if (header || rowCount % 2 == 0) {
            weight = "\033[1m";
            if (header) {
                colorCode += 10;
            }
        }
        std::string color = std::format("\033[{}m", std::to_string(colorCode));
        std::cout << weight << color << std::setw(10) << items[i];
    }
    std::cout << "\033[0m" << std::endl;
    rowCount++;
    return *this;
}

Explorer& Explorer::display() {
    std::cout << "\033c" << std::endl;

    std::cout << "[Tree Explorer]" << std::endl;

    std::cout << "[Board]" << std::endl;
    std::cout << board.toString() << std::endl;

    std::string turn = (board.getTurn() == Player::CROSS ? "Cross" : "Circle");
    printRow({"Turn", "Outcome", "Score"}, true);
    printRow({turn, resultToString(board.getWinner()), std::to_string(board.getScore())});

    std::cout << "[Navigation]" << std::endl;
    printRow({"Index", "Move", "Bias", "Raw", "Adj", "UCT", "Action", "Visits", "Value Count", "Exp", "Tree"}, true);
    printRow({"-2", "Quit"});
    printRow({"-1", "Undo"});
    bool inverse = board.getTurn() != Player::CROSS;
    int exploitPick = tree->exploitPolicy(current, inverse);
    int treePick = tree->treePolicy(current, inverse);
    for (int i = 0; i < current->actions.size(); i++) {
        double bias = tree->bias(current, i);
        double value = current->children[i]->getValue();
        double adj = board.getTurn() == Player::CROSS ? value: -value;
        printRow({
            std::to_string(i),
            moveToString(current->actions[i]),
            std::to_string(bias),
            std::to_string(value),
            std::to_string(adj),
            std::to_string(adj + bias),
            std::to_string(current->actionCount[i]),
            std::to_string(current->children[i]->visitCount),
            std::to_string(current->children[i]->valueCount),
            i == exploitPick ? "*": "",
            i == treePick ? "*": ""
        });
    }

    return *this;
};

Explorer& Explorer::run() {
    bool running = true;
    int input;
    while (running) {
        display();
        std::cin >> input;
        if (input < 0) {
            if (input == -2) {
                running = false;
            }
            if (input == -1) {
                if (current->parent) {
                    moveUp();
                } else {
                    std::cerr << "Null Node";
                }
            }
        } else {
            if (input >= 0 && input < current->actions.size()) {
                moveDown(input);
            } else {
                std::cerr << "Null Node";
            }
        }
    }
    return *this;
}