#pragma once

#include <string>
#include <thread>
#include <condition_variable>
#include <mutex>

class ProgressBar
{
	std::string String;
	int Percentage = 0;
	std::string Animation;
	std::string::size_type AnimationState = 0;
	std::mutex Mutex;
	std::condition_variable Flag;
	
	void Draw(bool UpdateAnimation = true);
	void Service();

public:
	ProgressBar(std::string Message, std::string Spinner);
	
	std::thread StartService();
	int Get();
	void Update(int Delta);
};
