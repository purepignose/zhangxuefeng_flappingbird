#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <random>

class Application{
    private:
        sf::RenderWindow mainwindow_;

        sf::Music bgm_;

        sf::Clock clock_;
        sf::Clock deltatime_;

        sf::Texture texture_xuefeng_;
        sf::Sprite xuefeng_;
        sf::Vector2f velocity_xuefeng_;
        float dt = 0.0;

        sf::Sprite background_;
        sf::Texture texture_background_;

        sf::Texture texture_tumu_;
        sf::Sprite up_pipe1_;
        sf::Sprite up_pipe2_;
        sf::Sprite up_pipe3_;
        sf::Sprite up_pipe4_;
        sf::Sprite up_pipe5_;
        sf::Sprite down_pipe_1;
        sf::Sprite down_pipe_2;
        sf::Sprite down_pipe_3;
        sf::Sprite down_pipe_4;
        sf::Sprite down_pipe_5;

        // --- 新增：水管位置 & 随机数 ---
        float pipe_x_[5] = {};
        float pipe_gap_y_[5] = {};

        std::mt19937 rng_{std::random_device{}()};
        std::uniform_real_distribution<float> gap_dist_{300.f, 780.f};

        const float PIPE_SCALE   = 2.f;
        const float PIPE_WIDTH   = 180.f;
        const float PIPE_HALF_H  = 450.f;   // 纹理一半高度 (精灵原始高度)
        const float PIPE_GAP_H   = 433.f;   // 左右水管间距 (650 * 2/3)
        const float PIPE_GAP_V   = 550.f;   // 上下开口
        const float PIPE_SPEED   = 360.f;   // 水平速度 px/s
        const int   NUM_PIPES    = 5;
        const float SCREEN_W     = 1920.f;
        const float SCREEN_H     = 1080.f;

        sf::Sprite* up_pipes_[5];
        sf::Sprite* down_pipes_[5];

        bool islife_ = true;
        int heart_ = 1;
        float hurt_cooldown_ = 0.f;

        sf::RectangleShape losebackground_;
        sf::Text text_lose_;
        sf::Font font_;
        sf::Text text_restart_;


        sf::Texture texture_qiaolezi_;
        sf::Sprite qiaolezi_;
        
        bool qiaolezi_active_ = false;
        int  qiaolezi_pipe_index_ = -1;

        bool is_dashing_ = false;
        float dash_timer_ = 0.f;
        const float DASH_DURATION = 1.5f;
        const float DASH_PIPE_SPEED = 1200.f;

    public:
        Application();

        bool IsRunning() const;

        void Tick();

    private:
        void HandleResize(const sf::Event& event);
        void HandleMouseButton(const sf::Event& event);
        void HandleEvent(const sf::Event& event, sf::RenderWindow& window);

        void ProcessEvents();
        void Update();
        void Render();

        void HandleKeyPressed(const sf::Event& event);
        void SetXuefengMove();
        void InitPipes();
        void UpdatePipes();
        void RenderPipes();
        void IsHurt();
        void GameOver();
        void DrawGameOver();
        void Restart();
        
        void UpdateQiaolezi();
        void CheckQiaoleziCollection();
        void UpdateDash();



};