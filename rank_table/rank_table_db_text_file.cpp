#include "rank_table_db_text_file.h"
#include <fstream>
#include <string>

RankTableTextFileDb_t::RankTableTextFileDb_t(const char* file_name) : file_name(file_name) {}

void RankTableTextFileDb_t::save_score(Rank_t rank) {
	std::ofstream fout(file_name, std::ios::app);

	if (!fout) {
		throw CantOpenFileException_t();
	}

	fout << rank.player_name << ' ' << rank.score << std::endl;
}

std::vector<Rank_t> RankTableTextFileDb_t::load_scores() {
	std::ifstream fin(file_name);

	std::vector<Rank_t> ranks;

	if (!fin.is_open()) {
		throw CantOpenFileException_t();
	}

	fin.seekg(0, fin.beg);

	while (!fin.eof()) {
		std::string player_name;
		fin >> player_name;

		if (fin.eof()) {
			if (player_name.size() == 0) {
				return ranks;
			}
			throw FileIsCorruptedException_t();
		}

		if (player_name.size() > Rank_t::MAX_PLAYER_NAME_LEN) {
			throw FileIsCorruptedException_t();
		}

		unsigned int score;

		fin >> score;

		Rank_t rank(player_name.c_str(), score);

		ranks.push_back(rank);
	}

	return ranks;
}
