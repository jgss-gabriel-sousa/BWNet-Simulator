#include "Object.h"

Object::Object(GameDataRef data, string type, string ip, string routerIp, sf::Sprite spriteRef) : _data(data){
    sprite = spriteRef;
    this->type = type;
    this->ip = ip;
    this->routerIp = routerIp;

    sprite.setScale(0.5,0.5);
}

Object::Object(GameDataRef data, string ip, vector<string> ipsAggregates, sf::Sprite spriteRef) : _data(data){
    sprite = spriteRef;
    type = "Router";
    this->ip = ip;

    for(int i = 0; i < ipsAggregates.size(); i++){
        ipsInRouter.push_back(ipsAggregates[i]);
    }

    sprite.setScale(0.5,0.5);

    sprite.setOrigin(0,0);
    range.setFillColor(sf::Color(255,0,0,10));
    range.setRadius(200);
    range.setOutlineColor(sf::Color::Red);
    range.setOutlineThickness(1);
    range.setOrigin(range.getGlobalBounds().width/2,range.getGlobalBounds().height/2);
    range.setPosition(sprite.getPosition());
    sprite.setOrigin(sprite.getGlobalBounds().width/2,sprite.getGlobalBounds().height/2);
    ResetRouterIpTable();
}

void Object::Draw(){
    _data->window.draw(sprite);
    _data->window.draw(range);
}

bool Object::IsInIpList(string userIp){
    for(int i = 0; i<ipsInRouter.size(); i++){
        if(ipsInRouter[i] == userIp)
            return true;
    }
    return false;
}

string Object::GetNewIP(){
    string aux = ip;

    if(type == "Router"){
        while(aux.find(".") != string::npos){
            aux = aux.substr(aux.find(".")+1);
        }
        for(int i = 0; i<256; i++){
            if(ipList[i] == true){
                ipList[i] = false;
                return "192.168."+aux+"."+to_string(i);
            }
        }
    }
    return "0.0.0.0";
}

void Object::ResetRouterIpTable(){
    for(int i = 0; i < 256; i++){
        ipList[i] = true;
    }
}

/*
string Object::SearchInRouterTable(string ipToSearch){
    string nextHop = "0.0.0.0";

    for(int i = 0; i < RoutingTable.size(); i++){
        if(RoutingTable[i] == ipToSearch){
            ;
        }
    }
    return nextHop;
}
*/

string Object::Save(){
    string aux = type+";"+ip+";"+routerIp+";"+to_string(sprite.getPosition().x)+","+to_string(sprite.getPosition().y);

    if(type != "Router"){
        return aux;
    }
    else{
        aux += ";";

        for(int i = 0; i<ipsInRouter.size(); i++){
            aux += ipsInRouter[i];

            if(i != ipsInRouter.size()-1)
                aux += ",";
        }
    }
}
