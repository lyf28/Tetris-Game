#include "MainMenu.hpp"
#include "DrawText.hpp"
#include "Global.hpp"
#include "SoundManager.hpp"
#include <SFML/Audio.hpp>


MenuOption show_main_menu(sf::RenderWindow& window)
{
    window.setView(sf::View(sf::FloatRect(0, 0, 2 * CELL_SIZE * COLUMNS, CELL_SIZE * ROWS)));

    SoundManager& soundManager = SoundManager::getInstance();
    if (soundManager.isMusicStopped())
    {
        soundManager.playMusic("main_menu");
    }

    MenuOption selected_option = MenuOption::Exit;
    bool option_selected = false;

    unsigned short title_x = static_cast<unsigned short>(CELL_SIZE * 2.1);
    unsigned short title_y = static_cast<unsigned short>(CELL_SIZE * 4);
    unsigned short option1_x = static_cast<unsigned short>(CELL_SIZE * 6.5);
    unsigned short option1_y = static_cast<unsigned short>(CELL_SIZE * 9);
    unsigned short option2_x = static_cast<unsigned short>(CELL_SIZE * 6.5);
    unsigned short option2_y = static_cast<unsigned short>(CELL_SIZE * 11);
    unsigned short option3_x = static_cast<unsigned short>(CELL_SIZE * 6.5);
    unsigned short option3_y = static_cast<unsigned short>(CELL_SIZE * 13);

    while (!option_selected && window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                return MenuOption::Exit;
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1) || sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad1))
        {
            selected_option = MenuOption::PlayGame;
            option_selected = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2) || sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad2))
        {
            selected_option = MenuOption::Scoreboard;
            option_selected = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3) || sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad3))
        {
            selected_option = MenuOption::Exit;
            option_selected = true;
        }

        window.clear();

        draw_text(title_x, title_y, "TETRIS GAME", window, static_cast<unsigned short>(CELL_SIZE * 3));
        draw_text(option1_x, option1_y, "1 - PLAY GAME", window, static_cast<unsigned short>(CELL_SIZE * 1));
        draw_text(option2_x, option2_y, "2 - SCOREBOARD", window, static_cast<unsigned short>(CELL_SIZE * 1));
        draw_text(option3_x, option3_y, "3 - EXIT", window, static_cast<unsigned short>(CELL_SIZE * 1));

        window.display();
    }

    return selected_option;
}
