#pragma once
#include <vector>
#include "rank.h"
#include "rank_table_db.h"

class RankTableException_t {};
class BadPlayerNameException_t {};

class RankTable_t {
public:
	RankTable_t(RankTableDb_t& rank_table_db);

	virtual ~RankTable_t() = default;
	virtual void save_score(const char* player_name, unsigned int score);

	Rank_t operator[](unsigned int i);
	size_t size() const;
protected:
	std::vector<Rank_t> _ranks;
	RankTableDb_t& _rank_table_db;
};
