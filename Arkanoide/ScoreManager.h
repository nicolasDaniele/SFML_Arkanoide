#pragma once

class ScoreManager
{
public:
	static ScoreManager& get_instance()
	{
		static ScoreManager instance;
		return instance;
	}

	void init();
	void add_to_score(int addedScore);
	int get_score() const;
	void reset_score();

private:
	ScoreManager() = default;

	int score = 0;
};