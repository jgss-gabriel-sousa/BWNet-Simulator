#include "SimulationState.h"


void SimulationState::PacketButtonFunc(){
    if(_data->input.IsSpriteClicked(packetIcon,sf::Mouse::Left,_data->window)){
        newObject.setTexture(_data->assets.GetTexture("iconPacket"));
        newObject.setScale(0.5,0.5);
        creatingPackage = true;
        deletingObject = false;
        showPacketLine = false;
        resending = false;
        helpText.setString("Select Origin");
    }

    if(creatingPackage && creatingPackageDestiny == false){
        bool aux = false;
        for(int i = 0; i < obj.size(); i++){
            if(_data->input.IsSpriteClicked(obj[i].sprite,sf::Mouse::Left,_data->window)){
                sf::Sprite spriteAux = obj[i].sprite;

                origin = obj[i].ip;
                CreatePacket("origin",sf::Vector2f(spriteAux.getPosition().x+spriteAux.getGlobalBounds().width/2,
                                                    spriteAux.getPosition().y+spriteAux.getGlobalBounds().height/2));
                helpText.setString("Select Destination");
                aux = true;
                break;
            }
        }
        if(aux == false){
            if(_data->input.IsSpriteClicked(background,sf::Mouse::Left,_data->window)){
                if(creatingPackage)
                    CreatePacket("delete",background.getPosition());
            }
        }
    }

    if(creatingPackageDestiny){
        bool aux = false;
        for(int i = 0; i < obj.size(); i++){
            if(_data->input.IsSpriteClicked(obj[i].sprite,sf::Mouse::Left,_data->window)){
                sf::Sprite spriteAux = obj[i].sprite;

                destiny = obj[i].ip;
                CreatePacket("destiny",sf::Vector2f(spriteAux.getPosition().x+spriteAux.getGlobalBounds().width/2,
                                                    spriteAux.getPosition().y+spriteAux.getGlobalBounds().height/2));
                aux = true;
                break;
            }
        }
        if(aux == false){
            if(_data->input.IsSpriteClicked(background,sf::Mouse::Left,_data->window)){
                if(creatingPackage)
                    CreatePacket("delete",background.getPosition());
            }
        }
    }
}


void SimulationState::ResendButtonFunc(){
    if(auxClock2.getElapsedTime().asSeconds() > 0.5){
        auxClock2.restart();
        PlayButtonFunc();

        if(resending)
            resending = false;
        else
            resending = true;
    }
}


void SimulationState::PlayButtonFunc(){
    packetMovementAnimation = true;
    if(ObjectRefByIp(origin) != nullptr)
        packetSimulated.setPosition(ObjectRefByIp(origin)->sprite.getPosition());
    actualSimulationStep = 0;
    packetSimulated.setTexture(_data->assets.GetTexture("iconPacketSmall"));
}


void SimulationState::SimulationSpeedFunc(){
    if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
        if(_data->input.IsOverSprite(simulationSpeedPointer,_data->window))
            pointerFollowMouse = true;
    }else
        pointerFollowMouse = false;

    if(pointerFollowMouse){
        simulationSpeedPointer.setPosition(_data->input.GetMousePosition(_data->window,uiView).x,simulationSpeedPointer.getPosition().y);

        if(simulationSpeedPointer.getPosition().x > simulationSpeedBar.getPosition().x+simulationSpeedBar.getGlobalBounds().width){
            simulationSpeedPointer.setPosition(simulationSpeedBar.getPosition().x+simulationSpeedBar.getGlobalBounds().width,
                                               simulationSpeedPointer.getPosition().y);
        }else if(simulationSpeedPointer.getPosition().x < simulationSpeedBar.getPosition().x){
            simulationSpeedPointer.setPosition(simulationSpeedBar.getPosition().x,simulationSpeedPointer.getPosition().y);
        }
    }
}


void SimulationState::DeleteButtonFunc(){
    if(auxClock2.getElapsedTime().asSeconds() > 0.5){
        auxClock2.restart();
        newObject.setTexture(_data->assets.GetTexture("buttonDelete"));
        newObject.setScale(0.5,0.5);
        deletingObject = true;
        helpText.setString("Delete");
    }
}
