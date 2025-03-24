#include <chrono>
#include <random>
#include <SFML/Graphics.hpp>
#include <sstream>
#include <iomanip>
#include <SFML/Audio.hpp>

#include "DrawText.hpp"
#include "Global.hpp"
#include "GetTetromino.hpp"
#include "GetWallKickData.hpp"
#include "Tetromino.hpp"
#include "Scoreboard.hpp"
#include "DifficultySelection.hpp"
#include "MainMenu.hpp"
#include "GameLogic.hpp"
#include "SoundManager.hpp"



bool start_game(sf::RenderWindow& window, unsigned char START_FALL_SPEED, unsigned char& current_fall_speed, unsigned int& lines_cleared)
{
    SoundManager& soundManager = SoundManager::getInstance();

    // 確保進入遊戲時切換到遊戲音樂
    soundManager.stopMusic();
    soundManager.playMusic("playing");


    lines_cleared = 0;

    bool game_over = 0;

    bool hard_drop_pressed = 0;

    bool rotate_pressed = 0;

    unsigned lag = 0;


    unsigned char clear_effect_timer = 0;

    // 用於結束後的停留計時
    sf::Clock end_game_clock;

    current_fall_speed = START_FALL_SPEED;


    //方塊掉落計時
    unsigned char fall_timer = 0;
    //方塊水平移動計時
    unsigned char move_timer = 0;

    unsigned char next_shape;

    unsigned char soft_drop_timer = 0;


    std::chrono::time_point<std::chrono::steady_clock> previous_time;


    std::random_device random_device;


    std::default_random_engine random_engine(random_device());

    //讓方塊隨機出現
    std::uniform_int_distribution<unsigned short> shape_distribution(0, 6);

    //儲存當前列狀態
    std::vector<bool> clear_lines(ROWS, 0);

    std::vector<sf::Color> cell_colors =
    {
        sf::Color(36, 36, 85),
        sf::Color(0, 219, 255),
        sf::Color(0, 36, 255),
        sf::Color(255, 146, 0),
        sf::Color(255, 219, 0),
        sf::Color(0, 219, 0),
        sf::Color(146, 0, 255),
        sf::Color(219, 0, 0),
        sf::Color(73, 73, 85)
    };

    std::vector<std::vector<unsigned char>> matrix(COLUMNS, std::vector<unsigned char>(ROWS));

    sf::Event event;

    window.setView(sf::View(sf::FloatRect(0, 0, 2 * CELL_SIZE * COLUMNS, CELL_SIZE * ROWS)));


    Tetromino tetromino(static_cast<unsigned char>(shape_distribution(random_engine)), matrix);

    next_shape = static_cast<unsigned char>(shape_distribution(random_engine));

    previous_time = std::chrono::steady_clock::now();

    while (1 == window.isOpen())
    {
        unsigned delta_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - previous_time).count();

        lag += delta_time;

        previous_time += std::chrono::microseconds(delta_time);

        while (FRAME_DURATION <= lag)
        {
            lag -= FRAME_DURATION;

            while (1 == window.pollEvent(event))
            {
                switch (event.type)
                {
                case sf::Event::Closed:
                {
                    window.close();

                    break;
                }
                case sf::Event::KeyReleased:
                {
                    switch (event.key.code)
                    {
                    case sf::Keyboard::C:
                    case sf::Keyboard::Z:
                    {
                        rotate_pressed = 0;

                        break;
                    }
                    case sf::Keyboard::Down:
                    {
                        soft_drop_timer = 0;

                        break;
                    }
                    case sf::Keyboard::Left:
                    case sf::Keyboard::Right:
                    {
                        move_timer = 0;

                        break;
                    }
                    case sf::Keyboard::Space:
                    {
                        hard_drop_pressed = 0;
                    }
                    }
                }
                }
            }

            if (0 == clear_effect_timer)
            {
                if (0 == game_over)
                {
                    if (0 == rotate_pressed)
                    {
                        if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::C))
                        {
                            rotate_pressed = 1;
                            soundManager.playSound("rotate"); // 播放旋轉音效

                            tetromino.rotate(1, matrix);
                        }
                        else if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
                        {
                            rotate_pressed = 1;
                            soundManager.playSound("rotate"); // 播放旋轉音效
                            tetromino.rotate(0, matrix);
                        }
                    }

                    if (0 == move_timer)
                    {
                        if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                        {
                            move_timer = 1;

                            tetromino.move_left(matrix);
                        }
                        else if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                        {
                            move_timer = 1;

                            tetromino.move_right(matrix);
                        }
                    }
                    else
                    {
                        move_timer = (1 + move_timer) % MOVE_SPEED;
                    }

                    if (0 == hard_drop_pressed)
                    {
                        if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
                        {
                            hard_drop_pressed = 1;

                            fall_timer = current_fall_speed;

                            tetromino.hard_drop(matrix);
                        }
                    }

                    if (0 == soft_drop_timer)
                    {
                        if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                        {
                            if (1 == tetromino.move_down(matrix))
                            {
                                fall_timer = 0;
                                soft_drop_timer = 1;
                            }
                        }
                    }
                    else
                    {
                        soft_drop_timer = (1 + soft_drop_timer) % SOFT_DROP_SPEED;
                    }

                    if (current_fall_speed == fall_timer)
                    {
                        if (0 == tetromino.move_down(matrix))
                        {
                            tetromino.update_matrix(matrix);

                            // 播放堆疊音效
                            soundManager.playSound("stack");

                            for (unsigned char a = 0; a < ROWS; a++)
                            {
                                bool clear_line = 1;

                                for (unsigned char b = 0; b < COLUMNS; b++)
                                {
                                    if (0 == matrix[b][a])
                                    {
                                        clear_line = 0;

                                        break;
                                    }
                                }

                                if (1 == clear_line)
                                {
                                    lines_cleared++;

                                    clear_effect_timer = CLEAR_EFFECT_DURATION;

                                    clear_lines[a] = 1;

                                    if (0 == lines_cleared % LINES_TO_INCREASE_SPEED)
                                    {
                                        //增加方塊下落速度
                                        current_fall_speed = std::max<unsigned char>(SOFT_DROP_SPEED, current_fall_speed - 1);
                                    }
                                }
                            }

                            if (0 == clear_effect_timer)
                            {
                                if (!tetromino.reset(next_shape, matrix))
                                {
                                    game_over = true;
                                    soundManager.stopMusic();
                                    soundManager.playSound("gameover");

                                    // 顯示遊戲結束畫面
                                    sf::Clock clock;

                                    // 在遊戲結束後顯示灰色矩陣，停留 4 秒
                                    while (clock.getElapsedTime().asSeconds() < 4)
                                    {
                                        window.clear();

                                        // 繪製矩陣，將所有方塊設置為灰色
                                        for (unsigned char a = 0; a < COLUMNS; a++)
                                        {
                                            for (unsigned char b = 0; b < ROWS; b++)
                                            {
                                                sf::RectangleShape cell(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
                                                cell.setPosition(static_cast<float>(CELL_SIZE * a), static_cast<float>(CELL_SIZE * b));

                                                if (matrix[a][b] > 0)   // 如果方塊存在
                                                {
                                                    cell.setFillColor(cell_colors[8]);
                                                }
                                                else
                                                {
                                                    cell.setFillColor(cell_colors[0]);
                                                }
                                                window.draw(cell);
                                            }
                                        }

                                        window.display();

                                        // 處理事件以防止程式無回應
                                        sf::Event event;
                                        while (window.pollEvent(event))
                                        {
                                            if (event.type == sf::Event::Closed)
                                            {
                                                window.close();
                                                return false;
                                            }
                                        }
                                    }

                                    return true;
                                }




                                next_shape = static_cast<unsigned char>(shape_distribution(random_engine));
                            }
                        }

                        fall_timer = 0;
                    }
                    else
                    {
                        fall_timer++;
                    }
                }
                else if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
                {
                    game_over = 0;
                    hard_drop_pressed = 0;
                    rotate_pressed = 0;

                    lines_cleared = 0;

                    current_fall_speed = START_FALL_SPEED;
                    fall_timer = 0;
                    move_timer = 0;
                    soft_drop_timer = 0;

                    //清除矩陣
                    for (std::vector<unsigned char>& a : matrix)
                    {
                        std::fill(a.begin(), a.end(), 0);
                    }
                }
            }
            else
            {
                clear_effect_timer--;

                if (0 == clear_effect_timer)
                {
                    for (unsigned char a = 0; a < ROWS; a++)
                    {
                        if (1 == clear_lines[a])
                        {
                            for (unsigned char b = 0; b < COLUMNS; b++)
                            {
                                matrix[b][a] = 0;

                                for (unsigned char c = a; 0 < c; c--)
                                {
                                    matrix[b][c] = matrix[b][c - 1];
                                    matrix[b][c - 1] = 0;
                                }
                            }
                        }
                    }

                    if (!tetromino.reset(next_shape, matrix))
                    {
                        game_over = true;
                        soundManager.stopMusic();
                        soundManager.playSound("gameover");

                        // 顯示遊戲結束畫面
                        sf::Clock clock;

                        // 在遊戲結束後顯示灰色矩陣，停留 4 秒
                        while (clock.getElapsedTime().asSeconds() < 4)
                        {
                            window.clear();

                            // 繪製矩陣，將所有方塊設置為灰色
                            for (unsigned char a = 0; a < COLUMNS; a++)
                            {
                                for (unsigned char b = 0; b < ROWS; b++)
                                {
                                    sf::RectangleShape cell(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
                                    cell.setPosition(static_cast<float>(CELL_SIZE * a), static_cast<float>(CELL_SIZE * b));

                                    if (matrix[a][b] > 0)
                                    {
                                        cell.setFillColor(cell_colors[8]);
                                    }
                                    else
                                    {
                                        cell.setFillColor(cell_colors[0]);
                                    }
                                    window.draw(cell);
                                }
                            }

                            window.display();

                            // 處理事件以防止程式無回應
                            sf::Event event;
                            while (window.pollEvent(event))
                            {
                                if (event.type == sf::Event::Closed)
                                {
                                    window.close();
                                    return false;
                                }
                            }
                        }

                        return true;
                    }





                    next_shape = static_cast<unsigned char>(shape_distribution(random_engine));

                    std::fill(clear_lines.begin(), clear_lines.end(), 0);
                }
            }

            //繪製
            if (FRAME_DURATION > lag)
            {
                unsigned char clear_cell_size = static_cast<unsigned char>(2 * round(0.5f * CELL_SIZE * (clear_effect_timer / static_cast<float>(CLEAR_EFFECT_DURATION))));

                sf::RectangleShape cell(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
                sf::RectangleShape preview_border(sf::Vector2f(5 * CELL_SIZE, 5 * CELL_SIZE));
                preview_border.setFillColor(sf::Color(0, 0, 0));
                preview_border.setOutlineThickness(-1);
                preview_border.setPosition(CELL_SIZE * (1.5f * COLUMNS - 2.5f), CELL_SIZE * (0.25f * ROWS - 2.5f));

                window.clear();

                for (unsigned char a = 0; a < COLUMNS; a++)
                {
                    for (unsigned char b = 0; b < ROWS; b++)
                    {
                        if (0 == clear_lines[b])
                        {
                            cell.setPosition(static_cast<float>(CELL_SIZE * a), static_cast<float>(CELL_SIZE * b));

                            if (1 == game_over && 0 < matrix[a][b])
                            {
                                cell.setFillColor(cell_colors[8]);
                            }
                            else
                            {
                                cell.setFillColor(cell_colors[matrix[a][b]]);
                            }

                            window.draw(cell);
                        }
                    }
                }

                cell.setFillColor(cell_colors[8]);

                if (0 == game_over)
                {
                    for (Position& mino : tetromino.get_ghost_minos(matrix))
                    {
                        cell.setPosition(static_cast<float>(CELL_SIZE * mino.x), static_cast<float>(CELL_SIZE * mino.y));

                        window.draw(cell);
                    }

                    cell.setFillColor(cell_colors[1 + tetromino.get_shape()]);
                }

                for (Position& mino : tetromino.get_minos())
                {
                    cell.setPosition(static_cast<float>(CELL_SIZE * mino.x), static_cast<float>(CELL_SIZE * mino.y));

                    window.draw(cell);
                }

                for (unsigned char a = 0; a < COLUMNS; a++)
                {
                    for (unsigned char b = 0; b < ROWS; b++)
                    {
                        if (1 == clear_lines[b])
                        {
                            cell.setFillColor(cell_colors[0]);
                            cell.setPosition(static_cast<float>(CELL_SIZE * a), static_cast<float>(CELL_SIZE * b));
                            cell.setSize(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));

                            window.draw(cell);

                            cell.setFillColor(sf::Color(255, 255, 255));
                            cell.setPosition(floor(CELL_SIZE * (0.5f + a) - 0.5f * clear_cell_size), floor(CELL_SIZE * (0.5f + b) - 0.5f * clear_cell_size));
                            cell.setSize(sf::Vector2f(clear_cell_size, clear_cell_size));

                            window.draw(cell);
                        }
                    }
                }

                cell.setFillColor(cell_colors[1 + next_shape]);
                cell.setSize(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));

                window.draw(preview_border);

                //繪製下一個方塊
                for (Position& mino : get_tetromino(next_shape, static_cast<unsigned char>(1.5f * COLUMNS), static_cast<unsigned char>(0.25f * ROWS)))
                {
                    unsigned short next_tetromino_x = CELL_SIZE * mino.x;
                    unsigned short next_tetromino_y = CELL_SIZE * mino.y;

                    if (0 == next_shape)
                    {
                        next_tetromino_y += static_cast<unsigned char>(round(0.5f * CELL_SIZE));
                    }
                    else if (3 != next_shape)
                    {
                        next_tetromino_x -= static_cast<unsigned char>(round(0.5f * CELL_SIZE));
                    }

                    cell.setPosition(next_tetromino_x, next_tetromino_y);

                    window.draw(cell);
                }

                //繪製文字
                std::ostringstream oss;
                oss << "Score:" << lines_cleared
                    << "\nSpeed:" << std::fixed << std::setprecision(2)
                    << static_cast<float>(START_FALL_SPEED) / current_fall_speed << "x";

                draw_text(static_cast<unsigned short>(CELL_SIZE * (0.5f + COLUMNS)),
                          static_cast<unsigned short>(0.5f * CELL_SIZE * ROWS),
                          oss.str(), window);

                unsigned short title_x = static_cast<unsigned short>(CELL_SIZE * 10.5);
                unsigned short title_y = static_cast<unsigned short>(CELL_SIZE * 14);
                unsigned short option1_x = static_cast<unsigned short>(CELL_SIZE * 10.5);
                unsigned short option1_y = static_cast<unsigned short>(CELL_SIZE * 15.5);
                unsigned short option2_x = static_cast<unsigned short>(CELL_SIZE * 10.5);
                unsigned short option2_y = static_cast<unsigned short>(CELL_SIZE * 16.5);
                unsigned short option3_x = static_cast<unsigned short>(CELL_SIZE * 10.5);
                unsigned short option3_y = static_cast<unsigned short>(CELL_SIZE * 17.5);

                draw_text(title_x, title_y, "Controls:", window, static_cast<unsigned short>(CELL_SIZE * 1));
                draw_text(option1_x, option1_y, "Z - Rotate", window, static_cast<unsigned short>(CELL_SIZE * 0.8));
                draw_text(option2_x, option2_y, "Space - Hard drop", window, static_cast<unsigned short>(CELL_SIZE * 0.8));
                draw_text(option3_x, option3_y, "Left/Right/Down - Move  ", window, static_cast<unsigned short>(CELL_SIZE * 0.8));


                window.display();

            }
        }
    }
    return false;
}
