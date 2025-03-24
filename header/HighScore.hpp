#ifndef HIGHSCORE_HPP
#define HIGHSCORE_HPP

#include <string>

// ���J��������
unsigned int load_high_score(const std::string& filename);

// �x�s��������
void save_high_score(const std::string& filename, unsigned int new_score);

#endif // HIGHSCORE_HPP

