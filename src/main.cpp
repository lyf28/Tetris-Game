#include <chrono>
#include <random>
#include <SFML/Graphics.hpp>
#include <sstream>
#include <iomanip>
#include <fstream>

#include "DrawText.hpp"
#include "Global.hpp"
#include "GetTetromino.hpp"
#include "GetWallKickData.hpp"
#include "Tetromino.hpp"
#include "Scoreboard.hpp"
#include "DifficultySelection.hpp"
#include "MainMenu.hpp"
#include "Gamelogic.hpp"
#include "GameOver.hpp"
#include "SoundManager.hpp"
#include <SFML/Audio.hpp>

// 載入最高分紀錄
unsigned int load_high_score(const std::string& filename);

int main()
{
    sf::RenderWindow window(
        sf::VideoMode(2 * CELL_SIZE * COLUMNS * SCREEN_RESIZE, CELL_SIZE * ROWS * SCREEN_RESIZE),
        "Tetris", sf::Style::Close
    );

    SoundManager& soundManager = SoundManager::getInstance();

    // 載入背景音樂
    soundManager.loadMusic("main_menu", "Resources/audio/main.wav");
    soundManager.loadMusic("playing", "Resources/audio/playing.wav");

    // 載入音效
    soundManager.loadSound("rotate", "Resources/audio/rotation.wav");
    soundManager.loadSound("stack", "Resources/audio/stack.wav");
    soundManager.loadSound("gameover", "Resources/audio/gameover.wav");

    soundManager.playMusic("main_menu");


    const std::string high_score_file = "highscore.txt";
    unsigned int high_score = load_high_score(high_score_file); // 載入最高分數
    unsigned int lines_cleared = 0;

    while (window.isOpen())
    {
        // 主選單
        MenuOption choice = show_main_menu(window);

        if (choice == MenuOption::PlayGame)
        {
            unsigned char START_FALL_SPEED = 0;
            unsigned char current_fall_speed = 0;

            START_FALL_SPEED = select_difficulty(window, current_fall_speed);
            if (START_FALL_SPEED == 0) continue; // 如果沒有選擇難度，返回主選單

            bool play_again = true;
            while (play_again)
            {
                lines_cleared = 0;
                bool game_over = start_game(window, START_FALL_SPEED, current_fall_speed, lines_cleared);

                if (game_over)
                {
                    // 更新高分
                    if (lines_cleared > high_score)
                    {
                        high_score = lines_cleared;
                        save_high_score(high_score_file, high_score);
                    }

                    play_again = show_game_over(window, lines_cleared, high_score);

                    if (!play_again)
                    {
                        break;
                    }
                }
            }
        }
        else if (choice == MenuOption::Scoreboard)
        {
            display_scoreboard(window, high_score);
        }
        else if (choice == MenuOption::Exit)
        {
            save_high_score(high_score_file, high_score);
            window.close();
        }
    }

    return 0;
}
