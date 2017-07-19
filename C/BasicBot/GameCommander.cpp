#include "GameCommander.h"

using namespace MyBot;

GameCommander::GameCommander(){
	isToFindError = false;

	isFlag1 = false;
	isFlag2 = false;
	isFlag3 = false;
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
	if (BWAPI::Broodwar->isPaused() 
		|| BWAPI::Broodwar->self() == nullptr || BWAPI::Broodwar->self()->isDefeated() || BWAPI::Broodwar->self()->leftGame()
		|| BWAPI::Broodwar->enemy() == nullptr || BWAPI::Broodwar->enemy()->isDefeated() || BWAPI::Broodwar->enemy()->leftGame()) {
		return;
	}

	if (isToFindError) std::cout << "(a";

	// 아군 베이스 위치. 적군 베이스 위치. 각 유닛들의 상태정보 등을 Map 자료구조에 저장/업데이트
	InformationManager::Instance().update();

	if (isToFindError) std::cout << "b";

	// 각 유닛의 위치를 자체 MapGrid 자료구조에 저장
	MapGrid::Instance().update();
		
	if (isToFindError) std::cout << "c";

	// economy and base managers
	// 일꾼 유닛에 대한 명령 (자원 채취, 이동 정도) 지시 및 정리
	WorkerManager::Instance().update();

	if (isToFindError) std::cout << "d";

	// 빌드오더큐를 관리하며, 빌드오더에 따라 실제 실행(유닛 훈련, 테크 업그레이드 등)을 지시한다.
	BuildManager::Instance().update();

	if (isToFindError) std::cout << "e";

	// 빌드오더 중 건물 빌드에 대해서는, 일꾼유닛 선정, 위치선정, 건설 실시, 중단된 건물 빌드 재개를 지시한다
	ConstructionManager::Instance().update();

	if (isToFindError) std::cout << "f";

	// 게임 초기 정찰 유닛 지정 및 정찰 유닛 컨트롤을 실행한다
	ScoutManager::Instance().update();

	if (isToFindError) std::cout << "g";

	// 전략적 판단 및 유닛 컨트롤
	StrategyManager::Instance().update();

	if (isToFindError) std::cout << "h)";
}

void GameCommander::onUnitShow(BWAPI::Unit unit)			
{ 
	InformationManager::Instance().onUnitShow(unit); 
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

	// ResourceDepot 및 Worker 에 대한 처리
	WorkerManager::Instance().onUnitComplete(unit);
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
	if (isFlag1 == false
		&& text == "l") 
	{
		for (auto & unit : BWAPI::Broodwar->self()->getUnits())
		{
			if (unit->getType() == BWAPI::UnitTypes::Zerg_Lair && unit->isCompleted() )
			{
				// No
				//unit->issueCommand(BWAPI::UnitCommand::research(unit, BWAPI::TechTypes::Burrowing));
				
				// No
				//unit->useTech(BWAPI::TechTypes::Burrowing);
				
				//BWAPI::Unitset unitset;
				//unitset.insert(unit);
				BWAPI::Broodwar->self()->issu
				break;
			}
		}

		//BuildManager::Instance().buildQueue.queueAsLowestPriority(MetaType(BWAPI::TechTypes::Burrowing), BuildOrderItem::SeedPositionStrategy::FirstExpansionLocation);
		isFlag1 = true;
	}

	if (isFlag2 == false
		&& text == "z")
	{
		for (auto & unit : BWAPI::Broodwar->self()->getUnits())
		{
			if (unit->getType() == BWAPI::UnitTypes::Zerg_Greater_Spire && unit->isCompleted())
			{
				unit->issueCommand(BWAPI::UnitCommand::upgrade(unit, BWAPI::UpgradeTypes::Zerg_Flyer_Attacks));

				break;
			}
		}


		//BuildManager::Instance().buildQueue.queueAsLowestPriority(MetaType(BWAPI::UpgradeTypes::Ventral_Sacs), BuildOrderItem::SeedPositionStrategy::FirstExpansionLocation);
		isFlag2 = true;
	}

	if (isFlag3 == false
		&& text == "e")
	{
		BWAPI::TilePosition closestGeyser = BWAPI::TilePositions::None;
		double minGeyserDistanceFromHome = std::numeric_limits<double>::max();

		for (BWTA::BaseLocation * baseLocation : InformationManager::Instance().getOccupiedBaseLocations(InformationManager::Instance().selfPlayer)) {
			for (auto & geyser : baseLocation->getGeysers()) {

				BWAPI::Position geyserPos = geyser->getInitialPosition();
				BWAPI::TilePosition geyserTilePos = geyser->getInitialTilePosition();

				// geyser 근처에 ResourceDepot 이 존재하면
				bool isResourceDepotCompleted = false;
				for (auto & unit : BWAPI::Broodwar->self()->getUnits())
				{
					if (unit->getType().isResourceDepot() && unit->isCompleted() && unit->getDistance(geyserPos) < 300)
					{
						isResourceDepotCompleted = true;
					}
				}

				if (isResourceDepotCompleted)
				{
					// geyser 위치에 Refinery 가 건설되어있지 않으면
					bool isAlreadyRefineryBuilt = false;
					for (auto & uot : BWAPI::Broodwar->getUnitsOnTile(geyserTilePos)) {
						if (uot->getType().isRefinery()) {
							isAlreadyRefineryBuilt = true;
							break;
						}
					}

					if (isAlreadyRefineryBuilt == false) {
						BuildManager::Instance().buildQueue.queueAsLowestPriority(BWAPI::UnitTypes::Zerg_Extractor, 
							geyserTilePos);
					}
				}
			}
		}

		isFlag3 = true;
	}

}

void GameCommander::onReceiveText(BWAPI::Player player, std::string text)
{	
}

