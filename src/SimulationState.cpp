#include "SimulationState.h"
#include "MainMenuState.h"

SimulationState::SimulationState(GameDataRef data,string projectName) : _data(data){
    this->projectName = projectName;

    Load();
}

SimulationState::~SimulationState(){
    SaveLog();
}

void SimulationState::Init(){
    background.setTexture(_data->assets.GetTexture("background"));
    selectionBarBackground.setTexture(_data->assets.GetTexture("selectionBarBackground"));
    closeButton.setTexture(_data->assets.GetTexture("buttonClose"));
    playButton.setTexture(_data->assets.GetTexture("buttonPlay"));
    packetIcon.setTexture(_data->assets.GetTexture("iconPacket"));
    packetSimulated.setTexture(_data->assets.GetTexture("iconPacketSmall"));
    statsWindow.setTexture(&_data->assets.GetTexture("blackScreen"),false);
    simulationSpeedBar.setTexture(&_data->assets.GetTexture("blackScreen"),false);
    simulationSpeedPointer.setTexture(_data->assets.GetTexture("simulationSpeedPointer"));

    helpText.setFont(_data->assets.GetFont("arial"));
    statsText[0].setFont(_data->assets.GetFont("arial"));
    statsText[1].setFont(_data->assets.GetFont("arial"));
    simulationSpeedText.setFont(_data->assets.GetFont("arial"));

    closeButton.setScale(0.8,0.8);
    background.setScale(SCREEN_WIDTH/2,SCREEN_HEIGHT/2);
    statsWindow.setSize(sf::Vector2f(300,100));
    statsWindow.setSize(sf::Vector2f(300,100));
    simulationSpeedBar.setSize(sf::Vector2f(200,16));

    background.setPosition(0,selectionBarBackground.getGlobalBounds().height);
    selectionBarBackground.setPosition(0,0);
    closeButton.setPosition(SCREEN_WIDTH-closeButton.getGlobalBounds().width,0);
    playButton.setPosition(0,-80);
    packetIcon.setPosition(10,0);
    packetSimulated.setPosition(0,-80);
    simulationSpeedBar.setPosition(SCREEN_WIDTH/2-simulationSpeedBar.getGlobalBounds().width/2,simulationSpeedBar.getGlobalBounds().height+24);
    simulationSpeedPointer.setOrigin(simulationSpeedPointer.getGlobalBounds().width/2,simulationSpeedPointer.getGlobalBounds().height/2);
    simulationSpeedPointer.setPosition(SCREEN_WIDTH/2,simulationSpeedBar.getPosition().y);
    simulationSpeedText.setCharacterSize(14);

    helpText.setPosition(0,SCREEN_HEIGHT-40);

    uiView.setSize(SCREEN_WIDTH,SCREEN_HEIGHT);
    uiView.setCenter(SCREEN_WIDTH/2,SCREEN_HEIGHT/2);
    objView.setSize(SCREEN_WIDTH,SCREEN_HEIGHT);
    objView.setCenter(SCREEN_WIDTH/2,SCREEN_HEIGHT/2);

    helpText.setColor(sf::Color::Black);
    helpText.setOutlineColor(sf::Color::White);
    helpText.setOutlineThickness(1);
    auxClock.restart();
}

void SimulationState::HandleInput(){
    sf::Event event;

    while(_data->window.pollEvent(event)){
        if(sf::Event::Closed == event.type){
            _data->window.close();
        }
        if(_data->input.IsSpriteClicked(closeButton,sf::Mouse::Left,_data->window)){
            if(auxClock.getElapsedTime().asSeconds() > 1){
                SaveLog();
                _data->machine.AddState(StateRef(new MainMenuState(_data)),true);
                _data->assets.PlayAudio("click");
            }
        }

        /*
        MOVIMENTAÇÃO DA CÂMERA
        Provoca Bugs com as Views
        Possível Solução: Alterar método IsOverObject no input manager para reconhecer a View em uso

        if(event.type == sf::Event::MouseWheelMoved){
            if(event.mouseWheel.delta < 0){
                objView.zoom(1.05);
            }
            if(event.mouseWheel.delta > 0){
                objView.zoom(0.95);
            }
        }

        if(event.type == sf::Event::MouseButtonPressed){
            if(event.mouseButton.button == sf::Mouse::Right){
                movingCamera = true;
                cursor.loadFromSystem(sf::Cursor::Hand);
                _data->window.setMouseCursor(cursor);
            }
        }else if(event.type == sf::Event::MouseButtonReleased){
            if(event.mouseButton.button == sf::Mouse::Right){
                movingCamera = false;
                cursor.loadFromSystem(sf::Cursor::Arrow);
                _data->window.setMouseCursor(cursor);
            }
        }

        if(event.type == sf::Event::KeyPressed){
            if(event.key.code == sf::Keyboard::Left)
                objView.setCenter(objView.getCenter().x-5,objView.getCenter().y);
            if(event.key.code == sf::Keyboard::Right)
                objView.setCenter(objView.getCenter().x+5,objView.getCenter().y);
            if(event.key.code == sf::Keyboard::Up)
                objView.setCenter(objView.getCenter().x,objView.getCenter().y-5);
            if(event.key.code == sf::Keyboard::Down)
                objView.setCenter(objView.getCenter().x,objView.getCenter().y+5);
        }
        */
        showStatsWindow = false;
        for(int i = 0; i<obj.size(); i++){
            if(_data->input.IsOverSprite(obj[i].sprite,_data->window)){
                statsText[0].setString("IP: "+obj[i].ip);
                if(obj[i].type != "Router")
                    statsText[1].setString("Roteador: "+obj[i].routerIp);
                else
                    statsText[1].setString("");
                showStatsWindow = true;
                break;
            }
        }

        //Selection Bar items
        if(_data->input.IsSpriteClicked(packetIcon,sf::Mouse::Left,_data->window)){
            newObject.setTexture(_data->assets.GetTexture("iconPacket"));
            newObject.setScale(0.5,0.5);
            creatingPackage = true;
            showPacketLine = false;
            helpText.setString("Selecione a Origem");
        }

        if(creatingPackage && creatingPackageDestiny == false){
            bool aux = false;
            for(int i = 0; i < obj.size(); i++){
                if(_data->input.IsSpriteClicked(obj[i].sprite,sf::Mouse::Left,_data->window)){
                    if(obj[i].type != "Router"){
                        sf::Sprite spriteAux = obj[i].sprite;

                        origin = obj[i].ip;
                        CreatePacket("origin",sf::Vector2f(spriteAux.getPosition().x+spriteAux.getGlobalBounds().width/2,
                                                            spriteAux.getPosition().y+spriteAux.getGlobalBounds().height/2));
                        helpText.setString("Selecione o Destino");
                        aux = true;
                        break;
                    }
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

        if(_data->input.IsSpriteClicked(playButton,sf::Mouse::Left,_data->window)){
            packetMovementAnimation = true;
            packetSimulated.setPosition(ObjectByIp(origin).sprite.getPosition());
            actualSimulationStep = 0;
            packetSimulated.setTexture(_data->assets.GetTexture("iconPacketSmall"));
        }

        if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
            if(_data->input.IsOverSprite(simulationSpeedPointer,_data->window)){
                pointerFollowMouse = true;
            }
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
}

void SimulationState::Update(float dt){
    if(movingCamera){
        objView.setCenter(sf::Vector2f(_data->window.mapPixelToCoords(sf::Vector2i(sf::Mouse::getPosition()))));
    }
    newObject.setPosition(_data->input.GetMousePosition(_data->window,uiView));

    if(packetMovementAnimation){
        PacketMovement();
    }

    if(playButtonVisible){
        playButton.setPosition(SCREEN_WIDTH-128,0);
    }else{
        playButton.setPosition(SCREEN_WIDTH-128,-80);
    }

    if(showStatsWindow){
        statsWindow.setPosition(SCREEN_WIDTH-statsWindow.getSize().x,SCREEN_HEIGHT-statsWindow.getSize().y);
        statsText[0].setPosition(SCREEN_WIDTH-statsWindow.getSize().x+10,SCREEN_HEIGHT-statsWindow.getSize().y+10);
        statsText[1].setPosition(SCREEN_WIDTH-statsWindow.getSize().x+10,SCREEN_HEIGHT-statsWindow.getSize().y+50);
    }else{
        statsWindow.setPosition(0,-200);
        statsText[0].setPosition(0,-200);
        statsText[1].setPosition(0,-200);
    }

    pointerPos = simulationSpeedPointer.getPosition().x - 300;
    larguraBox = simulationSpeedBar.getPosition().x+simulationSpeedBar.getGlobalBounds().width - 300;
    simulationSpeed = (pow(pointerPos,2)/pow(larguraBox,2))/0.25;

    simulationSpeedText.setString("Velocidade da Simulação: "+to_string(simulationSpeed));
    simulationSpeedText.setPosition(SCREEN_WIDTH/2-simulationSpeedText.getGlobalBounds().width/2,2);
}

void SimulationState::CreatePacket(string action,sf::Vector2f point){
    if(action == "delete"){
        creatingPackage = false;
        creatingPackageDestiny = false;
        showPacketLine = false;
        playButtonVisible = false;
        helpText.setString("");
    }
    else if(action == "origin"){
        creatingPackageDestiny = true;
        playButtonVisible = true;
        packetMovementAnimation = false;
        packetSimulated.setPosition(0,-80);
        packetLine[0] = sf::Vertex(point,sf::Color::Green);
        auxClock.restart();
    }
    else if(action == "destiny" && auxClock.getElapsedTime().asSeconds()>1){
        creatingPackageDestiny = false;
        creatingPackage = false;
        showPacketLine = true;
        Simulation(origin,destiny);
        playButtonVisible = true;
        packetLine[1] = sf::Vertex(point,sf::Color::Green);
        helpText.setString("");
    }
}

void SimulationState::PacketMovement(){
    sf::Vector2f targetPos = ObjectByIp(simulationSteps[actualSimulationStep]).sprite.getPosition();
    sf::Vector2f actualPos = packetSimulated.getPosition();
    float movSpeed = simulationSpeed;
    float distance = sqrt(pow(targetPos.x-actualPos.x,2)+pow(targetPos.y-actualPos.y,2));

    if(distance < 10){
        actualSimulationStep++;
        if(actualSimulationStep == simulationSteps.size()){
            packetMovementAnimation = false;
            actualSimulationStep = 0;
            if(simulationError)
                packetSimulated.setTexture(_data->assets.GetTexture("iconPacketSmallRed"));
            else
                packetSimulated.setTexture(_data->assets.GetTexture("iconPacketSmallGreen"));
            return;
        }
    }

    if(targetPos.x-movSpeed > actualPos.x){
        packetSimulated.setPosition(packetSimulated.getPosition().x + movSpeed,
                                    packetSimulated.getPosition().y);
    }else if(targetPos.x+movSpeed < actualPos.x){
        packetSimulated.setPosition(packetSimulated.getPosition().x - movSpeed,
                                    packetSimulated.getPosition().y);
    }
    if(targetPos.y-movSpeed > actualPos.y){
        packetSimulated.setPosition(packetSimulated.getPosition().x,
                                    packetSimulated.getPosition().y + movSpeed);
    }else if(targetPos.y+movSpeed < actualPos.y){
        packetSimulated.setPosition(packetSimulated.getPosition().x,
                                    packetSimulated.getPosition().y - movSpeed);
    }
}

void SimulationState::Simulation(string ipOrigin,string ipDestiny){
    Object destiny;
    Object actual;
    Object next;
    int TTL = 0;
    string previousRouter="";
    actualSimulationStep = 0;

    packetSimulated.setTexture(_data->assets.GetTexture("iconPacketSmall"));

    if(ObjectByIp(ipOrigin).type != "null"){
        actual = ObjectByIp(ipOrigin);
    }else
        return;
    if(ObjectByIp(ipDestiny).type != "null"){
        destiny = ObjectByIp(ipDestiny);
    }else
        return;

    simulationLog.push_back("Inicio transmissao de pacote:");
    simulationLog.push_back(" Origem: "+actual.ip);
    simulationLog.push_back("Destino: "+destiny.ip);
    simulationSteps.clear();
    simulationError = false;

    for(int i = 0; i<simulationLog.size(); i++){
        cout<<simulationLog[i]<<endl;
    }

    while(TTL++ < 30){
        cout<<"Salto ("<<TTL<<"): "<<actual.ip<<","<<actual.type<<endl;

        if(actual.ip == destiny.ip){
            simulationSteps.push_back(actual.ip);
            simulationLog.push_back("Salto("+to_string(TTL)+"): "+actual.ip);
            for(int i = 0; i<simulationSteps.size(); i++){
                cout<<simulationSteps[i]<<endl;
            }
            break;
        }

        //Não possui Conexão
        if(actual.ip == "" || actual.ip == "0.0.0.0"){
            simulationSteps.push_back(actual.ip);
            simulationLog.push_back("-----------------------------");
            simulationLog.push_back("Sem conexão");
            simulationLog.push_back("Salto("+to_string(TTL)+"): "+actual.ip);
            TTL = 30;
            break;
        }
        //Se não for um roteador e não estiver no fim da rede, então siga para o Roteador conectado
        else if(actual.type != "Router"){
            cout<<"forwarding to my router"<<endl;
            next = ObjectByIp(actual.routerIp);
        }
        //Se for um Roteador e o IP Destino estiver na sua lista, o próximo salto será o destino
        //else if(actual.IsInIpList(destiny.ip)){
        else if(actual.ip == destiny.routerIp){
            cout<<"router, connected with destiny"<<endl;
            next = destiny;
        }
        //Se for um Roteador e o IP Destino não estiver na sua lista, siga para o roteador mais próximo
        else if(GetNearestRouter(actual,previousRouter).type != "null"){
            cout<<"go to nearest router, excluding the last"<<endl;
            next = GetNearestRouter(actual,previousRouter);
            previousRouter = actual.ip;
        }else if(GetNearestRouter(actual,previousRouter).type == "null"){
            cout<<"go to nearest router, including the last"<<endl;
            next = GetNearestRouter(actual,"");
        }
        //Se não se encaixar em nenhuma característica anterior é porque está sem conexão
        else{
            cout<<"not connection"<<endl;
            next = actual;
            simulationSteps.push_back(actual.ip);
            simulationLog.push_back("-----------------------------");
            simulationLog.push_back("Salto("+to_string(TTL)+"): "+actual.ip);
            continue;
        }

        simulationSteps.push_back(actual.ip);
        simulationLog.push_back("-----------------------------");
        simulationLog.push_back("Salto("+to_string(TTL)+"): "+actual.ip);
        actual = next;
    }
    if(TTL >= 30){
        simulationError = true;
        simulationLog.push_back("-----------------------------");
        simulationLog.push_back("Destino Inacessivel");
    }else{
        simulationLog.push_back("-----------------------------");
        simulationLog.push_back("Pacote Transferido");
    }
    simulationLog.push_back("##############################");
}

Object SimulationState::ObjectByIp(string ip){
    for(int i = 0; i<obj.size(); i++){
        if(obj[i].ip == ip)
            return obj[i];
    }

    Object nullObject;
    nullObject.type = "null";
    return nullObject;
}

Object SimulationState::GetNearestRouter(Object objReference,string ipToIgnore){
    float nearestDistance = -1, faux;
    int nearestID;

    for(int i = 0; i<obj.size(); i++){
        if(obj[i].type == "Router" && obj[i].ip != objReference.ip && obj[i].ip != ipToIgnore){
            faux = sqrt(pow(obj[i].sprite.getPosition().x-objReference.sprite.getPosition().x,2)+pow(obj[i].sprite.getPosition().y-objReference.sprite.getPosition().y,2));
            if(faux < nearestDistance || nearestDistance == -1){
                nearestDistance = faux;
                nearestID = i;
            }
        }
    }
    if(nearestDistance == -1 || nearestDistance>objReference.range.getRadius()){
        Object nullObject;
        nullObject.type = "null";
        return nullObject;
    }else{
        return obj[nearestID];
    }
}

void SimulationState::Load(){
    std::ifstream file;
    string line, type, ip, routerIP;
    float x, y;
    sf::Sprite spriteAux;
    vector<string> ipList;

    file.open("simulations/"+projectName+".txt");

    while(!file.eof() && !file.bad()){
        getline(file,line);

        if(line.size() == 0)
            continue;

        if(line.find("/") != string::npos){
            continue;
        }

        type = line.substr(0,line.find(";"));
        line = line.substr(line.find(";")+1);

        ip = line.substr(0,line.find(";"));
        line = line.substr(line.find(";")+1);

        routerIP = line.substr(0,line.find(";"));
        line = line.substr(line.find(";")+1);

        x = stod(line.substr(0,line.find(",")));
        y = stod(line.substr(line.find(",")+1));
        spriteAux.setPosition(sf::Vector2f(x,y));
        line = line.substr(line.find(";")+1);

        if(type == "Router"){
            while(1){
                if(line.find(",") == string::npos){
                    ipList.push_back(line);
                    break;
                }
                ipList.push_back(line.substr(0,line.find(",")));
                line = line.substr(line.find(",")+1);
            }
        }

        spriteAux.setTexture(_data->assets.GetTexture("icon"+type));

        if(type == "Router"){
            obj.push_back(Object(_data,ip,ipList,spriteAux));
        }else{
            obj.push_back(Object(_data,type,ip,routerIP,spriteAux));
        }
    }
}

void SimulationState::SaveLog(){
    ofstream file;

    file.open("logs/"+projectName+".txt",ios::out);

    for(int i = 0; i < simulationLog.size(); i++){
        file<<simulationLog[i]<<endl;
    }
}

void SimulationState::Draw(float dt){
    _data->window.clear();

    _data->window.draw(background);

    _data->window.setView(objView);
    for(int i = 0; i < obj.size(); i++){
        obj[i].Draw();
    }
    if(showPacketLine)
        _data->window.draw(packetLine,2,sf::Lines);

    _data->window.setView(uiView);

    _data->window.draw(selectionBarBackground);
    _data->window.draw(closeButton);
    _data->window.draw(playButton);
    _data->window.draw(packetIcon);
    _data->window.draw(helpText);
    _data->window.draw(packetSimulated);

    if(creatingPackage)
        _data->window.draw(newObject);

    _data->window.draw(statsWindow);
    _data->window.draw(statsText[0]);
    _data->window.draw(statsText[1]);
    _data->window.draw(simulationSpeedBar);
    _data->window.draw(simulationSpeedPointer);
    _data->window.draw(simulationSpeedText);

    _data->window.display();
}







