import java.util.ArrayList;
import java.util.List;

import bwapi.Position;

/// 봇 프로그램 개발의 편의성 향상을 위해 게임 화면에 추가 정보들을 표시하는 class<br>
/// 여러 Manager 들로부터 정보를 조회하여 Screen 혹은 Map 에 정보를 표시합니다
public class UXManager {
	
	private static UXManager instance = new UXManager();
	
	/// static singleton 객체를 리턴합니다
	public static UXManager Instance() {
		return instance;
	}
	
	/// 경기가 시작될 때 일회적으로 추가 정보를 출력합니다
	public void onStart() {
	}

	/// 경기 진행 중 매 프레임마다 추가 정보를 출력하고 사용자 입력을 처리합니다
	public void update() {
	}
}