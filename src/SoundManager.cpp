#include "SoundManager.hpp"
#include <iostream>


SoundManager& SoundManager::getInstance()
{
    static SoundManager instance;
    return instance;
}

// 加載音樂
void SoundManager::loadMusic(const std::string& name, const std::string& filepath)
{
    if (!musicPlayer.openFromFile(filepath))
    {
        std::cerr << "Failed to load music: " << filepath << std::endl;
    }
}

// 播放音樂
void SoundManager::playMusic(const std::string& name, bool loop)
{
    // 如果當前播放的音樂與要播放的相同，則不重新播放
    if (currentMusic == name)
    {
        return;
    }

    stopMusic();
    currentMusic = name;

    musicPlayer.setLoop(loop);
    musicPlayer.play();
}

// 停止音樂
void SoundManager::stopMusic()
{
    musicPlayer.stop();
    currentMusic.clear();
}

// 加載短音效
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

// 播放短音效
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




