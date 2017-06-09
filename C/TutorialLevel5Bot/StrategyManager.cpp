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
	setInitialBuildOrder();
}

void StrategyManager::onEnd(bool isWinner)
{	
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

			std::cout << "currentSupplyShortage : " << currentSupplyShortage << " onBuildingSupplyCount : " << onBuildingSupplyCount << std::endl;

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
					std::cout << "enqueue supply provider " << InformationManager::Instance().getBasicSupplyProviderUnitType().getName().c_str() << std::endl;
					BuildManager::Instance().buildQueue.queueAsHighestPriority(MetaType(InformationManager::Instance().getBasicSupplyProviderUnitType()), true);
				}
			}

		}
	}
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

