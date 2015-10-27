#pragma once

#include <cstdint>
#include <random>
#include <memory>
#include <Baked Potatoes\Crypt.hpp>

class BaseRNG
{
protected:
	typedef std::mt19937 prng_type;
	typedef std::mt19937::result_type result_type;

public:
	BaseRNG() = default;
	virtual ~BaseRNG() = default;

	virtual std::uint16_t GetInt() = 0;
	virtual std::unique_ptr<BaseRNG> NewLocalInstance() = 0;

	static std::unique_ptr<BaseRNG> New();
};

class HardwareRNG : public BaseRNG
{
	bpl::crypt::IvyRNG Engine;

public:
	HardwareRNG() = default;
	~HardwareRNG() override = default;

	std::uint16_t GetInt() override;
	std::unique_ptr<BaseRNG> NewLocalInstance() override;
};

class SoftwareRNG : public BaseRNG
{
	prng_type Engine;
	const std::uniform_int_distribution<result_type> Distribution;

public:
	explicit SoftwareRNG(result_type Seed);
	~SoftwareRNG() override = default;

	std::uint16_t GetInt() override;
	std::unique_ptr<BaseRNG> NewLocalInstance() override;
};
