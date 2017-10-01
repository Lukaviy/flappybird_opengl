#pragma once

struct Rank_t {
	static const size_t MAX_PLAYER_NAME_LEN = 10;
	char player_name[MAX_PLAYER_NAME_LEN];
	unsigned int score;
	Rank_t();
	Rank_t(const char* player_name, unsigned int score);

	bool operator<(const Rank_t& rank) const;

	struct greater {
		bool operator()(Rank_t const &a, Rank_t const &b) const { return a.score > b.score; }
	};
};


