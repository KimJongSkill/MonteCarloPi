#include "simulation.hpp"

#include <iostream>
#include <cstdint>
#include <string>
#include <docopt.h>

const std::string Documentation =
R"(
Monte Carlo Pi

    Usage:
        mcpi <rounds>
)";

int main(int argc, char* argv[])
{
	auto Arguments = docopt::docopt(Documentation, { std::next(argv), std::next(argv, argc) }, true);
	
	const std::intmax_t Rounds = std::stoll(Arguments["<rounds>"].asString());
	Simulation Sim;
	auto Result = Sim(Rounds);
	
	std::cout << "Completed " << Rounds << " rounds\nHits: " << Result.first << "\nPi approximated as " << Result.second << '\n';
}
