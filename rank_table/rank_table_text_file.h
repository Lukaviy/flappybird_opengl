#pragma once
#include "rank_table.h"

class CantOpenFileException_t : public RankTableException_t {};
class FileIsCorruptedException_t : public RankTableException_t {};

class RankTableTextFile_t : public RankTable_t {
public:
	~RankTableTextFile_t() override = default;
	RankTableTextFile_t(const char* file_name);
	void save_score(const char* player_name, unsigned score) override;
	void load_ranks() override;
protected:
	std::string _file_name;
};
