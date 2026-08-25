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

private:
	LivesManager() = default;

	static const int maxLives = 5;
	int lives = 0;
};
