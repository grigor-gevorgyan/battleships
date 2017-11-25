#pragma once

#include "tournament.h"
#include "judge.h"
#include <vector>
#include <mutex>

class Match : public Tournament {
public:
	Match(PlayerStrategy* player_1, PlayerStrategy* player_2, OutputStrategy* output);
	
	virtual void run(int games);
	virtual void print_result();
	virtual std::pair<int, int> get_score();
private:
	Judge _judge;
	PlayerStrategy* _player_1;
	PlayerStrategy* _player_2;
	OutputStrategy* _output;
	std::vector<GameResult> _results; 
	std::pair<int, int> _score;
};

std::pair<int, int> get_match_score(
	PlayerStrategy* player_1,
	PlayerStrategy* player_2,
	OutputStrategy* output,
	int games,
	std::mutex* print_lock = nullptr
);