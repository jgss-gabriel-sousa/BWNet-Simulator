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

    computerIcon.setTexture(_data->assets.GetTexture("iconComputer"));
    smartphoneIcon.setTexture(_data->assets.GetTexture("iconSmartphone"));
    routerIcon.setTexture(_data->assets.GetTexture("iconRouter"));

    helpText.setFont(_data->assets.GetFont("arial"));

    closeButton.setScale(0.8,0.8);
    background.setScale(SCREEN_WIDTH/2,SCREEN_HEIGHT/2);

    background.setPosition(0,selectionBarBackground.getGlobalBounds().height);
    selectionBarBackground.setPosition(0,0);
    closeButton.setPosition(SCREEN_WIDTH-closeButton.getGlobalBounds().width,0);
    saveButton.setPosition(SCREEN_WIDTH-128,0);

    computerIcon.setPosition(selectionBar::pos1,0);
    smartphoneIcon.setPosition(selectionBar::pos2,0);
    routerIcon.setPosition(selectionBar::pos3,0);

    helpText.setColor(sf::Color::Black);
    helpText.setOutlineColor(sf::Color::White);
    helpText.setOutlineThickness(1);
}

void EditorState::HandleInput(){
    sf::Event event;

    while(_data->window.pollEvent(event)){
        if(sf::Event::Closed == event.type){
            _data->window.close();
        }
        if(_data->input.IsSpriteClicked(closeButton,sf::Mouse::Left,_data->window)){
            _data->machine.AddState(StateRef(new MainMenuState(_data)),true);
            _data->assets.PlayAudio("click");
        }

        //Selection Bar items
        if(_data->input.IsSpriteClicked(computerIcon,sf::Mouse::Left,_data->window)){
            newObject.setTexture(_data->assets.GetTexture("iconComputer"));
            addingObject = true;
            newObjectType = "Computer";
            helpText.setString("Computador");
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
            helpText.setString("Roteador");
        }

        if(_data->input.IsSpriteClicked(background,sf::Mouse::Left,_data->window)){
            if(addingObject){
                AddObject();
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

    helpText.setPosition(SCREEN_WIDTH-10-helpText.getGlobalBounds().width,SCREEN_HEIGHT-10-helpText.getGlobalBounds().height);

    for(int i = 0; i < obj.size(); i++){
        ;
    }
}

void EditorState::AddObject(){
    sf::Sprite spriteAux;
    string aux;

    aux = "icon"+newObjectType;
    spriteAux.setTexture(_data->assets.GetTexture(aux));
    spriteAux.setPosition(newObject.getPosition());

    obj.push_back(Object(_data,newObjectType,"0",spriteAux));
    addingObject = false;
}

void EditorState::Load(){
    std::ifstream file;
    string line, type, ip;
    float x, y;
    sf::Sprite spriteAux;

    file.open("saves/"+projectName+".txt");

    while(!file.eof() && !file.bad()){
        getline(file,line);

        if(line.size() == 0)
            continue;

        type = line.substr(0,line.find(";"));
        line = line.substr(line.find(";")+1);
        ip = line.substr(0,line.find(";"));
        line = line.substr(line.find(";")+1);

        x = stod(line.substr(0,line.find(",")));
        y = stod(line.substr(line.find(",")+1));
        spriteAux.setPosition(sf::Vector2f(x,y));

        spriteAux.setTexture(_data->assets.GetTexture("icon"+type));
        obj.push_back(Object(_data,type,ip,spriteAux));
    }
}

void EditorState::Save(){
    cursor.loadFromSystem(sf::Cursor::ArrowWait);
    _data->window.setMouseCursor(cursor);

    ofstream file,file2;

    file.open("saves/"+projectName+".txt");

    for(int i = 0; i < obj.size(); i++){
        file<<obj[i].Save()<<endl;
        cout<<obj[i].Save()<<endl;
    }

    if(newSimulation){
        file2.open("saves/saveList.txt",ios_base::app);
        file2<<endl<<projectName<<endl;
        newSimulation = false;
    }


    // CHANGE CURSOR TO NORMAL
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
    _data->window.draw(computerIcon);
    _data->window.draw(smartphoneIcon);
    _data->window.draw(routerIcon);

    if(addingObject){
        newObject.setScale(0.5,0.5);
        cursor.loadFromSystem(sf::Cursor::Cross);
        _data->window.setMouseCursor(cursor);
        _data->window.draw(helpText);
        _data->window.draw(newObject);
    }else{
        cursor.loadFromSystem(sf::Cursor::Arrow);
        _data->window.setMouseCursor(cursor);
    }

    _data->window.display();
}







