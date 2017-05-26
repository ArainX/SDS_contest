#include "GameCommander.h"

using namespace MyBot;

GameCommander::GameCommander(){
}
GameCommander::~GameCommander(){
}

void GameCommander::onStart() 
{
	BWAPI::TilePosition startLocation = BWAPI::Broodwar->self()->getStartLocation();
	if (startLocation == BWAPI::TilePositions::None || startLocation == BWAPI::TilePositions::Unknown) {
		return;
	}

	StrategyManager::Instance().onStart();
}

void GameCommander::onEnd(bool isWinner)
{
	StrategyManager::Instance().onEnd(isWinner);
}

void GameCommander::onFrame()
{
	if (BWAPI::Broodwar->isPaused()) {
		return;
	}

	if (Config::Debug::LogToConsole) std::cout << "a";

	// 아군 베이스 위치. 적군 베이스 위치. 각 유닛들의 상태정보 등을 Map 자료구조에 저장/업데이트
	InformationManager::Instance().update();

	if (Config::Debug::LogToConsole) std::cout << "b";

	// 각 유닛의 위치를 자체 MapGrid 자료구조에 저장
	MapGrid::Instance().update();

	if (Config::Debug::LogToConsole) std::cout << "c";

	// economy and base managers
	// 일꾼 유닛에 대한 명령 (자원 채취, 이동 정도) 지시 및 정리
	WorkerManager::Instance().update();

	if (Config::Debug::LogToConsole) std::cout << "d";

	// 빌드오더큐를 관리하며, 빌드오더에 따라 실제 실행(유닛 훈련, 테크 업그레이드 등)을 지시한다.
	BuildManager::Instance().update();

	if (Config::Debug::LogToConsole) std::cout << "e";

	// 빌드오더 중 건물 빌드에 대해서는, 일꾼유닛 선정, 위치선정, 건설 실시, 중단된 건물 빌드 재개를 지시한다
	ConstructionManager::Instance().update();

	if (Config::Debug::LogToConsole) std::cout << "f";

	// 게임 초기 정찰 유닛 지정 및 정찰 유닛 컨트롤을 실행한다
	ScoutManager::Instance().update();

	if (Config::Debug::LogToConsole) std::cout << "g";

	// 전략적 판단 및 유닛 컨트롤
	StrategyManager::Instance().update();

	if (Config::Debug::LogToConsole) std::cout << "h";

	// 화면 출력 및 사용자 입력 처리
	UXManager::Instance().update();

	if (Config::Debug::LogToConsole) std::cout << "i";
}

void GameCommander::onUnitShow(BWAPI::Unit unit)			
{ 
	InformationManager::Instance().onUnitShow(unit); 

	// ResourceDepot 및 Worker 에 대한 처리
	WorkerManager::Instance().onUnitShow(unit);
}

void GameCommander::onUnitHide(BWAPI::Unit unit)			
{
	InformationManager::Instance().onUnitHide(unit); 
}

void GameCommander::onUnitCreate(BWAPI::Unit unit)		
{ 
	InformationManager::Instance().onUnitCreate(unit);
}

void GameCommander::onUnitComplete(BWAPI::Unit unit)
{
	InformationManager::Instance().onUnitComplete(unit);
}

void GameCommander::onUnitDestroy(BWAPI::Unit unit)		
{
	// ResourceDepot 및 Worker 에 대한 처리
	WorkerManager::Instance().onUnitDestroy(unit);

	InformationManager::Instance().onUnitDestroy(unit); 
}

void GameCommander::onUnitRenegade(BWAPI::Unit unit)
{
	// Vespene_Geyser (가스 광산) 에 누군가가 건설을 했을 경우
	//BWAPI::Broodwar->sendText("A %s [%p] has renegaded. It is now owned by %s", unit->getType().c_str(), unit, unit->getPlayer()->getName().c_str());

	InformationManager::Instance().onUnitRenegade(unit);
}

void GameCommander::onUnitMorph(BWAPI::Unit unit)
{ 
	InformationManager::Instance().onUnitMorph(unit);

	// Zerg 종족 Worker 의 Morph 에 대한 처리
	WorkerManager::Instance().onUnitMorph(unit);
}

void GameCommander::onUnitDiscover(BWAPI::Unit unit)
{
}

void GameCommander::onUnitEvade(BWAPI::Unit unit)
{
}

void GameCommander::onSendText(std::string text)
{
}

void GameCommander::onReceiveText(BWAPI::Player player, std::string text)
{
}

