# Change Log

## v1.1 Release (2017-07-19)

* fix major bug : 가스 채취 건물의 경우 BuildManager 및 ConstructionManager 에서 데드락 여부를 체크하느라 많은 시간을 소요해서 타임아웃을 발생시키는 문제에 대해 속도 개선

  * BuildManager

  * ConstructionManager

  * 참고로, BuildManager 및 ConstructionManager 에 현재 구현되어있는 데드락 체크 및 해결 로직이 모든 케이스를 커버하지는 않기 때문에 참가자가 스스로 데드락을 유발하지 않도록 빌드큐에 입력을 하거나, 혹은 직접 데드락 체크 로직을 추가해야함

    * 예를들어, 건물이 아닌 유닛의 생산 도중 서플라이가 파괴된 경우, StrategyManager 등에서 이를 체크하여 서플라이를 최우선적으로 빌드큐에 추가하는 로직을 구현해야 함

    * 프로토스 종족의 경우 파일런이 파괴되거나, 저그 종족의 경우 크립이 없어져서 건설 예정이던 건물이 건설 불가능하게 되는 경우 StrategyManager 등에서 이를 체크하여 파일런이나 크립 건물을 최우선적으로 빌드큐에 추가하는 로직을 구현해야 함

* fix major bug : 멀티 기지 확장 혹은 파괴에 따라 일꾼 유닛들의 리밸런싱이 자동으로 잘 작동하도록 수정

  * WorkerManager 의 onUnitShow 메소드를 onUnitComplete 메소드로 이름 변경 및 내용 수정

  * GameCommander 의 onUnitShow 메소드가 아니라 onUnitComplete 메소드에서 WorkerManager 의 onUnitComplete 메소드를 호출하도록 내용 수정

  * WorkerData 의 depotHasEnoughMineralWorkers 메소드에서 리밸런싱 조건을 일꾼유닛 수 = 미네랄수 * 3 에서 일꾼유닛 수 = 미네랄수 * 2 로 조건 완화

  * WorkerManager 의 closestDist 초기값을 0에서 1000000000 으로 수정

* fix major bug : 적군의 빠른 앞마당 건물 건설 및 아군의 느린 정찰 시 적군의 MainBaseLocation 을 잘못 판단하게 되는 버그 수정

  * InformationManager 의 updateBaseLocationInfo 메소드 내용 수정

* major update : MyBotModule 에 빌드서버에서 사용하는 TimeOut 체크 로직, 자동 패배 체크 로직을 추가

  * 현재 빌드서버에서 사용하는 TimeOut 체크 로직, 자동 패배 체크 로직을 참가자들이 평소 개발할 때부터 숙지하여 개발하도록 하는 것이 유용하겠다고 판단

  * 참고로, 이후에도 공정하고 효율적인 운영을 위해 실제 빌드서버에서는 Main, MyBotModule, UXManager 코드를 일부 수정해서 빌드하게 할 수 있음

* minor update : Config 의 ReadDirectory, WriteDirectory, LogFilename 기본값 수정

  * ReadDirectory, WriteDirectory 기본값을 "bwapi-data\read\", "bwapi-data\write\" 로 수정

  * LogFilename 기본값을 BotName + ".dat" 로 수정

* minor update : StrategyManager 의 onStart, onEnd, onFrame 에 게임 기록을 로딩하고 저장하는 메소드를 예시적으로 추가

  * StrategyManager 에 loadGameRecordList, saveGameRecordList, saveGameLog() 메소드 추가 및 GameRecord 자료구조 추가

  * 참가자들께서는 자유롭게 추가/수정하셔서 사용하시면 됩니다

* minor update : Common.java 의 함수들을 static 으로 수정

* minor update : BuildManager 의 getProducer 메소드 내용 수정

  * 저그 종족의 경우 Hatchery 에서 수행할 수 있는 것은 Lair 나 Hive 에서도 수행할 수 있고, Lair 에서 수행할 수 있는 것은 Hive 에서도 수행할 수 있고, Spire 에서 수행할 수 있는 것은 Greater Spire 에서도 수행할 수 있도록 로직 개선

* minor update : MyBotModule 및 GameCommander 에서 onNukeDetect, onSaveGame, onPlayerLeft 이벤트를 처리할 수 있도록 추가 / 변경

* minor update : BasicBot update 에 따라 TutorialLevel0 ~ TutorialLevel5 도 코드 업데이트 반영

## v1.0 Release (2017-06-16)

* fix minor bug : BuildManager.java 의 update, checkBuildOrderQueueDeadlockAndAndFixIt 함수 수정

  * 변경 사유 : 특정 unitID 혹은 seedLocation 를 지정해서 빌드오더를 입력하는 경우에 대해 잘 처리하도록 수정

* change Config default value : drawMapGrid 등 일부 값을 false 로 변경

* remove SeedPositionStrategy.SecondExpansionLocation : 참가자의 전략적 선택에 관한것이라서 제거

## v0.96 beta (2017-06-15)

* fix critical bug : CommandUtil.java 의 attackMove, move, rightClick 함수 버그 수정

* fix critical bug : CommandUtil.java 의 IsValidUnit 함수 버그 수정

## v0.95 beta (2017-06-09)

* change C++ encoding : UTF-8 without signature 에서 UTF-8 with signature 로 변경

  * 변경 사유 : Visual Studio 2013 에서 UTF-8 without signature 는 컴파일 에러 발생

* fix minor error : GameCommander.cpp 수정

  * MapTools::Instance().draw() 문장 잘못 삽입되어있던것 삭제

## v0.94 beta (2017-06-02)

* change C++/JAVA encoding : MS-949 에서 UTF-8 without signature 로 변경

  * 변경 사유 : GitHub 및 일반 텍스트에디터에서도 잘 보이도록 변경

## v0.93 beta (2017-05-31)

* fix miner bugs

## v0.92 beta (2017-05-31)

* README 수정

* docs 재생성

## v0.91 beta (2017-05-30)

* MIT License 라이센스 표기 및 저작자 표기

* fix critical error : UnitData.removeBadUnits() 등

* change code : 플레이어 이탈 시 작동 중지하도록 수정

## v0.90 beta (2017-05-29)

* 알고리즘 경진대회 BasicBot 소스코드 사전 공개 및 오픈 베타 테스트 시작
