#include "random_strategy.h"

#include <cmath>

Move RandomStrategy::make_move() {
	int i = rand() % _valid_moves.size();
	Move move = _valid_moves[i];
	_valid_moves.erase(_valid_moves.begin() + i);
	return move;
}

Board RandomStrategy::game_start(const std::string& opponent_name, PlayerNumber number) {
	BoardEntry x = BoardEntry::HealthyShip;
	BoardEntry o = BoardEntry::Empty;
	Board b = {
		BoardRow{ x, x, x, x, o, o, o, o, o, o },
		BoardRow{ o, o, o, o, o, o, x, o, x, o },
		BoardRow{ x, o, o, o, o, o, x, o, o, o },
		BoardRow{ o, o, x, x, x, o, x, o, o, o },
		BoardRow{ o, o, o, o, o, o, o, o, x, o },
		BoardRow{ o, o, o, o, o, o, o, o, x, o },
		BoardRow{ o, x, o, x, o, o, o, o, o, o },
		BoardRow{ o, o, o, x, o, x, x, o, o, o },
		BoardRow{ o, o, o, o, o, o, o, o, o, o },
		BoardRow{ o, o, o, o, o, o, o, o, o, x },
	};

	_valid_moves.clear();
	for (int i = 0; i < BoardSize; ++i)
	{
		for (int j = 0; j < BoardSize; ++j)
		{
			_valid_moves.push_back(Move(i, j));
		}
	}

	return b;
}