#ifndef SCOREBOARD_HPP
#define SCOREBOARD_HPP

#include <string>
#include <SFML/Graphics.hpp>

void save_high_score(const std::string& filename, unsigned int high_score);

unsigned int load_high_score(const std::string& filename);

void update_high_score(unsigned int& high_score, unsigned int current_score);

void display_scoreboard(sf::RenderWindow& window, unsigned int high_score);

#endif

