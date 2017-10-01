#pragma once
#include <vector>
#include "rank.h"
#include <unordered_map>

class RankTableException_t {};
class BadPlayerNameException_t {};

class RankTable_t {
public:
	virtual ~RankTable_t();
	virtual void save_score(const char* player_name, unsigned int score);
	virtual void load_ranks();

	Rank_t operator[](unsigned int i);
	Rank_t operator[](std::string player_name);
	bool exists(std::string player_name);
	unsigned int find_place(unsigned int score);
	size_t size() const;
protected:
	std::unordered_map<std::string, Rank_t*> _player_name_to_rank;
	std::vector<Rank_t*> _ranks;
};
