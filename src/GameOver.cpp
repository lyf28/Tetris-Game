#include "GameOver.hpp"
#include "DrawText.hpp"
#include "Global.hpp"
#include "SoundManager.hpp"
#include <SFML/Audio.hpp>

bool show_game_over(sf::RenderWindow& window, unsigned int lines_cleared, unsigned int high_score)
{
    sf::Event event;
    SoundManager& soundManager = SoundManager::getInstance();
    soundManager.stopMusic();
    soundManager.playSound("gameover");

    while (window.isOpen())
    {
        window.clear();

        unsigned short title_x = static_cast<unsigned short>(CELL_SIZE * 2.4);
        unsigned short title_y = static_cast<unsigned short>(CELL_SIZE * 4);

        unsigned short lines_cleared_x = static_cast<unsigned short>(CELL_SIZE * 5.2);
        unsigned short lines_cleared_y = static_cast<unsigned short>(CELL_SIZE * 8);

        unsigned short high_score_x = static_cast<unsigned short>(CELL_SIZE * 3.5);
        unsigned short high_score_y = static_cast<unsigned short>(CELL_SIZE * 10);

        unsigned short option1_x = static_cast<unsigned short>(CELL_SIZE * 7.5);
        unsigned short option1_y = static_cast<unsigned short>(CELL_SIZE * 14);

        unsigned short option2_x = static_cast<unsigned short>(CELL_SIZE * 6);
        unsigned short option2_y = static_cast<unsigned short>(CELL_SIZE * 16);


        draw_text(title_x, title_y, "GAME OVER", window, static_cast<unsigned short>(CELL_SIZE * 3.5));
        draw_text(lines_cleared_x, lines_cleared_y, "Your Score: " + std::to_string(lines_cleared), window, static_cast<unsigned short>(CELL_SIZE * 1.5));
        draw_text(high_score_x, high_score_y, "Highest Record: " + std::to_string(high_score), window, static_cast<unsigned short>(CELL_SIZE * 1.5));
        draw_text(option1_x, option1_y, "P - PLAY AGAIN", window, static_cast<unsigned short>(CELL_SIZE * 0.8));
        draw_text(option2_x, option2_y, "R - RETURN TO MAIN MENU", window, static_cast<unsigned short>(CELL_SIZE * 0.8));


        window.display();

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                return false;
            }
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::P)
                {
                    return true; // 再玩一場
                }
                else if (event.key.code == sf::Keyboard::R)
                {
                    return false; // 返回主選單
                }
            }
        }
    }

    return false; // 預設返回主選單
}

