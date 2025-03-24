#ifndef HIGHSCORE_HPP
#define HIGHSCORE_HPP

#include <string>

// 載入高分紀錄
unsigned int load_high_score(const std::string& filename);

// 儲存高分紀錄
void save_high_score(const std::string& filename, unsigned int new_score);

#endif // HIGHSCORE_HPP

