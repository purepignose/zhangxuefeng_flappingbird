#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <random>
#include <vector>
#include <cstddef>

class Application{
    private:
        sf::RenderWindow mainwindow_;

        sf::Music bgm_;
        sf::SoundBuffer score_sound_buffer_;
        sf::SoundBuffer hurt_sound_buffer_;
        sf::SoundBuffer heal_sound_buffer_;
        sf::SoundBuffer dash_sound_buffer_;
        sf::SoundBuffer death_sound_buffer_;
        sf::SoundBuffer flap_sound_buffer_;
        sf::Sound score_sound_;
        sf::Sound hurt_sound_;
        sf::Sound heal_sound_;
        sf::Sound dash_sound_;
        sf::Sound death_sound_;
        sf::Sound flap_sound_;

        sf::Clock clock_;
        sf::Clock deltatime_;

        sf::Texture texture_xuefeng_;
        sf::Sprite xuefeng_;
        sf::Sprite xuefeng_visual_;
        sf::Vector2f velocity_xuefeng_;
        float dt = 0.0;
        float flight_rotation_ = 0.f;
        float flap_effect_timer_ = 0.f;
        float flight_particle_timer_ = 0.f;
        sf::CircleShape flap_ring_;
        float flap_ring_timer_ = 0.f;
        static constexpr float FLAP_EFFECT_DURATION = 0.18f;

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
        bool pipe_scored_[5] = {};

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
        static constexpr int MAX_HEARTS = 3;
        int heart_ = MAX_HEARTS;
        float hurt_cooldown_ = 0.f;
        int score_ = 0;
        int high_score_ = 0;
        bool waiting_to_start_ = true;
        std::vector<int> score_history_;
        std::size_t history_page_ = 0;
        bool confirm_clear_history_ = false;
        bool clear_history_error_ = false;
        static constexpr std::size_t HISTORY_PAGE_SIZE = 10;

        sf::RectangleShape losebackground_;
        sf::Text text_lose_;
        sf::Font font_;
        sf::Text text_restart_;
        sf::Text text_final_score_;
        sf::Text text_score_;
        sf::Text text_heart_;
        sf::Text text_status_;
        float status_timer_ = 0.f;
        sf::RectangleShape screen_flash_;
        float screen_flash_timer_ = 0.f;
        sf::CircleShape death_halo_;
        bool death_animation_ = false;
        float death_timer_ = 0.f;
        float death_particle_timer_ = 0.f;
        float death_start_rotation_ = 0.f;
        sf::Vector2f death_start_position_;
        static constexpr float DEATH_DURATION = 1.85f;


        sf::Texture texture_qiaolezi_;
        sf::Sprite qiaolezi_;
        
        bool qiaolezi_active_ = false;
        int  qiaolezi_pipe_index_ = -1;

        sf::Texture texture_xuebi_;
        sf::Sprite xuebi_;
        bool xuebi_active_ = false;
        int xuebi_pipe_index_ = -1;

        bool is_dashing_ = false;
        float dash_timer_ = 0.f;
        const float DASH_DURATION = 1.5f;
        const float DASH_PIPE_SPEED = 1200.f;

        struct Particle {
            sf::RectangleShape shape;
            sf::Vector2f velocity;
            float lifetime = 0.f;
            float max_lifetime = 0.f;
        };
        std::vector<Particle> particles_;

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
        void UpdateFlightVisual();
        void SpawnFlapParticles();
        void InitPipes();
        void UpdatePipes();
        void UpdateScore();
        void UpdateHud();
        void RenderPipes();
        void IsHurt();
        void GameOver();
        void DrawGameOver();
        void DrawStartOverlay();
        void LoadScoreHistory();
        void RecordScore();
        void ClearScoreHistory();
        int ExamScoreForGameScore(int score) const;
        sf::String UniversityForScore(int score) const;
        void Restart();
        
        void UpdateQiaolezi();
        void CheckQiaoleziCollection();
        void UpdateXuebi();
        void CheckXuebiCollection();
        void UpdateDash();
        void StartDeathAnimation();
        void UpdateDeathAnimation();
        void UpdateEffects();
        void SpawnParticles(sf::Vector2f position, sf::Color color, int count);
        void ShowStatus(const sf::String& message, sf::Color color);



};
