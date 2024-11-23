#include <tgmath.h>
#include <yaml-cpp/node/parse.h>
#include <yaml-cpp/yaml.h>

#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <vector>

#include "Board.h"
#include "Explorer.h"
#include "Logger.h"
#include "Node.h"
#include "SearchTree.h"

static Logger logger(LogLevel::WARN, "Main", &std::cout);

int main() {
    YAML::Node config = YAML::LoadFile("config.yaml");
    int numExpansions = config["numExpansions"].as<int>();
    double c = config["c"].as<double>();
    double alpha = config["alpha"].as<double>();
    int epochs = config["epochs"].as<int>();

    Board board;
    SearchTree tree(c);

    // Todo: make this a loop
    for (int i = 0; i < epochs; i++) {
        logger.info("Trial: " + std::to_string(i));
        std::shared_ptr<Node> selected = tree.select(&SearchTree::treePolicy);
        std::stack<Move> path = tree.traverse(selected);
        while (path.size()) {
            Move move = path.top();
            board.step(move);
            path.pop();
        }

        if (selected->expandable) {
            std::vector<Move> newMoves = board.getMoves();
            Move move;
            do {
                int index = random(0, newMoves.size() - 1);
                move = newMoves[index];
            } while (selected->exploredSet.contains(move));
            board.step(move);
            std::shared_ptr<Node> branch = tree.expand(selected, move);

            if (newMoves.size() == selected->exploredSet.size()) {
                selected->expandable = false;
            }

            bool simulate = true;
            if (board.getWinner() != Result::NONE) {
                branch->expandable = false;
                tree.backPropagate(branch, board.getScore(), alpha);
                simulate = false;
            }

            // the simulation
            for (int ii = 0; ii < numExpansions; ii++) {
                int counter = 0;

                while (board.getWinner() == Result::NONE) {
                    std::vector<Move> moves = board.getMoves();

                    // generate a random move
                    std::random_device device;
                    std::default_random_engine engine(device());
                    std::uniform_int_distribution<int> dist(0, moves.size() - 1);
                    int selected = dist(engine);
                    Move move = moves[selected];

                    board.step(move);
                    counter++;
                }

                tree.backPropagate(branch, board.getScore(), alpha);

                for (int iii = 0; iii < counter; iii++) {
                    board.undo();
                }
            }
        } else {
            selected->expandable = false;
            tree.backPropagate(selected, board.getScore(), alpha);
        }
        board.reset();
    }

    std::cout << board.toString() << std::endl;
    std::shared_ptr<Node> path = tree.select(&SearchTree::exploitPolicy);
    std::stack<Move> moves = tree.traverse(path);
    std::cout << moves.size();
    while (moves.size()) {
        Move move = moves.top();
        board.step(move);
        std::cout << board.toString() << std::endl;
        moves.pop();
    }

    std::cout << "Input anything to proceed to explorer" << std::endl;
    std::string input;
    std::cin >> input;

    Explorer explorer(&tree);
    explorer.run();
}