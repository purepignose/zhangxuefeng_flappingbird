#pragma once

#include <SFML/Graphics.hpp>

class StartMenu {
public:
    StartMenu();
    bool Run();   // true = 点了开始, false = 关了窗口

private:
    sf::RenderWindow window_;
    sf::Font font_;
    sf::RectangleShape start_button_;
    sf::Text text_start_;
    sf::Text text_title_;
    sf::RectangleShape teaching_button_;
    sf::RectangleShape saying_button_;
    sf::Text text_teaching_;
    sf::Text text_saying_;
    sf::Text text_teachingwords_;
    sf::Text text_sayingwords_;
    sf::RectangleShape back_button_;
    sf::Text text_back_;
    sf::Text text_sayingwords_shadow_;
    sf::RectangleShape background_;

    bool teaching_ = false;
    bool showing_teaching_ = false;
    bool saying_ = false;
    bool showing_saying_ = false;
};