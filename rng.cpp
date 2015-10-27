#include "rng.hpp"

#include <chrono>
#include <limits>

std::uint16_t HardwareRNG::GetInt()
{
	std::uint16_t Value;
	Engine(Value);

	return Value;
}

std::unique_ptr<BaseRNG> HardwareRNG::NewLocalInstance()
{
	return std::make_unique<HardwareRNG>(*this);
}

std::unique_ptr<BaseRNG> BaseRNG::New()
{
	if (bpl::crypt::IvyRNG::SupportsRDRAND())
		return std::make_unique<HardwareRNG>();
	else
		return std::make_unique<SoftwareRNG>(static_cast<result_type>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
}

SoftwareRNG::SoftwareRNG(result_type Seed) : Engine(Seed), Distribution(std::numeric_limits<uint16_t>::min(), std::numeric_limits<uint16_t>::max()) {};

std::uint16_t SoftwareRNG::GetInt()
{
	return Distribution(Engine);
}

std::unique_ptr<BaseRNG> SoftwareRNG::NewLocalInstance()
{
	return std::make_unique<SoftwareRNG>(Engine());
}
