#include "simulation.hpp"
#include "progress_bar.hpp"

#include <iostream>
#include <cstdint>
#include <string>
#include <thread>
#include <chrono>
#include <iostream>
#include <map>
#include <type_traits>
#include <docopt.h>

const std::string Documentation =
R"(Monte Carlo Pi

    Usage:
        mcpi <rounds> [--software --threads=<n>]

    Options:
        -s, --software     Disable hardware acceleration for random number generation
        -t, --threads=<n>  Run simulation with n threads
)";

template <class T>
std::string FormatInteger(T Number)
{
	static_assert(std::is_integral<T>::value, "Type is not an integer");

	std::string String = std::to_string(Number);
	std::string Buffer;
	/*
	*	Reserve space to avoid reallocations
	*	Remember that for every 3 characters
	*	we are inserting a comma
	*/
	Buffer.reserve(String.length() + String.length() / 3);

	int i = String.length() - 3;
	while (i > 0)
	{
		Buffer.insert(0, String, i, 3);
		Buffer.insert(Buffer.begin(), ',');

		i -= 3;
	}
	Buffer.insert(0, String, 0, 3 - 1);

	return Buffer;
}

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

	auto TimeTaken = std::chrono::duration_cast<std::chrono::milliseconds>(End - Start).count();

	std::cout << "Completed " << FormatInteger(Rounds) << " rounds in " << TimeTaken  << " ms"
		" [" << FormatInteger((Rounds / TimeTaken) * 1000) << " rounds per second]\n"
		"Hits: " << FormatInteger(Result.first) << "\n"
		"Pi approximated as " << (Result.second) << '\n';
}
