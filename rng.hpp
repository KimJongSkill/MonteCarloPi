#pragma once

#include <cstdint>
#include <random>
#include <memory>
#include <utility>
#include <Baked Potatoes\Crypt.hpp>

namespace rng
{
	constexpr struct auto_select_t {} auto_select;
	constexpr struct force_software_t {} force_software;
	constexpr struct force_hardware_t {} force_hardware;
}

class BaseRNG
{
protected:
	typedef std::mt19937 prng_type;
	typedef std::mt19937::result_type result_type;

public:
	BaseRNG() = default;
	virtual ~BaseRNG() = default;

	virtual std::pair<std::uint_fast64_t, std::uint_fast64_t> GetPair() = 0;
	virtual std::unique_ptr<BaseRNG> NewLocalInstance() = 0;

	static std::unique_ptr<BaseRNG> New(rng::auto_select_t);
	static std::unique_ptr<BaseRNG> New(rng::force_software_t);
	static std::unique_ptr<BaseRNG> New(rng::force_hardware_t);
};

class HardwareRNG : public BaseRNG
{
	bpl::crypt::IvyRNG Engine;

public:
	HardwareRNG() = default;
	~HardwareRNG() override = default;

	std::pair<std::uint_fast64_t, std::uint_fast64_t> GetPair() override;
	std::unique_ptr<BaseRNG> NewLocalInstance() override;
};

class SoftwareRNG : public BaseRNG
{
	prng_type Engine;
	const std::uniform_int_distribution<result_type> Distribution;

public:
	explicit SoftwareRNG(result_type Seed);
	~SoftwareRNG() override = default;

	std::pair<std::uint_fast64_t, std::uint_fast64_t> GetPair() override;
	std::unique_ptr<BaseRNG> NewLocalInstance() override;
};
