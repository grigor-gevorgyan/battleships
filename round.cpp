#include "round.h"
#include "match.h"
#include <iostream>

RoundRobin::RoundRobin(std::vector<PlayerStrategy*> players,
	OutputStrategy* output,
	bool use_threads)
	: _players(players), _output(output), _use_threads(use_threads) 
{
	_results.resize(_players.size(), std::vector<int>(_players.size() + 2));
}

void RoundRobin::thread_func(int games)
{
	srand(time(0));
	while (1)
	{
		_pool_lock.lock();
		bool pair_found = false;
		bool game_exist = false;

		int i, j;
		for (i = 0; i < _results.size(); ++i)
		{
			for (j = i + 1; j < _results.size(); ++j)
			{
				if (!_played[i][j])
				{
					game_exist = true;
					if (!_busy[i] && !_busy[j])
					{
						pair_found = true;
						break;
					}
				}
			}
			if (pair_found)
				break;
		}
		if (!pair_found)
		{
			_pool_lock.unlock();
			if (!game_exist)
				return;
			else
				continue;
		}
		_busy[i] = 1;
		_busy[j] = 1;
		_played[i][j] = _played[j][i] = 1;
		_pool_lock.unlock();

		auto result = get_match_score(_players[i], _players[j],
			_output, games, &_print_lock);
		_results[i][j] = result.first;
		_results[j][i] = result.second;
		_results[i].back() += _results[i][j];
		_results[j].back() += _results[j][i];

		_busy[i] = 0;
		_busy[j] = 0;
	}
}


void RoundRobin::run(int games)
{
	if (_use_threads) {
		std::vector<std::thread> threads(std::thread::hardware_concurrency());
		_busy = std::vector<char>(_players.size(), 0);
		_played = std::vector<std::vector<char>>(
			_players.size(),
			std::vector<char>(_players.size(), 0)
		);

		std::mutex pool_lock;
		std::mutex print_lock;

		for (std::thread& t : threads)
			t = std::thread(
				&RoundRobin::thread_func,
				this,
				games
			);

		for (std::thread& t : threads)
			t.join();
		for (int i = 0; i < _players.size(); ++i)
			_results[i][_results[i].size() - 2] = i;
	}
	else {
		for (int i = 0; i < _players.size(); ++i)
		{
			for (int j = i + 1; j < _players.size(); ++j)
			{
				auto result = get_match_score(_players[i], _players[j],
					_output, games);
				_results[i][j] = result.first;
				_results[j][i] = result.second;
				_results[i].back() += _results[i][j];
				_results[j].back() += _results[j][i];
			}
			_results[i][_results[i].size() - 2] = i;
		}
	}
}

void RoundRobin::print_result()
{
	/*std::sort(_results.begin(), _results.end(), [](const std::vector<int>& v1,
		const std::vector<int>& v2)
	{
		return v1.back() > v2.back();
	});*/

	for (int i = 0; i < _results.size(); ++i)
	{
		for (int j = 0; j < _results.size(); ++j)
		{
			std::cout << _results[i][j] << " ";
		}
		std::cout << _results[i].back() << " " << 
			_players[_results[i][_results[i].size() - 2]]->get_name() 
			<< std::endl;
	}
}
