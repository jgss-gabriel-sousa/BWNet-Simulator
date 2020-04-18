#pragma once

#include <SFML/Graphics.hpp>
#include <fstream>
#include <cstdio>
#include "State.h"
#include "Game.h"

class MainMenuState : public State{
public:
    MainMenuState(GameDataRef data);

    void Init();

    void HandleInput();
    void Update(float dt);
    void Draw(float dt);

    void Load();

    void MenuSelectSimulation();
    void DeleteSimulation();
    void InsertText();
    void Options(string);

private:
    GameDataRef _data;

    bool showCredits = false;
    bool showOptions = false;
    bool showSelectSimulation = false;
    bool insertingText = false;

    vector<sf::Text> simulationList;
    string menuSelection;
    int simulationSelected = -1;
    vector<sf::Text> creditsText;

    sf::Text textInputed;
    sf::Text helpText;
    sf::Text optionsText[4];

    sf::Sprite buttonSimulacao;
    sf::Sprite buttonEditor;
    sf::Sprite buttonCreditos;
    sf::Sprite buttonOpcoes;
    sf::Sprite buttonSair;
    sf::Sprite buttonReturn;
    sf::Sprite newSimulation;
    sf::Sprite buttonDeleteSimulation;
    sf::Sprite buttonSelect;
    sf::Sprite newFileBackground;
    sf::Sprite logo;

    sf::Sprite optionFullscreen;
};
