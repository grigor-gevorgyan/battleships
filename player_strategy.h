#pragma once

#include <string>

#include "utils.h"

class PlayerStrategy {
public:
	virtual Move make_move() = 0;
	virtual void handle_result(MoveResult) = 0;
	virtual void handle_move(Move) = 0;
	virtual void game_over(GameResult) = 0;
	virtual Board game_start(
		const std::string& opponent_name,
		PlayerNumber number
	) = 0;
	// TODO handle cheats
	virtual std::string get_name() = 0;
};