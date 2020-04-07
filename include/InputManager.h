#pragma once

#include <SFML/Graphics.hpp>

class InputManager{
public:
    InputManager(){}
    ~InputManager(){}

    bool IsSpriteClicked(sf::Sprite object, sf::Mouse::Button button, sf::RenderWindow &window);
    bool IsSpriteClicked(sf::Sprite object, sf::Mouse::Button button, sf::RenderWindow &window, sf::View view);
    bool IsOverSprite(sf::Sprite object, sf::RenderWindow &window);

    sf::Vector2i GetMousePosition(sf::RenderWindow &window);
    sf::Vector2f GetMousePosition(sf::RenderWindow &window, sf::View view);
};
