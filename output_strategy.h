#pragma once

#include "utils.h"

struct OutputStrategy {
	virtual void draw(
		const Board& board_1,
		const Board& board_2,
		const PlayerNumber& turn,
		const Move& move
		) = 0;
	virtual ~OutputStrategy() {}
};

