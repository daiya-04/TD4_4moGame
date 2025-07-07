#include"IBossBehavior.h"
#include<memory>
#include <random>

DaiEngine::Camera* IBossBehavior::camera_ = nullptr;

int GetRandomInt(int min, int max)
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dist(min, max);
	return dist(gen);
}
