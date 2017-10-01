#include "rank_table.h"
#include <algorithm>

RankTable_t::~RankTable_t() {
	for (int i = 0; i < _ranks.size(); i++) {
		delete _ranks[i];
	}
}

void RankTable_t::save_score(const char* player_name, unsigned int score) {
	auto rank = new Rank_t(player_name, score);

	auto finded_rank = _player_name_to_rank.find(player_name);

	if (finded_rank != _player_name_to_rank.end()) {
		finded_rank->second->score = std::max(finded_rank->second->score, score);
	} else {
		_ranks.push_back(rank);
		auto place = find_place(score);
		_ranks.insert(_ranks.begin() + place, rank);
		_player_name_to_rank[player_name] = rank;
	}
}

void RankTable_t::load_ranks() {
	for (int i = 0; i < _ranks.size(); i++) {
		_player_name_to_rank[std::string(_ranks[i]->player_name)] = _ranks[i];
	}
}

Rank_t RankTable_t::operator[](unsigned int i) {
	return *_ranks[i];
}

Rank_t RankTable_t::operator[](std::string player_name) {
	return *_player_name_to_rank[player_name];
}

unsigned int RankTable_t::find_place(unsigned int score) {
	auto pos = std::lower_bound(_ranks.begin(), _ranks.end(), score, [](const Rank_t* a, unsigned int b) { return a->score >= b; });
 	return pos - _ranks.begin();
}

size_t RankTable_t::size() const {
	return _ranks.size();
}
