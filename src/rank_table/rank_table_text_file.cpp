#include "rank_table_text_file.h"
#include <fstream>
#include <string>

RankTableTextFile_t::RankTableTextFile_t(const char* file_name) {
	_file_name = file_name;
}

void RankTableTextFile_t::save_score(const char* player_name, unsigned score) {
	RankTable_t::save_score(player_name, score);

	std::ofstream fout(_file_name);

	if (!fout) {
		throw CantOpenFileException_t();
	}

	for (int i = 0; i < _ranks.size(); i++) {
		fout << _ranks[i]->player_name << ' ' << _ranks[i]->score << std::endl;
	}
}

void RankTableTextFile_t::load_ranks() {
	std::ifstream fin(_file_name);

	if (!fin.is_open()) {
		throw CantOpenFileException_t();
	}

	while (!fin.eof()) {
		std::string player_name;
		fin >> player_name;

		if (fin.eof()) {
			if (player_name.size() == 0) {
				return;
			}
			throw FileIsCorruptedException_t();
		}

		if (player_name.size() > Rank_t::MAX_PLAYER_NAME_LEN) {
			throw FileIsCorruptedException_t();
		}

		if (_player_name_to_rank.find(player_name) != _player_name_to_rank.end()) {
			throw FileIsCorruptedException_t();
		}

		unsigned int score;

		fin >> score;

		if (_ranks.size() > 0 && _ranks.back()->score < score) {
			throw FileIsCorruptedException_t();
		}

		auto rank = new Rank_t(player_name.c_str(), score);

		_ranks.push_back(rank);
		_player_name_to_rank[player_name] = rank;
	}

	RankTable_t::load_ranks();
}
