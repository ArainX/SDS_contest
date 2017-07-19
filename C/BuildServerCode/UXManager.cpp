#include "UXManager.h"

using namespace MyBot;

UXManager::UXManager()
{
}

UXManager & UXManager::Instance()
{
	static UXManager instance;
	return instance;
}


void UXManager::onStart()
{
}

void UXManager::update()
{
}

