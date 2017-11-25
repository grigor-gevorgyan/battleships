#include "sequential.h"

Move SequentialStrategy::make_move() {
	Move move(x, y);

	if (x == BoardSize - 1) {
		x = 0;
		y++;
	}
	else{
		x++;
	}
	return move;
}

Board SequentialStrategy::game_start(const std::string& opponent_name, PlayerNumber number) {
	x = 0;
	y = 0;
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
		BoardRow{ o, o, o, o, o, o, o, o, x, o },
		BoardRow{ o, o, o, o, o, o, o, o, o, o },
	};
	return b;
}