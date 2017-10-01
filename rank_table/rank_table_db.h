#pragma once
#include "rank.h"
#include <queue>

class RankTableDb_t {
public:
	virtual ~RankTableDb_t() = default;
	virtual void save_score(Rank_t rank) = 0;
	virtual std::vector<Rank_t> load_scores() = 0;
};
