#pragma once

class LivesManager
{
public:
	static LivesManager& get_instance()
	{
		static LivesManager instance;
		return instance;
	}

	void init();
	void lose_life();
	void add_life();
	void reset_lives();
	int get_lives() const;
	bool is_game_over() const;
	bool is_at_max_lives() const;

private:
	LivesManager() = default;

	static const int startLives = 5;
	static const int maxLives = 9;
	int lives = 0;
};
