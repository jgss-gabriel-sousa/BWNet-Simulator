#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include "Game.h"

class Packet{
    public:
        Packet();
        virtual ~Packet(){};

        string source;
        string destination;
        string data;
        string type;
        unsigned int hops;
        unsigned char ttl;
};
