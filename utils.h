#pragma once

#include <array>
#include <algorithm>

const int BoardSize = 10;

enum BoardEntry {
	Hidden,
	Empty,
	HealthyShip,
	DamagedShip,
};

typedef std::pair<int, int> Move;
typedef std::array<BoardEntry, BoardSize> BoardRow;
typedef std::array<BoardRow, BoardSize> Board;

enum MoveResult {
	Miss,
	Hit,
	Kill,
};

enum PlayerNumber {
	FirstPlayer = 0,
	SecondPlayer = 1,
};

enum GameOverReason {
	GameCompleted,
	IllegalMove,
	TimeLimitExceeded,
	BothLost,
	InvalidSetup,
};

struct GameResult {
	PlayerNumber winner;
	GameOverReason reason;

	GameResult(PlayerNumber winner,
		GameOverReason reason)
		:winner(winner), reason(reason)
	{}
	GameResult() {}
};
