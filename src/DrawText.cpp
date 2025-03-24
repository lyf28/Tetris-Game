#include <SFML/Graphics.hpp>
#include "DrawText.hpp"
#include <iostream>


void draw_text(unsigned short i_x, unsigned short i_y, const std::string& i_text, sf::RenderWindow& i_window, unsigned int character_size)
{

    short character_x = i_x;
    short character_y = i_y;

    unsigned char character_width;

    sf::Sprite character_sprite;

    sf::Texture font_texture;
    if (!font_texture.loadFromFile("Resources/Fonts/Font.png"))
    {
        std::cerr << "Error: Could not load font texture!" << std::endl;
        return;
    }

    // 計算字元寬度
    character_width = font_texture.getSize().x / 96;

    character_sprite.setTexture(font_texture);

    // 計算縮放比例，讓字體大小符合 character_size
    float scale = static_cast<float>(character_size) / font_texture.getSize().y;
    character_sprite.setScale(scale, scale);

    for (const char a : i_text)
    {
        if ('\n' == a)
        {
            character_x = i_x;
            character_y += static_cast<unsigned int>(font_texture.getSize().y * scale);

            continue;
        }


        character_sprite.setPosition(character_x, character_y);

        // 切割字元
        character_sprite.setTextureRect(sf::IntRect(character_width * (a - 32), 0, character_width, font_texture.getSize().y));

        i_window.draw(character_sprite);

        character_x += static_cast<unsigned int>(character_width * scale);
    }
}
