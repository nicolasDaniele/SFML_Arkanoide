#pragma once

class ScoreManager
{
private:
    ScoreManager();
	~ScoreManager() { }
	static ScoreManager* instance;
    int score;

public:
    static ScoreManager* get_instance()
    {
        if (instance == nullptr)
        {
            instance = new ScoreManager();
        }
        return instance;
    }

    void add_to_score(int score);
    int get_score();
    void reset_score();
};