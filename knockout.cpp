#include "knockout.h"
#include "match.h"
#include <iostream>
#include <thread>
#include <mutex>

Knockout::Knockout(
	std::vector<PlayerStrategy*> players,
	OutputStrategy* output,
	bool use_threads
) : _players(players), _output(output), _use_threads(use_threads)
{}



void thread_func(
	std::vector<PlayerStrategy*>& players,
	std::vector<PlayerStrategy*>& next_round,
	OutputStrategy* output,
	size_t& next_to_play,
	int games,
	std::mutex& pool_lock,
	std::mutex& next_lock,
	std::mutex& print_lock
) {
	srand(time(0));
	size_t i;

	while (1) {
		pool_lock.lock();
		i = next_to_play;
		next_to_play += 2;
		pool_lock.unlock();

		if (i >= players.size() - 1)
			break;

		auto score = get_match_score(
			players[i], players[i + 1], output, games, &print_lock);

		next_lock.lock();
		if (score.first > score.second)
			next_round.push_back(players[i]);
		else
			next_round.push_back(players[i + 1]);
		next_lock.unlock();
	}
}

void Knockout::run(int games) {
	while (_players.size() > 1) {
		std::vector<PlayerStrategy*> next_round;
		std::random_shuffle(_players.begin(), _players.end());
		if (_use_threads) {
			std::vector<std::thread> threads(std::thread::hardware_concurrency());
			size_t next_to_play = 0;

			std::mutex pool_lock;
			std::mutex next_lock;
			std::mutex print_lock;

			for (std::thread& t : threads)
				t = std::thread(
					thread_func,
					std::ref(_players),
					std::ref(next_round),
					_output,
					std::ref(next_to_play),
					games,
					std::ref(pool_lock),
					std::ref(next_lock),
					std::ref(print_lock)
				);

			for (std::thread& t : threads)
				t.join();
		}
		else {
			for (int i = 0; i < _players.size() - 1; i += 2) {
				auto score = get_match_score(
					_players[i], _players[i + 1], _output, games);
				if (score.first > score.second)
					next_round.push_back(_players[i]);
				else
					next_round.push_back(_players[i + 1]);
			}
		}
		if (_players.size() % 2 == 1)
			next_round.push_back(_players.back());
		_players = next_round;
	}
}

void Knockout::print_result() {
	std::cout << "WINNER: " << _players[0]->get_name() << std::endl;
}

