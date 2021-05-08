// Lecture 13 - Type casting
/**
    ITP Final. Variant of new function number 3
    @file Inchin_Ivan.cpp
    @author Inchin Ivan BS20-05

*/
#include <iostream>
#include <memory>
#include <tuple>
#include <thread>
#include <chrono>
#include <vector>
#include <string>
#include <ctime>
#include <algorithm>


using namespace std;
using namespace std::chrono_literals;

const int TIMEOUT = 400; // maximum number of milliseconds that a player is allowed to take

//ITEM 3.a Pure virtual function and their further implementation
//ITEM 3.g Non-copyable object count
class Controller{
public:
    virtual void const print(int size) = 0;
private:
    int counterOfMs = 400;
};
/**
* Object position.
 * Contains tuple with coordinates
 * Operators ==, !=, =, +, > are overloaded.
*
*/
//Item 3.c =, +, > are overloaded for easier usage of class
class Position{
public:
    std::tuple<int, int> pos;
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
    bool operator != (Position right) const{
        auto[il, jl] = pos;
        auto[ir, jr] = right.pos;
        if(il != ir || jl != jr)
            return true;
        else return false;
    }
    Position& operator + (Position temp){
        auto[i,j] = this->pos;
        auto[n,m] = temp.pos;
        this->pos = {i+n, j+m};
        return *this;
    }
    bool operator > (Position temp) const{
        auto[i,j] = this->pos;
        auto[n,m] = temp.pos;
        if(i > n && j > m)
            return true;
        else return false;
    }
};

/**
* Global variables. Provides fast access to them in some functions without copying them
*/
Position currentLeader;
Position nextMove;
Position spawner0, spawner1;
bool mount = false, c0 = true, c1 = false;
bool smallFlag = false;

/**
* Object Unit.
 * Contains field name(name of the symbol) and name of the
 * symbol that it can kill
*
*/
//ITEM 3.k Template to make code more generic
//ITEM 3.b Constructors and destructure for usefullness
template<class T>class Unit{
public:
    string name, canKill;
    bool killed = false;
    T position;
    Unit(string Name, string Kill){
        this->name = Name;
        this->canKill = Kill;
    }
    Unit(){
        this->name = " ";
        this->canKill = "Non-Movable";
    }
    Unit(const Unit& unit) : name(unit.name), canKill(unit.canKill){}
    Unit(Unit&& unit) noexcept : name(move(unit.name)), canKill(move(unit.canKill)){}
    ~Unit(){}
};

/**
* Object World.
 * Contains containers for Units, using smart pointers.
 * Constructor creates all fields and fill it with symbols.
*
*/
//ITEM 3.k Usage of vector container.
//ITEM 3.d Static and dynamic typing(vectors and pointers)
class World : public Controller{
public:
    string field[15][15];
    vector<shared_ptr<Unit<Position>>> player0;
    vector<shared_ptr<Unit<Position>>> player1;
    vector<shared_ptr<Unit<Position>>> mountains;
    vector<vector<shared_ptr<Unit<Position>>>> fieldPTR;
    World() {
        for (auto & i : field) {
            vector<shared_ptr<Unit<Position>>> temp;
            for (auto & j : i) {
                j = " ";
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
                    shared_ptr<Unit<Position>> temp(new Unit<Position>("r", "S"));
                    fieldPTR[i][j] = temp;
                    player0.push_back(temp);
                    player0.back()->position.pos = {i,j};
                    field[i + 14][j + 8] = "R";
                    shared_ptr<Unit<Position>> temp1(new Unit<Position>("R", "s"));
                    fieldPTR[i + 14][j+8] = temp1;
                    player1.push_back(temp1);
                    player1.back()->position.pos = {i+14,j+8};
                } else if (i == 3) {
                    field[i][j] = "r";
                    shared_ptr<Unit<Position>> temp(new Unit<Position>("r", "S"));
                    fieldPTR[i][j] = temp;
                    player0.push_back(temp);
                    player0.back()->position.pos = {i,j};
                    field[i + 8][j + 8] = "R";
                    shared_ptr<Unit<Position>> temp1(new Unit<Position>("R", "s"));
                    fieldPTR[i + 8][j+8] = temp1;
                    player1.push_back(temp1);
                    player1.back()->position.pos = {i+8,j+8};
                } else if (i == 1) {
                    field[i][j] = "p";
                    shared_ptr<Unit<Position>> temp(new Unit<Position>("p", "R"));
                    fieldPTR[i][j] = temp;
                    player0.push_back(temp);
                    player0.back()->position.pos = {i,j};
                    field[i + 12][j + 8] = "P";
                    shared_ptr<Unit<Position>> temp1(new Unit<Position>("P", "r"));
                    fieldPTR[i + 12][j+8] = temp1;
                    player1.push_back(temp1);
                    player1.back()->position.pos = {i+12,j+8};
                } else if (i == 4) {
                    field[i][j] = "p";
                    shared_ptr<Unit<Position>> temp(new Unit<Position>("p", "R"));
                    fieldPTR[i][j] = temp;
                    player0.push_back(temp);
                    player0.back()->position.pos = {i,j};
                    field[i + 6][j + 8] = "P";
                    shared_ptr<Unit<Position>> temp1(new Unit<Position>("P", "r"));
                    fieldPTR[i + 6][j+8] = temp1;
                    player1.push_back(temp1);
                    player1.back()->position.pos = {i+6,j+8};
                } else if (i == 2) {
                    field[i][j] = "s";
                    shared_ptr<Unit<Position>> temp(new Unit<Position>("s", "P"));
                    fieldPTR[i][j] = temp;
                    player0.push_back(temp);
                    player0.back()->position.pos = {i,j};
                    field[i + 10][j + 8] = "S";
                    shared_ptr<Unit<Position>> temp1(new Unit<Position>("S", "p"));
                    fieldPTR[i + 10][j+8] = temp1;
                    player1.push_back(temp1);
                    player1.back()->position.pos = {i+10,j+8};
                } else if (i == 5) {
                    field[i][j] = "s";
                    shared_ptr<Unit<Position>> temp(new Unit<Position>("s", "P"));
                    fieldPTR[i][j] = temp;
                    player0.push_back(temp);
                    player0.back()->position.pos = {i,j};
                    field[i + 4][j + 8] = "S";
                    shared_ptr<Unit<Position>> temp1(new Unit<Position>("S", "p"));
                    fieldPTR[i + 4][j+8] = temp1;
                    player1.push_back(temp1);
                    player1.back()->position.pos = {i+4,j+8};
                }
            }
        }
        player0.push_back(std::make_shared<Unit<Position>>("f",""));
        player0.back()->position.pos = {0,0};
        player1.push_back(std::make_shared<Unit<Position>>("F", ""));
        player1.back()->position.pos = {14,14};
        currentLeader = fieldPTR[13][9]->position.pos;
        player0.push_back((std::make_shared<Unit<Position>>("c","")));
        player1.push_back((std::make_shared<Unit<Position>>("C","")));
        fieldPTR[1][6] = std::make_shared<Unit<Position>>("c","");
        fieldPTR[1][6]->position = {1,6};
        fieldPTR[8][13] = std::make_shared<Unit<Position>>("C","");
        fieldPTR[8][13]->position = {8,13};
        player1.back()->position = {8,13};
        player0.back()->position = {1,6};
        field[1][6] = "c";
        field[8][13] = "C";
        spawner0 = fieldPTR[1][6]->position.pos;
        spawner1 = fieldPTR[8][13]->position.pos;
        //Vector of Mountains coordinates for easier implementation;
        vector<tuple<int,int>> MPosition = {{2,10},{2,12},{4,9},{4,13},
                                            {5,10},{5,11},{5,12},{7,6},
                                            {7,7},{7,8},{10,2},{11,2},
                                            {12,2},{10,4},{11,4},{12,4},
                                            {12,5},{10,6},{11,6},{12,6}};
        for(auto elem:MPosition){
            mountains.push_back(std::make_shared<Unit<Position>>("M", "Non-Movable"));
            mountains.back()->position.pos = elem;
            auto[i, j] = elem;
            field[i][j] = "M";
        }
    }
    /**
 * Printing out all elements on a field
 *
 * @param const size of world
 * @return empty
 */
    //ITEM 3.f call to a const function
    void const print(int size) override{
        for (auto & i : field) {
            for (auto & j : i) {
                cout << j << " ";
            }
            cout << endl;
        }
    }
};

/**
* Object Action.
 * Contains old and new positions of symbol.
 * Operator = overloaded.
*
*/
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

/**
* Spawn 1 random unit around c
*
* @param reference on object World
* @return empty
*/
//ITEM 3.1.3.a Spawns units
//ITEM 3.1.3.d Spawns no more than 40 in total for each player
void Spawn0(World &world){
    // 0 - p, 1 -r, 2 -s
    Position posit;
    posit = {-1, -1};
    if(world.player0.size()<40 && spawner0 != posit) {
        auto[n, m] = spawner0.pos;
        int symbol = rand() % 3;
        string symbolString, toKill;
        if (symbol == 0) {
            symbolString = "p";
            toKill = "R";
        } else if (symbol == 1) {
            symbolString = "r";
            toKill = "S";
        } else {
            symbolString = "s";
            toKill = "P";
        }
        if (world.field[n - 1][m] == " ") {
            world.field[n - 1][m] = symbolString;
            world.fieldPTR[n - 1][m] = std::make_shared<Unit<Position>>(symbolString, toKill);
            world.player0.push_back(std::make_shared<Unit<Position>>(symbolString, toKill));
            world.player0.back()->position = {n-1, m};
        }
        else if (world.field[n + 1][m] == " ") {
            world.field[n + 1][m] = symbolString;
            world.fieldPTR[n + 1][m] = std::make_shared<Unit<Position>>(symbolString, toKill);
            world.player0.push_back(std::make_shared<Unit<Position>>(symbolString, toKill));
            world.player0.back()->position = {n+1, m};
        }
        else if (world.field[n][m+1] == " ") {
            world.field[n][m + 1] = symbolString;
            world.fieldPTR[n][m + 1] = std::make_shared<Unit<Position>>(symbolString, toKill);
            world.player0.push_back(std::make_shared<Unit<Position>>(symbolString, toKill));
            world.player0.back()->position = {n, m + 1};
        }
        else if (world.field[n][m-1] == " ") {
            world.field[n][m - 1] = symbolString;
            world.fieldPTR[n][m - 1] = std::make_shared<Unit<Position>>(symbolString, toKill);
            world.player0.push_back(std::make_shared<Unit<Position>>(symbolString, toKill));
            world.player0.back()->position = {n, m - 1};
        }
    }
 }
/**
* Spawn 1 random unit around C
*
* @param reference on object World
* @return empty
*/
void Spawn1(World &world){
    // 0 - p, 1 -r, 2 -s
    Position posit;
    posit = {-1,-1};
    if(world.player1.size()<40 && spawner1 != posit) {
        auto[n, m] = spawner1.pos;
        int symbol = rand() % 3;
        string symbolString, toKill;
        if (symbol == 0) {
            symbolString = "P";
            toKill = "r";
        } else if (symbol == 1) {
            symbolString = "R";
            toKill = "s";
        } else {
            symbolString = "S";
            toKill = "p";
        }
        if (world.field[n - 1][m] == " ") {
            world.field[n - 1][m] = symbolString;
            world.fieldPTR[n - 1][m] = std::make_shared<Unit<Position>>(symbolString, toKill);
            world.player1.push_back(std::make_shared<Unit<Position>>(symbolString, toKill));
            world.player1.back()->position = {n-1, m};
        }
        else if (world.field[n + 1][m] == " ") {
            world.field[n + 1][m] = symbolString;
            world.fieldPTR[n + 1][m] = std::make_shared<Unit<Position>>(symbolString, toKill);
            world.player1.push_back(std::make_shared<Unit<Position>>(symbolString, toKill));
            world.player1.back()->position = {n+1, m};
        }
        else if (world.field[n][m - 1] == " ") {
            world.field[n][m - 1] = symbolString;
            world.fieldPTR[n][m - 1] = std::make_shared<Unit<Position>>(symbolString, toKill);
            world.player1.push_back(std::make_shared<Unit<Position>>(symbolString, toKill));
            world.player1.back()->position = {n, m - 1};
        }
        else if (world.field[n][m+1] == " ") {
            world.field[n][m + 1] = symbolString;
            world.fieldPTR[n][m + 1] = std::make_shared<Unit<Position>>(symbolString, toKill);
            world.player1.push_back(std::make_shared<Unit<Position>>(symbolString, toKill));
            world.player1.back()->position = { n, m + 1};
        }
    }
}
/**
 * Deletes spawner   and 5 random units around it
 *
 * @param reference on object World
 * @return empty
 */
//ITEM 3.j Using predicate function for erasing Spawner c
//ITEM 3.1.3.b If Spawner is destoyed, 5 units die
void Despawn0(World &world){
    auto it = find_if(world.player0.begin(), world.player0.end(), [](auto value){return value.get()->name == "c";});
    auto [k,l] = it->get()->position.pos;
    world.field[k][l] = " ";
    world.fieldPTR[k][l] = nullptr;
    world.player0.erase(it);
    spawner0 = {-1,-1};
    c0 = false;
    for(int i = 0; i < 5; i++){
        int pos = rand() % world.player0.size();
        auto[n,m] = world.player0[pos]->position.pos;
        world.player0.erase(world.player0.begin() + pos);
        world.field[n][m] = " ";
        world.fieldPTR[n][m] = nullptr;
    }
}
/**
 * Deletes spawner C and 5 random units around it
 *
 * @param reference on object World
 * @return empty
 */
//ITEM 3.j Using predicate function for erasing Spawner C
void Despawn1(World &world){
    auto it = find_if(world.player1.begin(), world.player1.end(), [](auto value){return value.get()->name == "C";});
    auto [k,l] = it->get()->position.pos;
    world.field[k][l] = " ";
    world.fieldPTR[k][l] = nullptr;
    world.player1.erase(it);
    spawner1 = {-1,-1};
    c1 = false;
    for(int i = 0; i < 5; i++){
        int pos = rand() % world.player1.size();
        if(world.player1[pos]->position != currentLeader) {
            auto[n,m] = world.player1[pos]->position.pos;
            world.player1.erase(world.player1.begin() + pos);
            world.field[n][m] = " ";
            world.fieldPTR[n][m] = nullptr;
        }
    }
}
/**
* Makes random move for player 0
*
* @param const object World
* @return object Action for player 0
*/
//ITEM 3.i Decltype is used for guessing a return type of a function
//ITEM 3.1.3.c Spawner can move
decltype(auto) actionPlayerZero(const World world) {
    // 0 -left, 1 - up, 2 - right, 3 - down;
    while(true){
        int position = rand() % 4;
        shared_ptr<Unit<Position>> unit = world.player0[rand() % world.player0.size()];
        auto[i, j] = unit->position.pos;
        Action action;
        if(j == 1 && i !=3 || world.field[i][j] == "f"){}
        else if(position == 0 && j - 1 != -1 && (world.field[i][j-1] == " " ||
        world.field[i][j-1] == unit->canKill || world.field[i][j-1] == "M" || world.field[i][j-1] == "C" )) {
            if(world.field[i][j-1] == "M")
                mount = true;
            if(world.field[i][j] == "c")
                spawner0 = {i,j - 1};
            if(world.field[i][j-1] == "C")
                c1 = true;
            action.from = unit->position.pos;
            action.to = {i, j - 1};
            return action;
        }
        else if(position == 1 && i - 1 != -1 && (world.field[i-1][j] == " "
        || world.field[i-1][j] == unit->canKill || world.field[i-1][j] == "M")){ ///////////
            if(world.field[i-1][j] == "M")
                mount = true;
            if(world.field[i][j] == "c")
                spawner0 = {i-1, j};
            if(world.field[i-1][j] == "C")
                c1 = true;
            action.from = unit->position.pos;
            action.to = {i - 1, j};
            //cout<<endl<<i<<" "<<j<<i-1<<" "<<j<<endl;
            return action;
        }
        else if(position == 2 && j + 1 != 15 && (world.field[i][j+1] == " "
        || world.field[i][j+1] == unit->canKill || world.field[i][j+1] == "M")){
            if(world.field[i][j+1] == "M")
                mount = true;
            if(world.field[i][j] == "c")
                spawner0 = {i,j + 1};
            if(world.field[i][j+1] == "C")
                c1 = true;
            action.from = unit->position.pos;
            action.to = {i, j + 1};
            return action;
        }
        else if(position == 3 && i + 1 != 15 && (world.field[i+1][j] == " "
        || world.field[i+1][j] == unit->canKill || world.field[i+1][j] == "M")){
            if(world.field[i+1][j] == "M")
                mount = true;
            if(world.field[i][j] == "c")
                spawner0 = {i + 1,j};
            if(world.field[i+1][j] == "C")
                c1 = true;
            action.from = unit->position.pos;
            action.to = {i + 1, j};
            return action;
        }
        else if(j + 1!= 15 &&(world.field[i][j+1] == " "
        || world.field[i][j+1] == unit->canKill || world.field[i][j+1] == "M")){ // right
            if(world.field[i][j+1] == "M")
                mount = true;
            if(world.field[i][j] == "c")
                spawner0 = {i,j + 1};
            if(world.field[i][j+1] == "C")
                c1 = true;
            action.from = unit->position.pos;
            action.to = {i, j + 1};
            return action;
        }
        else if(j - 1 != -1 && (world.field[i][j-1] == " "
        || world.field[i][j-1] == unit->canKill || world.field[i][j-1] == "M")){ // left
            if(world.field[i][j-1] == "M")
                mount = true;
            if(world.field[i][j] == "c")
                spawner0 = {i, j - 1};
            if(world.field[i][j-1] == "C")
                c1 = true;
            action.from = unit->position.pos;
            action.to = {i, j - 1};
            return action;
        }
        else if(i + 1 != 15 && (world.field[i+1][j] == " "
        || world.field[i+1][j] == unit->canKill || world.field[i+1][j] == "M")){ // down
            if(world.field[i+1][j] == "M")
                mount = true;
            if(world.field[i][j] == "c")
                spawner0 = {i + 1,j};
            if(world.field[i+1][j] == "C")
                c1 = true;
            action.from = unit->position.pos;
            action.to = {i + 1, j};
            return action;
        }
        else if(i - 1 != -1 && (world.field[i-1][j] == " "
        || world.field[i-1][j] == unit->canKill || world.field[i-1][j] == "M")){ // up
            if(world.field[i-1][j] == "M")
                mount = true;
            if(world.field[i][j] == "c")
                spawner0 = {i - 1,j};
            if(world.field[i-1][j] == "C")
                c1 = true;
            action.from = unit->position.pos;
            action.to = {i - 1, j };
            return action;
        }
    }
}

/**
 * Makes wise move for player 1
 *
 * @param const object World
 * @return object Action for player 1
 */
decltype(auto) actionPlayerOne(const World world) {
    Action action;
    auto[i,j] = currentLeader.pos;
    Position temp;
    temp = {-1,-1};
    if(currentLeader == temp){
        currentLeader = world.player1.front()->position.pos;
    }
    if(world.field[i-1][j] == "f"){
        smallFlag = true;
        action.from = currentLeader.pos;
        currentLeader = {i-1, j};
        action.to = {i-1, j};
        return action;
    }
    if(j - 1 != -1 && (world.field[i][j-1] == " " || world.field[i][j-1] == world.fieldPTR[i][j]->canKill )){
        action.from = currentLeader.pos;
        currentLeader = {i, j-1};
        action.to = {i, j-1};
        return action;
    }
    else if(j - 1 == -1 || world.field[i][j-1] == "M"){
        action.from = currentLeader.pos;
        currentLeader = {i-1, j};
        action.to = {i-1, j};
        return action;
    }
    else if(world.field[i-1][j] != " "){
        action.from = currentLeader.pos;
        currentLeader = {i,j+1};
        action.to = {i,j+1};
        return action;
    }
}

/**
 * Calls a function for move for player 0,1. Counts their executing time.
 *
 * @param  reference on the function, object World
 * @return tuple with object action for player + true if time < 0.4s, false otherwise.
 */
//Item 3.h Explicit specialization in function waitPlayer
template<int Timeout> std::tuple<Action, bool> waitPlayer(Action (*f)(World), World world) {
    auto start = std::chrono::high_resolution_clock::now();
    Action action = f(world);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    if (elapsed.count() > Timeout) // if time > 0.3 s
        return {action, true}; // player failed to answer in less than 400 ms
    else return {action, false};
}

/**
 * Initial point.
 * Controls and updates the world, makes a moves.
 *
 * @param empty
 * @return 0 for correct execution of program
 */
int main() {
    World world;
    const int sizeOfWorld = 15;
    world.print(sizeOfWorld);
    srand(time(nullptr));
    int k1 = 11 + rand() % 20;
    bool endGame = false;
    int ik = 0;
    while (!endGame) {
        string d;
        //cin>>d;
        ik++;
        //Item 3.h Explicit specialization in function waitPlayer
        auto[action0, timeout0] = waitPlayer<400>(actionPlayerZero, world);
        auto[iFrom0, jFrom0] = action0.from.pos;
        auto[iTo0, jTo0] = action0.to.pos;
        int k2 = k1 + 3 + rand() % 13;
        auto unit = world.fieldPTR[iFrom0][jFrom0];
        int i = 0;
        for (auto it = world.player1.begin(); it != world.player1.end(); it++, i++) {
            if (world.player1[i]->position == action0.to) {
                if (world.player1[i]->position == currentLeader)
                    currentLeader = {-1, -1};
                world.player1.erase(it);
                world.field[iTo0][jTo0] = " ";
                world.fieldPTR[iTo0][jTo0] = nullptr;
                break;
            }
        }
        auto[action1, timeout1] = waitPlayer<400>(actionPlayerOne, world);
        auto[iFrom1, jFrom1] = action1.from.pos;
        auto[iTo1, jTo1] = action1.to.pos;
        unit = world.fieldPTR[iFrom1][jFrom1];
        i = 0;
        for (auto it = world.player0.begin(); it != world.player0.end(); it++, i++) {
            if (world.player0[i]->position == action1.to) {
                world.player0.erase(it);
                world.field[iTo1][jTo1] = " ";
                world.fieldPTR[iTo1][jTo1] = nullptr;
                break;
            }
        }
        if (timeout0 || timeout1) {
            endGame = true;
            if (timeout0)
                cout << endl << "PLAYER 0 TIMED OUT!" << endl;
            else cout << endl << "PLAYER 1 TIMED OUT!" << endl;
            // ITEM 3.d Score for players 0 and 1 (In assignment)
            cout << "Score:" << k1 << ":" << k2 << endl;
            world.print(sizeOfWorld);
        } else if (mount) {
            cout << endl << "PLAYER 0 Crashed into the mountain!" << endl;
            cout << "Score:" << k1 << ":" << k2 << endl;
            endGame = true;
            world.print(sizeOfWorld);
        } else {
            world.field[iTo0][jTo0] = world.field[iFrom0][jFrom0];
            world.field[iFrom0][jFrom0] = " ";

            world.field[iTo1][jTo1] = world.field[iFrom1][jFrom1];
            world.field[iFrom1][jFrom1] = " ";
            int i = 0;
            for (auto it = world.player0.begin(); it != world.player0.end(); it++, i++) {
                if (world.player0[i]->position == action0.from) {
                    world.player0[i]->position = action0.to;
                    break;
                }
            }
            world.fieldPTR[iFrom0][jFrom0]->position = action0.to;
            world.fieldPTR[iTo0][jTo0] = world.fieldPTR[iFrom0][jFrom0];
            world.fieldPTR[iFrom0][jFrom0] = nullptr;
            i = 0;
            for (auto it = world.player1.begin(); it != world.player1.end(); it++, i++) {
                if (world.player1[i]->position == action1.from) {
                    world.player1[i]->position = action1.to;
                    break;
                }
            }
            world.fieldPTR[iFrom1][jFrom1]->position = action1.to;
            world.fieldPTR[iTo1][jTo1] = world.fieldPTR[iFrom1][jFrom1];
            world.fieldPTR[iFrom1][jFrom1] = nullptr;
            if (c1)
                Despawn1(world);
            if (c0) {
                Despawn0(world);
            }
            Spawn0(world);
            Spawn1(world);
            if (smallFlag) {
                endGame = true;
                cout << endl << "PLAYER 1 CAPTURED THE FLAG!" << endl;
                cout << "Score:" << k1 << ":" << k2 << endl;
            }
            world.print(sizeOfWorld);
        }
    }
    return 0;
}