#include "InputManager.h"

bool InputManager::IsSpriteClicked(sf::Sprite object, sf::Mouse::Button button, sf::RenderWindow &window){
    if(sf::Mouse::isButtonPressed(button)){
        sf::IntRect tempRect(object.getPosition().x, object.getPosition().y, object.getGlobalBounds().width, object.getGlobalBounds().height);

        if(tempRect.contains(sf::Mouse::getPosition(window))){
            return true;
        }
    }
    return false;
}

bool InputManager::IsSpriteClicked(sf::Sprite object, sf::Mouse::Button button, sf::RenderWindow &window, sf::View view){
    if(sf::Mouse::isButtonPressed(button)){
        sf::IntRect tempRect(object.getPosition().x, object.getPosition().y, object.getGlobalBounds().width, object.getGlobalBounds().height);

        if(tempRect.contains(sf::Vector2i(window.mapPixelToCoords(sf::Mouse::getPosition(window),view)))){
            return true;
        }
    }
    return false;
}

bool InputManager::IsOverSprite(sf::Sprite object, sf::RenderWindow &window){
    sf::IntRect tempRect(object.getPosition().x, object.getPosition().y, object.getGlobalBounds().width, object.getGlobalBounds().height);

    if(tempRect.contains(sf::Mouse::getPosition(window))){
        return true;
    }
    return false;
}

sf::Vector2i InputManager::GetMousePosition(sf::RenderWindow &window){
    return sf::Mouse::getPosition(window);
}

sf::Vector2f InputManager::GetMousePosition(sf::RenderWindow &window,sf::View view){
    return window.mapPixelToCoords(sf::Mouse::getPosition(window),view);
}
