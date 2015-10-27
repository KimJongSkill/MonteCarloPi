#include "progress_bar.hpp"

#include <iostream>
#include <chrono>
#include <iomanip>

ProgressBar::ProgressBar(std::string Message, std::string Spinner) : String(Message), Animation(Spinner) {}

void ProgressBar::Draw()
{
	std::cout << String << " [";

	int Percent = Percentage.load(std::memory_order_acquire);
	int Progress = std::abs(Percentage) / 10 <= 10 ? std::abs(Percentage) / 10 : 10;

	std::cout << std::setfill('-') << std::setw(10) << std::left;
	std::cout << std::string(Progress, '#');

	std::cout << "] ";

	std::cout << Animation[AnimationState];
	AnimationState = (AnimationState + 1) % Animation.length();

	std::cout.put('\r');
}

void ProgressBar::Service()
{
	do
	{
		Draw();
		std::this_thread::sleep_for(std::chrono::milliseconds(250));
	} while (Percentage != 100);

	Draw();
	std::cout.put('\n');
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
