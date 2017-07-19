#pragma once

#include <cmath>
#include <math.h>
#include <cstdio>
#include <cstdlib>

#include <stdarg.h>
#include <stdexcept>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <deque>
#include <list>
#include <set>
#include <map>
#include <array>
#include <ctime>
#include <iomanip>

#include <winsock2.h>
#include <windows.h>

#include <BWAPI.h>
#include <BWAPI/Client.h>
#include <BWTA.h>

#include <thread>
#include <chrono>

namespace MyBot
{
	/// 봇 프로그램 개발의 편의성 향상을 위해 게임 화면에 추가 정보들을 표시하는 class<br>
	/// 여러 Manager 들로부터 정보를 조회하여 Screen 혹은 Map 에 정보를 표시합니다
	class UXManager
	{
		UXManager();
		
	public:
		/// static singleton 객체를 리턴합니다
		static UXManager &	Instance();

		/// 경기가 시작될 때 일회적으로 추가 정보를 출력합니다
		void onStart();

		/// 경기 진행 중 매 프레임마다 추가 정보를 출력하고 사용자 입력을 처리합니다
		void update();
	};
}
