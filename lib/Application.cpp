#include "Application.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <random>


Application::Application():     //创建窗口
    mainwindow_(sf::VideoMode(1920, 1080), L"怀念张雪峰", sf::Style::Default)
{

    mainwindow_.setFramerateLimit(120);

    bgm_.openFromFile("assets/music/bgm.wav");
    bgm_.play();
    bgm_.setLoop(true);
    bgm_.setVolume(42.f);

    score_sound_buffer_.loadFromFile("assets/sounds/score.wav");
    hurt_sound_buffer_.loadFromFile("assets/sounds/hurt.wav");
    heal_sound_buffer_.loadFromFile("assets/sounds/heal.wav");
    dash_sound_buffer_.loadFromFile("assets/sounds/dash.wav");
    death_sound_buffer_.loadFromFile("assets/sounds/death.wav");
    score_sound_.setBuffer(score_sound_buffer_);
    hurt_sound_.setBuffer(hurt_sound_buffer_);
    heal_sound_.setBuffer(heal_sound_buffer_);
    dash_sound_.setBuffer(dash_sound_buffer_);
    death_sound_.setBuffer(death_sound_buffer_);
    score_sound_.setVolume(55.f);
    hurt_sound_.setVolume(65.f);
    heal_sound_.setVolume(70.f);
    dash_sound_.setVolume(65.f);
    death_sound_.setVolume(72.f);

    texture_background_.loadFromFile("assets/backgrounds/background.png");
    background_.setPosition({0,0});
    background_.setTexture(texture_background_);

    texture_xuefeng_.loadFromFile("assets/charactors/xuefeng.png");
    xuefeng_.setTexture(texture_xuefeng_);
    xuefeng_.setPosition({350,490});
    xuefeng_.setScale({1.5,1.5});
    velocity_xuefeng_ = {0.f, 0.f};

    texture_qiaolezi_.loadFromFile("assets/charactors/qiaolezi.png");
    qiaolezi_.setTexture(texture_qiaolezi_);
    qiaolezi_.setPosition({-200.f, -200.f});

    texture_xuebi_.loadFromFile("assets/charactors/xuebi.png");
    xuebi_.setTexture(texture_xuebi_);
    sf::FloatRect xuebi_bounds = xuebi_.getLocalBounds();
    xuebi_.setOrigin(xuebi_bounds.left + xuebi_bounds.width / 2.f,
                     xuebi_bounds.top + xuebi_bounds.height / 2.f);
    xuebi_.setPosition({-200.f, -200.f});

    texture_tumu_.loadFromFile("assets/charactors/tumu.png");

    InitPipes();

    losebackground_.setFillColor(sf::Color(0,0,0,100));
    losebackground_.setSize({1980,1080});
    font_.loadFromFile("assets/fonts/fonts1.ttf");
    text_lose_.setFont(font_);
    text_lose_.setString(L"学土木去吧");
    text_lose_.setCharacterSize(100);
    text_lose_.setLetterSpacing(5);
    text_lose_.setLineSpacing(1.f);
    sf::FloatRect tb = text_lose_.getLocalBounds();
    text_lose_.setOrigin(tb.left + tb.width / 2.f, tb.top + tb.height / 2.f);
    text_lose_.setPosition(SCREEN_W / 2.f, SCREEN_H / 2.f);
    text_restart_.setFont(font_);
    text_restart_.setString(L"再来一次");
    text_restart_.setCharacterSize(60);
    text_restart_.setLetterSpacing(3);
    sf::FloatRect tr = text_restart_.getLocalBounds();
    text_restart_.setOrigin(tr.left + tr.width / 2.f, tr.top + tr.height / 2.f);
    text_restart_.setPosition(SCREEN_W / 2.f, SCREEN_H / 2.f + 120.f);

    text_final_score_.setFont(font_);
    text_final_score_.setCharacterSize(46);
    text_final_score_.setFillColor(sf::Color(255, 230, 90));
    text_final_score_.setOutlineColor(sf::Color::Black);
    text_final_score_.setOutlineThickness(3.f);
    text_final_score_.setPosition(SCREEN_W / 2.f, SCREEN_H / 2.f - 105.f);

    text_score_.setFont(font_);
    text_score_.setCharacterSize(58);
    text_score_.setFillColor(sf::Color::White);
    text_score_.setOutlineColor(sf::Color::Black);
    text_score_.setOutlineThickness(4.f);
    text_score_.setPosition(50.f, 30.f);

    text_heart_.setFont(font_);
    text_heart_.setCharacterSize(42);
    text_heart_.setFillColor(sf::Color(255, 85, 85));
    text_heart_.setOutlineColor(sf::Color::Black);
    text_heart_.setOutlineThickness(3.f);
    text_heart_.setPosition(50.f, 105.f);

    text_status_.setFont(font_);
    text_status_.setCharacterSize(48);
    text_status_.setOutlineColor(sf::Color::Black);
    text_status_.setOutlineThickness(4.f);

    screen_flash_.setSize({SCREEN_W, SCREEN_H});
    screen_flash_.setFillColor(sf::Color::Transparent);

    death_halo_.setRadius(48.f);
    death_halo_.setPointCount(16);
    death_halo_.setOrigin(48.f, 48.f);
    death_halo_.setScale(1.f, 0.34f);
    death_halo_.setFillColor(sf::Color::Transparent);
    death_halo_.setOutlineColor(sf::Color(255, 225, 80));
    death_halo_.setOutlineThickness(9.f);
    UpdateHud();

}


bool Application::IsRunning() const
{
    return mainwindow_.isOpen();
}


void Application::Tick(){
    ProcessEvents();
    Update();
    Render();
}


void Application::ProcessEvents()
{
    sf::Event event;
    while(mainwindow_.pollEvent(event)){
        HandleEvent(event, mainwindow_);
        }
}


void Application::Update(){

    dt = deltatime_.restart().asSeconds();
    if (!islife_){
        if (death_animation_) {
            UpdateDeathAnimation();
        }
        UpdateEffects();
        return;
    }
    SetXuefengMove();
    UpdatePipes();
    UpdateScore();
    UpdateQiaolezi();
    CheckQiaoleziCollection();
    UpdateXuebi();
    CheckXuebiCollection();
    UpdateDash();
    IsHurt();
    UpdateEffects();
    UpdateHud();

}


void Application::Render(){
    mainwindow_.clear();
    mainwindow_.draw(background_);
    if (!death_animation_) {
        mainwindow_.draw(xuefeng_);
    }
    RenderPipes();
    if (death_animation_) {
        mainwindow_.draw(death_halo_);
        mainwindow_.draw(xuefeng_);
    }
    if (qiaolezi_active_) {
        mainwindow_.draw(qiaolezi_);
    }
    if (xuebi_active_) {
        mainwindow_.draw(xuebi_);
    }
    for (const auto& particle : particles_) {
        mainwindow_.draw(particle.shape);
    }
    if (screen_flash_timer_ > 0.f) {
        mainwindow_.draw(screen_flash_);
    }
    mainwindow_.draw(text_score_);
    mainwindow_.draw(text_heart_);
    if (status_timer_ > 0.f) {
        mainwindow_.draw(text_status_);
    }
    if(!islife_ && !death_animation_){
        DrawGameOver();
    }
    mainwindow_.display();
}

//void Application::Render(){      //碰撞箱检测用
//    mainwindow_.clear();
//    RenderPipes();
//    mainwindow_.draw(xuefeng_);
//
//    {
//        sf::FloatRect bird = xuefeng_.getGlobalBounds();
//        float shrink_w = bird.width  * 0.05f;
//        float shrink_h = bird.height * 0.05f;
//        sf::RectangleShape birdBox;
//        birdBox.setSize({bird.width - shrink_w * 2.f, bird.height - shrink_h * 2.f});
   //     birdBox.setPosition({bird.left + shrink_w, bird.top + shrink_h});
   //     birdBox.setFillColor(sf::Color(0, 255, 0, 60));
   //     birdBox.setOutlineColor(sf::Color::Green);
  //      birdBox.setOutlineThickness(2.f);
  //      mainwindow_.draw(birdBox);
//
  //      for (int i = 0; i < NUM_PIPES; ++i) {
  //          for (auto* pipe : {up_pipes_[i], down_pipes_[i]}) {
  //              sf::FloatRect r = pipe->getGlobalBounds();
  //              float pw = r.width, ph = r.height;
 //               r.left += pw * 0.05f;  r.width = pw * 0.45f;
  //              r.top  += ph * 0.03f;  r.height = ph * 0.94f;
   //             sf::RectangleShape box;
   //             box.setSize({r.width, r.height});
   //             box.setPosition({r.left, r.top});
   //             box.setFillColor(sf::Color(255, 0, 0, 50));
   //             box.setOutlineColor(sf::Color::Red);
   //             box.setOutlineThickness(2.f);
   //             mainwindow_.draw(box);
   //         }
   //     }
  //  }
   // if(!islife_){
  //      DrawGameOver();
  //  }
  //  mainwindow_.display();
//}

void Application::InitPipes() {
    up_pipes_[0]   = &up_pipe1_;
    up_pipes_[1]   = &up_pipe2_;
    up_pipes_[2]   = &up_pipe3_;
    up_pipes_[3]   = &up_pipe4_;
    up_pipes_[4]   = &up_pipe5_;

    down_pipes_[0] = &down_pipe_1;
    down_pipes_[1] = &down_pipe_2;
    down_pipes_[2] = &down_pipe_3;
    down_pipes_[3] = &down_pipe_4;
    down_pipes_[4] = &down_pipe_5;

    for (int i = 0; i < NUM_PIPES; ++i) {
        up_pipes_[i]->setTexture(texture_tumu_);
        up_pipes_[i]->setTextureRect(sf::IntRect(0, 450, (int)PIPE_WIDTH, (int)PIPE_HALF_H));
        up_pipes_[i]->setScale(PIPE_SCALE, PIPE_SCALE);      

        down_pipes_[i]->setTexture(texture_tumu_);
        down_pipes_[i]->setTextureRect(sf::IntRect(0, 0, (int)PIPE_WIDTH, (int)PIPE_HALF_H));
        down_pipes_[i]->setScale(PIPE_SCALE, PIPE_SCALE);        

        pipe_x_[i]     = SCREEN_W + 200.f + i * (PIPE_WIDTH * PIPE_SCALE + PIPE_GAP_H);
        pipe_gap_y_[i] = gap_dist_(rng_);
        pipe_scored_[i] = false;

        up_pipes_[i]->setPosition(pipe_x_[i],
            pipe_gap_y_[i] - PIPE_GAP_V / 2.f - PIPE_HALF_H*PIPE_SCALE);
        down_pipes_[i]->setPosition(pipe_x_[i],
            pipe_gap_y_[i] + PIPE_GAP_V / 2.f);
    }
}


void Application::UpdatePipes() {
    for (int i = 0; i < NUM_PIPES; ++i) {
        float speed = is_dashing_ ? DASH_PIPE_SPEED : PIPE_SPEED;
        pipe_x_[i] -= speed * dt;

        if (pipe_x_[i] < -PIPE_WIDTH*PIPE_SCALE) {
            if (qiaolezi_active_ && qiaolezi_pipe_index_ == i) {
                qiaolezi_active_ = false;
                qiaolezi_pipe_index_ = -1;
            }
            if (xuebi_active_ && xuebi_pipe_index_ == i) {
                xuebi_active_ = false;
                xuebi_pipe_index_ = -1;
            }
            pipe_x_[i]     += NUM_PIPES * (PIPE_WIDTH * PIPE_SCALE + PIPE_GAP_H);
            pipe_gap_y_[i]  = gap_dist_(rng_);
            pipe_scored_[i] = false;

            std::uniform_real_distribution<float> chance(0.f, 1.f);
            if (!qiaolezi_active_ && !xuebi_active_) {
                float roll = chance(rng_);
                if (roll < 0.32f) {
                    qiaolezi_active_ = true;
                    qiaolezi_pipe_index_ = i;
                } else if (roll < 0.55f) {
                    xuebi_active_ = true;
                    xuebi_pipe_index_ = i;
                }
            }
        }

        up_pipes_[i]->setPosition(pipe_x_[i],
            pipe_gap_y_[i] - PIPE_GAP_V / 2.f - PIPE_HALF_H*PIPE_SCALE);
        down_pipes_[i]->setPosition(pipe_x_[i],
            pipe_gap_y_[i] + PIPE_GAP_V / 2.f);
    }
}

void Application::UpdateScore() {
    sf::FloatRect bird = xuefeng_.getGlobalBounds();
    for (int i = 0; i < NUM_PIPES; ++i) {
        float pipe_right = pipe_x_[i] + PIPE_WIDTH * PIPE_SCALE;
        if (!pipe_scored_[i] && pipe_right < bird.left) {
            pipe_scored_[i] = true;
            ++score_;
            high_score_ = std::max(high_score_, score_);
            score_sound_.play();
            ShowStatus(L"得分 +1", sf::Color(255, 235, 90));
        }
    }
}

void Application::UpdateHud() {
    text_score_.setString(L"分数  " + std::to_wstring(score_));
    text_heart_.setString(L"生命  " + std::to_wstring(heart_) + L" / " +
                          std::to_wstring(MAX_HEARTS));
}


void Application::RenderPipes() {
    for (int i = 0; i < NUM_PIPES; ++i) {
        mainwindow_.draw(*up_pipes_[i]);
        mainwindow_.draw(*down_pipes_[i]);
    }
}


void Application::HandleResize(const sf::Event& event) {
    std::cout << "new size " << event.size.width << ", " << event.size.height << '\n';
}


void Application::HandleEvent(const sf::Event& event, sf::RenderWindow& window) {
    switch (event.type) {
        case sf::Event::Resized:       HandleResize(event);        break;
        case sf::Event::Closed: window.close();      break;
        case sf::Event::KeyPressed: HandleKeyPressed(event);   break;
        case sf::Event::MouseButtonPressed: HandleMouseButton(event);   break;
        default: break;
    }
}

void Application::HandleKeyPressed(const sf::Event& event) {
        if(!islife_){
            return;
        }   
        if (event.key.code == sf::Keyboard::Space) {
            velocity_xuefeng_.y = -800.f;
        }
}


void Application::SetXuefengMove(){
    velocity_xuefeng_.y += 1200.f * dt;
    xuefeng_.move(velocity_xuefeng_ * dt);

    sf::FloatRect bounds = xuefeng_.getGlobalBounds();
    if (bounds.top < 0.f) {
        xuefeng_.setPosition(xuefeng_.getPosition().x, 0.f);
        velocity_xuefeng_.y = 0.f;
    }
    if (bounds.top + bounds.height > 1080.f) {
        xuefeng_.setPosition(xuefeng_.getPosition().x, 1080.f - bounds.height);
        velocity_xuefeng_.y = 0.f;
    }
}


void Application::IsHurt() {

    if (is_dashing_ || hurt_cooldown_ > 0.f) {
        return;
    }

    sf::FloatRect bird = xuefeng_.getGlobalBounds();
    float shrink_w = bird.width  * 0.05f;
    float shrink_h = bird.height * 0.05f;
    bird.left   += shrink_w;
    bird.top    += shrink_h;
    bird.width  -= shrink_w * 2.f;
    bird.height -= shrink_h * 2.f;

    for (int i = 0; i < NUM_PIPES; ++i) {
        // 水管碰撞箱也缩小 8%，去掉纹理透明区域的影响
        sf::FloatRect upPipe = up_pipes_[i]->getGlobalBounds();
        sf::FloatRect downPipe = down_pipes_[i]->getGlobalBounds();

        float pw = upPipe.width, ph = upPipe.height;
        upPipe.left += pw * 0.05f;   upPipe.width = pw * 0.45f;
        upPipe.top  += ph * 0.03f;   upPipe.height = ph * 0.94f;
        downPipe.left += pw * 0.05f; downPipe.width = pw * 0.45f;
        downPipe.top  += ph * 0.03f; downPipe.height = ph * 0.94f;


        if (bird.intersects(upPipe) || bird.intersects(downPipe)) {
            heart_--;
            hurt_cooldown_ = 1.35f;
            hurt_sound_.play();
            screen_flash_timer_ = 0.22f;
            screen_flash_.setFillColor(sf::Color(255, 30, 30, 105));
            SpawnParticles({bird.left + bird.width / 2.f, bird.top + bird.height / 2.f},
                           sf::Color(255, 65, 65), 18);
            ShowStatus(L"受伤！短暂无敌", sf::Color(255, 90, 90));
            break;
        }
    }
    if (heart_ <= 0) {
        StartDeathAnimation();
    }
}


void Application::DrawGameOver(){
    text_final_score_.setString(
        L"本局 " + std::to_wstring(score_) + L" 分    最高 " +
        std::to_wstring(high_score_) + L" 分");
    sf::FloatRect score_bounds = text_final_score_.getLocalBounds();
    text_final_score_.setOrigin(score_bounds.left + score_bounds.width / 2.f,
                                score_bounds.top + score_bounds.height / 2.f);
    text_final_score_.setPosition(SCREEN_W / 2.f, SCREEN_H / 2.f - 105.f);
    mainwindow_.draw(losebackground_);
    mainwindow_.draw(text_final_score_);
    mainwindow_.draw(text_lose_);
    mainwindow_.draw(text_restart_);
}


void Application::HandleMouseButton(const sf::Event& event) {
    if (!islife_ && !death_animation_) {
        sf::FloatRect bounds = text_restart_.getGlobalBounds();
        if (bounds.contains((float)event.mouseButton.x, (float)event.mouseButton.y)) {
            Restart();
        }
    }
}

void Application::Restart() {
    islife_ = true;
    heart_ = MAX_HEARTS;
    score_ = 0;
    xuefeng_.setOrigin({0.f, 0.f});
    xuefeng_.setPosition({350.f, 490.f});
    xuefeng_.setColor(sf::Color::White);
    xuefeng_.setRotation(0.f);
    xuefeng_.setScale({1.5f, 1.5f});
    velocity_xuefeng_ = {0.f, 0.f};
    hurt_cooldown_ = 0.f;
    qiaolezi_active_ = false;
    qiaolezi_pipe_index_ = -1;
    xuebi_active_ = false;
    xuebi_pipe_index_ = -1;
    is_dashing_ = false;
    dash_timer_ = 0.f;
    status_timer_ = 0.f;
    screen_flash_timer_ = 0.f;
    death_animation_ = false;
    death_timer_ = 0.f;
    death_particle_timer_ = 0.f;
    death_halo_.setPosition({-200.f, -200.f});
    bgm_.setVolume(42.f);
    particles_.clear();

    for (int i = 0; i < NUM_PIPES; ++i) {
        pipe_x_[i]     = SCREEN_W + 200.f + i * (PIPE_WIDTH * PIPE_SCALE + PIPE_GAP_H);
        pipe_gap_y_[i] = gap_dist_(rng_);
        pipe_scored_[i] = false;
        up_pipes_[i]->setPosition(pipe_x_[i],
            pipe_gap_y_[i] - PIPE_GAP_V / 2.f - PIPE_HALF_H * PIPE_SCALE);
        down_pipes_[i]->setPosition(pipe_x_[i],
            pipe_gap_y_[i] + PIPE_GAP_V / 2.f);
    }
}

void Application::UpdateQiaolezi() {
    if (!qiaolezi_active_)
        return;

    int i = qiaolezi_pipe_index_;
    float cx = pipe_x_[i] + PIPE_WIDTH * PIPE_SCALE / 2.f;
    float cy = pipe_gap_y_[i];
    sf::FloatRect bounds = qiaolezi_.getGlobalBounds();
    float bob = std::sin(clock_.getElapsedTime().asSeconds() * 5.f) * 16.f;
    qiaolezi_.setPosition(cx - bounds.width / 2.f, cy - bounds.height / 2.f + bob);

    if (pipe_x_[i] < -PIPE_WIDTH * PIPE_SCALE) {
        qiaolezi_active_ = false;
    }
}

void Application::CheckQiaoleziCollection() {
    if (!qiaolezi_active_ || is_dashing_)
        return;

    sf::FloatRect bird = xuefeng_.getGlobalBounds();
    sf::FloatRect item = qiaolezi_.getGlobalBounds();

    if (bird.intersects(item)) {
        qiaolezi_active_ = false;
        is_dashing_ = true;
        dash_timer_ = DASH_DURATION;
        dash_sound_.play();
        SpawnParticles({item.left + item.width / 2.f, item.top + item.height / 2.f},
                       sf::Color(255, 205, 70), 28);
        ShowStatus(L"雪峰冲刺！", sf::Color(255, 220, 60));
        screen_flash_timer_ = 0.16f;
        screen_flash_.setFillColor(sf::Color(255, 225, 60, 60));
    }
}

void Application::UpdateXuebi() {
    if (!xuebi_active_)
        return;

    int i = xuebi_pipe_index_;
    float cx = pipe_x_[i] + PIPE_WIDTH * PIPE_SCALE / 2.f;
    float cy = pipe_gap_y_[i];
    float time = clock_.getElapsedTime().asSeconds();
    float bob = std::sin(time * 5.f + 1.5f) * 18.f;
    xuebi_.setRotation(std::sin(time * 3.5f) * 8.f);
    xuebi_.setPosition(cx, cy + bob);
}

void Application::CheckXuebiCollection() {
    if (!xuebi_active_)
        return;

    sf::FloatRect bird = xuefeng_.getGlobalBounds();
    sf::FloatRect item = xuebi_.getGlobalBounds();
    if (bird.intersects(item)) {
        xuebi_active_ = false;
        heal_sound_.play();
        SpawnParticles({item.left + item.width / 2.f, item.top + item.height / 2.f},
                       sf::Color(80, 255, 130), 26);
        screen_flash_timer_ = 0.2f;
        screen_flash_.setFillColor(sf::Color(70, 255, 120, 65));
        if (heart_ < MAX_HEARTS) {
            ++heart_;
            ShowStatus(L"雪碧续命！生命 +1", sf::Color(90, 255, 135));
        } else {
            ShowStatus(L"生命已满！", sf::Color(90, 255, 135));
        }
    }
}

void Application::UpdateDash() {
    if (!is_dashing_)
        return;

    dash_timer_ -= dt;
    if (dash_timer_ <= 0.f) {
        is_dashing_ = false;
        xuefeng_.setColor(sf::Color::White);
        return;
    }
}

void Application::StartDeathAnimation() {
    islife_ = false;
    death_animation_ = true;
    death_timer_ = 0.f;
    death_particle_timer_ = 0.f;
    status_timer_ = 0.f;
    high_score_ = std::max(high_score_, score_);
    death_sound_.play();
    screen_flash_timer_ = 0.3f;
    screen_flash_.setFillColor(sf::Color(255, 245, 210, 120));

    sf::FloatRect bird = xuefeng_.getGlobalBounds();
    sf::Vector2f center = {bird.left + bird.width / 2.f,
                           bird.top + bird.height / 2.f};
    sf::FloatRect local = xuefeng_.getLocalBounds();
    xuefeng_.setOrigin(local.left + local.width / 2.f,
                       local.top + local.height / 2.f);
    xuefeng_.setPosition(center);
    death_start_position_ = center;
    SpawnParticles(center, sf::Color(255, 230, 110), 34);
}

void Application::UpdateDeathAnimation() {
    death_timer_ += dt;
    death_particle_timer_ -= dt;

    const float freeze_time = 0.12f;
    float motion_time = std::max(0.f, death_timer_ - freeze_time);
    float progress = std::min(1.f, motion_time / (DEATH_DURATION - freeze_time));
    float eased = 1.f - std::pow(1.f - progress, 2.f);

    float spiral_width = 30.f + 105.f * progress;
    float x = death_start_position_.x +
              std::sin(progress * 6.f * 3.14159265f) * spiral_width;
    float y = death_start_position_.y - 900.f * eased;
    xuefeng_.setPosition(x, y);
    xuefeng_.setRotation(1080.f * eased);

    float scale = 1.5f - 0.85f * eased;
    xuefeng_.setScale(scale, scale);
    float fade = progress < 0.55f ? 1.f : 1.f - (progress - 0.55f) / 0.45f;
    sf::Uint8 alpha = static_cast<sf::Uint8>(255.f * std::max(0.f, fade));
    xuefeng_.setColor(sf::Color(255, 255, 235, alpha));

    sf::FloatRect bird = xuefeng_.getGlobalBounds();
    sf::Vector2f center = {bird.left + bird.width / 2.f,
                           bird.top + bird.height / 2.f};
    death_halo_.setPosition(center.x, bird.top - 22.f);
    sf::Color halo_color = death_halo_.getOutlineColor();
    halo_color.a = alpha;
    death_halo_.setOutlineColor(halo_color);

    if (progress > 0.f && progress < 0.93f && death_particle_timer_ <= 0.f) {
        SpawnParticles(center, sf::Color(255, 245, 190), 3);
        death_particle_timer_ = 0.055f;
    }

    bgm_.setVolume(42.f * (1.f - 0.78f * progress));

    if (death_timer_ >= DEATH_DURATION) {
        death_animation_ = false;
        xuefeng_.setColor(sf::Color::Transparent);
        death_halo_.setPosition({-200.f, -200.f});
        screen_flash_timer_ = 0.18f;
        screen_flash_.setFillColor(sf::Color(255, 255, 225, 80));
    }
}

void Application::UpdateEffects() {
    if (islife_ && hurt_cooldown_ > 0.f) {
        hurt_cooldown_ = std::max(0.f, hurt_cooldown_ - dt);
        bool visible = static_cast<int>(hurt_cooldown_ * 12.f) % 2 == 0;
        xuefeng_.setColor(visible ? sf::Color(255, 150, 150, 235)
                                  : sf::Color(255, 255, 255, 70));
    } else if (islife_ && is_dashing_) {
        xuefeng_.setColor(sf::Color(255, 245, 100));
        sf::FloatRect bird = xuefeng_.getGlobalBounds();
        SpawnParticles({bird.left, bird.top + bird.height / 2.f},
                       sf::Color(255, 220, 70), 2);
    } else if (islife_) {
        xuefeng_.setColor(sf::Color::White);
    }

    if (status_timer_ > 0.f) {
        status_timer_ = std::max(0.f, status_timer_ - dt);
        text_status_.move(0.f, -24.f * dt);
        sf::Color color = text_status_.getFillColor();
        color.a = static_cast<sf::Uint8>(255.f * std::min(1.f, status_timer_ / 0.35f));
        text_status_.setFillColor(color);
    }

    if (screen_flash_timer_ > 0.f) {
        screen_flash_timer_ = std::max(0.f, screen_flash_timer_ - dt);
    }

    for (auto& particle : particles_) {
        particle.lifetime -= dt;
        particle.velocity.y += 420.f * dt;
        particle.shape.move(particle.velocity * dt);
        sf::Color color = particle.shape.getFillColor();
        color.a = static_cast<sf::Uint8>(255.f *
            std::max(0.f, particle.lifetime / particle.max_lifetime));
        particle.shape.setFillColor(color);
    }
    particles_.erase(
        std::remove_if(particles_.begin(), particles_.end(),
                       [](const Particle& p) { return p.lifetime <= 0.f; }),
        particles_.end());
}

void Application::SpawnParticles(sf::Vector2f position, sf::Color color, int count) {
    std::uniform_real_distribution<float> angle_dist(0.f, 6.2831853f);
    std::uniform_real_distribution<float> speed_dist(90.f, 360.f);
    std::uniform_real_distribution<float> size_dist(5.f, 13.f);
    std::uniform_real_distribution<float> life_dist(0.35f, 0.75f);

    for (int i = 0; i < count; ++i) {
        float angle = angle_dist(rng_);
        float speed = speed_dist(rng_);
        Particle particle;
        float size = size_dist(rng_);
        particle.shape.setSize({size, size});
        particle.shape.setOrigin(size / 2.f, size / 2.f);
        particle.shape.setPosition(position);
        particle.shape.setFillColor(color);
        particle.velocity = {std::cos(angle) * speed, std::sin(angle) * speed};
        particle.lifetime = life_dist(rng_);
        particle.max_lifetime = particle.lifetime;
        particles_.push_back(particle);
    }
}

void Application::ShowStatus(const sf::String& message, sf::Color color) {
    text_status_.setString(message);
    text_status_.setFillColor(color);
    sf::FloatRect bounds = text_status_.getLocalBounds();
    text_status_.setOrigin(bounds.left + bounds.width / 2.f,
                           bounds.top + bounds.height / 2.f);
    text_status_.setPosition(SCREEN_W / 2.f, 175.f);
    status_timer_ = 1.15f;
}
