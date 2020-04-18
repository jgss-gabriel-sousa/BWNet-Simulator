#pragma once

#include <map>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "DEFINITIONS.h"
#include "log.h"

class AssetManager{
public:
    AssetManager();
    ~AssetManager(){}

    void LoadTexture(std::string name, std::string fileName);
    void LoadTexture(std::string name);
    sf::Texture &GetTexture(std::string name);

    void LoadAnimation(std::string name, int frames);
    sf::Texture& GetAnimation(std::string name, int frame);
    int GetAnimationFrames(std::string name);

    void LoadFont(std::string name, std::string fileName);
    void LoadFont(std::string name);
    sf::Font &GetFont(std::string name);

    void LoadAudio(std::string name, std::string fileName);
    void LoadAudio(std::string name);
    sf::SoundBuffer &GetAudio(std::string name);
    void PlayAudio(std::string name);

private:
    std::map<std::string, sf::Texture> _textures;
    std::map<std::string,sf::Texture> _animations;
    std::map<std::string,int> _animationsFrames;
    std::map<std::string, sf::Font> _fonts;
    std::map<std::string, sf::SoundBuffer> _audios;

    sf::Sound sound;
};
