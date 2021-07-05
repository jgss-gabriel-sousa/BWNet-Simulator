#pragma once

#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>

#include "State.h"
#include "Game.h"
#include "Object.h"

#define ROUTING_TICK 3000 //miliseconds
#define ResendingTick 3000 //miliseconds


class SimulationState : public State{
public:
    SimulationState(GameDataRef data,string projectName);
    ~SimulationState();

    void Init();

    void HandleInput();
    void Update(float dt);
    void Draw(float dt);

    void CreatePacket(string,sf::Vector2f);
    void ObjectDeletion();
    Object* ObjectRefByIp(string);
    void UpdateRoutingTables();

    void Simulation(string,string);
    void PacketMovement();

    float Distance(sf::Vector2f, sf::Vector2f);
    float Distance(sf::Sprite, sf::Sprite);

    void PlayButtonFunc();
    void SimulationSpeedFunc();
    void PacketButtonFunc();
    void ResendButtonFunc();
    void DeleteButtonFunc();

    void Load();
    void SaveLog();

private:
    GameDataRef _data;

    vector<Object> obj;
    vector<string> simulationLog;
    vector<string> simulationSteps;
    int actualSimulationStep;
    bool packetMovementAnimation = false;
    bool playButtonVisible = false;
    bool simulationError = true;
    string origin, destiny;
    sf::RectangleShape simulationSpeedBar;
    sf::Sprite simulationSpeedPointer;
    sf::Text simulationSpeedText;
    float simulationSpeed = 1;
    bool pointerFollowMouse = false;

    int pointerPos;
    int larguraBox;

    string projectName;
    string aux;
    bool movingCamera = false;
    float zoomRatio = 1;

    sf::Clock auxClock;
    sf::Clock auxClock2;
    sf::Clock RoutingTablesUpdateClock;
    sf::Clock resendingClock;

    sf::Cursor cursor;
    sf::Sprite newObject;
    bool creatingPackage = false;
    bool creatingPackageDestiny = false;
    bool resending = false;
    bool showPacketLine = false;
    bool deletingObject = false;
    sf::Vertex packetLine[2];
    sf::VertexArray packetRoute;

    sf::View uiView;
    sf::View objView;

    sf::Text helpText;

    bool showStatsWindow = false;
    sf::Text statsText[3];
    sf::RectangleShape statsWindow;

    sf::Sprite background;
    sf::Sprite selectionBarBackground;
    sf::Sprite closeButton;
    sf::Sprite playButton;

    sf::Sprite packetIcon;
    sf::Sprite resendIcon;
    sf::Sprite resendIcon1;
    sf::Sprite resendIcon2;
    sf::Sprite packetSimulated;
    sf::Sprite deleteButton;
};
