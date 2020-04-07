#include "MainMenuState.h"
#include "EditorState.h"

MainMenuState::MainMenuState(GameDataRef data) : _data(data){
    ifstream file;
    string line;
    sf::Text aux;

    file.open("saves/saveList.txt");

    while(!file.eof() && !file.bad()){
        getline(file,line);

        if(line.size() == 0)
            continue;

        aux.setString(line);
        aux.setFont(_data->assets.GetFont("arial"));
        aux.setOutlineColor(sf::Color::Black);
        aux.setOutlineThickness(2);

        simulationList.push_back(aux);//sf::Text(line,,30));
    }
}

void MainMenuState::Init(){
    buttonSimulacao.setTexture(_data->assets.GetTexture("buttonSimulacao"));
    buttonEditor.setTexture(_data->assets.GetTexture("buttonEditor"));
    buttonCreditos.setTexture(_data->assets.GetTexture("buttonCreditos"));
    buttonReturn.setTexture(_data->assets.GetTexture("buttonReturn"));
    newSimulation.setTexture(_data->assets.GetTexture("buttonNewSimulation"));
    deleteSimulation.setTexture(_data->assets.GetTexture("buttonDeleteSimulation"));
    buttonSelect.setTexture(_data->assets.GetTexture("buttonSelect"));
    newFileBackground.setTexture(_data->assets.GetTexture("newFileBackground"));

    textInputed.setFont(_data->assets.GetFont("arial"));
    helpText.setFont(_data->assets.GetFont("arial"));

    buttonSimulacao.setPosition((SCREEN_WIDTH/2)-(buttonSimulacao.getGlobalBounds().width/2),
                                (SCREEN_HEIGHT/4)-(buttonSimulacao.getGlobalBounds().height/2));
    buttonEditor.setPosition((SCREEN_WIDTH/2)-(buttonEditor.getGlobalBounds().width/2),
                                (SCREEN_HEIGHT/2)-(buttonEditor.getGlobalBounds().height/2));
    buttonCreditos.setPosition((SCREEN_WIDTH/2)-(buttonCreditos.getGlobalBounds().width/2),
                                (SCREEN_HEIGHT/1.3)-(buttonCreditos.getGlobalBounds().height/2));
    buttonReturn.setPosition(SCREEN_WIDTH-buttonReturn.getGlobalBounds().width-10,
                                SCREEN_HEIGHT-buttonReturn.getGlobalBounds().height-10);

    newFileBackground.setPosition(0,0);
    buttonSelect.setPosition(SCREEN_WIDTH-buttonSelect.getGlobalBounds().width-10,10);

    textInputed.setOutlineColor(sf::Color::Black);
    textInputed.setOutlineThickness(1);

    int creditsTextSize = 24;
    creditsText.push_back(sf::Text("Developed by Gabriel Sousa",_data->assets.GetFont("arial"),creditsTextSize));
    creditsText.push_back(sf::Text("github.com/JGSS-GabrielSousa",_data->assets.GetFont("arial"),creditsTextSize));
    creditsText.push_back(sf::Text("",_data->assets.GetFont("arial"),creditsTextSize));
    creditsText.push_back(sf::Text("",_data->assets.GetFont("arial"),creditsTextSize));
    creditsText.push_back(sf::Text("Special Thanks",_data->assets.GetFont("arial"),creditsTextSize));
    creditsText.push_back(sf::Text("Laurent Gomila by SFML",_data->assets.GetFont("arial"),creditsTextSize));
    creditsText.push_back(sf::Text("Bjarne Stroustrup by C++",_data->assets.GetFont("arial"),creditsTextSize));
    creditsText.push_back(sf::Text("",_data->assets.GetFont("arial"),creditsTextSize));
    creditsText.push_back(sf::Text("",_data->assets.GetFont("arial"),creditsTextSize));
    creditsText.push_back(sf::Text("Powered by BEngine v0.2",_data->assets.GetFont("arial"),creditsTextSize));
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
                deleteSimulation.setPosition(10,10);
            }
        }
        else if(_data->input.IsSpriteClicked(buttonEditor,sf::Mouse::Left,_data->window)){
            if(showCredits == false && showSelectSimulation == false){
                _data->assets.PlayAudio("click");
                showSelectSimulation = true;
                menuSelection = "editor";
                newSimulation.setPosition(10,10);
                deleteSimulation.setPosition(10,100);
            }
        }
        else if(_data->input.IsSpriteClicked(buttonCreditos,sf::Mouse::Left,_data->window)){
            if(showSelectSimulation == false){
                _data->assets.PlayAudio("click");
                showCredits = true;
            }
        }else if(_data->input.IsSpriteClicked(buttonReturn,sf::Mouse::Left,_data->window)){
            if(showCredits || showSelectSimulation){
                _data->assets.PlayAudio("click");
                showCredits = false;
                showSelectSimulation = false;
            }
        }else if(_data->input.IsSpriteClicked(newSimulation,sf::Mouse::Left,_data->window)){
            insertingText = true;
            textInputed.setString("");
        }

        if(showSelectSimulation){
            if(_data->input.IsOverSprite(newSimulation,_data->window)){
                helpText.setString("Criar Nova Simulação");
            }
            else if(_data->input.IsOverSprite(buttonSelect,_data->window)){
                helpText.setString("Carregar");
            }
            else if(_data->input.IsOverSprite(buttonReturn,_data->window)){
                helpText.setString("Retornar");
            }
            else if(insertingText){
                helpText.setString("Digite o nome do arquivo");
            }
            else if(_data->input.IsOverSprite(deleteSimulation,_data->window)){
                helpText.setString("Deletar Simulação");
            }
            else{
                helpText.setString("Selecione uma Simulação");
            }

            if(_data->input.IsSpriteClicked(buttonSelect,sf::Mouse::Left,_data->window)){
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

void MainMenuState::Draw(float dt){
    _data->window.clear(sf::Color(0,19,127,0));

    if(showCredits){
        float x = SCREEN_WIDTH/2;
        float y = 50;

        _data->window.draw(buttonReturn);

        for(int i = 0; i<creditsText.size(); i++){
            creditsText[i].setOrigin(creditsText[i].getGlobalBounds().width/2,creditsText[i].getGlobalBounds().height/2);
            creditsText[i].setPosition(x,y);
            creditsText[i].setFillColor(sf::Color::White);
            y += 40;
            _data->window.draw(creditsText[i]);
        }
    }else if(showSelectSimulation){
        _data->window.draw(newSimulation);
        _data->window.draw(deleteSimulation);
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
    }else{
        simulationSelected = -1;
        _data->window.draw(buttonSimulacao);
        _data->window.draw(buttonEditor);
        _data->window.draw(buttonCreditos);
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
        //_data->machine.AddState(StateRef(new SimulationState(_data)),true);
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











