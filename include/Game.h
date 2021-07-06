#pragma once

#include <memory>
#include <string>
#include <SFML/Graphics.hpp>
#include <sstream>
#include "log.h"

#include "StateMachine.h"
#include "AssetManager.h"
#include "InputManager.h"
#include "DEFINITIONS.h"

using namespace std;

struct GameData{
    StateMachine machine;
    sf::RenderWindow window;
    AssetManager assets;
    InputManager input;
};

typedef shared_ptr<GameData> GameDataRef;

class Game{
public:
    Game(int width, int height, string title, unsigned int framerate);

private:
    const float dt = 1.0f/45.0f;
    sf::Clock _clock;

    GameDataRef _data = make_shared<GameData>();

    void Run();
};






