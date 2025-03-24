#ifndef MAIN_MENU_HPP
#define MAIN_MENU_HPP

#include <SFML/Graphics.hpp>
#include <string>

enum class MenuOption {
    PlayGame,
    Scoreboard,
    Exit
};

// 主選單函式，返回使用者選擇的選項
MenuOption show_main_menu(sf::RenderWindow& window);

#endif /

