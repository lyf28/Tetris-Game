#ifndef GAME_OVER_HPP
#define GAME_OVER_HPP

#include <SFML/Graphics.hpp>

// 顯示遊戲結束畫面並詢問玩家是否重玩
bool show_game_over(sf::RenderWindow& window, unsigned int lines_cleared, unsigned int high_score);

#endif

