#include "DifficultySelection.hpp"
#include "DrawText.hpp"
#include "Global.hpp"
#include <SFML/Audio.hpp>

unsigned char select_difficulty(sf::RenderWindow& window, unsigned char& current_fall_speed)
{
    // 重置視窗的 View
    window.setView(sf::View(sf::FloatRect(0, 0, 2 * CELL_SIZE * COLUMNS, CELL_SIZE * ROWS)));

    sf::Event event;
    bool difficulty_selected = false;
    unsigned char START_FALL_SPEED = EASY_FALL_SPEED; // 預設難度為 EASY

    while (!difficulty_selected && window.isOpen())
    {
        window.clear();
        // 基準位置和字體大小計算
        unsigned short title_x = static_cast<unsigned short>(CELL_SIZE * 1.5);
        unsigned short title_y = static_cast<unsigned short>(CELL_SIZE * 4);

        unsigned short option_x = static_cast<unsigned short>(CELL_SIZE * 7);
        unsigned short option_y = static_cast<unsigned short>(CELL_SIZE * 8);
        unsigned short option_spacing = static_cast<unsigned short>(CELL_SIZE * 2.5);

        unsigned short return_x = static_cast<unsigned short>(CELL_SIZE * 7);
        unsigned short return_y = static_cast<unsigned short>(CELL_SIZE * 17);

        // 繪製標題
        draw_text(title_x, title_y, "SELECT DIFFICULTY", window, static_cast<unsigned short>(CELL_SIZE * 2.2));

        // 繪製選項
        draw_text(option_x, option_y, "E - EASY", window, static_cast<unsigned short>(CELL_SIZE * 1.25)); // EASY
        draw_text(option_x, option_y + option_spacing, "M - MEDIUM", window, static_cast<unsigned short>(CELL_SIZE * 1.25));
        draw_text(option_x, option_y + 2 * option_spacing, "H - HARD", window, static_cast<unsigned short>(CELL_SIZE * 1.25));

        // 繪製返回提示
        draw_text(return_x, return_y, "-Press ESC to Return-", window, static_cast<unsigned short>(CELL_SIZE * 0.5)); // 返回提示

        window.display();

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                break;
            }

            // 檢查按鍵是否按下
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::E)    // 選擇 Easy
                {
                    current_fall_speed = EASY_FALL_SPEED;
                    START_FALL_SPEED = EASY_FALL_SPEED;
                    difficulty_selected = true;
                }
                else if (event.key.code == sf::Keyboard::M)    // 選擇 Medium
                {
                    current_fall_speed = MEDIUM_FALL_SPEED;
                    START_FALL_SPEED = MEDIUM_FALL_SPEED;
                    difficulty_selected = true;
                }
                else if (event.key.code == sf::Keyboard::H)    // 選擇 Hard
                {
                    current_fall_speed = HARD_FALL_SPEED;
                    START_FALL_SPEED = HARD_FALL_SPEED;
                    difficulty_selected = true;
                }
                else if (event.key.code == sf::Keyboard::Escape)     // 按 ESC 返回主選單
                {
                    return 0; // 返回 0 表示返回主選單
                }
            }
        }
    }

    return START_FALL_SPEED; // 返回選擇的初始速度
}

