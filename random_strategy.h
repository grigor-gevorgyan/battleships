#pragma once
#include <vector>
#include "player_strategy.h"

class RandomStrategy : public PlayerStrategy {
public:
	RandomStrategy(int id) : _id(id) {}
	virtual Move make_move();
	virtual void handle_result(MoveResult) {}
	virtual void handle_move(Move) {};
	virtual void game_over(GameResult) {};
	virtual Board game_start(
		const std::string& opponent_name,
		PlayerNumber number
	);
	std::string get_name() { return "RAND " + std::to_string(_id); };
private:
	std::vector<Move> _valid_moves;
	int _id;
};