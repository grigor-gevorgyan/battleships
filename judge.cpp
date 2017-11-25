#include "judge.h"
#include <map>


PlayerNumber next_player(PlayerNumber player)
{
	return PlayerNumber(1 - player);
}

bool inside(int i, int j) {
	return i >= 0 && i < BoardSize && j >= 0 && j < BoardSize;
}

bool Judge::is_alive(PlayerNumber number) {
	for (int i = 0; i < BoardSize; ++i) {
		for (int j = 0; j < BoardSize; ++j) {
			if (_boards[number][i][j] == BoardEntry::HealthyShip) {
				return true;
			}
		}
	}
	return false;
}

bool Judge::is_valid_move(Move move)
{
	return !(
		move.first >= BoardSize || move.first < 0 ||
		move.second >= BoardSize || move.second < 0
		);
}

void Judge::end_game(GameResult result)
{
	_players[0]->game_over(result);
	_players[1]->game_over(result);
}

bool Judge::is_valid_board(PlayerNumber number) {
	const Board& board = _boards[number];
	int ships[4] = { 0, 0, 0, 0 };
	for (int i = 0; i < BoardSize; ++i)
		for (int j = 0; j < BoardSize; ++j)
			if (board[i][j] == BoardEntry::HealthyShip) {
				if (inside(i - 1, j - 1) && board[i - 1][j - 1] == BoardEntry::HealthyShip)
					return false;
				if (inside(i - 1, j + 1) && board[i - 1][j + 1] == BoardEntry::HealthyShip)
					return false;
				if (inside(i + 1, j - 1) && board[i + 1][j - 1] == BoardEntry::HealthyShip)
					return false;
				if (inside(i + 1, j + 1) && board[i + 1][j + 1] == BoardEntry::HealthyShip)
					return false;
			}

	for (int i = 0; i < BoardSize - 1; ++i) {
		for (int j = 0; j < BoardSize - 1; ++j) {
			size_t count = 0;
			if (board[i][j] == BoardEntry::HealthyShip) count++;
			if (board[i + 1][j] == BoardEntry::HealthyShip) count++;
			if (board[i][j + 1] == BoardEntry::HealthyShip) count++;
			if (board[i + 1][j + 1] == BoardEntry::HealthyShip) count++;
			if (count > 2)
				return false;
		}
	}
	for (int i = 0; i < BoardSize; ++i) {
		for (int j = 0; j < BoardSize; ++j) {
			if (board[i][j] == BoardEntry::HealthyShip &&
				(!inside(i - 1, j) || board[i - 1][j] != BoardEntry::HealthyShip) &&
				(!inside(i, j - 1) || board[i][j - 1] != BoardEntry::HealthyShip)) {
				int count = 1;
				for (int t = 1; j + t < BoardSize
					&& board[i][j + t] == BoardEntry::HealthyShip; ++t)
					++count;
				for (int t = 1; i + t < BoardSize
					&& board[i + t][j] == BoardEntry::HealthyShip; ++t)
					++count;
				if (count > 4)
					return false;
				ships[count - 1]++;
			}
		}
	}
	for (int i = 0; i < 4; ++i)	{
		if (ships[i] != 4 - i)
			return false;
	}
	return true;
}

MoveResult Judge::make_move(Move move, PlayerNumber number) {
	BoardEntry& cell = _boards[next_player(number)][move.first][move.second];
	if (cell == BoardEntry::HealthyShip) {
		cell = BoardEntry::DamagedShip;
		int dx[] = { 1, 0, -1, 0 };
		int dy[] = { 0, 1, 0, -1 };
		for (int t = 0; t < 4; ++t) {
			int i = move.first, j = move.second;
			while (is_valid_move(Move(i, j)) &&
				_boards[next_player(number)][i][j] == BoardEntry::DamagedShip)
			{
				i += dx[t];
				j += dy[t];
			}
			if (is_valid_move(Move(i, j)) &&
				_boards[next_player(number)][i][j] == BoardEntry::HealthyShip)
			{
				return MoveResult::Hit;
			}
		}
		return MoveResult::Kill;
	}
	return MoveResult::Miss;
}

GameResult Judge::host_game(
	PlayerStrategy* player_0,
	PlayerStrategy* player_1,
	OutputStrategy* output)
{
	// TODO handle crashes
	_players[0] = player_0;
	_players[1] = player_1;

	GameResult game_result;

	try {
		_boards[0] = _players[0]->game_start(
			_players[1]->get_name(), PlayerNumber::FirstPlayer);
	}
	catch (...) {
		game_result.winner = PlayerNumber::SecondPlayer;
		return game_result;
	}

	try {
		_boards[1] = _players[1]->game_start(
			_players[0]->get_name(), PlayerNumber::SecondPlayer);
	}
	catch (...) {
		game_result.winner = PlayerNumber::FirstPlayer;
		return game_result;
	}

	bool valid[] = {
		is_valid_board(PlayerNumber::FirstPlayer),
		is_valid_board(PlayerNumber::SecondPlayer),
	};

	if (!valid[0] || !valid[1]) {
		if (!valid[0] && !valid[1])
			game_result.reason = GameOverReason::BothLost;
		else {
			game_result.reason = GameOverReason::InvalidSetup;
			game_result.winner = valid[0] ? FirstPlayer : SecondPlayer;
		}
		end_game(game_result);
		return game_result;
	}

	PlayerNumber current_player = FirstPlayer;
	int remaining_moves = 256;
	while (remaining_moves) {
		remaining_moves--;
		PlayerNumber other_player = next_player(current_player);

		Move move;
		try {
			move = _players[current_player]->make_move();
		}
		catch (...) {
			game_result = GameResult(
				other_player, GameOverReason::IllegalMove
			);
			return game_result;
		}

		if (output)
			output->draw(_boards[0], _boards[1], current_player, move);

		if (!is_valid_move(move)) {
			game_result = GameResult(
				other_player, GameOverReason::IllegalMove
			);
			break;
		}
		MoveResult move_result = make_move(move, current_player);
		try {
			_players[current_player]->handle_result(move_result);
			_players[other_player]->handle_move(move);
		}
		catch (...) {}

		if (move_result == MoveResult::Kill && !is_alive(other_player)) {
			game_result = GameResult(
				current_player, GameOverReason::GameCompleted
			);
			break;
		}
		if (move_result == MoveResult::Miss)
			current_player = other_player;
	}
	if (remaining_moves == 0) {
		game_result.reason = GameOverReason::BothLost;
	}
	end_game(game_result);
	return game_result;
}
