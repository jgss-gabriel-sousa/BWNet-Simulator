#pragma once

#include <SFML/Graphics.hpp>
#include <fstream>

#include "State.h"
#include "Game.h"

class LoadingState : public State{
public:
    LoadingState(GameDataRef data);

    void Init();

    void HandleInput();
    void Update(float dt);
    void Draw(float dt);

private:
    GameDataRef _data;
};
