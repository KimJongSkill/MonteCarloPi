#include "progress_bar.hpp"

#include <iostream>
#include <chrono>
#include <iomanip>

ProgressBar::ProgressBar(std::string Message, std::string Spinner) : String(Message), Animation(Spinner) {}

void ProgressBar::Draw(bool UpdateAnimation)
{
	std::cout << String << " [";

	int Percent = Percentage.load(std::memory_order_acquire);
	int Progress = std::abs(Percentage) / 10 <= 10 ? std::abs(Percentage) / 10 : 10;

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
	
	do
	{
		Draw();
		std::this_thread::sleep_for(std::chrono::milliseconds(250));
	} while (Percentage != 100);

	Draw(false);
	std::cout.put('\n');

	std::cout.fill(PreviousFill);
	std::cout << std::right;
}

std::thread ProgressBar::StartService()
{
	auto Thread = std::thread(&ProgressBar::Service, this);

	return std::move(Thread);
}

int ProgressBar::Update(int Delta)
{
	return Percentage.fetch_add(Delta, std::memory_order_release);
}
