#pragma once

#include <cstdint>
#include <random>
#include <utility>
#include <limits>
#include <Baked Potatoes\Crypt.hpp>

class Simulation
{
	typedef std::mt19937_64 prng_type;
	typedef std::mt19937_64::result_type seed_type;

	bpl::crypt::IvyRNG TrueEngine;
	prng_type MasterEngine;
	const std::uniform_int_distribution<seed_type> IntDistribution;
	const std::uniform_real_distribution<double> RealDistribution;

	class TrueEngineWrapper
	{
		bpl::crypt::IvyRNG& Engine;

	public:
		TrueEngineWrapper(bpl::crypt::IvyRNG Object);

		std::uint64_t operator()();
		static constexpr std::uint64_t min();
		static constexpr std::uint64_t max();
	} Wrapper;

	seed_type GetSeed();
	double GetReal(prng_type& Fallback);

	std::intmax_t Task(std::intmax_t Rounds, seed_type Seed);

public:
	Simulation();

	std::pair<std::intmax_t, double> operator()(std::intmax_t Rounds, std::size_t Threads);
};
