#pragma once

#include "tournament.h"
#include "judge.h"
#include <vector>
#include <mutex>

class RoundRobin : public Tournament {
public:
	RoundRobin(std::vector<PlayerStrategy*> players, OutputStrategy* output, bool use_threads = false);

	virtual void run(int games);
	virtual void print_result();
private:
	std::vector<PlayerStrategy*> _players;
	OutputStrategy* _output;
	bool _use_threads;
	std::vector<std::vector<int>> _results;

private:
	std::mutex _pool_lock;
	std::mutex _print_lock;
	std::vector<std::vector<char>> _played;
	std::vector<char> _busy;
	void thread_func(int games);
};