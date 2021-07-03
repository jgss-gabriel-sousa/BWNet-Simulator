#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include "Game.h"

class Object{
    public:
        Object(GameDataRef data, string type, string ip, string routerIp, sf::Sprite spriteRef);
        Object(GameDataRef data, string ip, vector<string> ipsInRouter, sf::Sprite spriteRef);
        Object(){};
        ~Object(){};

        void Draw();
        string Save();
        bool IsInIpList(string);
        string GetNewIP();
        void ResetRouterIpTable();
        void UpdateTable(vector<pair<string,string>>);
        sf::Vector2f GetPosition(){return sprite.getPosition();}

        GameDataRef _data;

        sf::Sprite sprite;
        sf::CircleShape range;

        vector<string> ipsInRouter;
        bool ipList[256];

        vector<string> RoutingTable;

        string ip;
        string routerIp;
        string type;
};
