//
// Created by Milena Petrovic on 4/12/22.
//
#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include "Board.h"
using namespace std;



class TextureMang{
    static unordered_map<string, sf::Texture> textures;
    static void LoadTexture(string textName);
public:
    //static void LoadTexture(string textName);
    static sf::Texture& GetTexture(string textName);
    //void DrawBoard(sf::RenderWindow window, Board* board);
    static void Clear(); //call at end of main
};



