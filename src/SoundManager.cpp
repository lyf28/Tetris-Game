#include "SoundManager.hpp"
#include <iostream>


SoundManager& SoundManager::getInstance()
{
    static SoundManager instance;
    return instance;
}

// �[������
void SoundManager::loadMusic(const std::string& name, const std::string& filepath)
{
    if (!musicPlayer.openFromFile(filepath))
    {
        std::cerr << "Failed to load music: " << filepath << std::endl;
    }
}

// ���񭵼�
void SoundManager::playMusic(const std::string& name, bool loop)
{
    // �p�G��e���񪺭��ֻP�n���񪺬ۦP�A�h�����s����
    if (currentMusic == name)
    {
        return;
    }

    stopMusic();
    currentMusic = name;

    musicPlayer.setLoop(loop);
    musicPlayer.play();
}

// �����
void SoundManager::stopMusic()
{
    musicPlayer.stop();
    currentMusic.clear();
}

// �[���u����
void SoundManager::loadSound(const std::string& name, const std::string& filepath)
{
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(filepath))
    {
        std::cerr << "Failed to load sound: " << filepath << std::endl;
        return;
    }
    soundBuffers[name] = buffer;
    sounds[name].setBuffer(soundBuffers[name]);
}

// ����u����
void SoundManager::playSound(const std::string& name)
{
    if (sounds.find(name) != sounds.end())
    {
        sounds[name].play();
    }
    else
    {
        std::cerr << "Sound not found: " << name << std::endl;
    }
}

bool SoundManager::isMusicStopped() const
{
    return musicPlayer.getStatus() == sf::SoundSource::Stopped;
}




