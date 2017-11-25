#include "console_output_strategy.h"
#include "empty_output_strategy.h"
#include "judge.h"
#include "random_strategy.h"
#include "sequential.h"
#include "match.h"
#include "knockout.h"
#include "round.h"

#include <ctime>
#include <cmath>

int main () {
	srand(time(0));
	std::vector<PlayerStrategy*> players;
	int n;
	std::cin >> n;
	players.resize(n);
	for (int i = 0; i < n; ++i)
		players[i] = new RandomStrategy(i);

	int games;
	std::cin >> games;
	EmptyOutputStrategy output;
	RoundRobin ko(players, &output, true);
	ko.run(games);
	ko.print_result();
}
