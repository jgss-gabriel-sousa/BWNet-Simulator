#include "LoadingState.h"
#include "MainMenuState.h"

LoadingState::LoadingState(GameDataRef data) : _data(data){
    ;
}

void LoadingState::Init(){
    _data->assets.LoadTexture("buttonSimulacao");
    _data->assets.LoadTexture("buttonEditor");
    _data->assets.LoadTexture("buttonCreditos");
    _data->assets.LoadTexture("buttonSair");
    _data->assets.LoadTexture("buttonOpcoes");
    _data->assets.LoadTexture("buttonClose");
    _data->assets.LoadTexture("buttonReturn");
    _data->assets.LoadTexture("background");
    _data->assets.LoadTexture("selectionBarBackground");
    _data->assets.LoadTexture("newFileBackground");
    _data->assets.LoadTexture("blackScreen");
    _data->assets.LoadTexture("iconMenu");
    _data->assets.LoadTexture("iconSave");
    _data->assets.LoadTexture("buttonNewSimulation");
    _data->assets.LoadTexture("buttonDeleteSimulation");
    _data->assets.LoadTexture("buttonSelect");
    _data->assets.LoadTexture("buttonPlay");
    _data->assets.LoadTexture("buttonDelete");
    _data->assets.LoadTexture("simulationSpeedPointer");

    _data->assets.LoadTexture("iconSmartphone");
    _data->assets.LoadTexture("iconComputer");
    _data->assets.LoadTexture("iconRouter");
    _data->assets.LoadTexture("iconPacket");
    _data->assets.LoadTexture("iconPacketSmall");
    _data->assets.LoadTexture("iconPacketSmallRed");
    _data->assets.LoadTexture("iconPacketSmallGreen");

    _data->assets.LoadAudio("click");
}

void LoadingState::HandleInput(){
    sf::Event event;

    while(_data->window.pollEvent(event)){
        if(sf::Event::Closed == event.type){
            _data->window.close();
        }
    }
}

void LoadingState::Update(float dt){
    _data->machine.AddState(StateRef(new MainMenuState(_data)),true);
}

void LoadingState::Draw(float dt){
    _data->window.clear();
    _data->window.display();
}







