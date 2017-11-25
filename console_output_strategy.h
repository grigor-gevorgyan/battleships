#pragma once

#include "output_strategy.h"

#include <iostream>
#include <cassert>

class ConsoleOutputStrategy : public OutputStrategy {
public:
	virtual void draw(
		const Board& board_1,
		const Board& board_2,
		const PlayerNumber& turn,
		const Move& move
		) {
			std::cout << (turn == FirstPlayer ? "Player 1" : "Player 2") << ": ";
			std::cout << char(move.first + 'A') << ' ' <<
				move.second + 1 << std::endl;
	}
};