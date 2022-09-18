//
// Created by Milena Petrovic on 4/12/22.
//

#include <iostream>
#include <fstream>
#include "TextureMang.h"
#include "Board.h"

unordered_map<string, sf::Texture> TextureMang::textures;

void TextureMang::LoadTexture(string fileName) {
    string path = "images/";
    path += fileName + ".png";

    textures[fileName].loadFromFile(path);
}

sf::Texture& TextureMang::GetTexture(string textName) {
    if(textures.find(textName) == textures.end())
        LoadTexture(textName);
    return textures[textName];
}


void TextureMang::Clear(){
    textures.clear();
}