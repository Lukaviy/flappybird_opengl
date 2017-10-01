#include "rank.h"
#include <cstring>
#include <algorithm>

Rank_t::Rank_t() : score(0) {
	strcpy_s(player_name, MAX_PLAYER_NAME_LEN, "N/A");
}

Rank_t::Rank_t(const char* playername, unsigned int score) : score(score) {
	strcpy_s(player_name, MAX_PLAYER_NAME_LEN, playername);
}

bool Rank_t::operator<(const Rank_t& rank) const {
	return score < rank.score;
}
