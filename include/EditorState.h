#pragma once

#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>

#include "State.h"
#include "Game.h"
#include "Object.h"

enum selectionBar{
    pos1 = 0,
    pos2 = 64,
    pos3 = 128,
    pos4 = 192,
    pos5 = 256,
    pos6 = 320
};

class EditorState : public State{
public:
    EditorState(GameDataRef data,string projectName,bool newSimulation);

    void Init();

    void HandleInput();
    void Update(float dt);
    void Draw(float dt);

    void AddObject();

    void Load();
    void Save();

private:
    GameDataRef _data;

    vector<Object> obj;

    string projectName;
    bool newSimulation;

    sf::Cursor cursor;

    sf::Sprite newObject;
    string newObjectType;
    bool addingObject;

    sf::Text helpText;

    sf::Sprite background;
    sf::Sprite closeButton;
    sf::Sprite selectionBarBackground;
    sf::Sprite saveButton;

    sf::Sprite computerIcon;
    sf::Sprite smartphoneIcon;
    sf::Sprite routerIcon;
    sf::Sprite wireIcon;
};
