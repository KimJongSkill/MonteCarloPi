#include "progress_bar.hpp"

#include <iostream>
#include <chrono>
#include <iomanip>

ProgressBar::ProgressBar(std::string Message, std::string Spinner) : String(Message), Animation(Spinner) {}

void ProgressBar::Draw(bool UpdateAnimation)
{
	std::cout << String << " [";

	// Attempt to work with any value Percentage may hold
	int Percent = std::abs(Get());
	int Progress = Percent / 10 <= 10 ? Percent / 10 : 10;

	std::cout << std::setw(10) << std::string(Progress, '#');

	std::cout << "] ";

	if (UpdateAnimation)
	{
		std::cout << Animation[AnimationState];
		AnimationState = (AnimationState + 1) % Animation.length();
	}
	else
	{
		// Clear the previous animation frame
		std::cout.put(' ');
	}

	std::cout.put('\r');
}

void ProgressBar::Service()
{
	auto PreviousFill = std::cout.fill('-');
	std::cout << std::left;
	
	while (Get() != 100)
	{
		Draw();

		std::unique_lock<std::mutex> Lock(Mutex);
		Flag.wait_for(Lock, std::chrono::milliseconds(250));
	};

	Draw(false);
	std::cout.put('\n');

	std::cout.fill(PreviousFill);
	std::cout << std::right;
}

std::thread ProgressBar::StartService()
{
	return std::thread(&ProgressBar::Service, this);
}

int ProgressBar::Get()
{
	std::lock_guard<std::mutex> Lock(Mutex);
	return Percentage;
}

void ProgressBar::Update(int Delta)
{
	std::lock_guard<std::mutex> Lock(Mutex);
	Percentage += Delta;

	if (Percentage == 100)
		Flag.notify_all();
}

void ProgressBar::Set(int Value)
{
	std::lock_guard<std::mutex> Lock(Mutex);
	Percentage = Value;

	if (Percentage == 100)
		Flag.notify_all();
}
