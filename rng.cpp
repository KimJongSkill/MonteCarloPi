#include "rng.hpp"

#include <chrono>
#include <limits>

HardwareRNG::HardwareRNG()
{
	Engine(Buffer.begin(), Buffer.end());
}

std::pair<std::uint_fast64_t, std::uint_fast64_t> HardwareRNG::GetPair()
{
	std::pair<std::uint_fast64_t, std::uint_fast64_t> Pair = { Buffer[Index], Buffer[Index + 1] };
	Index += 2;

	if (Index >= Buffer.size())
	{
		Engine(Buffer.begin(), Buffer.end());
		Index = 0;
	}

	return Pair;
}

std::unique_ptr<BaseRNG> HardwareRNG::NewLocalInstance()
{
	return std::make_unique<HardwareRNG>();
}

std::unique_ptr<BaseRNG> BaseRNG::New(rng::auto_select_t)
{
	if (bpl::crypt::IvyRNG::SupportsRDRAND())
		return New(rng::force_hardware);
	else
		return New(rng::force_software);
}

std::unique_ptr<BaseRNG> BaseRNG::New(rng::force_software_t)
{
	return std::make_unique<SoftwareRNG>(static_cast<result_type>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
}

std::unique_ptr<BaseRNG> BaseRNG::New(rng::force_hardware_t)
{
	return std::make_unique<HardwareRNG>();
}

SoftwareRNG::SoftwareRNG(result_type Seed) : Engine(Seed), Distribution(std::numeric_limits<uint16_t>::min(), std::numeric_limits<uint16_t>::max()) {};

std::pair<std::uint_fast64_t, std::uint_fast64_t> SoftwareRNG::GetPair()
{
	return { Distribution(Engine), Distribution(Engine) };
}

std::unique_ptr<BaseRNG> SoftwareRNG::NewLocalInstance()
{
	return std::make_unique<SoftwareRNG>(Engine());
}
