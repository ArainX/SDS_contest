# 중요한 사항

## 프로젝트 개요

* **2017 삼성SDS 알고리즘 경진대회를 위한 프로젝트**

* **[Wiki](https://github.com/SamsungSDS-Contest/2017Guide/wiki) : 경진대회 개발환경 설정 방법, 봇 개발 튜터리얼 및 가이드 제공**

* **[Code](https://github.com/SamsungSDS-Contest/2017Guide) : 경진대회 봇 프로그램 개발의 시작점으로 사용할 BasicBot 소스코드 제공**

## BasicBot 개발 취지 및 특이사항

* **경진대회 결과물의 수준을 상향화 하기 위해 스타크래프트 봇 프로그램의 기본 기능 (미네랄 채취, 건물 짓기, 빌드오더 실행 등) 을 BasicBot 으로 개발하여 제공함**

* **경진대회 참가자들은 BasicBot 을 자유롭게 수정하여 개발을 수행한 후 소스코드를 제출하면 됨**

  * **단, Main, MyBotModule, UXManager 는 제출하더라도 원활한 대회 진행을 위해 봇 컴파일에 반영하지않음**

* **먼저 [wiki](https://github.com/SamsungSDS-Contest/2017Guide/wiki) 에서 개발환경 설정 방법, 튜터리얼 및 가이드를 읽어본 후 개발하는 것을 권장함**

## BasicBot 설치방법

* **master 브랜치를 Clone or download**

## BasicBot Version

* **2017-05-29 : v0.9 beta Release (프리뷰 및 오픈 테스트를 위해 공개)**

* **2017-06-16 : v1.0 Release 예정 (실제 알고리즘 경진대회에서 사용)**

* **[버그를 발견하여 알려주시거나 이슈를 제기하여주시면 감사의 선물을 드리겠습니다](https://github.com/SamsungSDS-Contest/2017Guide/issues)**


## BasicBot API Documentation

* **BasicBot API Documentation 웹사이트 : [https://samsungsds-contest.github.io/2017Guide/](https://samsungsds-contest.github.io/2017Guide/)**

# 상세 설명

## Code 폴더 설명

|폴더명|설명|
|----|----|
|C|BasicBot 및 TutorialBot 소스코드 C++ 버전|
|JAVA|BasicBot 및 TutorialBot 소스코드 JAVA 버전|
|docs\\C|BasicBot API Documentation - C++ 버전|
|docs\\JAVA|BasicBot API Documentation - JAVA 버전|
|Doxygen|BasicBot API Documentation 웹사이트 생성 자동화를 위한 Doxygen 설정 파일|

## BasicBot 디펜던시

* **StarCraft : Brood War 1.16.1**

* **C++**

  * **[BWAPI 4.1.2](https://github.com/bwapi/bwapi/tree/4.1.2_VC140_compat)**

  * **[BWTA 2.2](https://bitbucket.org/auriarte/bwta2)**

* **JAVA**

  * **[BWMirror 2.5](https://github.com/vjurenka/BWMirror) : BWAPI 4.1.2 및 BWTA 2 가 포함되어 있음**


## 권장 개발 환경

* C++

  * [Visual Studio 2013 Express](https://www.microsoft.com/en-US/download/details.aspx?id=44914)

* JAVA

  * [JDK 32bit](http://www.oracle.com/technetwork/java/javase/downloads/jdk8-downloads-2133151.html) 

  * [Eclipse](https://eclipse.org/)


## BasicBot 개발 History

* 삼성SDS 알고리즘경진대회 준비 T/F에서 오픈소스 [uAlbertaBot](https://github.com/davechurchill/ualbertabot), [Atlantis](https://github.com/Ravaelles/Atlantis), [BWSAL](https://github.com/Fobbah/bwsal) 등을 참고하여 BasicBot (C++ 버전 및 JAVA 버전) 을 개발하여 배포함

## BasicBot API Documentation 생성 툴

* [Doxygen 1.8.8](http://www.doxygen.org/index.html) 을 사용하여 BasicBot API Documentation 웹사이트를 생성하였음
