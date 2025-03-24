#include "Scoreboard.hpp"
#include "DrawText.hpp"
#include <sstream>
#include <fstream>
#include "Global.hpp"
#include <SFML/Audio.hpp>

// 保存最高分到檔案
void save_high_score(const std::string& filename, unsigned int high_score) {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << high_score;
        file.close();
    }
}

// 從檔案讀取最高分
unsigned int load_high_score(const std::string& filename) {
    std::ifstream file(filename);
    unsigned int high_score = 0;
    if (file.is_open()) {
        file >> high_score;
        file.close();
    }
    return high_score;
}

// 更新最高分
void update_high_score(unsigned int& high_score, unsigned int current_score) {
    if (current_score > high_score) {
        high_score = current_score;
        save_high_score("highscore.txt", high_score);
    }
}

// 顯示記分板畫面
void display_scoreboard(sf::RenderWindow& window, unsigned int high_score) {
    sf::Event event;
    window.setView(sf::View(sf::FloatRect(0, 0, 2 * CELL_SIZE * COLUMNS, CELL_SIZE * ROWS)));

    while (window.isOpen()) {
        window.clear();

        unsigned short title_x = static_cast<unsigned short>(CELL_SIZE * 2.5);
        unsigned short title_y = static_cast<unsigned short>(CELL_SIZE * 4);

        unsigned short score_x = static_cast<unsigned short>(CELL_SIZE * 4.7);
        unsigned short score_y = static_cast<unsigned short>(CELL_SIZE * 9);

        unsigned short return_x = static_cast<unsigned short>(CELL_SIZE * 6);
        unsigned short return_y = static_cast<unsigned short>(CELL_SIZE * 16);

        draw_text(title_x, title_y, "SCOREBOARD", window, static_cast<unsigned short>(CELL_SIZE * 3));
        draw_text(score_x, score_y, "Highest Record: " + std::to_string(high_score), window, static_cast<unsigned short>(CELL_SIZE * 1.3));
        draw_text(return_x, return_y, "-Press ESC to Return-", window, static_cast<unsigned short>(CELL_SIZE * 0.8));
        window.display();

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                return; // 返回主選單
            }
        }
    }
}



