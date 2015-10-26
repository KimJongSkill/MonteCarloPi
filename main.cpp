#include "simulation.hpp"

#include <iostream>
#include <cstdint>

int main(int argc, char* argv[])
{
	constexpr std::intmax_t Rounds = 1000000;
	Simulation Sim;

	std::cout << Sim(Rounds) << std::endl;
}