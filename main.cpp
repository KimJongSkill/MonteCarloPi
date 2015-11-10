#include "simulation.hpp"
#include "progress_bar.hpp"

#include <iostream>
#include <cstdint>
#include <string>
#include <thread>
#include <chrono>
#include <iostream>
#include <map>
#include <docopt.h>

const std::string Documentation =
R"(Monte Carlo Pi

    Usage:
        mcpi <rounds> [--software --threads=<n>]

    Options:
        -s, --software     Disable hardware acceleration for random number generation
        -t, --threads=<n>  Run simulation with n threads
)";

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << Documentation;

		return EXIT_FAILURE;
	}

	std::map<std::string, docopt::value> Arguments;
	std::intmax_t Rounds;
	std::size_t Threads;
	try
	{
		Arguments = docopt::docopt(Documentation, { std::next(argv), std::next(argv, argc) }, true);

		Rounds = std::stoll(Arguments["<rounds>"].asString());
		Threads = Arguments["--threads"].isString() ? std::stoull(Arguments["--threads"].asString()) : std::thread::hardware_concurrency();
	}
	catch (const std::exception& Exception)
	{
		std::cout << "[EXCEPTION] " << Exception.what() << '\n';
		std::cout << Documentation;
		
		return EXIT_FAILURE;
	}

	ProgressBar Bar("Calculating...", R"(|/-\)");
	auto ProgressThread = Bar.StartService();

	Simulation Sim(Bar, Arguments["--software"].asBool());

	auto Start = std::chrono::steady_clock::now();
	auto Result = Sim(Rounds, Threads);
	auto End = std::chrono::steady_clock::now();

	ProgressThread.join();

	std::cout << "Completed " << Rounds << " rounds in " << std::chrono::duration_cast<std::chrono::milliseconds>(End - Start).count() 
		<< " ms\nHits: " << Result.first << "\nPi approximated as " << Result.second << '\n';
}
