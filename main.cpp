#include "simulation.hpp"

#include <iostream>
#include <cstdint>
#include <string>
#include <thread>
#include <chrono>
#include <docopt.h>

const std::string Documentation =
R"(
Monte Carlo Pi

    Usage:
        mcpi <rounds> [--threads COUNT] 

    Options:
        -t, --threads  Run simulation with COUNT threads
)";

int main(int argc, char* argv[])
{
	auto Arguments = docopt::docopt(Documentation, { std::next(argv), std::next(argv, argc) }, true);
	
	const std::intmax_t Rounds = std::stoll(Arguments["<rounds>"].asString());
	const std::size_t Threads = Arguments["--threads"].asBool() ? std::stoull(Arguments["COUNT"].asString()) : std::thread::hardware_concurrency();
	Simulation Sim;

	auto Start = std::chrono::steady_clock::now();
	auto Result = Sim(Rounds, Threads);
	auto End = std::chrono::steady_clock::now();

	std::cout << "Completed " << Rounds << " rounds in " << std::chrono::duration_cast<std::chrono::milliseconds>(End - Start).count() 
		<< " ms\nHits: " << Result.first << "\nPi approximated as " << Result.second << '\n';
}
