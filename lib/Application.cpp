#include "Application.h"
#include <iostream>
#include <random>


Application::Application():     //创建窗口
    mainwindow_(sf::VideoMode(1920, 1080), L"怀念张雪峰", sf::Style::Default)
{

    mainwindow_.setFramerateLimit(120);

    bgm_.openFromFile("assets/music/bgm.wav");
    bgm_.play();
    bgm_.setLoop(true);

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

    texture_tumu_.loadFromFile("assets/charactors/tumu.png");

    InitPipes();

    losebackground_.setFillColor(sf::Color(0,0,0,100));
    losebackground_.setSize({1980,1080});
    font_.loadFromFile("assets/fonts/fonts1.ttf");
    text_lose_.setFont(font_);
    text_lose_.setString(L"学土木去吧");
    text_lose_.setCharacterSize(100);
    text_lose_.setLetterSpacing(5);
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
        return;
    }
    SetXuefengMove();
    UpdatePipes();
    UpdateQiaolezi();
    CheckQiaoleziCollection();
    UpdateDash();
    if (!is_dashing_) {
        IsHurt();
    }

}


void Application::Render(){
    mainwindow_.clear();
    mainwindow_.draw(background_);
    mainwindow_.draw(xuefeng_);
    RenderPipes();
    if (qiaolezi_active_) {
        mainwindow_.draw(qiaolezi_);
    }
    if(!islife_){
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
            pipe_x_[i]     += NUM_PIPES * (PIPE_WIDTH * PIPE_SCALE + PIPE_GAP_H);
            pipe_gap_y_[i]  = gap_dist_(rng_);

            std::uniform_real_distribution<float> chance(0.f, 1.f);
            if (!qiaolezi_active_ && chance(rng_) < 0.5f) {
                qiaolezi_active_ = true;
                qiaolezi_pipe_index_ = i;
            }
        }

        up_pipes_[i]->setPosition(pipe_x_[i],
            pipe_gap_y_[i] - PIPE_GAP_V / 2.f - PIPE_HALF_H*PIPE_SCALE);
        down_pipes_[i]->setPosition(pipe_x_[i],
            pipe_gap_y_[i] + PIPE_GAP_V / 2.f);
    }
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
            hurt_cooldown_ = 1.0f;
            break;
        }
    }
    if (heart_ <= 0) {
        islife_ = false;
    }
}


void Application::DrawGameOver(){
    mainwindow_.draw(losebackground_);
    mainwindow_.draw(text_lose_);
    mainwindow_.draw(text_restart_);
}


void Application::HandleMouseButton(const sf::Event& event) {
    if (!islife_) {
        sf::FloatRect bounds = text_restart_.getGlobalBounds();
        if (bounds.contains((float)event.mouseButton.x, (float)event.mouseButton.y)) {
            Restart();
        }
    }
}

void Application::Restart() {
    islife_ = true;
    heart_ = 1;
    xuefeng_.setPosition({350.f, 490.f});
    velocity_xuefeng_ = {0.f, 0.f};
    hurt_cooldown_ = 0.f;
    qiaolezi_active_ = false;
    is_dashing_ = false;
    dash_timer_ = 0.f;

    for (int i = 0; i < NUM_PIPES; ++i) {
        pipe_x_[i]     = SCREEN_W + 200.f + i * (PIPE_WIDTH * PIPE_SCALE + PIPE_GAP_H);
        pipe_gap_y_[i] = gap_dist_(rng_);
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
    qiaolezi_.setPosition(cx - bounds.width / 2.f, cy - bounds.height / 2.f);

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
    }
}

void Application::UpdateDash() {
    if (!is_dashing_)
        return;

    dash_timer_ -= dt;
    if (dash_timer_ <= 0.f) {
        is_dashing_ = false;
        return;
    }
}