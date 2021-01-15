#include "JumpingRobot.h"

//���¶���֡����(����Ϊ��̬)
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

// ������Ϸ��ͼ����
void CJumpingRobot::LoadMap()
{
	if (t_scene == NULL) t_scene = new T_Scene();
	t_scene->LoadTxtMap("JumpMap\\jump_map.txt");
	scn_width = t_scene->getSceneLayers()->back().layer->GetWidth();
	scn_height = t_scene->getSceneLayers()->back().layer->GetHeight();

	// ��ͼ��ʼλ���Ե�ͼ��Ϊ����
	int scn_x = 0;
	int scn_y = 0;
	// ����Ϸ��ͼ��ʼ��Ϊ���Ͻ�
	t_scene->InitScene(scn_x, scn_y, scn_width, scn_height, wnd_width, wnd_height);

	// �����е�ͼͼ�㶨λ�����Ͻ�
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
		// ������ҵ���Ծ�������ƶ�	
		if (player->getJumping())
		{
			player->SetSequence(FRAME_UP, 20);
			player->jumpUpDown(t_scene->getBarrier());
			t_scene->ScrollScene(player);//��������
		}
		if (player->GetDir() == DIR_LEFT)
		{
			player->updatePostion(DIR_LEFT, 0, 0, t_scene->getBarrier());
			t_scene->ScrollScene(player);//��������
		}
		if (player->GetDir() == DIR_RIGHT)
		{
			player->updatePostion(DIR_RIGHT, 0, 0, t_scene->getBarrier());
			t_scene->ScrollScene(player);//��������
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

// ������Ϸ��ҽ�ɫ
void CJumpingRobot::LoadPlayer()
{
	GAMELAYER gameLayer;
	SPRITEINFO spInfo;

	// ������ҽ�ɫ
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

//������������������������������������������������������������������������������
// ����T_Engine���е��麯��ʵ����Ϸ����
//������������������������������������������������������������������������������
//��Ϸ��ʼ���ӿں���
void CJumpingRobot::GameInit()
{
	GameState = GAME_RUN;
	LoadMap();
	LoadPlayer();
}

//��Ϸ�߼�����(����GameState)
void CJumpingRobot::GameLogic()
{
	GameKeyAction();//���������Ϸ����
	if (GameState == GAME_RUN)
	{
		UpdatePlayerPos();//�ƶ���ҽ�ɫ
		UpdateAnimation();//���¶�����
	}
}

//��Ϸ��ʾ(����ֵΪGameState)
void CJumpingRobot::GamePaint(HDC hdc)
{
	if (GameState == GAME_RUN)
	{
		back.PaintBlank(hdc, 0, 0, wnd_width, wnd_height, Color(3, 127, 237));
		t_scene->Draw(hdc, 0, 0);
	}
}

//��Ϸ�˳�����
void CJumpingRobot::GameEnd()
{
	
}

//��Ϸ��������
void CJumpingRobot::GameKeyAction(int Action)
{
	if (Action == KEY_SYS_NONE)//����keys����״̬��������Ϊ
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

//��Ϸ�����Ϊ����(����ֵΪKEY_MOUSE_ACTION)
void CJumpingRobot::GameMouseAction(int x, int y, int Action)
{

}
