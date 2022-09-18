

#include "Board.h"
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include <iostream>
#include <fstream>
#include <random>

using namespace std;

void Board::grid(string name ){ //string name
    ifstream file(name);
    int cols, rows, mines;
    if(file.is_open()){
        string temp;

        getline(file,temp);
        cols = stoi(temp);
        getline(file, temp);
        rows = stoi(temp);
        getline(file, temp);
        mines = stoi(temp);
    }
    Reset();
    columns = cols;
    this->rows = rows;
    this->mines = mines;

    allTiles = new Tile[columns * rows];
    mt19937 rand;
    rand.seed(time(0));
    uniform_int_distribution<int> dist(0, columns*rows);
    for(int i = 0; i < mines; i++){
        if(!allTiles[dist(rand)].isMine) {
            allTiles[dist(rand)].isMine = true;
        }
    }
    file.close();
}

void Board::testGrid(string path) {
    ifstream file(path);

    if (file.is_open()) {
        string temp;
        for (int i = 0; i < 16; i++) {
            getline(file, temp);
            testColumns.push_back(temp);
        }
    }
    Reset();
    rows = testColumns.size();
    columns = testColumns[0].length();
    allTiles = new Tile[columns*rows];

    for(int r = 0; r < rows; r++){
        for(int c = 0; c < testColumns[r].length(); c++){
            if(testColumns[r].at(c) == '1'){
                mines++;
            }
        }
    }

    file.close();
}

int Board::GetCol() {
    return columns;
}

int Board::GetRow(){
    return rows;
}

int Board::GetMines(){
    return mines;
}

string Board::GetFlags(){

    return to_string((flgCount/3)+1);
}

void Board::Reset() {
        for (int i = 0; i < rows * columns; i++) {
            allTiles[i].reset();
        }
        mines = 0;
        columns = 0;
        rows = 0;
        flgCount = 0;
        hiddenTiles = 0;
        gameOver = false;
}

void Board::initTestTiles(map<string, Sprite> Sprites) {
    allSprites = Sprites;
    int index = 0;
    for(int r = 0; r < rows; r++){
        for(int c = 0; c < columns; c++){
            Tile newTile(c*32, r*32);
            if(testColumns[r].at(c) == '1'){
                newTile.isMine = true;
            }
            tilesVicinity(newTile, index);
            allTiles[index] = newTile;
            allTiles[index].ind = index;
            hiddenTiles++;
            index++;
        }
    }
}

void Board::initConfigTiles(map<string, Sprite> Sprites) {
    allSprites = Sprites;
    int index = 0;
    for(int r = 0; r < rows; r++){
        for(int c = 0; c <columns; c++) {
            //index = r*columns + c;
            Tile newTile(c * 32, r * 32);

            if(allTiles[index].isMine){
                newTile.isMine = true;
            }

            tilesVicinity(newTile, index);

            allTiles[index] = newTile;
            allTiles[index].ind = index;
            hiddenTiles++;
            index++;
        }
    }
}

int Board::minesVicinity(int index){
    int mineCount = 0;
    for (int i = 0; i < allTiles[index].adjTiles.size(); i++) {
        if (allTiles[index].adjTiles[i]->isMine) {
            mineCount++;
        }
    }
    return mineCount;
}

int Board::RowColInd(int r, int c) const {
    return r*columns + c-1;
}

void Board::tilesVicinity(Tile &tile, int index) const {
    int r = index/columns;
    int c = index%columns;

    for(int i = -1; i <= 1; i++){
        for(int j = -1; j <=1; j++){
            int num = (r + i)* columns + c + j;
            if(c+j < columns && (c+j >= 0) && num >= 0 && (num < rows*columns) && num != index){
                tile.adjTiles.push_back(&allTiles[num]);
            }

        }
    }
}

void Board::ExposeTiles(int index) {

    int newInd;
    if(!allTiles[index].isMine && !allTiles[index].isFlag) {
        allTiles[index].isExposed = true;
        --hiddenTiles;
    }
    if(minesVicinity(index) == 0 && !allTiles[index].isMine && !allTiles[index].isFlag) {
        for(int i = 0; i < allTiles[index].adjTiles.size(); i++) {
            if(!allTiles[index].adjTiles[i]->isMine && minesVicinity(index) > 0){//&& !allTiles[index].adjTiles[i]->isFlag) {
                allTiles[index].adjTiles[i]->isExposed = true;
            }
            newInd = allTiles[index].adjTiles[i]->ind;//RowColInd(allTiles[index].adjTiles[i]->y/32, allTiles[index].adjTiles[i]->x/32);//allTiles[index].adjTiles[i]->ind;
            if(!allTiles[newInd].isExposed && !allTiles[newInd].isFlag ) {// && minesVicinity(newInd) == 0
                ExposeTiles(newInd);
            }
        }
    }
}

const void Board::DrawTiles(RenderWindow &window, bool isUI) {
    window.clear();
    int index = 0;
    //int k = GetMines()%10;
    string n = "";

    for (int h = 0; h < rows; h++) {
        for (int w = 0; w < columns; w++) {
            index = h*columns + w;
            Tile currtile = allTiles[index];

            if (currtile.isExposed ) {

                allSprites.at("shownTile").setPosition(currtile.x, currtile.y);
                window.draw(allSprites.at("shownTile"));

                if (currtile.isMine) {
                    allSprites.at("mine").setPosition(currtile.x, currtile.y);
                    window.draw(allSprites.at("mine"));
                } else {
                    //print numbers
                    int mineCount = minesVicinity(index);

                    if (mineCount > 0) {
                        allSprites.at(to_string(mineCount)).setPosition(currtile.x, currtile.y);
                        window.draw(allSprites.at(to_string(mineCount)));
                    }
                }
            } else {
                allSprites.at("hiddenTile").setPosition(currtile.x, currtile.y);
                window.draw(allSprites.at("hiddenTile"));

                if(!gameOver){
                    allSprites.at("happy").setPosition(sf::Vector2f(columns*32/2-32, rows*32));
                    window.draw(allSprites.at("happy"));

               }else{
                    if(currtile.isMine ){
                        currtile.isExposed = true;
                        allSprites.at("mine").setPosition(currtile.x, currtile.y);
                        window.draw(allSprites.at("mine"));
                    }
                    allSprites.at("dead").setPosition(Vector2f(columns*32/2-32, rows*32));
                    window.draw(allSprites.at("dead"));
                }
                if(hiddenTiles == mines){
                    allSprites.at("coolness").setPosition(Vector2f(columns*32/2-32, rows*32));
                    window.draw(allSprites.at("coolness"));
                    if(currtile.isMine){
                        currtile.isFlag = true;
                        flgCount = mines;
                    }
                }

                if (isUI) {
                    if (currtile.isMine) {
                        currtile.isExposed = true;
                        allSprites.at("mine").setPosition(currtile.x, currtile.y);
                        window.draw(allSprites.at("mine"));
                    }
                }
            }
            if (currtile.isFlag) {
                allSprites.at("flag").setPosition(currtile.x, currtile.y);
                window.draw(allSprites.at("flag"));
                currtile.isFlag = false;
            }
        }
    }

    allSprites.at("test3").setPosition(sf::Vector2f(columns*32-64,rows*32));
    window.draw(allSprites.at("test3"));
    allSprites.at("test2").setPosition(Vector2f(columns*32-128, rows*32));
    window.draw(allSprites.at("test2"));
    allSprites.at("test1").setPosition(Vector2f(columns*32-192, rows*32));
    window.draw(allSprites.at("test1"));
    allSprites.at("debug").setPosition(Vector2f(columns*32-256, rows*32));
    window.draw(allSprites.at("debug"));

    allSprites.at("n0").setPosition(Vector2f(0, rows*32));
    window.draw(allSprites.at("n0"));

    int f = GetMines() - flgCount;
    if(mines > 10 && mines< 100){
        n = "n" + to_string(f/10);
        allSprites.at(n).setPosition(Vector2f(21, rows*32));
        window.draw(allSprites.at(n));

        n = "n" + to_string(f%10);
        allSprites.at(n).setPosition(Vector2f(42, rows * 32));
        window.draw(allSprites.at(n));
    }else if(mines>= 100){
        if(f >= 100) {
            n = "n" + to_string(f%10);
            allSprites.at(n).setPosition(Vector2f(72, rows*32));
            window.draw(allSprites.at(n));
            n = "n" + to_string((f/10)%10);
            allSprites.at(n).setPosition(Vector2f(42, rows*32));
            window.draw(allSprites.at(n));
            n = "n" + to_string((f / 10) / 10);
            allSprites.at(n).setPosition(Vector2f(21, rows * 32));
        }else{
            n = "n" + to_string(f%10);
            allSprites.at(n).setPosition(Vector2f(42, rows*32));
            window.draw(allSprites.at(n));
            n = "n" + to_string((f/10)%10);
            allSprites.at(n).setPosition(Vector2f(21, rows*32));
            window.draw(allSprites.at(n));
        }
        window.draw(allSprites.at(n));
    }else if (f>= 0 && mines < 10){
        n = "n" + to_string(f);
        allSprites.at(n).setPosition(Vector2f(21, rows*32));
        window.draw(allSprites.at(n));
    }else if (f < 0 && f > -10){
        allSprites.at("n-").setPosition(Vector2f(21, rows*32));
        window.draw(allSprites.at("n-"));

        n = "n" + to_string(abs(f));
        allSprites.at(n).setPosition(Vector2f(42, rows*32));
        window.draw(allSprites.at(n));
    }
    window.display();
}

Board::Board(){
    Reset();
}












