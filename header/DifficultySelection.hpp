#ifndef DIFFICULTY_SELECTION_HPP
#define DIFFICULTY_SELECTION_HPP

#include <SFML/Graphics.hpp>

// 選擇難度並返回初始速度
unsigned char select_difficulty(sf::RenderWindow& window, unsigned char& current_fall_speed);

#endif

