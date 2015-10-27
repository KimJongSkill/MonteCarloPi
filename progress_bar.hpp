#pragma once

#include <string>
#include <atomic>
#include <thread>

class ProgressBar
{
	std::string String;
	std::atomic_int Percentage = 0;
	std::string Animation;
	std::string::size_type AnimationState = 0;
	
	void Draw();
	void Service();

public:
	ProgressBar(std::string Message, std::string Spinner);
	
	std::thread StartService();
	int Update(int Delta);
};
