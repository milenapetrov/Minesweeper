#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include <vector>

using namespace std;

class Tile {

public:
    bool isMine = false;
    bool isExposed = false;
    bool isFlag = false;
    vector<Tile*> adjTiles;
    int ind = 0;

    int x;
    int y;

    Tile(){}
    Tile(int x, int y);
    void reset();
};



