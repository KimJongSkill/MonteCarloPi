#include "rng.hpp"

#include <chrono>
#include <limits>

std::pair<std::uint_fast64_t, std::uint_fast64_t> HardwareRNG::GetPair()
{
	union
	{
		std::uint16_t Short[2];
		std::uint32_t Long;
	} Values;
	Engine(Values.Long);

	return { Values.Short[0], Values.Short[1] };
}

std::unique_ptr<BaseRNG> HardwareRNG::NewLocalInstance()
{
	return std::make_unique<HardwareRNG>(*this);
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
