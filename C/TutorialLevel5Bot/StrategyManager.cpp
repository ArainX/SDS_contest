#include "StrategyManager.h"

using namespace MyBot;

StrategyManager & StrategyManager::Instance()
{
	static StrategyManager instance;
	return instance;
}

StrategyManager::StrategyManager()
{
	isFullScaleAttackStarted = false;
	isInitialBuildOrderFinished = false;
}

void StrategyManager::onStart()
{
	// BasicBot 1.1 Patch Start ////////////////////////////////////////////////

	// 과거 게임 기록을 로딩합니다
	loadGameRecordList();

	// BasicBot 1.1 Patch End //////////////////////////////////////////////////

	setInitialBuildOrder();
}

void StrategyManager::onEnd(bool isWinner)
{
	// BasicBot 1.1 Patch Start ////////////////////////////////////////////////

	// 과거 게임 기록 + 이번 게임 기록을 저장합니다
	saveGameRecordList(isWinner);

	// BasicBot 1.1 Patch End //////////////////////////////////////////////////
}

void StrategyManager::update()
{
	if (BuildManager::Instance().buildQueue.isEmpty()) {
		isInitialBuildOrderFinished = true;
	}

	executeWorkerTraining();

	executeSupplyManagement();

	executeBasicCombatUnitTraining();

	executeCombat();

	// BasicBot 1.1 Patch Start ////////////////////////////////////////////////

	// 이번 게임의 로그를 남깁니다
	saveGameLog();

	// BasicBot 1.1 Patch End //////////////////////////////////////////////////
}

void StrategyManager::setInitialBuildOrder()
{

	if (BWAPI::Broodwar->self()->getRace() == BWAPI::Races::Protoss) {

		BuildManager::Instance().buildQueue.queueAsLowestPriority(InformationManager::Instance().getWorkerType());
		BuildManager::Instance().buildQueue.queueAsLowestPriority(InformationManager::Instance().getWorkerType());
		BuildManager::Instance().buildQueue.queueAsLowestPriority(InformationManager::Instance().getWorkerType());

		// SupplyUsed가 7 일때 파일런 빌드
		BuildManager::Instance().buildQueue.queueAsLowestPriority(InformationManager::Instance().getBasicSupplyProviderUnitType(), BuildOrderItem::SeedPositionStrategy::MainBaseLocation);

		BuildManager::Instance().buildQueue.queueAsLowestPriority(InformationManager::Instance().getWorkerType());

		// SupplyUsed가 8 일때 1번째 게이트웨이 빌드
		BuildManager::Instance().buildQueue.queueAsLowestPriority(BWAPI::UnitTypes::Protoss_Gateway, BuildOrderItem::SeedPositionStrategy::MainBaseLocation);

		BuildManager::Instance().buildQueue.queueAsLowestPriority(InformationManager::Instance().getWorkerType());

		// SupplyUsed가 9 일때 가스 리파이너리 빌드
		BuildManager::Instance().buildQueue.queueAsLowestPriority(InformationManager::Instance().getRefineryBuildingType());

		BuildManager::Instance().buildQueue.queueAsLowestPriority(InformationManager::Instance().getWorkerType());

		// SupplyUsed가 10 일때 사이버네틱스 코어 빌드
		BuildManager::Instance().buildQueue.queueAsLowestPriority(BWAPI::UnitTypes::Protoss_Cybernetics_Core, BuildOrderItem::SeedPositionStrategy::MainBaseLocation);

		// 1번째 질럿 빌드
		BuildManager::Instance().buildQueue.queueAsLowestPriority(BWAPI::UnitTypes::Protoss_Zealot);

		// SupplyUsed가 12 일때 시타델 오브 아둔 빌드
		BuildManager::Instance().buildQueue.queueAsLowestPriority(BWAPI::UnitTypes::Protoss_Citadel_of_Adun);

		BuildManager::Instance().buildQueue.queueAsLowestPriority(InformationManager::Instance().getWorkerType());
		BuildManager::Instance().buildQueue.queueAsLowestPriority(InformationManager::Instance().getWorkerType());

		// SupplyUsed가 14 일때 템플러 아카이브, 2번째 게이트웨이 빌드
		BuildManager::Instance().buildQueue.queueAsLowestPriority(BWAPI::UnitTypes::Protoss_Templar_Archives);
		BuildManager::Instance().buildQueue.queueAsLowestPriority(BWAPI::UnitTypes::Protoss_Gateway, BuildOrderItem::SeedPositionStrategy::MainBaseLocation);

		// 2번째 질럿 빌드
		BuildManager::Instance().buildQueue.queueAsLowestPriority(BWAPI::UnitTypes::Protoss_Zealot);

		// SupplyUsed가 16 일때 파일런 빌드
		BuildManager::Instance().buildQueue.queueAsLowestPriority(InformationManager::Instance().getBasicSupplyProviderUnitType(), BuildOrderItem::SeedPositionStrategy::MainBaseLocation);

		BuildManager::Instance().buildQueue.queueAsLowestPriority(InformationManager::Instance().getWorkerType());

		// 4마리 다크 템플러 빌드 후 파일런 빌드
		BuildManager::Instance().buildQueue.queueAsLowestPriority(BWAPI::UnitTypes::Protoss_Dark_Templar);
		BuildManager::Instance().buildQueue.queueAsLowestPriority(BWAPI::UnitTypes::Protoss_Dark_Templar);
		BuildManager::Instance().buildQueue.queueAsLowestPriority(BWAPI::UnitTypes::Protoss_Dark_Templar);
		BuildManager::Instance().buildQueue.queueAsLowestPriority(BWAPI::UnitTypes::Protoss_Dark_Templar);
		BuildManager::Instance().buildQueue.queueAsLowestPriority(InformationManager::Instance().getBasicSupplyProviderUnitType(), BuildOrderItem::SeedPositionStrategy::MainBaseLocation);

	}
}

// 일꾼 계속 추가 생산
void StrategyManager::executeWorkerTraining()
{
	// InitialBuildOrder 진행중에는 아무것도 하지 않습니다
	if (isInitialBuildOrderFinished == false) {
		return;
	}

	if (BWAPI::Broodwar->self()->minerals() >= 50) {
		// workerCount = 현재 일꾼 수 + 생산중인 일꾼 수
		int workerCount = BWAPI::Broodwar->self()->allUnitCount(InformationManager::Instance().getWorkerType());

		if (BWAPI::Broodwar->self()->getRace() == BWAPI::Races::Zerg) {

			for (auto & unit : BWAPI::Broodwar->self()->getUnits())
			{
				if (unit->getType() == BWAPI::UnitTypes::Zerg_Egg) {
					// Zerg_Egg 에게 morph 명령을 내리면 isMorphing = true, isBeingConstructed = true, isConstructing = true 가 된다
					// Zerg_Egg 가 다른 유닛으로 바뀌면서 새로 만들어진 유닛은 잠시 isBeingConstructed = true, isConstructing = true 가 되었다가, 
					if (unit->isMorphing() && unit->getBuildType() == BWAPI::UnitTypes::Zerg_Drone) {
						workerCount++;
					}
				}
			}
		}
		else {
			for (auto & unit : BWAPI::Broodwar->self()->getUnits())
			{
				if (unit->getType().isResourceDepot())
				{
					if (unit->isTraining()) {
						workerCount += unit->getTrainingQueue().size();
					}
				}
			}
		}

		if (workerCount < 30) {
			for (auto & unit : BWAPI::Broodwar->self()->getUnits())
			{
				if (unit->getType().isResourceDepot())
				{
					if (unit->isTraining() == false || unit->getLarva().size() > 0) {

						// 빌드큐에 일꾼 생산이 1개는 있도록 한다
						if (BuildManager::Instance().buildQueue.getItemCount(InformationManager::Instance().getWorkerType()) == 0) {
							//std::cout << "worker enqueue" << std::endl;
							BuildManager::Instance().buildQueue.queueAsLowestPriority(MetaType(InformationManager::Instance().getWorkerType()), false);
						}
					}
				}
			}
		}
	}
}

// Supply DeadLock 예방 및 SupplyProvider 가 부족해질 상황 에 대한 선제적 대응으로서 SupplyProvider를 추가 건설/생산한다
void StrategyManager::executeSupplyManagement()
{
	// BasicBot 1.1 Patch Start ////////////////////////////////////////////////

	// 주석 추가
	// InitialBuildOrder 진행중 혹은 그후라도 서플라이 건물이 파괴되어 데드락이 발생할 수 있는데, 이 상황에 대한 해결은 참가자께서 해주셔야 합니다.
	// 오버로드가 학살당하거나, 서플라이 건물이 집중 파괴되는 상황에 대해  무조건적으로 서플라이 빌드 추가를 실행하기 보다 먼저 전략적 대책 판단이 필요할 것입니다

	// InitialBuildOrder 진행중에는 아무것도 하지 않습니다
	if (isInitialBuildOrderFinished == false) {
		return;
	}

	// 1초에 한번만 실행
	if (BWAPI::Broodwar->getFrameCount() % 24 != 0) {
		return;
	}

	// 게임에서는 서플라이 값이 200까지 있지만, BWAPI 에서는 서플라이 값이 400까지 있다
	// 저글링 1마리가 게임에서는 서플라이를 0.5 차지하지만, BWAPI 에서는 서플라이를 1 차지한다
	if (BWAPI::Broodwar->self()->supplyTotal() <= 400)
	{
		// 서플라이가 다 꽉찼을때 새 서플라이를 지으면 지연이 많이 일어나므로, supplyMargin (게임에서의 서플라이 마진 값의 2배)만큼 부족해지면 새 서플라이를 짓도록 한다
		// 이렇게 값을 정해놓으면, 게임 초반부에는 서플라이를 너무 일찍 짓고, 게임 후반부에는 서플라이를 너무 늦게 짓게 된다
		int supplyMargin = 12;

		// currentSupplyShortage 를 계산한다
		int currentSupplyShortage = BWAPI::Broodwar->self()->supplyUsed() + supplyMargin - BWAPI::Broodwar->self()->supplyTotal();

		if (currentSupplyShortage > 0) {

			// 생산/건설 중인 Supply를 센다
			int onBuildingSupplyCount = 0;

			// 저그 종족인 경우, 생산중인 Zerg_Overlord (Zerg_Egg) 를 센다. Hatchery 등 건물은 세지 않는다
			if (BWAPI::Broodwar->self()->getRace() == BWAPI::Races::Zerg) {
				for (auto & unit : BWAPI::Broodwar->self()->getUnits())
				{
					if (unit->getType() == BWAPI::UnitTypes::Zerg_Egg && unit->getBuildType() == BWAPI::UnitTypes::Zerg_Overlord) {
						onBuildingSupplyCount += BWAPI::UnitTypes::Zerg_Overlord.supplyProvided();
					}
					// 갓태어난 Overlord 는 아직 SupplyTotal 에 반영안되어서, 추가 카운트를 해줘야함 
					if (unit->getType() == BWAPI::UnitTypes::Zerg_Overlord && unit->isConstructing()) {
						onBuildingSupplyCount += BWAPI::UnitTypes::Zerg_Overlord.supplyProvided();
					}
				}
			}
			// 저그 종족이 아닌 경우, 건설중인 Protoss_Pylon, Terran_Supply_Depot 를 센다. Nexus, Command Center 등 건물은 세지 않는다
			else {
				onBuildingSupplyCount += ConstructionManager::Instance().getConstructionQueueItemCount(InformationManager::Instance().getBasicSupplyProviderUnitType()) * InformationManager::Instance().getBasicSupplyProviderUnitType().supplyProvided();
			}

			// 주석처리
			//std::cout << "currentSupplyShortage : " << currentSupplyShortage << " onBuildingSupplyCount : " << onBuildingSupplyCount << std::endl;

			if (currentSupplyShortage > onBuildingSupplyCount) {

				// BuildQueue 최상단에 SupplyProvider 가 있지 않으면 enqueue 한다
				bool isToEnqueue = true;
				if (!BuildManager::Instance().buildQueue.isEmpty()) {
					BuildOrderItem currentItem = BuildManager::Instance().buildQueue.getHighestPriorityItem();
					if (currentItem.metaType.isUnit() && currentItem.metaType.getUnitType() == InformationManager::Instance().getBasicSupplyProviderUnitType()) {
						isToEnqueue = false;
					}
				}
				if (isToEnqueue) {
					// 주석처리
					//std::cout << "enqueue supply provider " << InformationManager::Instance().getBasicSupplyProviderUnitType().getName().c_str() << std::endl;
					BuildManager::Instance().buildQueue.queueAsHighestPriority(MetaType(InformationManager::Instance().getBasicSupplyProviderUnitType()), true);
				}
			}

		}
	}
	// BasicBot 1.1 Patch End ////////////////////////////////////////////////
}

void StrategyManager::executeBasicCombatUnitTraining()
{
	// InitialBuildOrder 진행중에는 아무것도 하지 않습니다
	if (isInitialBuildOrderFinished == false) {
		return;
	}

	// 기본 병력 추가 훈련
	if (BWAPI::Broodwar->self()->minerals() >= 200 && BWAPI::Broodwar->self()->supplyUsed() < 390) {
		{
			for (auto & unit : BWAPI::Broodwar->self()->getUnits())
			{
				if (unit->getType() == InformationManager::Instance().getBasicCombatBuildingType()) {
					if (unit->isTraining() == false || unit->getLarva().size() > 0) {
						if (BuildManager::Instance().buildQueue.getItemCount(InformationManager::Instance().getBasicCombatUnitType()) == 0) {
							BuildManager::Instance().buildQueue.queueAsLowestPriority(InformationManager::Instance().getBasicCombatUnitType());
						}
					}
				}
			}
		}
	}
}


void StrategyManager::executeCombat()
{
	// 공격 모드가 아닐 때에는 전투유닛들을 아군 진영 길목에 집결시켜서 방어
	if (isFullScaleAttackStarted == false)
	{
		BWTA::Chokepoint* firstChokePoint = BWTA::getNearestChokepoint(InformationManager::Instance().getMainBaseLocation(InformationManager::Instance().selfPlayer)->getTilePosition());

		for (auto & unit : BWAPI::Broodwar->self()->getUnits())
		{
			if (unit->getType() == InformationManager::Instance().getBasicCombatUnitType() && unit->isIdle()) {
				CommandUtil::attackMove(unit, firstChokePoint->getCenter());
			}
		}

		// Protoss_Dark_Templar 유닛이 2개 이상 생산되었고, 적군 위치가 파악되었으면 총공격 모드로 전환
		if (BWAPI::Broodwar->self()->completedUnitCount(BWAPI::UnitTypes::Protoss_Dark_Templar) >= 2) {

			if (InformationManager::Instance().enemyPlayer != nullptr
				&& InformationManager::Instance().enemyRace != BWAPI::Races::Unknown
				&& InformationManager::Instance().getOccupiedBaseLocations(InformationManager::Instance().enemyPlayer).size() > 0)
			{
				isFullScaleAttackStarted = true;
			}
		}

	}
	// 공격 모드가 되면, 모든 전투유닛들을 적군 Main BaseLocation 로 공격 가도록 합니다
	else {
		//std::cout << "enemy OccupiedBaseLocations : " << InformationManager::Instance().getOccupiedBaseLocations(InformationManager::Instance().enemyPlayer).size() << std::endl;

		if (InformationManager::Instance().enemyPlayer != nullptr
			&& InformationManager::Instance().enemyRace != BWAPI::Races::Unknown
			&& InformationManager::Instance().getOccupiedBaseLocations(InformationManager::Instance().enemyPlayer).size() > 0)
		{
			// 공격 대상 지역 결정
			BWTA::BaseLocation * targetBaseLocation = nullptr;
			double closestDistance = 100000000;

			for (BWTA::BaseLocation * baseLocation : InformationManager::Instance().getOccupiedBaseLocations(InformationManager::Instance().enemyPlayer)) {

				double distance = BWTA::getGroundDistance(
					InformationManager::Instance().getMainBaseLocation(InformationManager::Instance().selfPlayer)->getTilePosition(),
					baseLocation->getTilePosition());

				if (distance < closestDistance) {
					closestDistance = distance;
					targetBaseLocation = baseLocation;
				}
			}

			if (targetBaseLocation != nullptr) {

				for (auto & unit : BWAPI::Broodwar->self()->getUnits())
				{
					// 건물은 제외
					if (unit->getType().isBuilding()) continue;
					// 모든 일꾼은 제외
					if (unit->getType().isWorker()) continue;

					// canAttack 유닛은 attackMove Command 로 공격을 보냅니다
					if (unit->canAttack()) {

						if (unit->isIdle()) {
							CommandUtil::attackMove(unit, targetBaseLocation->getPosition());
						}
					}
				}
			}
		}
	}
}

void StrategyManager::loadGameRecordList()
{
	// 과거의 게임에서 bwapi-data\write 폴더에 기록했던 파일은 대회 서버가 bwapi-data\read 폴더로 옮겨놓습니다
	// 따라서, 파일 로딩은 bwapi-data\read 폴더로부터 하시면 됩니다
	std::string gameRecordFileName = "bwapi-data\\read\\BasicBot_GameRecord.dat";

	FILE *file;
	errno_t err;
	if ((err = fopen_s(&file, gameRecordFileName.c_str(), "r")) != 0)
	{
		std::cout << "loadGameRecord failed. Could not open file :" << gameRecordFileName.c_str() << std::endl;
	}
	else
	{
		std::cout << "loadGameRecord from file: " << gameRecordFileName.c_str() << std::endl;
		char line[4096];
		while (fgets(line, sizeof line, file) != nullptr)
		{
			std::stringstream ss(line);

			GameRecord tempGameRecord;
			ss >> tempGameRecord.mapName;
			ss >> tempGameRecord.myName;
			ss >> tempGameRecord.myRace;
			ss >> tempGameRecord.myWinCount;
			ss >> tempGameRecord.myLoseCount;
			ss >> tempGameRecord.enemyName;
			ss >> tempGameRecord.enemyRace;
			ss >> tempGameRecord.enemyRealRace;
			ss >> tempGameRecord.gameFrameCount;

			gameRecordList.push_back(tempGameRecord);
		}
		fclose(file);
	}
}

void StrategyManager::saveGameRecordList(bool isWinner)
{
	// 이번 게임의 파일 저장은 bwapi-data\write 폴더에 하시면 됩니다.
	// bwapi-data\write 폴더에 저장된 파일은 대회 서버가 다음 경기 때 bwapi-data\read 폴더로 옮겨놓습니다
	std::string gameRecordFileName = "bwapi-data\\write\\BasicBot_GameRecord.dat";

	std::cout << "saveGameRecord to file: " << gameRecordFileName.c_str() << std::endl;

	std::string mapName = BWAPI::Broodwar->mapFileName();
	std::replace(mapName.begin(), mapName.end(), ' ', '_');
	std::string enemyName = BWAPI::Broodwar->enemy()->getName();
	std::replace(enemyName.begin(), enemyName.end(), ' ', '_');
	std::string myName = BWAPI::Broodwar->self()->getName();
	std::replace(myName.begin(), myName.end(), ' ', '_');

	/// 이번 게임에 대한 기록
	GameRecord thisGameRecord;
	thisGameRecord.mapName = mapName;
	thisGameRecord.myName = myName;
	thisGameRecord.myRace = BWAPI::Broodwar->self()->getRace().c_str();
	thisGameRecord.enemyName = enemyName;
	thisGameRecord.enemyRace = BWAPI::Broodwar->enemy()->getRace().c_str();
	thisGameRecord.enemyRealRace = InformationManager::Instance().enemyRace.c_str();
	thisGameRecord.gameFrameCount = BWAPI::Broodwar->getFrameCount();
	if (isWinner) {
		thisGameRecord.myWinCount = 1;
		thisGameRecord.myLoseCount = 0;
	}
	else {
		thisGameRecord.myWinCount = 0;
		thisGameRecord.myLoseCount = 1;
	}
	// 이번 게임 기록을 전체 게임 기록에 추가
	gameRecordList.push_back(thisGameRecord);

	// 전체 게임 기록 write
	std::stringstream ss;
	for (GameRecord gameRecord : gameRecordList) {
		ss << gameRecord.mapName << " "
			<< gameRecord.myName << " "
			<< gameRecord.myRace << " "
			<< gameRecord.myWinCount << " "
			<< gameRecord.myLoseCount << " "
			<< gameRecord.enemyName << " "
			<< gameRecord.enemyRace << " "
			<< gameRecord.enemyRealRace << " "
			<< gameRecord.gameFrameCount << "\n";

	}
	Logger::overwriteToFile(gameRecordFileName, ss.str());
}

void StrategyManager::saveGameLog()
{
	// 100 프레임 (5초) 마다 1번씩 로그를 기록합니다
	// 참가팀 당 용량 제한이 있고, 타임아웃도 있기 때문에 자주 하지 않는 것이 좋습니다
	// 로그는 봇 개발 시 디버깅 용도로 사용하시는 것이 좋습니다
	if (BWAPI::Broodwar->getFrameCount() % 100 != 0) {
		return;
	}

	std::string gameLogFileName = "bwapi-data\\write\\BasicBot_LastGameLog.dat";

	std::string mapName = BWAPI::Broodwar->mapFileName();
	std::replace(mapName.begin(), mapName.end(), ' ', '_');
	std::string enemyName = BWAPI::Broodwar->enemy()->getName();
	std::replace(enemyName.begin(), enemyName.end(), ' ', '_');
	std::string myName = BWAPI::Broodwar->self()->getName();
	std::replace(myName.begin(), myName.end(), ' ', '_');

	std::stringstream ss;
	ss << mapName << " "
		<< myName << " "
		<< BWAPI::Broodwar->self()->getRace().c_str() << " "
		<< enemyName << " "
		<< InformationManager::Instance().enemyRace.c_str() << " "
		<< BWAPI::Broodwar->getFrameCount() << " "
		<< BWAPI::Broodwar->self()->supplyUsed() << " "
		<< BWAPI::Broodwar->self()->supplyTotal() << " "
		<< "\n";

	Logger::appendTextToFile(gameLogFileName, ss.str());
}

