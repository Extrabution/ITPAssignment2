// Lecture 13 - Type casting
#include <iostream>
#include <tuple>
#include <thread>
#include <chrono>
#include <vector>

using namespace std;
using namespace std::chrono_literals;

const int TIMEOUT = 400; // maximum number of milliseconds that a player is allowed to take
class Symbol{
public:
    string name;
    Symbol(string name){
        this->name = name;
    }
};

class World {
public:
    // TODO Create a container of smart pointers of units and flags of player 0
    string field[15][15];
    World() {
        field[0][0] = "f";
        field[14][14] = "F";
        for (int i = 0; i < 6; i++) {
            for (int j = 1; j < 6; j++) {
                if (i == 0) {
                    field[i][j] = "r";
                    field[i + 14][j + 7] = "R";
                } else if (i == 3) {
                    field[i][j] = "r";
                    field[i + 8][j + 7] = "R";
                } else if (i == 1) {
                    field[i][j] = "p";
                    field[i + 12][j + 7] = "P";
                } else if (i == 4) {
                    field[i][j] = "p";
                    field[i + 6][j + 7] = "P";
                } else if (i == 2) {
                    field[i][j] = "s";
                    field[i + 10][j + 7] = "S";
                } else if (i == 5) {
                    field[i][j] = "s";
                    field[i + 4][j + 7] = "S";
                }
            }
        }
    }
    void print() {
        for (int i = 0; i < 15; i++) {
            for (int j = 0; j < 15; j++) {
                cout << field[i][j];
                if(j != 13)
                    cout<<" ";
            }
            cout << endl;
        }
    }
    // TODO Create a container of smart pointers of units and flags of player 1
    // TODO Create a container of smart pointers of mountains
};

class Position{
public:
    std::tuple<int, int> pos;
    // TODO Implement the operator == and other operators if necessary
};

class Action {
    Position from; // current row, column of the unit to be moved
    Position to; // position to where the unit must be moved
};

Action actionPlayerZero(const World world) {
    // TODO Implement some basic strategy and remove the lines below
    std::cout << "actionPlayerZero()" << endl;
    std::this_thread::sleep_for(385ms); // 0.4 seconds
}

Action actionPlayerOne(const World world) {
    // TODO Implement some basic strategy and remove the lines below
    std::cout << "actionPlayerOne()" << endl;
    std::this_thread::sleep_for(385ms); // 0.4 seconds
}

/**
 * The return is a pair: action and a boolean whether a timeout happened
 */
std::tuple<Action, bool> waitPlayer(Action (*f)(World), World world) {
    auto start = std::chrono::high_resolution_clock::now();
    // TODO Important. Below, action should be the result of calling the function passed as parameter
    Action action;
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    // TODO This line can be removed
    std::cout << "Waited " << elapsed.count() << " ms\n";
    if (elapsed.count() > TIMEOUT) // if time > 0.3 s
        return {action, true}; // player failed to answer in less than 400 ms
    else return {action, false};
}

int main() {
    World world;
    world.print();
    // TODO Initialize the world
    bool endGame = false;
    while (endGame) {
        auto[action0, timeout0] = waitPlayer(actionPlayerZero, world);
        auto[action1, timeout1] = waitPlayer(actionPlayerOne, world);
        if (timeout0 || timeout1) {
            endGame = true;
            // TODO Show the result of the game. Who caused the timeout?
        } else {
            // TODO Validate the actions: actionPlayer0 and actionPlayer1
            // TODO Update the world
            // TODO Show the world
            // TODO End the game and show the result if a player won
        }
    }
    return 0;
}