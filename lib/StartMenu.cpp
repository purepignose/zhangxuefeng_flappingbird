#include "StartMenu.h"

StartMenu::StartMenu()
    : window_(sf::VideoMode(1920, 1080), L"张雪峰", sf::Style::Default)
{
    font_.loadFromFile("assets/fonts/fonts1.ttf");

    background_.setSize({1920,1080});
    background_.setFillColor(sf::Color::White);

    start_button_.setSize(sf::Vector2f(300.f, 100.f));
    start_button_.setFillColor(sf::Color(80, 80, 180));
    start_button_.setOutlineThickness(3.f);
    start_button_.setOutlineColor(sf::Color::White);
    start_button_.setOrigin(150.f, 50.f);
    start_button_.setPosition(960.f, 650.f);

    text_start_.setFont(font_);
    text_start_.setCharacterSize(40);
    text_start_.setString(L"开始游戏");
    sf::FloatRect tsBounds = text_start_.getLocalBounds();
    text_start_.setOrigin(tsBounds.left + tsBounds.width / 2.f,
                          tsBounds.top + tsBounds.height / 2.f);
    text_start_.setPosition(960.f, 650.f);

    text_title_.setFont(font_);
    text_title_.setCharacterSize(80);
    text_title_.setString(L"拯救张雪峰");
    text_title_.setFillColor(sf::Color::Red);
    sf::FloatRect ttBounds = text_title_.getLocalBounds();
    text_title_.setOrigin(ttBounds.left + ttBounds.width / 2.f,
                          ttBounds.top + ttBounds.height / 2.f);
    text_title_.setPosition(960.f, 300.f);

    teaching_button_.setSize(sf::Vector2f(300.f, 100.f));
    teaching_button_.setFillColor(sf::Color(80, 80, 180));
    teaching_button_.setOutlineThickness(3.f);
    teaching_button_.setOutlineColor(sf::Color::White);
    teaching_button_.setOrigin(150.f, 50.f);
    teaching_button_.setPosition(960.f, 800.f);

    text_teaching_.setFont(font_);
    text_teaching_.setCharacterSize(40);
    text_teaching_.setString(L"教程");
    sf::FloatRect teBounds = text_teaching_.getLocalBounds();
    text_teaching_.setOrigin(teBounds.left + teBounds.width / 2.f,
                             teBounds.top + teBounds.height / 2.f);
    text_teaching_.setPosition(960.f, 800.f);

    saying_button_.setSize(sf::Vector2f(300.f, 100.f));
    saying_button_.setFillColor(sf::Color(80, 80, 180));
    saying_button_.setOutlineThickness(3.f);
    saying_button_.setOutlineColor(sf::Color::White);
    saying_button_.setOrigin(150.f, 50.f);
    saying_button_.setPosition(960.f, 950.f);

    text_saying_.setFont(font_);
    text_saying_.setCharacterSize(40);
    text_saying_.setString(L"作者想说的");
    sf::FloatRect saBounds = text_saying_.getLocalBounds();
    text_saying_.setOrigin(saBounds.left + saBounds.width / 2.f,
                           saBounds.top + saBounds.height / 2.f);
    text_saying_.setPosition(960.f, 950.f);

    back_button_.setSize(sf::Vector2f(200.f, 70.f));
    back_button_.setFillColor(sf::Color(120, 60, 60));
    back_button_.setOutlineThickness(3.f);
    back_button_.setOutlineColor(sf::Color::White);
    back_button_.setOrigin(100.f, 35.f);
    back_button_.setPosition(960.f, 1000.f);

    text_back_.setFont(font_);
    text_back_.setCharacterSize(36);
    text_back_.setString(L"返回");
    sf::FloatRect bkBounds = text_back_.getLocalBounds();
    text_back_.setOrigin(bkBounds.left + bkBounds.width / 2.f,
                         bkBounds.top + bkBounds.height / 2.f);
    text_back_.setPosition(960.f, 1000.f);

    text_teachingwords_.setFont(font_);
    text_teachingwords_.setCharacterSize(32);
    text_teachingwords_.setFillColor(sf::Color::Red);
    text_teachingwords_.setString(
        L"张雪峰老师被土木圣杯追杀！\n\n"
        L"【空格键】让张雪峰向上飞\n"
        L"不按键时会自动下落\n\n"
        L"穿过柱子间隙即可得分\n"
        L"你有三条命，受伤后会短暂无敌\n"
        L"捡到雪碧可以恢复一条命\n"
        L"捡到巧乐兹可以发动雪峰冲刺\n"
        L"生命耗尽则游戏结束\n\n"
        L"祝你好运，别报土木！"
    );
    sf::FloatRect twBounds = text_teachingwords_.getLocalBounds();
    text_teachingwords_.setOrigin(twBounds.left + twBounds.width / 2.f,
                                  twBounds.top + twBounds.height / 2.f);
    text_teachingwords_.setPosition(960.f, 450.f);

    text_sayingwords_shadow_.setFont(font_);
    text_sayingwords_shadow_.setCharacterSize(100);
    text_sayingwords_shadow_.setFillColor(sf::Color(60, 0, 0));    // 暗红
    text_sayingwords_shadow_.setLetterSpacing(5);
    text_sayingwords_shadow_.setLineSpacing(2);
    text_sayingwords_shadow_.setString(
        L"拿起刀我就无法拥抱你，\n"
        L"放下刀我就无法保护你\n"
    );
    text_sayingwords_shadow_.setStyle(sf::Text::Bold);
    sf::FloatRect swsBounds = text_sayingwords_shadow_.getLocalBounds();
    text_sayingwords_shadow_.setOrigin(swsBounds.left + swsBounds.width / 2.f,
                                swsBounds.top + swsBounds.height / 2.f);
    text_sayingwords_shadow_.setPosition(960.f + 4.f, 450.f + 4.f);  // 偏移4像素

    text_sayingwords_.setFont(font_);
    text_sayingwords_.setCharacterSize(100);
    text_sayingwords_.setFillColor(sf::Color(200, 20, 20));          // 血红
    text_sayingwords_.setLetterSpacing(5);
    text_sayingwords_.setLineSpacing(2);
    text_sayingwords_.setString(
        L"拿起刀我就无法拥抱你，\n"
        L"放下刀我就无法保护你\n"
    );
    text_sayingwords_.setOutlineThickness(3);
    text_sayingwords_.setOutlineColor(sf::Color::Black);
    text_sayingwords_.setStyle(sf::Text::Bold);
    sf::FloatRect swBounds = text_sayingwords_.getLocalBounds();
    text_sayingwords_.setOrigin(swBounds.left + swBounds.width / 2.f,
                                swBounds.top + swBounds.height / 2.f);
    text_sayingwords_.setPosition(960.f, 450.f);
}

bool StartMenu::Run() {
    while (window_.isOpen()) {
        sf::Event event;
        while (window_.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window_.close();
                return false;
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2f mousePos(
                        static_cast<float>(event.mouseButton.x),
                        static_cast<float>(event.mouseButton.y));

                    if (!showing_teaching_ && !showing_saying_) {
                        if (start_button_.getGlobalBounds().contains(mousePos)) {
                            window_.close();
                            return true;
                        }
                        if (teaching_button_.getGlobalBounds().contains(mousePos)) {
                            showing_teaching_ = true;
                        }
                        if (saying_button_.getGlobalBounds().contains(mousePos)) {
                            showing_saying_ = true;
                        }
                    } else {
                        if (back_button_.getGlobalBounds().contains(mousePos)) {
                            showing_teaching_ = false;
                            showing_saying_   = false;
                        }
                    }
                }
            }
        }

        window_.clear();

        window_.draw(background_);

        if (!showing_teaching_ && !showing_saying_) {
            window_.draw(start_button_);
            window_.draw(text_start_);
            window_.draw(text_title_);
            window_.draw(teaching_button_);
            window_.draw(text_teaching_);
            window_.draw(saying_button_);
            window_.draw(text_saying_);
        } else if (showing_teaching_) {
            window_.draw(text_teachingwords_);
            window_.draw(back_button_);
            window_.draw(text_back_);
        } else {
            window_.draw(text_sayingwords_shadow_);   
            static sf::Clock flickerClock;
            static bool flickerOn = true;
            if (flickerClock.getElapsedTime().asMilliseconds() > 80) {
                flickerOn = (rand() % 100) < 90;
                flickerClock.restart();
            }
            if (flickerOn) {
                window_.draw(text_sayingwords_);
            }

            window_.draw(back_button_);
            window_.draw(text_back_);
        }

        window_.display();
    }
    return false;
}
