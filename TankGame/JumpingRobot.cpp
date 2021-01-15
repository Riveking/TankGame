#include "JumpingRobot.h"

//重新定义帧序列(必须为静态)
int CJumpingRobot::FRAME_UP[20] = { 9, 9, 10, 10, 10, 10, 11, 11, 11, 11, 9, 9, 10, 10, 10, 10, 11, 11, 11, 11 };

CJumpingRobot::CJumpingRobot(HINSTANCE h_instance, LPCTSTR sz_winclass, LPCTSTR sz_title,
WORD icon, WORD sm_icon, int winwidth, int winheight)
	:T_Engine(h_instance, sz_winclass, sz_title, icon, sm_icon, winwidth, winheight)
{
	wnd_width = winwidth;
	wnd_height = winheight;
	t_scene = NULL;
}


CJumpingRobot::~CJumpingRobot()
{
}

// 加载游戏地图场景
void CJumpingRobot::LoadMap()
{
	if (t_scene == NULL) t_scene = new T_Scene();
	t_scene->LoadTxtMap("JumpMap\\jump_map.txt");
	scn_width = t_scene->getSceneLayers()->back().layer->GetWidth();
	scn_height = t_scene->getSceneLayers()->back().layer->GetHeight();

	// 视图初始位置以地图作为参照
	int scn_x = 0;
	int scn_y = 0;
	// 将游戏地图初始化为左上角
	t_scene->InitScene(scn_x, scn_y, scn_width, scn_height, wnd_width, wnd_height);

	// 将所有地图图层定位在左上角
	SCENE_LAYERS::iterator p;
	for (p = t_scene->getSceneLayers()->begin(); p != t_scene->getSceneLayers()->end(); p++)
	{
		if (p->layer->ClassName() == "T_Map") p->layer->SetPosition(scn_x, scn_y);
	}
}

void CJumpingRobot::UpdatePlayerPos()
{
	if (player == NULL) return;
	if (player->IsDead() == false && player->IsVisible() == true && player->IsActive() == true)
	{
		// 处理玩家的跳跃和左右移动	
		if (player->getJumping())
		{
			player->SetSequence(FRAME_UP, 20);
			player->jumpUpDown(t_scene->getBarrier());
			t_scene->ScrollScene(player);//滚动背景
		}
		if (player->GetDir() == DIR_LEFT)
		{
			player->updatePostion(DIR_LEFT, 0, 0, t_scene->getBarrier());
			t_scene->ScrollScene(player);//滚动背景
		}
		if (player->GetDir() == DIR_RIGHT)
		{
			player->updatePostion(DIR_RIGHT, 0, 0, t_scene->getBarrier());
			t_scene->ScrollScene(player);//滚动背景
		}

	}
}

void CJumpingRobot::UpdateAnimation()
{
	if (player != NULL)
	{
		if (player->IsVisible() == true && player->IsActive() == true)
		{
			player->LoopFrame();
		}
	}
}

// 加载游戏玩家角色
void CJumpingRobot::LoadPlayer()
{
	GAMELAYER gameLayer;
	SPRITEINFO spInfo;

	// 加载玩家角色
	player = new JRobot(L".\\res\\blockorange.png", 80, 80);

	spInfo.Active = false;
	spInfo.Dead = false;
	spInfo.Dir = DIR_RIGHT;
	spInfo.Rotation = TRANS_NONE;
	spInfo.Ratio = 0.5f;
	spInfo.Level = 0;
	spInfo.Score = 0;
	spInfo.Speed = 3;
	spInfo.Alpha = 255;
	spInfo.Visible = true;
	player->SetSequence(FRAME_UP, 20);
	player->Initiate(spInfo);
	player->SetLayerTypeID(LAYER_PLY);

	int x = 10;
	int y = wnd_height - 64 - player->GetRatioSize().cy;
	player->SetPosition(x, y);

	gameLayer.layer = player;
	gameLayer.type_id = LAYER_PLY;
	gameLayer.z_order = t_scene->getSceneLayers()->size() + 1;
	gameLayer.layer->setZorder(gameLayer.z_order);
	t_scene->Append(gameLayer);
}

//………………………………………………………………………………………………………
// 重载T_Engine类中的虚函数实现游戏功能
//………………………………………………………………………………………………………
//游戏初始化接口函数
void CJumpingRobot::GameInit()
{
	GameState = GAME_RUN;
	LoadMap();
	LoadPlayer();
}

//游戏逻辑处理(处理GameState)
void CJumpingRobot::GameLogic()
{
	GameKeyAction();//处理快速游戏按键
	if (GameState == GAME_RUN)
	{
		UpdatePlayerPos();//移动玩家角色
		UpdateAnimation();//更新动画的
	}
}

//游戏显示(参数值为GameState)
void CJumpingRobot::GamePaint(HDC hdc)
{
	if (GameState == GAME_RUN)
	{
		back.PaintBlank(hdc, 0, 0, wnd_width, wnd_height, Color(3, 127, 237));
		t_scene->Draw(hdc, 0, 0);
	}
}

//游戏退出处理
void CJumpingRobot::GameEnd()
{
	
}

//游戏按键处理
void CJumpingRobot::GameKeyAction(int Action)
{
	if (Action == KEY_SYS_NONE)//根据keys数组状态处理按键行为
	{
		if (GameState == GAME_RUN)
		{
			if (CheckKey(VK_LEFT))
			{
				player->setFalling(false);
				player->SetDir(DIR_LEFT);
				player->SetActive(true);
			}

			if (CheckKey(VK_RIGHT))
			{
				player->setFalling(false);
				player->SetDir(DIR_RIGHT);
				player->SetActive(true);
			}
			if (CheckKey(VK_SPACE))
			{
				if (player->getJumping() == false)
				{
					player->SetDir(DIR_UP);
					player->SetActive(true);
					player->setJumping(true);
					player->setJumpSpeed(-16);
				}
			}
			if (CheckKey(VK_LEFT) == false && CheckKey(VK_RIGHT) == false && CheckKey(VK_SPACE) == false)
			{
				if (player->getJumping() == false)
				{
					player->SetActive(false);
					player->SetFrame(0);
				}

			}
		}
	}

}

//游戏鼠标行为处理(参数值为KEY_MOUSE_ACTION)
void CJumpingRobot::GameMouseAction(int x, int y, int Action)
{

}
