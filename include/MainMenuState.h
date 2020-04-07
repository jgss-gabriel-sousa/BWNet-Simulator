#pragma once

#include <SFML/Graphics.hpp>
#include "State.h"
#include "Game.h"

class MainMenuState : public State{
public:
    MainMenuState(GameDataRef data);

    void Init();

    void HandleInput();
    void Update(float dt);
    void Draw(float dt);

    void MenuSelectSimulation();
    void InsertText();

private:
    GameDataRef _data;

    bool showCredits = false;
    bool showSelectSimulation = false;
    bool insertingText = false;

    vector<sf::Text> simulationList;
    string menuSelection;
    int simulationSelected = -1;
    vector<sf::Text> creditsText;

    sf::Text textInputed;
    sf::Text helpText;

    sf::Sprite buttonSimulacao;
    sf::Sprite buttonEditor;
    sf::Sprite buttonCreditos;
    sf::Sprite buttonReturn;
    sf::Sprite newSimulation;
    sf::Sprite deleteSimulation;
    sf::Sprite buttonSelect;
    sf::Sprite newFileBackground;
};
