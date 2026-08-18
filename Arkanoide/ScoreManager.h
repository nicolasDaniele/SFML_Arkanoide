#pragma once

class ScoreManager
{
private:
    ScoreManager() = default;

    int score;

public:
    static ScoreManager& get_instance()
    {
        static ScoreManager instance;
        return instance;
    }

    void init();
    void add_to_score(int score);
    int get_score();
    void reset_score();
};