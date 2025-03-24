#ifndef SOUNDMANAGER_HPP
#define SOUNDMANAGER_HPP

#include <SFML/Audio.hpp>
#include <string>
#include <map>
#include <unordered_map>

class SoundManager
{
private:
    sf::Music musicPlayer;
    std::unordered_map<std::string, sf::SoundBuffer> soundBuffers;
    std::unordered_map<std::string, sf::Sound> sounds;
    std::string currentMusic; //儲存當前播放的音樂名稱

    SoundManager() = default;

public:
    static SoundManager& getInstance();
    void loadMusic(const std::string& name, const std::string& filepath);
    void playMusic(const std::string& name, bool loop = true);
    void stopMusic();
    void loadSound(const std::string& name, const std::string& filepath);
    void playSound(const std::string& name);

    bool isMusicStopped() const;
};

#endif

