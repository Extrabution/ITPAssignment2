// Lecture 13 - Type casting
#include <iostream>
#include <memory>
#include <tuple>
#include <thread>
#include <chrono>
#include <vector>
#include <string>

using namespace std;
using namespace std::chrono_literals;

const int TIMEOUT = 400; // maximum number of milliseconds that a player is allowed to take

class Position{
public:
    std::tuple<int, int> pos;
    // TODO Implement the operator == and other operators if necessary
    Position& operator = (tuple<int,int> temp){
        this->pos = temp;
        return *this;
    }
    bool operator == (Position right) const{
        auto[il, jl] = pos;
        auto[ir, jr] = right.pos;
        if(il == ir && jl == jr)
            return true;
        else return false;
    }
};

class Unit{
public:
    string name, canKill;
    bool killed = false;
    Position position;
    Unit(string Name, string Kill){
        this->name = Name;
        this->canKill = Kill;
    }
};
class World {
public:
    string field[15][15];
    vector<shared_ptr<Unit>> player0;
    vector<shared_ptr<Unit>> player1;
    vector<shared_ptr<Unit>> mountains;
    vector<vector<shared_ptr<Unit>>> fieldPTR;
    Position currentLeader;
    World() {
        for (int i = 0; i < 15; i++) {
            vector<shared_ptr<Unit>> temp;
            for (int j = 0; j < 15; j++) {
                field[i][j] = " ";
                temp.push_back(nullptr);
            }
            fieldPTR.push_back(temp);
        }
        field[0][0] = "f";
        field[14][14] = "F";
        for (int i = 0; i < 6; i++) {
            for (int j = 1; j < 6; j++) {
                if (i == 0) {
                    field[i][j] = "r";
                    shared_ptr<Unit> temp(new Unit("r", "S"));
                    fieldPTR[i][j] = temp;
                    player0.push_back(temp);
                    player0.back()->position.pos = {i,j};
                    field[i + 14][j + 8] = "R";
                    shared_ptr<Unit> temp1(new Unit("r", "S"));
                    fieldPTR[i + 14][j+8] = temp1;
                    player1.push_back(temp1);
                    player1.back()->position.pos = {i+14,j+8};
                } else if (i == 3) {
                    field[i][j] = "r";
                    shared_ptr<Unit> temp(new Unit("r", "S"));
                    fieldPTR[i][j] = temp;
                    player0.push_back(temp);
                    player0.back()->position.pos = {i,j};
                    field[i + 8][j + 8] = "R";
                    shared_ptr<Unit> temp1(new Unit("r", "S"));
                    fieldPTR[i + 8][j+8] = temp1;
                    player1.push_back(temp1);
                    player1.back()->position.pos = {i+8,j+8};
                } else if (i == 1) {
                    field[i][j] = "p";
                    shared_ptr<Unit> temp(new Unit("p", "R"));
                    fieldPTR[i][j] = temp;
                    player0.push_back(temp);
                    player0.back()->position.pos = {i,j};
                    field[i + 12][j + 8] = "P";
                    shared_ptr<Unit> temp1(new Unit("p", "R"));
                    fieldPTR[i + 12][j+8] = temp1;
                    player1.push_back(temp1);
                    player1.back()->position.pos = {i+12,j+8};
                } else if (i == 4) {
                    field[i][j] = "p";
                    shared_ptr<Unit> temp(new Unit("p", "R"));
                    fieldPTR[i][j] = temp;
                    player0.push_back(temp);
                    player0.back()->position.pos = {i,j};
                    field[i + 6][j + 8] = "P";
                    shared_ptr<Unit> temp1(new Unit("P", "R"));
                    fieldPTR[i + 6][j+8] = temp1;
                    player1.push_back(temp1);
                    player1.back()->position.pos = {i+6,j+8};
                } else if (i == 2) {
                    field[i][j] = "s";
                    shared_ptr<Unit> temp(new Unit("s", "P"));
                    fieldPTR[i][j] = temp;
                    player0.push_back(temp);
                    player0.back()->position.pos = {i,j};
                    field[i + 10][j + 8] = "S";
                    shared_ptr<Unit> temp1(new Unit("r", "S"));
                    fieldPTR[i + 10][j+8] = temp1;
                    player1.push_back(temp1);
                    player1.back()->position.pos = {i+10,j+8};
                } else if (i == 5) {
                    field[i][j] = "s";
                    shared_ptr<Unit> temp(new Unit("s", "P"));
                    fieldPTR[i][j] = temp;
                    player0.push_back(temp);
                    player0.back()->position.pos = {i,j};
                    field[i + 4][j + 8] = "S";
                    shared_ptr<Unit> temp1(new Unit("S", "p"));
                    fieldPTR[i + 4][j+8] = temp1;
                    player1.push_back(temp1);
                    player1.back()->position.pos = {i+4,j+8};
                }
            }
        }
        player0.push_back(std::make_shared<Unit>("f",""));
        player0.back()->position.pos = {0,0};
        player1.push_back(std::make_shared<Unit>("F", ""));
        player1.back()->position.pos = {14,14};
        currentLeader = fieldPTR[13][9]->position;
        //Vector of Mountains coordinates for easier implementation;
        vector<tuple<int,int>> MPosition = {{2,10},{2,12},{4,9},{4,13},
                                            {5,10},{5,11},{5,12},{7,6},
                                            {7,7},{7,8},{10,2},{11,2},
                                            {12,2},{10,4},{11,4},{12,4},
                                            {12,5},{10,6},{11,6},{12,6}};
        for(auto elem:MPosition){
            mountains.push_back(std::make_shared<Unit>("M", "Non-Movable"));
            mountains.back()->position.pos = elem;
            auto[i, j] = elem;
            field[i][j] = "M";
        }
    }
    void print() {
        for (auto & i : field) {
            for (auto & j : i) {
                cout << j << " ";
            }
            cout << endl;
        }
    }
};

class Action {
public:
    Position from; // current row, column of the unit to be moved
    Position to; // position to where the unit must be moved
    Action& operator = (Action temp){
        this->from = temp.from;
        this->to = temp.to;
        return *this;
    }
};

Action actionPlayerZero(const World world) {
    // 0 -left, 1 - up, 2 - right, 3 - down;
    while(true){
        cout<<170;
        int position = rand() % 4;
        shared_ptr<Unit> unit = world.player0[rand() % world.player0.size()];
        auto[i,j] = unit->position.pos;
        Action action;
        if(world.field[i][j] == "f")
            continue;
        else if(position == 0 && j - 1 != -1 && (world.field[i][j-1] == " " || world.field[i][j-1] == unit->canKill)) {
            action.from = unit->position.pos;
            action.to = {i, j - 1};
            unit->killed = true;
            return action;
        }
        else if(position == 1 && i -1 != -1 && (world.field[i-1][j] == " " || world.field[i-1][j] == unit->canKill)){
            action.from = unit->position.pos;
            action.to = {i - 1, j};
            unit->killed = true;
            return action;
        }
        else if(position == 2 && j + 1 != 15 && (world.field[i][j+1] == " " || world.field[i][j+1] == unit->canKill)){
            action.from = unit->position.pos;
            action.to = {i, j + 1};
            unit->killed = true;
            return action;
        }
        else if(position == 3 && i + 1 != 15 && (world.field[i+1][j] == " " || world.field[i+1][j] == unit->canKill)){
            action.from = unit->position.pos;
            action.to = {i + 1, j};
            unit->killed = true;
            return action;
        }
        else if(j + 1!= 15 &&(world.field[i][j+1] == " " || world.field[i][j+1] == unit->canKill)){ // right
            action.from = unit->position.pos;
            action.to = {i, j + 1};
            unit->killed = true;
            return action;
        }
        else if(j - 1 != -1 && (world.field[i][j-1] == " " || world.field[i][j-1] == unit->canKill )){ // left
            action.from = unit->position.pos;
            action.to = {i, j - 1};
            unit->killed = true;
            return action;
        }
        else if(i + 1 != 15 && (world.field[i+1][j] == " " || world.field[i+1][j] == unit->canKill )){ // down
            action.from = unit->position.pos;
            action.to = {i + 1, j};
            unit->killed = true;
            return action;
        }
        else if(i - 1 != -1 && (world.field[i-1][j] == " " || world.field[i-1][j] == unit->canKill )){ // up
            action.from = unit->position.pos;
            unit->killed = true;
            action.to = {i - 1, j };
            return action;
        }
    }
}

Action actionPlayerOne(const World world) {
    // TODO Implement some basic strategy and remove the lines below
    auto[i,j] = world.currentLeader.pos;
    Position temp;
    temp = {-1,-1};
    if(world.currentLeader == temp){

    }
    else{
        world.currentLeader.pos = world.player1.back()->position.pos;
    }
    std::cout << "actionPlayerOne()" << endl;
    std::this_thread::sleep_for(385ms); // 0.4 seconds
}

/**
 * The return is a pair: action and a boolean whether a timeout happened
 */
std::tuple<Action, bool> waitPlayer(Action (*f)(World), World world) {
    auto start = std::chrono::high_resolution_clock::now();
    Action action = f(world);
    cout<<238;
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
    bool endGame = false;
    int ik = 0;
    while (ik !=1000) {
        string d;
        cin>>d;
        ik++;
        cout<<endl<<ik<<endl;
        auto[action0, timeout0] = waitPlayer(actionPlayerZero, world);
        cout<<256;
        auto[iFrom0,jFrom0] = action0.from.pos;
        auto[iTo0,jTo0] = action0.to.pos;
        auto unit = world.fieldPTR[iFrom0][jFrom0];
        cout<<259;
        if(unit->killed){
            world.field[iTo0][jTo0] = " ";
            world.fieldPTR[iTo0][jTo0] = nullptr;
            Position temp;
            int i = 0;
            for(auto it = world.player1.begin(); it != world.player1.end(); it++, i++) {
                if (world.player1[i]->position == action0.to){
                    world.player1.erase(it);
                    break;
                }
            }
        }
        cout<<278;
        auto[action1, timeout1] = waitPlayer(actionPlayerOne, world);
        if (timeout0 || timeout1) {
            endGame = true;
            if(timeout0)
                cout<<endl<<"PLAYER 0 TIMED OUT!"<<endl;
            else cout<<endl<<"PLAYER 1 TIMED OUT!"<<endl;
            world.print();
        } else {
            // TODO Validate the actions: actionPlayer0 and actionPlayer1

            // TODO Update the world
            cout<<282;
            world.field[iTo0][jTo0] = world.field[iFrom0][jFrom0];
            world.field[iFrom0][jFrom0] = " ";
            int i = 0;
            for(auto it = world.player0.begin(); it != world.player0.end(); it++, i++){
                if( world.player0[i]->position == action0.from)
                    world.player0[i]->position = action0.to;
            }
            world.fieldPTR[iFrom0][jFrom0]->position = action0.to;
            world.fieldPTR[iTo0][jTo0] = world.fieldPTR[iFrom0][jFrom0];
            world.fieldPTR[iFrom0][jFrom0] = nullptr;
            // TODO Show the world
            world.print();
            // TODO End the game and show the result if a player won
        }
    }
    return 0;
}