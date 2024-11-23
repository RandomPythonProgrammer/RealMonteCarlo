#include "Node.h"

#include <memory>
#include <format>
#include <string>

Node::Node(std::shared_ptr<Node> parent) {
    this->parent = parent;
}

std::shared_ptr<Node> Node::addChild(Move action) {
    std::shared_ptr<Node> child = std::make_shared<Node>(shared_from_this());
    child->lastAction = action;
    actions.push_back(action);
    children.push_back(child);
    actionCount.push_back(0);
    exploredSet.insert(action);
    return child;
}

Node& Node::addValue(double value) {
    this->value += value;
    valueCount++;
    return *this;
}

double Node::getValue() {
    return value / valueCount;
}

std::string Node::toString() {
    std::string actionString;
    for (int i = 0; i < actionCount.size(); i++) {
        actionString += std::to_string(i) + ": " + std::to_string(actionCount[i]);
        if (i != actionCount.size() - 1) {
            actionString += " | ";
        }
    }
    return std::format(
        "[Node Object]\n"
        "Value: {}\n"
        "Visit Count: {}\n"
        "Action Count: {}\n",
        getValue(), visitCount, actionString
    );
};
