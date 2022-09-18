#include <iostream>
//#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include <iostream>
#include <fstream>
#include "TextureMang.h"
#include "Tile.h"
#include "Board.h"
#include <random>


using namespace std;
using namespace sf;




int main() {

    Sprite hiddenTile(TextureMang::GetTexture("tile_hidden"));
    Sprite shownTile(TextureMang::GetTexture("tile_revealed"));
    Sprite happy(TextureMang::GetTexture("face_happy"));
    Sprite dead(TextureMang::GetTexture("face_lose"));
    Sprite coolness(TextureMang::GetTexture("face_win"));
    Sprite test1(TextureMang::GetTexture("test_1"));
    Sprite test2(TextureMang::GetTexture("test_2"));
    Sprite test3(TextureMang::GetTexture("test_3"));
    Sprite digits(TextureMang::GetTexture("digits"));
    Sprite debug(TextureMang::GetTexture("debug"));
    Sprite mine(TextureMang::GetTexture("mine"));
    Sprite flag(TextureMang::GetTexture("flag"));
    Sprite num1(TextureMang::GetTexture("number_1"));
    Sprite num2(TextureMang::GetTexture("number_2"));
    Sprite num3(TextureMang::GetTexture("number_3"));
    Sprite num4(TextureMang::GetTexture("number_4"));
    Sprite num5(TextureMang::GetTexture("number_5"));
    Sprite num6(TextureMang::GetTexture("number_6"));
    Sprite num7(TextureMang::GetTexture("number_7"));
    Sprite num8(TextureMang::GetTexture("number_8"));

    map<string, Sprite> Sprites;

    Sprites.emplace("hiddenTile", hiddenTile);
    Sprites.emplace("shownTile", shownTile);
    Sprites.emplace("happy", happy);
    Sprites.emplace("dead", dead);
    Sprites.emplace("coolness", coolness);
    Sprites.emplace("test1", test1);
    Sprites.emplace("test2", test2);
    Sprites.emplace("test3", test3);
    Sprites.emplace("debug", debug);
    Sprites.emplace("mine", mine);
    Sprites.emplace("flag", flag);
    Sprites.emplace("1", num1);
    Sprites.emplace("2", num2);
    Sprites.emplace("3", num3);
    Sprites.emplace("4", num4);
    Sprites.emplace("5", num5);
    Sprites.emplace("6", num6);
    Sprites.emplace("7", num7);
    Sprites.emplace("8", num8);

    Board board;

//    ifstream fileConf("boards/config.cfg");
//    ifstream fileT1("boards/testboard1.brd");
//    ifstream fileT2("board.testboard2.brd");
//    ifstream fileT3("board.testboard3.brd");

    board.grid("boards/config.cfg");
    //board.grid(fileConf);

    happy.setPosition(sf::Vector2f(board.GetCol()*32/2-32, board.GetRow()*32));


    digits.setTextureRect(IntRect(0,0, 32, 32));
    Sprites.emplace("n0", digits);
    digits.setTextureRect(IntRect(21, 0, 21, 32));
    Sprites.emplace("n1", digits);
    digits.setTextureRect(IntRect(42, 0, 21, 32));
    Sprites.emplace("n2", digits);
    digits.setTextureRect(IntRect(63, 0, 21, 32));
    Sprites.emplace("n3", digits);
    digits.setTextureRect(IntRect(84, 0, 21, 32));
    Sprites.emplace("n4", digits);
    digits.setTextureRect(IntRect(105, 0, 21, 32));
    Sprites.emplace("n5", digits);
    digits.setTextureRect(IntRect(126, 0, 21, 32));
    Sprites.emplace("n6", digits);
    digits.setTextureRect(IntRect(147, 0, 21, 32));
    Sprites.emplace("n7", digits);
    digits.setTextureRect(IntRect(168, 0, 21, 32));
    Sprites.emplace("n8", digits);
    digits.setTextureRect(IntRect(189,0, 21, 32));
    Sprites.emplace("n9", digits);
    digits.setTextureRect(IntRect(210, 0, 21, 32));
    Sprites.emplace("n-", digits);



    board.initConfigTiles(Sprites);

    sf::RenderWindow window(sf::VideoMode(board.GetCol()*32,(board.GetRow()*32)+100), "Yo momma");

    board.DrawTiles(window, false);

    ////Create range for x and y variables for mouse click up here to adhere to tile
    int flagCount = 0;
    while (window.isOpen())
    {
        int index = 0;
        bool UI;


        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if(event.type == sf::Event::MouseButtonReleased)
                if(event.mouseButton.button == Mouse::Right){
                    Vector2i mouse = Mouse::getPosition(window);
                    index = (mouse.y/32)*board.GetCol() +mouse.x/32;

                    if(mouse.y/32 < board.GetRow())
                        if(!board.allTiles[index].isExposed ) {
                            //++board.flgCount;
                            if(!board.allTiles[index].isFlag){
                                ++board.flgCount;
                                board.allTiles[index].isFlag = true;
                            }else{
                                --board.flgCount;
                                board.allTiles[index].isFlag = false;
                            }
                            //board.allTiles[index].isFlag = !board.allTiles[index].isFlag;
                            board.DrawTiles(window, UI);
                        }




                }else if(event.mouseButton.button == Mouse::Left) {
                    Vector2i mouse = Mouse::getPosition(window);
                    index = (mouse.y/32)*board.GetCol() + mouse.x/32;

                    if (mouse.y / 32 < board.GetRow()){
                        if (!board.allTiles[index].isFlag && !board.gameOver && !board.allTiles[index].isExposed)
                            board.ExposeTiles(index);
                            board.DrawTiles(window, UI);


                        if(board.allTiles[index].isMine && !board.allTiles[index].isFlag && !board.gameOver){
                            board.gameOver = true;
                            board.DrawTiles(window, UI);
                        }
                    }

                    if(mouse.y/32 > board.GetRow()-1 && (mouse.y/32)*32 < board.GetRow()*32+64) {
                        if ((mouse.x / 32) * 32 < (board.GetCol()*32) - 192 && (mouse.x/32)*32 > board.GetCol()*32-256) {
                            if(board.gameOver){
                                UI = false;
                            }
                            UI = !UI;
                            board.DrawTiles(window, UI);
                        }
                        if((mouse.x/32)*32 < board.GetCol()*32-128 && (mouse.x/32)*32 > board.GetCol()*32-196){
                            board.testColumns.clear();
                            board.testGrid("boards/testboard1.brd");
                            board.initTestTiles(Sprites);
                            //board.gameOver = false;
                            board.DrawTiles(window, UI);
                        }
                        if((mouse.x/32)*32 < board.GetCol()*32-64 && (mouse.x/32)*32 > board.GetCol()*32-128){
                            board.testColumns.clear();
                            board.testGrid("boards/testboard2.brd");
                            board.initTestTiles(Sprites);
                            board.DrawTiles(window, UI);
                        }
                        if((mouse.x)/32*32 < board.GetCol()*32 && (mouse.x)/32*32 > board.GetCol()*32-64){
                            board.testColumns.clear();
                            board.testGrid("boards/testboard3.brd");
                            board.initTestTiles(Sprites);
                            board.DrawTiles(window, UI);
                        }
                        if((mouse.x/32)*32 < board.GetCol()*32/2 && mouse.x/32 *32 > board.GetCol()*32/2-64){
                            board.grid("boards/config.cfg");
                            board.initConfigTiles(Sprites);
                            //board.gameOver = false;
                            board.DrawTiles(window, UI);
                        }
                    }
                }


        }
    }

    return 0;
}




