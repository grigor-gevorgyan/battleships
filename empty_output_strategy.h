#pragma once

#include "output_strategy.h"

#include <iostream>
#include <cassert>

class EmptyOutputStrategy : public OutputStrategy {
public:
	virtual void draw(
		const Board& board_1,
		const Board& board_2,
		const PlayerNumber& turn,
		const Move& move
	) {}
};
