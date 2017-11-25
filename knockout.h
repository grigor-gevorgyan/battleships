#pragma once

#include "tournament.h"
#include "judge.h"
#include <vector>

class Knockout : public Tournament {
public:
	Knockout(std::vector<PlayerStrategy*> players, OutputStrategy* output, bool use_threads = false);

	virtual void run(int games);
	virtual void print_result();
private:
	std::vector<PlayerStrategy*> _players;
	OutputStrategy* _output;
	std::vector<GameResult> _results;
	bool _use_threads;
};