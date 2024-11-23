#include "SearchTree.h"
#include <cmath>
#include <cstdlib>
#include <memory>
#include "Node.h"
#include "Logger.h"
#include <iostream>
#include <random>
#include <string>

static Logger logger(LogLevel::INFO, "SearchTree", &std::cout);

SearchTree::SearchTree(double c) {
    root = std::make_shared<Node>();
    this->c = c;
}

double SearchTree::bias(std::shared_ptr<Node> node, int i) {
    return 2 * c * std::sqrt((2 * std::log(node->visitCount)/node->actionCount[i]));
}

int SearchTree::treePolicy(std::shared_ptr<Node> start, bool inverse) {
    int selected = -1;
    std::vector<double> scores;
    for (int i = 0; i < start->children.size(); i++) {
        std::shared_ptr<Node>& child = start->children[i];
        double value = child->getValue();
        if (inverse) {
            value = -value;
        }
        scores.push_back(value + 1 + bias(start, i));
    }
    std::random_device device;
    std::default_random_engine engine(device());
    std::discrete_distribution<> dist(scores.begin(), scores.end());
    selected = dist(engine);
    logger.debug("Selected: " + std::to_string(selected));

    return selected;
}

int SearchTree::defaultPolicy(std::shared_ptr<Node> start, bool inverse) {
    std::random_device device;
    std::default_random_engine engine(device());
    std::uniform_int_distribution<int> dist(0, start->children.size()-1);
    int selected = dist(engine);
    logger.debug("Selected: " + std::to_string(selected));

    return selected;
}

int SearchTree::exploitPolicy(std::shared_ptr<Node> start, bool inverse) {
    int selected = -1;
    double largest = 0;
    std::vector<double> scores;
    for (int i = 0; i < start->children.size(); i++) {
        std::shared_ptr<Node>& child = start->children[i];
        double value = child->getValue();
        if (inverse) {
            value = -value;
        }
        if (selected == -1 || value > largest) {
            largest = value;
            selected = i;
        }
    }

    return selected;
}

std::shared_ptr<Node> SearchTree::select(int (SearchTree::*policy)(std::shared_ptr<Node>, bool)) {
    std::shared_ptr<Node> current = root;
    bool inverse = false;
    while (current->children.size() && !current->expandable) {
        int index = (this->*policy)(current, inverse);
        current->visitCount++;
        current->actionCount[index]++;
        current = current->children[index];
        inverse = !inverse;
    }
    current->visitCount++;
    return current;
}

std::shared_ptr<Node>  SearchTree::expand(std::shared_ptr<Node> node, Move move) {
    std::shared_ptr<Node> child = node->addChild(move);
    child->visitCount++;
    node->actionCount[node->actionCount.size()-1]++;
    return child;
}

///This resets the board and traverses it to the end node
std::stack<Move> SearchTree::traverse(std::shared_ptr<Node> end) {
    std::stack<Move> moves;
    std::shared_ptr current = end;
    while (current->parent) {
        moves.push(current->lastAction);
        current = current->parent;
    }
    return moves;
}

SearchTree& SearchTree::backPropagate(std::shared_ptr<Node> start, double result, double alpha) {
    std::shared_ptr<Node> current = start;
    std::stack<std::shared_ptr<Node>> nodes;
    nodes.push(current);
    while (current->parent) {
        current = current->parent;
        nodes.push(current);
    }
    for (int i = 0; !nodes.empty(); i++) {
        std::shared_ptr<Node> node = nodes.top();
        node->addValue(result * exp(-i * alpha));
        nodes.pop();
    }

    return *this;
}

int random(int start, int stop) {
    std::random_device device;
    std::default_random_engine engine(device());
    std::uniform_int_distribution<int> dist(start, stop);
    return dist(engine);
}