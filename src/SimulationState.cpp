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
    resendIcon.setTexture(_data->assets.GetTexture("iconResend"));
    resendIcon1.setTexture(_data->assets.GetTexture("iconResend1"));
    resendIcon2.setTexture(_data->assets.GetTexture("iconResend2"));
    packetSimulated.setTexture(_data->assets.GetTexture("iconPacketSmall"));
    statsWindow.setTexture(&_data->assets.GetTexture("blackScreen"),false);
    simulationSpeedBar.setTexture(&_data->assets.GetTexture("blackScreen"),false);
    simulationSpeedPointer.setTexture(_data->assets.GetTexture("simulationSpeedPointer"));
    deleteButton.setTexture(_data->assets.GetTexture("buttonDelete"));

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
    resendIcon1.setPosition(packetIcon.getGlobalBounds().width+20+resendIcon1.getGlobalBounds().width/2,resendIcon1.getGlobalBounds().height/2);
    resendIcon2.setPosition(packetIcon.getGlobalBounds().width+20,0);
    deleteButton.setPosition(148,0);
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

    resendIcon1.setOrigin(resendIcon1.getGlobalBounds().width/2,resendIcon1.getGlobalBounds().height/2);

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
                _data->assets.PlayAudio("click");
                _data->machine.AddState(StateRef(new MainMenuState(_data)),true);
            }
        }

        showStatsWindow = false;
        for(int i = 0; i < obj.size(); i++){
            if(_data->input.IsOverSprite(obj[i].sprite,_data->window)){
                statsText[0].setString("IP: "+obj[i].ip);
                if(obj[i].type != "Router")
                    statsText[1].setString("Router: "+obj[i].routerIp);
                else
                    statsText[1].setString("");
                showStatsWindow = true;
                break;
            }
        }

        if(_data->input.IsSpriteClicked(playButton,sf::Mouse::Left,_data->window))
            PlayButtonFunc();
        if(_data->input.IsSpriteClicked(resendIcon,sf::Mouse::Left,_data->window))
            ResendButtonFunc();
        if(_data->input.IsSpriteClicked(deleteButton,sf::Mouse::Left,_data->window))
            DeleteButtonFunc();

        PacketButtonFunc();
        SimulationSpeedFunc();
    }
}


void SimulationState::Update(float dt){
    if(movingCamera)
        objView.setCenter(sf::Vector2f(_data->window.mapPixelToCoords(sf::Vector2i(sf::Mouse::getPosition()))));

    newObject.setPosition(_data->input.GetMousePosition(_data->window,uiView));
    if(deletingObject){
        creatingPackage = false;
        newObject.setTexture(_data->assets.GetTexture("buttonDelete"));
        ObjectDeletion();
    }else
        newObject.setTexture(_data->assets.GetTexture("iconPacket"));

    if(packetMovementAnimation)
        PacketMovement();

    if(playButtonVisible){
        playButton.setPosition(SCREEN_WIDTH-128,0);
        resendIcon.setPosition(packetIcon.getGlobalBounds().width+20,0);
    }
    else{
        playButton.setPosition(SCREEN_WIDTH-128,-1000);
        resendIcon.setPosition(SCREEN_WIDTH-128,-1000);
    }

    if(resending && playButtonVisible && actualSimulationStep == -1){
        if(resendingClock.getElapsedTime().asMilliseconds() > ResendingTick){
            Simulation(origin, destiny);
            PlayButtonFunc();
            resendingClock.restart();
        }
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

    aux = to_string(simulationSpeed);
    aux = aux.substr(0,aux.find(".")+2);
    simulationSpeedText.setString("Simulation Speed: "+aux);
    simulationSpeedText.setPosition(SCREEN_WIDTH/2-simulationSpeedText.getGlobalBounds().width/2,2);

    int RoutingTablesUpdateTick = ROUTING_TICK/simulationSpeed;
    if(RoutingTablesUpdateClock.getElapsedTime().asMilliseconds() > RoutingTablesUpdateTick){
        UpdateRoutingTables();
        RoutingTablesUpdateClock.restart();
    }
}


void SimulationState::ObjectDeletion(){
    for(int i = 0; i < obj.size(); i++){
        if(_data->input.IsSpriteClicked(obj[i].sprite,sf::Mouse::Left,_data->window)){
            for(int j = 0; j < simulationSteps.size(); j++){
                if(simulationSteps[j] == obj[i].ip){
                    simulationSteps.clear();
                    break;
                }
            }
            Simulation(origin, destiny);

            simulationLog.push_back(obj[i].ip+" has been deleted");
            helpText.setString("");

            for(int j = 0; j < obj.size(); j++){
                if(obj[j].routerIp == obj[i].ip){
                    string oldIp = obj[j].ip;
                    obj[j].routerIp = "0.0.0.0";
                    obj[j].ip = "0.0.0.0";

                    float minimumRouterDistance = 0;
                    float routerDistance;
                    int closestRouter = -1;
                    int k = 0;
                    for(; k < obj.size(); k++){
                        routerDistance = Distance(obj[j].sprite, obj[k].sprite);
                        if(routerDistance <= obj[k].range.getRadius() && routerDistance < minimumRouterDistance){
                            closestRouter = k;
                        }
                        else if(routerDistance <= obj[k].range.getRadius() && minimumRouterDistance == 0){
                            closestRouter = k;
                        }
                    }

                    if(closestRouter != -1){
                        obj[j].routerIp = obj[closestRouter].ip;
                        obj[j].ip = obj[closestRouter].GetNewIP();
                    }

                    if(oldIp == destiny){
                        destiny = obj[j].ip;
                    }
                    else if(oldIp == origin){
                        origin = obj[j].ip;
                    }
                }
            }

            obj.erase(obj.begin()+i);
            deletingObject = false;
            break;
        }
    }
}


void SimulationState::CreatePacket(string action, sf::Vector2f point){
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
        packetSimulated.setPosition(0, -80);
        packetLine[0] = sf::Vertex(point, sf::Color::Green);
        auxClock.restart();
    }
    else if(action == "destiny" && auxClock.getElapsedTime().asSeconds() > 1){
        creatingPackageDestiny = false;
        creatingPackage = false;
        showPacketLine = true;
        Simulation(origin, destiny);
        playButtonVisible = true;
        packetLine[1] = sf::Vertex(point, sf::Color::Green);
        helpText.setString("");
    }
}


void SimulationState::PacketMovement(){
    float movSpeed = simulationSpeed;
    float distance;

    if(ObjectRefByIp(simulationSteps[actualSimulationStep]) == nullptr){
        distance = 0;
        simulationError = true;
        actualSimulationStep = simulationSteps.size()+1;
    }
    else{
        sf::Vector2f targetPos = ObjectRefByIp(simulationSteps[actualSimulationStep])->sprite.getPosition();
        sf::Vector2f actualPos = packetSimulated.getPosition();
        distance = Distance(targetPos,actualPos);

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

    if(distance < 10){
        actualSimulationStep++;
    }
    if(actualSimulationStep >= simulationSteps.size()){
        packetMovementAnimation = false;
        actualSimulationStep = -1;
        if(simulationError)
            packetSimulated.setTexture(_data->assets.GetTexture("iconPacketSmallRed"));
        else if(!simulationError && showPacketLine && actualSimulationStep != simulationSteps.size())
            packetSimulated.setTexture(_data->assets.GetTexture("iconPacketSmallGreen"));
    }
}


void SimulationState::Simulation(string ipOrigin,string ipDestiny){
    Object* destiny;
    Object* actual;
    unsigned char TTL = 0;
    vector<string> route;

    packetSimulated.setTexture(_data->assets.GetTexture("iconPacketSmall"));

    if(ObjectRefByIp(ipOrigin) != nullptr &&
       ObjectRefByIp(ipDestiny) != nullptr &&
       ObjectRefByIp(ipOrigin)->type != "null" &&
       ObjectRefByIp(ipDestiny)->type != "null"){
        actual = ObjectRefByIp(ipOrigin);
        destiny = ObjectRefByIp(ipDestiny);
    }else{
        simulationError = true;
        return;
    }

    simulationLog.push_back("##############################");
    simulationLog.push_back(" Origin: "+actual->ip);
    simulationLog.push_back("Destiny: "+destiny->ip);
    simulationLog.push_back("");
    simulationSteps.clear();

    string fullRoute;
    if(actual->type != "Router"){
        if(ObjectRefByIp(actual->routerIp) != nullptr)
            fullRoute = ObjectRefByIp(actual->routerIp)->GetRoute(ipDestiny);
        else
            fullRoute = "error";
    }
    else
        fullRoute = actual->GetRoute(ipDestiny);

    if(fullRoute != "error"){
        route.push_back(actual->ip);

        if(actual->type != "Router")
            route.push_back(actual->routerIp);

        while(fullRoute != "L"){
            route.push_back(fullRoute.substr(0,fullRoute.find("-")));
            fullRoute = fullRoute.substr(fullRoute.find("-")+1);
        }
        route.push_back(destiny->ip);
    }

    while(TTL++ < 64){
        if(route.size() == 0){
            simulationSteps.push_back(actual->ip);
            simulationLog.push_back("Failed Transmission: No connection");
            simulationError = true;
            break;
        }

        simulationSteps.push_back(route[0]);
        simulationLog.push_back("Hop("+to_string(TTL)+"): "+route[0]);
        route.erase(route.begin());

        if(route.size() == 0){
            simulationLog.push_back("Successful Transmission");
            simulationError = false;
            break;
        }
    }
    if(TTL > 64){
        simulationSteps.push_back(actual->ip);
        simulationLog.push_back("Failed Transmission: TTL exceeded");
        simulationError = true;
    }
}


void SimulationState::UpdateRoutingTables(){
    vector<pair<string,string>> newTable;

    for(int i = 0; i < obj.size(); i++){
        if(obj[i].type != "Router" || rand()%3 != 0) //this rand is to simulate the difference in update speed between the routers
            continue;

        newTable.clear();

        for(int j = 0; j < obj.size(); j++){
            if(Distance(obj[i].sprite, obj[j].sprite) <= obj[i].range.getRadius()){
                newTable.push_back(make_pair(obj[j].ip, "L"));

                for(int k = 0; k < obj[j].RoutingTable.size(); k++){
                    newTable.push_back(make_pair(obj[j].RoutingTable[k].first, obj[j].ip+"-"+obj[j].RoutingTable[k].second));
                }
            }

            string aux;
            if(obj[j].type == "Router"){
                for(int k = 0; k < obj[j].RoutingTable.size(); k++){
                    if(ObjectRefByIp(obj[j].RoutingTable[k].first) == nullptr){
                        obj[j].RoutingTable.erase(obj[j].RoutingTable.begin()+k);
                        continue;
                    }
                    aux = obj[j].RoutingTable[k].second;
                    while(aux != "L"){
                        if(ObjectRefByIp(aux.substr(0,aux.find("-"))) == nullptr){
                            obj[j].RoutingTable.erase(obj[j].RoutingTable.begin()+k);
                            break;
                        }

                        aux = aux.substr(aux.find("-")+1);
                    }
                }
            }
        }

        unsigned int updates = obj[i].UpdateTable(newTable);
        if(updates > 0)
            simulationLog.push_back(obj[i].ip+" updated "+to_string(updates)+" routes");
    }
}


Object* SimulationState::ObjectRefByIp(string ip){
    for(int i = 0; i<obj.size(); i++){
        if(obj[i].ip == ip)
            return &obj[i];
    }
    return nullptr;
}


float SimulationState::Distance(sf::Vector2f a, sf::Vector2f b){
    return sqrt(pow((a.x - b.x),2) + pow((a.y - b.y),2));
}

float SimulationState::Distance(sf::Sprite a, sf::Sprite b){
    return sqrt(pow((a.getPosition().x - b.getPosition().x),2) + pow((a.getPosition().y - b.getPosition().y),2));
}


void SimulationState::Load(){
    std::ifstream file;
    string line, type, ip, routerIP;
    float x, y;
    sf::Sprite spriteAux;
    vector<string> ipList;

    file.open("simulations/"+projectName+".sim");

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
    if(showPacketLine){
        _data->window.draw(packetLine,2,sf::Lines);

        sf::Vector2f aux;

        packetRoute = sf::VertexArray(sf::LinesStrip, simulationSteps.size());

        for(int i = 0; i < simulationSteps.size(); i++){
            packetRoute[i].color = sf::Color::Blue;

            if(ObjectRefByIp(simulationSteps[i]) != nullptr){
                aux = ObjectRefByIp(simulationSteps[i])->sprite.getPosition();
                aux.x += ObjectRefByIp(simulationSteps[i])->sprite.getGlobalBounds().width/2;
                aux.y += ObjectRefByIp(simulationSteps[i])->sprite.getGlobalBounds().height/2;
            }
            else if(i > 0){
                packetRoute[i-1].color = sf::Color::Transparent;
                packetRoute[i].color = sf::Color::Transparent;
                break;
            }

            packetRoute[i].position = aux;
        }
        _data->window.draw(packetRoute);
    }

    _data->window.setView(uiView);

    _data->window.draw(selectionBarBackground);
    _data->window.draw(closeButton);
    _data->window.draw(playButton);
    _data->window.draw(packetIcon);
    _data->window.draw(helpText);
    _data->window.draw(packetSimulated);
    _data->window.draw(deleteButton);

    if(resending){
        _data->window.draw(resendIcon1);
        resendIcon1.rotate(-3);
        _data->window.draw(resendIcon2);
    }else{
        _data->window.draw(resendIcon);
    }

    if(creatingPackage || deletingObject)
        _data->window.draw(newObject);

    _data->window.draw(statsWindow);
    _data->window.draw(statsText[0]);
    _data->window.draw(statsText[1]);
    _data->window.draw(simulationSpeedBar);
    _data->window.draw(simulationSpeedPointer);
    _data->window.draw(simulationSpeedText);

    _data->window.display();
}
