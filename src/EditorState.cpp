#include "EditorState.h"
#include "MainMenuState.h"

EditorState::EditorState(GameDataRef data,string projectName,bool newSimulation) : _data(data){
    addingObject = false;
    this->projectName = projectName;
    this->newSimulation = newSimulation;

    if(!newSimulation){
        Load();
    }
}

void EditorState::Init(){
    background.setTexture(_data->assets.GetTexture("background"));
    selectionBarBackground.setTexture(_data->assets.GetTexture("selectionBarBackground"));
    closeButton.setTexture(_data->assets.GetTexture("buttonClose"));
    saveButton.setTexture(_data->assets.GetTexture("iconSave"));
    deleteButton.setTexture(_data->assets.GetTexture("buttonDelete"));

    computerIcon.setTexture(_data->assets.GetTexture("iconComputer"));
    smartphoneIcon.setTexture(_data->assets.GetTexture("iconSmartphone"));
    routerIcon.setTexture(_data->assets.GetTexture("iconRouter"));

    statsWindow.setTexture(&_data->assets.GetTexture("blackScreen"),false);

    helpText.setFont(_data->assets.GetFont("arial"));
    statsText[0].setFont(_data->assets.GetFont("arial"));
    statsText[1].setFont(_data->assets.GetFont("arial"));

    closeButton.setScale(0.8,0.8);
    background.setScale(SCREEN_WIDTH/2,SCREEN_HEIGHT/2);
    statsWindow.setSize(sf::Vector2f(300,100));

    background.setPosition(0,selectionBarBackground.getGlobalBounds().height);
    selectionBarBackground.setPosition(0,0);
    closeButton.setPosition(SCREEN_WIDTH-closeButton.getGlobalBounds().width,0);
    saveButton.setPosition(SCREEN_WIDTH-128,0);
    deleteButton.setPosition(SCREEN_WIDTH-192,0);

    computerIcon.setPosition(selectionBar::pos1,0);
    smartphoneIcon.setPosition(selectionBar::pos2,0);
    routerIcon.setPosition(selectionBar::pos3,0);

    helpText.setColor(sf::Color::Black);
    helpText.setOutlineColor(sf::Color::White);
    helpText.setOutlineThickness(1);
    helpText.setPosition(10,SCREEN_HEIGHT-40);
}

void EditorState::HandleInput(){
    sf::Event event;

    while(_data->window.pollEvent(event)){
        if(sf::Event::Closed == event.type){
            _data->window.close();
        }
        if(_data->input.IsSpriteClicked(closeButton,sf::Mouse::Left,_data->window)){
            if(clock.getElapsedTime().asSeconds()>1){
                cursor.loadFromSystem(sf::Cursor::Arrow);
                _data->window.setMouseCursor(cursor);
                _data->assets.PlayAudio("click");
                _data->machine.AddState(StateRef(new MainMenuState(_data)),true);
            }
        }

        //Selection Bar items
        if(_data->input.IsSpriteClicked(computerIcon,sf::Mouse::Left,_data->window)){
            newObject.setTexture(_data->assets.GetTexture("iconComputer"));
            addingObject = true;
            newObjectType = "Computer";
            helpText.setString("Laptop");
        }
        else if(_data->input.IsSpriteClicked(smartphoneIcon,sf::Mouse::Left,_data->window)){
            newObject.setTexture(_data->assets.GetTexture("iconSmartphone"));
            addingObject = true;
            newObjectType = "Smartphone";
            helpText.setString("Smartphone");
        }
        else if(_data->input.IsSpriteClicked(routerIcon,sf::Mouse::Left,_data->window)){
            newObject.setTexture(_data->assets.GetTexture("iconRouter"));
            addingObject = true;
            newObjectType = "Router";
            helpText.setString("Router");
        }
        else if(_data->input.IsSpriteClicked(deleteButton,sf::Mouse::Left,_data->window)){
            newObject.setTexture(_data->assets.GetTexture("buttonDelete"));
            deletingObject = true;
            helpText.setString("Delete");
        }

        showStatsWindow = false;
        for(int i = 0; i<obj.size(); i++){
            if(_data->input.IsSpriteClicked(obj[i].sprite,sf::Mouse::Left,_data->window)){
                if(deletingObject){
                    DeleteObject(i);
                    break;
                }
            }
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


        if(_data->input.IsSpriteClicked(background,sf::Mouse::Left,_data->window)){
            if(addingObject){
                AddObject();
            }
            if(deletingObject){
                deletingObject = false;
            }
        }

        if(_data->input.IsSpriteClicked(saveButton,sf::Mouse::Left,_data->window)){
            Save();
        }
    }
}

void EditorState::Update(float dt){
    newObject.setPosition(sf::Vector2f(_data->input.GetMousePosition(_data->window).x - (newObject.getGlobalBounds().width/2),
                                       _data->input.GetMousePosition(_data->window).y - (newObject.getGlobalBounds().height/2)));


    if(showStatsWindow){
        statsWindow.setPosition(SCREEN_WIDTH-statsWindow.getSize().x,SCREEN_HEIGHT-statsWindow.getSize().y);
        statsText[0].setPosition(SCREEN_WIDTH-statsWindow.getSize().x+10,SCREEN_HEIGHT-statsWindow.getSize().y+10);
        statsText[1].setPosition(SCREEN_WIDTH-statsWindow.getSize().x+10,SCREEN_HEIGHT-statsWindow.getSize().y+50);
    }else{
        statsWindow.setPosition(0,-200);
        statsText[0].setPosition(0,-200);
        statsText[1].setPosition(0,-200);
    }
}

void EditorState::AddObject(){
    sf::Sprite spriteAux;
    Object objAux;
    string aux;
    float closest = -1,faux;
    int closestID;
    vector<string> ips;

    aux = "icon"+newObjectType;
    spriteAux.setTexture(_data->assets.GetTexture(aux));
    spriteAux.setPosition(newObject.getPosition());

    if(newObjectType != "Router")
        obj.push_back(Object(_data,newObjectType,"","",spriteAux));
    else
        obj.push_back(Object(_data,GetNewRouterIP(),ips,spriteAux));


    for(int i = 0; i<obj.size(); i++){
        closest = -1;

        if(obj[i].type != "Router"){
            for(int j = 0; j < obj.size(); j++){
                objAux = obj[j];
                if(objAux.type == "Router"){
                    faux = sqrt(pow(objAux.sprite.getPosition().x-obj[i].sprite.getPosition().x,2)+pow(objAux.sprite.getPosition().y-obj[i].sprite.getPosition().y,2));
                    if(faux < closest || closest == -1){
                        closest = faux;
                        closestID = j;
                    }
                }
            }
            if(closest != -1){
                if(closest <= obj[closestID].range.getRadius()){
                    obj[i].routerIp = obj[closestID].ip;
                    obj[i].ip = obj[closestID].GetNewIP();
                }
            }
        }
    }

    for(int i = 0; i<obj.size(); i++){
        if(obj[i].type == "Router"){
            obj[i].ipsInRouter.clear();
            for(int j = 0;j<obj.size();j++){
                objAux = obj[j];
                if(objAux.routerIp == obj[i].ip){
                    obj[i].ipsInRouter.push_back(objAux.ip);
                }
            }
        }
    }

    addingObject = false;
}

void EditorState::DeleteObject(int id){
    obj.erase(obj.begin()+id);
    deletingObject = false;
}

string EditorState::GetNewRouterIP(){
    ipListRouters++;
    return "10.0.0."+to_string(ipListRouters);
}

void EditorState::Load(){
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
            ipListRouters = stoi(line.substr(0,line.find("/")));
            line = line.substr(line.find("/")+1);
            ipListUsers = stoi(line.substr(line.find("/")+1));
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

void EditorState::Save(){
    cursor.loadFromSystem(sf::Cursor::ArrowWait);
    _data->window.setMouseCursor(cursor);

    ofstream file,file2;

    file.open("simulations/"+projectName+".txt");

    file<<ipListRouters<<"/"<<ipListUsers<<endl;

    for(int i = 0; i < obj.size(); i++){
        file<<obj[i].Save()<<endl;
    }

    if(newSimulation){
        file2.open("simulations/saveConfig.dat",ios_base::app);
        file2<<projectName<<endl;
        newSimulation = false;
    }

    cursor.loadFromSystem(sf::Cursor::Arrow);
    _data->window.setMouseCursor(cursor);
}

void EditorState::Draw(float dt){
    _data->window.clear();

    _data->window.draw(background);

    for(int i = 0; i < obj.size(); i++){
        obj[i].Draw();
    }

    _data->window.draw(selectionBarBackground);
    _data->window.draw(closeButton);
    _data->window.draw(saveButton);
    _data->window.draw(deleteButton);
    _data->window.draw(computerIcon);
    _data->window.draw(smartphoneIcon);
    _data->window.draw(routerIcon);

    if(addingObject || deletingObject){
        newObject.setScale(0.5,0.5);
        cursor.loadFromSystem(sf::Cursor::Cross);
        _data->window.setMouseCursor(cursor);
        _data->window.draw(helpText);
        _data->window.draw(newObject);
    }else{
        cursor.loadFromSystem(sf::Cursor::Arrow);
        _data->window.setMouseCursor(cursor);
    }
    _data->window.draw(statsWindow);
    _data->window.draw(statsText[0]);
    _data->window.draw(statsText[1]);

    _data->window.display();
}







