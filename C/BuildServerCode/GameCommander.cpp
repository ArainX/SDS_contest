
// GameCommander 소스코드는 예시입니다

#include "GameCommander.h"

using namespace MyBot;

GameCommander::GameCommander(){
}

GameCommander::~GameCommander(){
}

void GameCommander::onStart() 
{
}

void GameCommander::onEnd(bool isWinner)
{
}

void GameCommander::onFrame()
{
}

void GameCommander::onUnitShow(BWAPI::Unit unit)			
{ 
}

void GameCommander::onUnitHide(BWAPI::Unit unit)			
{
}

void GameCommander::onUnitCreate(BWAPI::Unit unit)		
{ 
}

void GameCommander::onUnitComplete(BWAPI::Unit unit)
{
}

void GameCommander::onUnitDestroy(BWAPI::Unit unit)		
{
}

void GameCommander::onUnitRenegade(BWAPI::Unit unit)
{
}

void GameCommander::onUnitMorph(BWAPI::Unit unit)
{ 
}

void GameCommander::onUnitDiscover(BWAPI::Unit unit)
{
}

void GameCommander::onUnitEvade(BWAPI::Unit unit)
{
}

void GameCommander::onNukeDetect(BWAPI::Position target)
{
}

void GameCommander::onPlayerLeft(BWAPI::Player player)
{
}

void GameCommander::onSaveGame(std::string gameName)
{
}

void GameCommander::onSendText(std::string text)
{
}

void GameCommander::onReceiveText(BWAPI::Player player, std::string text)
{
}

