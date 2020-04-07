#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "Game.h"

class Object{
    public:
        Object(GameDataRef data,string type,string ip, sf::Sprite spriteRef);
        ~Object(){};

        void Draw();
        string Save();

    private:
        GameDataRef _data;

        sf::Sprite sprite;
        sf::CircleShape range;

        string ip;
        string type;
};
