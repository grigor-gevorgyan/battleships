#pragma once

#include "utils.h"
#include "player_strategy.h"

class Tournament {
public:
	virtual void run(int games) = 0;
	virtual void print_result() = 0;
	virtual ~Tournament() {}
};