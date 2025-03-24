#include "HighScore.hpp"
#include <fstream>

unsigned int load_high_score(const std::string& filename) {
    std::ifstream file(filename);
    unsigned int score = 0;

    if (file.is_open()) {
        file >> score;
        file.close();
    }

    return score;
}

void save_high_score(const std::string& filename, unsigned int new_score) {
    std::ifstream file_check(filename);
    unsigned int existing_score = 0;

    if (file_check.is_open()) {
        file_check >> existing_score;
        file_check.close();
    }

    if (new_score > existing_score) {
        std::ofstream file(filename, std::ios::out | std::ios::trunc);
        if (file.is_open()) {
            file << new_score;
            file.close();
        }
    }
}

