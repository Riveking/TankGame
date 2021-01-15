#pragma once
#include "TinyEngine\\T_Engine.h"
#include "TinyEngine\\T_Map.h"
#include "TinyEngine\\T_Audio.h"
#include "TinyEngine\\T_Sprite.h" 
#include "TinyEngine\\T_Scene.h"
#include "JRobot.h"

class CJumpingRobot : public T_Engine
{
private:
	static int FRAME_UP[20];

	int scn_width, scn_height;						// 地图场景宽高
	int wnd_width, wnd_height;						// 游戏窗口宽高

	JRobot* player;									// 游戏玩家
	T_Scene* t_scene;								// 游戏地图场景
	T_Graph back;

public:
	CJumpingRobot(HINSTANCE h_instance, LPCTSTR sz_winclass, LPCTSTR sz_title,
		                        WORD icon = NULL, WORD sm_icon = NULL,
		                        int winwidth = 1024, int winheight = 768);
	virtual ~CJumpingRobot();

	void LoadPlayer();								// 加载游戏玩家角色
	void LoadMap();									// 加载游戏地图场景	

	void UpdatePlayerPos();							// 更新玩家角色的位置
	void UpdateAnimation();							// 更新角色动画帧序列号
	//………………………………………………………………………………………………………
	// 重载T_Engine类中的虚函数实现游戏功能
	//………………………………………………………………………………………………………
	void GameInit();								// 游戏初始化	
	void GameLogic();								// 游戏逻辑处理
	void GameEnd();									// 游戏结束处理
	void GamePaint(HDC hdc);						// 游戏显示
	void GameKeyAction(int Action = KEY_SYS_NONE);	// 按键行为处理	
	void GameMouseAction(int x, int y, int Action); // 鼠标行为处理	
};

