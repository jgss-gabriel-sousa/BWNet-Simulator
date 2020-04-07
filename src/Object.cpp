#include "Object.h"

Object::Object(GameDataRef data, string type, string ip, sf::Sprite spriteRef) : _data(data){
    sprite = spriteRef;
    this->type = type;
    this->ip = ip;

    sprite.setScale(0.5,0.5);

    if(type == "Router"){
        sprite.setOrigin(0,0);
        range.setFillColor(sf::Color(255,0,0,50));
        range.setRadius(100);
        range.setOrigin(range.getGlobalBounds().width/2,range.getGlobalBounds().height/2);
        range.setPosition(sprite.getPosition());
        sprite.setOrigin(sprite.getGlobalBounds().width/2,sprite.getGlobalBounds().height/2);
    }
}

void Object::Draw(){
    _data->window.draw(sprite);
    _data->window.draw(range);
}

string Object::Save(){
    return type+";"+ip+";"+to_string(sprite.getPosition().x)+","+to_string(sprite.getPosition().y);
}
