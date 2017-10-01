#pragma once
#include "rank.h"
#include "rank_table_db.h"
#include "rank_table.h"
#include <fstream>

class CantOpenFileException_t : public RankTableException_t {};
class FileIsCorruptedException_t : public RankTableException_t {};

class RankTableTextFileDb_t : public RankTableDb_t {
public:
	RankTableTextFileDb_t(const char* file_name);
	void save_score(Rank_t) override;
	std::vector<Rank_t> load_scores() override;
private:
	std::string file_name;
};
