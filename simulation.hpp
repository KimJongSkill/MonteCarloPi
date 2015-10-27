#pragma once

#include <cstdint>
#include <random>
#include <utility>
#include <limits>
#include <Baked Potatoes\Crypt.hpp>

class Simulation
{
	typedef std::mt19937 prng_type;
	typedef std::mt19937::result_type seed_type;

	bpl::crypt::IvyRNG TrueEngine;
	prng_type MasterEngine;
	const std::uniform_int_distribution<seed_type> IntDistribution;

	seed_type GetSeed();
	std::uint16_t GetInt(prng_type& Fallback);

	std::intmax_t Task(std::intmax_t Rounds, seed_type Seed);

public:
	Simulation();

	std::pair<std::intmax_t, double> operator()(std::intmax_t Rounds, std::size_t Threads);
};
