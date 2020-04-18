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
    void DeleteObject(int);
    string GetNewRouterIP();

    void Load();
    void Save();

private:
    GameDataRef _data;

    vector<Object> obj;
    int ipListRouters = 0;
    int ipListUsers = 0;

    string projectName;
    bool newSimulation;

    sf::Cursor cursor;
    sf::Clock clock;

    sf::Sprite newObject;
    string newObjectType;
    bool addingObject = false;
    bool deletingObject = false;
    bool showStatsWindow = false;

    sf::Text helpText;
    sf::Text statsText[3];
    sf::RectangleShape statsWindow;

    sf::Sprite background;
    sf::Sprite closeButton;
    sf::Sprite selectionBarBackground;
    sf::Sprite saveButton;
    sf::Sprite deleteButton;

    sf::Sprite computerIcon;
    sf::Sprite smartphoneIcon;
    sf::Sprite routerIcon;
    sf::Sprite wireIcon;
};
