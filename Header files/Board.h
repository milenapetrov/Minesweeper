//
// Created by Milena Petrovic on 4/12/22.
//

#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include "TextureMang.h"
#include "Tile.h"
#include <array>
#include <random>
#include <ctime>
using namespace std;
using namespace sf;


class Board {

    int columns;
    int rows;
    int mines;

public:

    Tile* allTiles;
    vector<string> testColumns;
    map<string, Sprite> allSprites;
    bool gameOver = false;
    int flgCount = 0;
    int hiddenTiles = 0;


    Board();
    const void DrawTiles(RenderWindow &window, bool isUI);

    int RowColInd(int r, int c) const;
    void grid(string path);
    void testGrid(string path);
    void initConfigTiles(map<string, Sprite> Sprites);
    void initTestTiles(map<string, Sprite> Sprites);
    void tilesVicinity(Tile &tile, int index) const;
    int minesVicinity(int index);


    int GetCol();
    int GetRow();
    int GetMines();
    string GetFlags();

    void ExposeTiles(int index);
    bool ExposeAround(vector<int> t);

    void Reset();


};



