#include "rank_table.h"

RankTable_t::RankTable_t(RankTableDb_t& rank_table_db): _rank_table_db(rank_table_db) {
	_ranks = rank_table_db.load_scores();
	sort(_ranks.begin(), _ranks.end(), Rank_t::greater());
}

void RankTable_t::save_score(const char* player_name, unsigned int score) {
	Rank_t rank(player_name, score);

	_rank_table_db.save_score(rank);

	_ranks.push_back(rank);
	sort(_ranks.begin(), _ranks.end(), Rank_t::greater());
}

Rank_t RankTable_t::operator[](unsigned int i) {
	return _ranks[i];
}

size_t RankTable_t::size() const {
	return _ranks.size();
}
