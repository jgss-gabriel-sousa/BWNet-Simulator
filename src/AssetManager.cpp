#include "AssetManager.h"

AssetManager::AssetManager(){
    //Load standard items for errors when loading
    LoadTexture("missing");
    LoadFont("arial");
    LoadAudio("missing");
}

//################## TEXTURES #################################

void AssetManager::LoadTexture(std::string name, std::string fileName){
    sf::Texture tex;

    if(tex.loadFromFile(fileName)){
        LogPush("Texture Loaded to Asset Manager with tag: \""+name+"\" from: \""+fileName+"\"");
    }else{
        tex.loadFromFile(MISSING_TEXTURE_FILEPATH);
        LogPush("Texture not Loaded correctly to Asset Manager with tag: \""+name+"\" from: \""+fileName+"\" (Missing Texture applied)");
    }

    this->_textures[name] = tex;
}

void AssetManager::LoadTexture(std::string name){
    sf::Texture tex;

    std::string filePath = TEXTURES_FOLDER_FILEPATH+name+".png";

    if(tex.loadFromFile(filePath)){
        LogPush("Texture Loaded to Asset Manager with tag: \""+name+"\" from: \""+filePath+"\"");
    }else{
        tex = _textures.at("missing");
        LogPush("Texture not Loaded correctly to Asset Manager with tag: \""+name+"\" from: \""+filePath+"\" (Missing Texture applied)");
    }
    this->_textures[name] = tex;
}

sf::Texture& AssetManager::GetTexture(std::string name){
    auto found = _textures.find(name);

    if(found != _textures.end()){
        return this->_textures.at(name);
    }else{
        return this->_textures.at("Missing");
    }
}

//################## ANIMATIONS ####################################

void AssetManager::LoadAnimation(std::string name, int frames){
    sf::Texture tex;
    std::string filePath;
    std::string frameName;
    frames--;

    _animationsFrames[name] = frames+1;

    for(int i = 0; i <= frames; i++){
        filePath = TEXTURES_FOLDER_FILEPATH+name+"/"+std::to_string(i)+".png";

        if(tex.loadFromFile(filePath)){
            LogPush(filePath);
            LogPush("Frame "+std::to_string(i)+" of "+std::to_string(frames)+" correctly loaded");
        }else{
            LogPush("Frame "+std::to_string(i)+" of "+std::to_string(frames)+" error in loading");
            tex.loadFromFile(MISSING_TEXTURE_FILEPATH);
        }
        frameName = name+"-"+std::to_string(i);
        LogPush(frameName);
        _animations[frameName] = tex;
    }
}

sf::Texture& AssetManager::GetAnimation(std::string name, int frame){
    name = name+"-"+std::to_string(frame);
    auto found = _animations.find(name);

    if(found != _animations.end()){
        return this->_animations.at(name);
    }else{
        return this->_textures.at("Missing");
    }
}

int AssetManager::GetAnimationFrames(std::string name){
    return _animationsFrames[name];
}

//################## FONTS ####################################

void AssetManager::LoadFont(std::string name, std::string fileName){
    sf::Font font;

    if(font.loadFromFile(fileName)){
        LogPush("Font Loaded to Asset Manager with tag: \""+name+"\" from: \""+fileName+"\"");
        this->_fonts[name] = font;
    }else{
        LogPush("Font not Loaded correctly to Asset Manager with tag: \""+name+"\" from: \""+fileName+"\" (Default Font (Arial) applied)");
        font.loadFromFile(FONT_ARIAL_FILEPATH);
        this->_fonts[name] = font;
    }
}

void AssetManager::LoadFont(std::string name){
    sf::Font font;

    std::string filePath = FONTS_FOLDER_FILEPATH+name+".ttf";

    if(font.loadFromFile(filePath)){
        LogPush("Font Loaded to Asset Manager with tag: \""+name+"\" from: \""+filePath+"\"");
        this->_fonts[name] = font;
    }else{
        font.loadFromFile(FONT_ARIAL_FILEPATH);
        this->_fonts[name] = font;
        LogPush("Font not Loaded correctly to Asset Manager with tag: \""+name+"\" from: \""+filePath+"\" (Default Font (Arial) applied)");
    }
}

sf::Font& AssetManager::GetFont(std::string name){
    auto found = _fonts.find(name);

    if(found != _fonts.end()){
        return this->_fonts.at(name);
    }else{
        return this->_fonts.at("arial");
    }
}

//################## AUDIO ####################################

void AssetManager::LoadAudio(std::string name, std::string fileName){
    sf::SoundBuffer sound;

    if(sound.loadFromFile(fileName)){
        this->_audios[name] = sound;
        LogPush("Audio Loaded to Asset Manager with tag: \""+name+"\" from: \""+fileName+"\"");
    }else{
        LogPush("Audio not Loaded correctly to Asset Manager with tag: \""+name+"\" from: \""+fileName+"\" (Missing Audio applied)");
        sound.loadFromFile(MISSING_AUDIO_FILEPATH);
        this->_audios[name] = sound;
    }
}

void AssetManager::LoadAudio(std::string name){
    sf::SoundBuffer sound;

    std::string filePath = AUDIO_FOLDER_FILEPATH+name+".wav";

    if(sound.loadFromFile(filePath)){
        LogPush("Audio Loaded to Asset Manager with tag: \""+name+"\" from: \""+filePath+"\"");
    }else{
        sound.loadFromFile(MISSING_AUDIO_FILEPATH);
        LogPush("Audio not Loaded correctly to Asset Manager with tag: \""+name+"\" from: \""+filePath+"\" (Missing Audio applied)");
    }
    this->_audios[name] = sound;
}

sf::SoundBuffer& AssetManager::GetAudio(std::string name){
    auto found = _audios.find(name);

    if(found != _audios.end()){
        return this->_audios.at(name);
    }else{
        return this->_audios.at("missing");
    }
}

void AssetManager::PlayAudio(std::string name){
    sound.setBuffer(GetAudio(name));
    sound.play();
}










