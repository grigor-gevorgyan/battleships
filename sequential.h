#pragma once

#include "player_strategy.h"

class SequentialStrategy : public PlayerStrategy {
public:
	SequentialStrategy(int id): x(0), y(0), _id(id) {}

	virtual Move make_move();
	virtual void handle_result(MoveResult) {};
	virtual void handle_move(Move) {};
	virtual void game_over(GameResult) {};
	virtual Board game_start(
		const std::string& opponent_name,
		PlayerNumber number
	);
	std::string get_name() { return "SEQ" + std::to_string(_id); };
private:
	int x;
	int y;
	int _id;
};