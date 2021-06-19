#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <vector>
#include "Game.h"

class Packet{
    public:
        Packet();
        virtual ~Packet(){};

        vector<string> log;
        vector<string> hops;
        string source;
        string destination;
        string actual;
        string data;
        string type;
        unsigned char ttl;
};
