#pragma once

#include "output_strategy.h"
#include "player_strategy.h"
#include "utils.h"

PlayerNumber next_player(PlayerNumber player);

class Judge {
public:
	GameResult host_game(
		PlayerStrategy* player_0,
		PlayerStrategy* player_1,
		OutputStrategy* output = nullptr);

private:
	Board _boards[2];
	PlayerStrategy* _players[2];

	bool is_alive(PlayerNumber number);
	bool is_valid_move(Move move);
	bool is_valid_board(PlayerNumber number);
	void end_game(GameResult result); 
	MoveResult make_move(Move move, PlayerNumber number);
};