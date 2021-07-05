#include "MainMenuState.h"
#include "EditorState.h"
#include "SimulationState.h"

MainMenuState::MainMenuState(GameDataRef data) : _data(data){
    Load();
}

void MainMenuState::Init(){
    buttonSimulacao.setTexture(_data->assets.GetTexture("buttonSimulacao"));
    buttonEditor.setTexture(_data->assets.GetTexture("buttonEditor"));
    buttonCreditos.setTexture(_data->assets.GetTexture("buttonCreditos"));
    buttonSair.setTexture(_data->assets.GetTexture("buttonSair"));
    buttonOpcoes.setTexture(_data->assets.GetTexture("buttonOpcoes"));
    buttonReturn.setTexture(_data->assets.GetTexture("buttonReturn"));
    newSimulation.setTexture(_data->assets.GetTexture("buttonNewSimulation"));
    buttonDeleteSimulation.setTexture(_data->assets.GetTexture("buttonDeleteSimulation"));
    buttonSelect.setTexture(_data->assets.GetTexture("buttonSelect"));
    newFileBackground.setTexture(_data->assets.GetTexture("newFileBackground"));
    logo.setTexture(_data->assets.GetTexture("iconMenu"));

    textInputed.setFont(_data->assets.GetFont("arial"));
    helpText.setFont(_data->assets.GetFont("arial"));

    logo.setPosition((SCREEN_WIDTH/2)-(logo.getGlobalBounds().width/2),10);
    buttonSimulacao.setPosition((SCREEN_WIDTH/2)-(buttonSimulacao.getGlobalBounds().width/2),
                                (SCREEN_HEIGHT/2.3)-(buttonSimulacao.getGlobalBounds().height/2));
    buttonEditor.setPosition(buttonSimulacao.getPosition().x,buttonSimulacao.getPosition().y+buttonSimulacao.getGlobalBounds().height+10);
    buttonOpcoes.setPosition(-100,-100);
    buttonCreditos.setPosition(buttonEditor.getPosition().x,buttonEditor.getPosition().y+buttonEditor.getGlobalBounds().height+10);
    buttonSair.setPosition(buttonCreditos.getPosition().x,buttonCreditos.getPosition().y+buttonCreditos.getGlobalBounds().height+10);

    buttonReturn.setPosition(SCREEN_WIDTH-buttonReturn.getGlobalBounds().width-10,
                                SCREEN_HEIGHT-buttonReturn.getGlobalBounds().height-10);

    newFileBackground.setPosition(0,0);
    buttonSelect.setPosition(SCREEN_WIDTH-buttonSelect.getGlobalBounds().width-10,10);

    textInputed.setOutlineColor(sf::Color::Black);
    textInputed.setOutlineThickness(1);

    int creditsTextSize = 24;
    vector<int> highlited;
    highlited.push_back(creditsText.size());
    creditsText.push_back(sf::Text("Developed by Gabriel Sousa",_data->assets.GetFont("arial"),creditsTextSize));
    creditsText.push_back(sf::Text("github.com/JGSS-GabrielSousa",_data->assets.GetFont("arial"),creditsTextSize));
    creditsText.push_back(sf::Text("",_data->assets.GetFont("arial"),creditsTextSize));
    creditsText.push_back(sf::Text("",_data->assets.GetFont("arial"),creditsTextSize));
    highlited.push_back(creditsText.size());
    creditsText.push_back(sf::Text("Powered by BEngine v0.2 @ 04/2020",_data->assets.GetFont("arial"),creditsTextSize));
    creditsText.push_back(sf::Text("BWNet Simulator v2.0 @ 06/2021",_data->assets.GetFont("arial"),creditsTextSize));

    int x = SCREEN_WIDTH/2;
    int y = 50;

    for(int i = 0; i<creditsText.size(); i++){
        creditsText[i].setOrigin(creditsText[i].getGlobalBounds().width/2,creditsText[i].getGlobalBounds().height/2);
        creditsText[i].setPosition(x,y);
        creditsText[i].setFillColor(sf::Color::White);
        for(int j=0;j<highlited.size();j++){
            if(i==highlited[j])
                creditsText[i].setFillColor(sf::Color(251,197,44,255));
        }
        creditsText[i].setOutlineColor(sf::Color::Black);
        creditsText[i].setOutlineThickness(1);
        y += 40;
    }
}

void MainMenuState::HandleInput(){
    sf::Event event;

    while(_data->window.pollEvent(event)){
        if(sf::Event::Closed == event.type){
            _data->window.close();
        }
        if(_data->input.IsSpriteClicked(buttonSimulacao,sf::Mouse::Left,_data->window)){
            if(showCredits == false && showSelectSimulation == false){
                _data->assets.PlayAudio("click");
                showSelectSimulation = true;
                menuSelection = "simulation";
                newSimulation.setPosition(-500,0);
                buttonDeleteSimulation.setPosition(10,10);
            }
        }
        else if(_data->input.IsSpriteClicked(buttonEditor,sf::Mouse::Left,_data->window)){
            if(showCredits == false && showSelectSimulation == false){
                _data->assets.PlayAudio("click");
                showSelectSimulation = true;
                menuSelection = "editor";
                newSimulation.setPosition(10,10);
                buttonDeleteSimulation.setPosition(10,100);
            }
        }
        else if(_data->input.IsSpriteClicked(buttonCreditos,sf::Mouse::Left,_data->window)){
            if(showSelectSimulation == false){
                _data->assets.PlayAudio("click");
                showCredits = true;
            }
        }
        else if(_data->input.IsSpriteClicked(buttonOpcoes,sf::Mouse::Left,_data->window)){
            if(showCredits == false && showSelectSimulation == false){
                _data->assets.PlayAudio("click");
                showOptions = true;
                Options("show");
            }
        }
        else if(_data->input.IsSpriteClicked(buttonSair,sf::Mouse::Left,_data->window)){
            if(showCredits == false && showSelectSimulation == false){
                _data->assets.PlayAudio("click");
                exit(0);
            }
        }else if(_data->input.IsSpriteClicked(buttonReturn,sf::Mouse::Left,_data->window)){
            if(showCredits || showSelectSimulation || showOptions){
                _data->assets.PlayAudio("click");
                showCredits = false;
                showSelectSimulation = false;
                showOptions = false;
            }
        }else if(_data->input.IsSpriteClicked(newSimulation,sf::Mouse::Left,_data->window)){
            if(showSelectSimulation){
                _data->assets.PlayAudio("click");
                insertingText = true;
                textInputed.setString("");
            }
        }
        else if(_data->input.IsSpriteClicked(buttonDeleteSimulation,sf::Mouse::Left,_data->window)){
            if(showSelectSimulation){
                _data->assets.PlayAudio("click");
                DeleteSimulation();
            }
        }

        if(showSelectSimulation){
            if(_data->input.IsOverSprite(newSimulation,_data->window)){
                helpText.setString("Create New Simulation");
            }
            else if(_data->input.IsOverSprite(buttonSelect,_data->window)){
                if(insertingText)
                    helpText.setString("Create");
                else
                    helpText.setString("Load");
            }
            else if(_data->input.IsOverSprite(buttonReturn,_data->window)){
                helpText.setString("Return");
            }
            else if(insertingText){
                helpText.setString("Enter the file name");
            }
            else if(_data->input.IsOverSprite(buttonDeleteSimulation,_data->window)){
                helpText.setString("Delete Simulation");
            }
            else{
                helpText.setString("Select a Simulation");
            }

            if(_data->input.IsSpriteClicked(buttonSelect,sf::Mouse::Left,_data->window)){
                _data->assets.PlayAudio("click");
                if(showSelectSimulation && simulationSelected != -1)
                    MenuSelectSimulation();
                else if(showSelectSimulation && insertingText){
                    MenuSelectSimulation();
                }
            }

            helpText.setPosition(SCREEN_WIDTH/2,20);
            helpText.setOrigin(helpText.getGlobalBounds().width/2,helpText.getGlobalBounds().height/2);
        }

        if(insertingText){
            if(event.type == sf::Event::TextEntered){
                if(event.text.unicode>64 && event.text.unicode<128 || event.text.unicode>47 && event.text.unicode<58)
                    textInputed.setString(textInputed.getString()+static_cast<char>(event.text.unicode));
            }
            if(event.type == sf::Event::KeyPressed){
                if(textInputed.getString().getSize() > 0){
                    if(event.key.code == sf::Keyboard::Backspace){
                        textInputed.setString(textInputed.getString().substring(0,textInputed.getString().getSize()-1));
                    }
                }
            }
            simulationSelected = -1;
        }
    }
}

void MainMenuState::Update(float dt){
    if(insertingText)
        InsertText();

}

void MainMenuState::Options(string){
    optionsText[0].setPosition(SCREEN_WIDTH/4,SCREEN_HEIGHT/4);
    optionsText[1].setPosition(SCREEN_WIDTH,SCREEN_WIDTH);
    optionsText[2].setPosition(SCREEN_WIDTH,SCREEN_WIDTH);
    optionsText[3].setPosition(SCREEN_WIDTH,SCREEN_WIDTH);
}

void MainMenuState::Draw(float dt){
    _data->window.clear(sf::Color(0,19,127,0));

    if(showCredits){
        _data->window.draw(buttonReturn);
        for(int i = 0; i<creditsText.size(); i++)
            _data->window.draw(creditsText[i]);

    }else if(showSelectSimulation){
        _data->window.draw(newSimulation);
        _data->window.draw(buttonDeleteSimulation);
        _data->window.draw(buttonReturn);
        _data->window.draw(buttonSelect);
        _data->window.draw(helpText);

        float x = SCREEN_WIDTH/2;
        float y = 80;

        for(int i = 0; i<simulationList.size(); i++){
            simulationList[i].setOrigin(simulationList[i].getGlobalBounds().width/2,simulationList[i].getGlobalBounds().height/2);
            simulationList[i].setPosition(x,y);
            y += 40;

            if(i == simulationSelected)
                simulationList[i].setFillColor(sf::Color::Red);
            else
                simulationList[i].setFillColor(sf::Color(251,197,44,255));

            _data->window.draw(simulationList[i]);

            if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){

                sf::IntRect tempRect(simulationList[i].getGlobalBounds());

                if(tempRect.contains(sf::Mouse::getPosition(_data->window))){
                    simulationSelected = i;
                }
            }
        }
    }
    else if(showOptions){
        _data->window.draw(buttonReturn);
        _data->window.draw(optionFullscreen);
        _data->window.draw(optionsText[0]);
        _data->window.draw(optionsText[1]);
        _data->window.draw(optionsText[2]);
    }
    else{
        simulationSelected = -1;
        _data->window.draw(buttonSimulacao);
        _data->window.draw(buttonEditor);
        _data->window.draw(buttonCreditos);
        _data->window.draw(buttonSair);
        _data->window.draw(buttonOpcoes);
        _data->window.draw(logo);
    }

    if(insertingText){
        _data->window.draw(newFileBackground);
        _data->window.draw(textInputed);
    }

    _data->window.display();
}

void MainMenuState::MenuSelectSimulation(){
    string filename;
    bool newSimulation;

    if(insertingText){
        filename = textInputed.getString();
        newSimulation = true;
    }else{
        filename = simulationList[simulationSelected].getString();
        newSimulation = false;
    }

    if(menuSelection == "editor")
        _data->machine.AddState(StateRef(new EditorState(_data,filename,newSimulation)),true);
    else{
        _data->machine.AddState(StateRef(new SimulationState(_data,filename)),true);
    }
}

void MainMenuState::InsertText(){
    textInputed.setOrigin(textInputed.getGlobalBounds().width/2,0);
    textInputed.setPosition(SCREEN_WIDTH/2,SCREEN_HEIGHT/2);

    if(textInputed.getString().getSize() > 16){
        textInputed.setString(textInputed.getString().substring(0,textInputed.getString().getSize()-1));
    }
    if(showSelectSimulation == false){
        insertingText = false;
    }
}

void MainMenuState::DeleteSimulation(){
    if(simulationSelected != -1){
        string aux;

        aux = "simulations/"+simulationList[simulationSelected].getString()+".sim";
        remove(const_cast<char*>(aux.c_str()));
        aux = "logs/"+simulationList[simulationSelected].getString()+".txt";
        remove(const_cast<char*>(aux.c_str()));
        Load();
    }
}

void MainMenuState::Load(){
    ifstream file;
    ofstream fileResave;
    string line;
    sf::Text aux;

    file.open("simulations/saveConfig.dat");
    if(!file.is_open()){
        fileResave.open("simulations/saveConfig.dat", ios::out);
        for(int i = 0; i<simulationList.size(); i++){
            line = simulationList[i].getString();
            fileResave<<line<<endl;
        }
        return;
    }

    simulationList.clear();

    while(!file.eof() && !file.bad()){
        getline(file,line);

        if(line.size() == 0)
            continue;

        ifstream fileTest("simulations/"+line+".sim");
        if(!fileTest.good()){
            continue;
        }

        aux.setString(line);
        aux.setFont(_data->assets.GetFont("arial"));
        aux.setOutlineColor(sf::Color::Black);
        aux.setOutlineThickness(2);

        simulationList.push_back(aux);
    }
    fileResave.open("simulations/saveConfig.dat", ios::out);
    for(int i = 0; i<simulationList.size(); i++){
        line = simulationList[i].getString();
        fileResave<<line<<endl;
    }
}

