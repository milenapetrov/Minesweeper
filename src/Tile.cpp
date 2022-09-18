
#include "Tile.h"
#include "Board.h"

Tile::Tile(int x, int y){
    this->x = x;
    this->y = y;
}

void Tile::reset(){
    isFlag = false;
    isMine = false;
    isExposed = false;
    ind = 0;
}



