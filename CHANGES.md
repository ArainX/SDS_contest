# Change Log

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
