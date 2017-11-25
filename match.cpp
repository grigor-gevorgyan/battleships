#include "match.h"
#include "console_output_strategy.h"

Match::Match(PlayerStrategy* player_1,
	PlayerStrategy* player_2,
	OutputStrategy* output)
	: _player_1(player_1), _player_2(player_2), _output(output),
	_judge() {}

std::pair<int, int> get_match_score(
	PlayerStrategy* player_1,
	PlayerStrategy* player_2,
	OutputStrategy* output,
	int games,
	std::mutex* print_lock
) {
	Match match(player_1, player_2, output);
	match.run(games);

	if (print_lock)
		print_lock->lock();
	match.print_result();
	if (print_lock)
		print_lock->unlock();

	return match.get_score();
}

void Match::run(int games)
{
	while(games--)
	{
		if(games & 1)
			_results.push_back(_judge.host_game(_player_1, _player_2, _output));
		else
		{
			_results.push_back(_judge.host_game(_player_2, _player_1, _output));
			_results.back().winner = next_player(_results.back().winner);
		}
	}
}

void Match::print_result()
{
	int p1_wins = 0;
	int p2_wins = 0;
	for(int i = 0; i < _results.size(); ++i)
	{
		std::cout << "Game " << i + 1 << ": ";
		if(_results[i].reason != GameOverReason::BothLost)
		{
			if(_results[i].winner == PlayerNumber::FirstPlayer)
			{
				++p1_wins;
				std::cout << _player_1->get_name();
			}
			else
			{
				++p2_wins;
				std::cout << _player_2->get_name();				
			}
		}
		else
		{
			std::cout << "Nobody";
		}

		std::cout << " wins" << std::endl;
	}

	std::cout << _player_1->get_name() << ": " << p1_wins 
		<< " | " << _player_2->get_name() << ": " << p2_wins
		<< std::endl;
	_score = std::make_pair(p1_wins, p2_wins);
}

std::pair<int, int> Match::get_score() {
	return _score;
}