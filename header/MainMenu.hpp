#ifndef MAIN_MENU_HPP
#define MAIN_MENU_HPP

#include <SFML/Graphics.hpp>
#include <string>

enum class MenuOption {
    PlayGame,
    Scoreboard,
    Exit
};

// �D���禡�A��^�ϥΪ̿�ܪ��ﶵ
MenuOption show_main_menu(sf::RenderWindow& window);

#endif /

