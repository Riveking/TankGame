//*******************************************************************
// ����TinyEngine�����д̹�˴�ս��Ϸ  
// ����: ������(WanLizhong)
// ����: www.wanlizhong.com 
// ����: 2013-08-02
// ��Ȩ���� 2007-2013 ������
// (C) 2007-2013 WanLizhong All Rights Reserved
//*******************************************************************

#include "TankGame.h"
#include "TinyEngine\\T_Util.h"

//���¶���֡����(����Ϊ��̬)
int TankGame::TANK_FRAME_LEFT[4]	= {0, 1, 2, 3};				
int TankGame::TANK_FRAME_RIGHT[4]	= {4, 5, 6, 7};
int TankGame::TANK_FRAME_UP[4]		= {8, 9, 10, 11};
int TankGame::TANK_FRAME_DOWN[4]	= {12, 13, 14, 15};
int TankGame::EXPLOSION_FRAME[8]	= {0, 1, 2, 3, 4, 5, 6, 7};
int TankGame::BONUS_FRAME[3]		= {0, 1, 2};

//������������������������������������������������������������������������������
// ��Ĺ��캯������������
//������������������������������������������������������������������������������
// ��Ĺ��캯�� 
TankGame::TankGame(HINSTANCE h_instance, LPCTSTR sz_winclass, LPCTSTR sz_title, 
				   WORD icon, WORD sm_icon, int winwidth, int winheight)
:T_Engine(h_instance, sz_winclass, sz_title, icon, sm_icon, winwidth, winheight)
{	
	wnd_width = winwidth;
	wnd_height = winheight;
}

// �����������
TankGame::~TankGame(void)
{
	ClearGameLevel();
}

//������������������������������������������������������������������������������
// ����T_Engine���е��麯��ʵ����Ϸ����
//������������������������������������������������������������������������������
//��Ϸ��ʼ���ӿں���
void TankGame::GameInit() 
{
	GameLevel = 0; //��Ϸ������ʼ��
	GameState = GAME_START;
	LoadGameLevel(GameLevel);
}

//��Ϸ�߼�����(����GameState)
void TankGame::GameLogic()	
{
	GameKeyAction();//���������Ϸ����
	if(GameState == GAME_RUN)
	{
		player->SetEndTime(GetTickCount()); //��¼��ɫ���ɺ��ʱ��

		//��ɫ���ɺ��ܱ�����ʱ��
		if(player->GetEndTime() - player->GetStartTime() >= SUPER_TIME)
		{
			player->SetStartTime(player->GetEndTime());
			isDelayCollision = false;
		}
		if(isDelayCollision == true) player->SetImage(superPlayerImg);
		if(isDelayCollision == false) player->ResetImage();

		LoadNpc(NPC_NUM, NPC_LOAD_TIME);
		UpdateNpcPos();//�ƶ�����ҽ�ɫ(NPC)
		UpdatePlayerPos();//�ƶ���ҽ�ɫ
		UpdateBombPos(&npc_bomb_set);
		UpdateBombPos(&player_bomb_set);
		UpdateAnimation();//���¶�����

		if(updateLifeCount == true) UpdatePlayerLife();
		if(upgradeGameLevel == true) UpgradePlayerLevel();

		LoadBonus();
		UpdateBonus(bonus_life_set);
		UpdateBonus(bonus_bomb_set);
	}
}

//��Ϸ��ʾ(����ֵΪGameState)
void TankGame::GamePaint(HDC hdc)	
{
	if(GameState != GAME_RUN)
	{
		if(GameState == GAME_START)
		{
			if(gameMenu != NULL) gameMenu->DrawMenu(hdc);			
		}
		else
		{
			if(t_scene != NULL && gameMenu != NULL)
			{
				t_scene->Draw(hdc, 0, 0);
				gameMenu->DrawMenu(hdc);
			}
		}
	}
	
	if(GameState == GAME_RUN)
	{
		t_scene->Draw(hdc, 0, 0);
		if(looseBlood == true)
		{
			T_Graph::PaintBlank(hdc, 0, 0, WIN_WIDTH, WIN_HEIGHT, 
				                RGB(255,0,0),150);

			if(GetTickCount()-player->GetStartTime()>200) 
			{
				looseBlood = false;
			}
		}
	}
	DisplayInfo(hdc, GameState);
}

//��Ϸ�˳�����
void TankGame::GameEnd()
{
	ClearGameLevel();
}

//��Ϸ��������
void TankGame::GameKeyAction(int Action)
{
	if(Action == KEY_SYS_NONE)//����keys����״̬��������Ϊ
	{
		if(GameState == GAME_RUN)
		{
			if(CheckKey(VK_LEFT) && !CheckKey(VK_DOWN) && !CheckKey(VK_UP))
			{
				player->SetActive(true);
				player->SetSequence(TANK_FRAME_LEFT, 4);
				player->SetDir(DIR_LEFT);
			}

			if(CheckKey(VK_RIGHT) && !CheckKey(VK_DOWN) && !CheckKey(VK_UP))
			{
				player->SetActive(true);
				player->SetSequence(TANK_FRAME_RIGHT, 4);
				player->SetDir(DIR_RIGHT);
			}
			if(CheckKey(VK_UP) && !CheckKey(VK_LEFT) && !CheckKey(VK_RIGHT))
			{
				player->SetActive(true);
				player->SetSequence(TANK_FRAME_UP, 4);
				player->SetDir(DIR_UP);
			}
			if(CheckKey(VK_DOWN) && !CheckKey(VK_LEFT) && !CheckKey(VK_RIGHT))
			{
				player->SetActive(true);
				player->SetSequence(TANK_FRAME_DOWN, 4);
				player->SetDir(DIR_DOWN);
			}

			if(CheckKey(VK_LEFT) == false && CheckKey(VK_RIGHT)==false &&
				CheckKey(VK_UP)==false && CheckKey(VK_DOWN)==false)
			{
				player->SetActive(false);
			}
		}
	}

	if(Action == KEY_DOWN)
	{
		if(GameState != GAME_RUN)
		{
			if(GameState == GAME_START)
			{
				if(GetAsyncKeyState(VK_UP)<0) gameMenu->MenuKeyDown(VK_UP);
				if(GetAsyncKeyState(VK_DOWN)<0) gameMenu->MenuKeyDown(VK_DOWN);
			}
			if(GameState == GAME_HELP || GameState == GAME_ABOUT ||
		       GameState == GAME_UPGRADE || GameState == GAME_WIN || 
               GameState == GAME_OVER)
			{
				if(GetAsyncKeyState(VK_LEFT)<0) gameMenu->MenuKeyDown(VK_LEFT);
				if(GetAsyncKeyState(VK_RIGHT)<0) gameMenu->MenuKeyDown(VK_RIGHT);	
			}

			if(GetAsyncKeyState(VK_RETURN)<0)
			{
				if(gameMenu->GetMenuIndex() >=0 )
				{
					if(GameState == GAME_START)
					{
						switch(gameMenu->GetMenuIndex())
						{
						case 0:
							GameState = GAME_RUN;
							PlayGameSound();
							break;
						case 1:
							GameState = GAME_HELP;
							delete gameMenu;
							gameMenu = NULL;
							LoadGameMenu(GAME_HELP);
							break;
						case 2:
							GameState = GAME_ABOUT;
							delete gameMenu;
							gameMenu = NULL;
							LoadGameMenu(GAME_ABOUT);
							break;
						case 3:
							SendMessage(m_hWnd, WM_SYSCOMMAND, SC_CLOSE, 0);
							break;
						}
					}
					if(GameState == GAME_UPGRADE || GameState == GAME_WIN || GameState == GAME_OVER)
					{
						switch(gameMenu->GetMenuIndex())
						{
						case 0:
							GameState = GAME_RUN;
							PlayGameSound();
							break;
						case 1:
							SendMessage(m_hWnd, WM_SYSCOMMAND, SC_CLOSE, 0);
							break;
						}
					}

					if(GameState == GAME_ABOUT || GameState == GAME_HELP)
					{
						switch(gameMenu->GetMenuIndex())
						{
						case 0:
							GameState = GAME_START;
							delete gameMenu;
							gameMenu = NULL;
							LoadGameMenu(GAME_START);
							break;
						case 1:
							SendMessage(m_hWnd, WM_SYSCOMMAND, SC_CLOSE, 0);
							break;
						}
					}
				}
			}
		}
		if(GameState == GAME_RUN)
		{
			if(GetAsyncKeyState(VK_SPACE)<0)
			{
				if(player->IsDead() == false && player->IsVisible()==true)
				{
					if(ChargeCount>0)
					{
						LoadBomb(player, player_bomb_set, 400);
						ChargeCount = ChargeCount - 1;
					}
				}
			}
		}
	}
}

//��Ϸ�����Ϊ����(����ֵΪKEY_MOUSE_ACTION)
void TankGame::GameMouseAction(int x, int y, int Action)	
{
	if(Action == MOUSE_MOVE)
	{
		if(GameState != GAME_RUN)
		{
			if(gameMenu != NULL) gameMenu->MenuMouseMove(x, y);
		}
	}

	if(Action == MOUSE_LCLICK)
	{
		if(GameState == GAME_START)
		{
			if(gameMenu != NULL)
			{
				int index = gameMenu->MenuMouseClick(x, y);
				if(index >=0)
				{
					switch(index)
					{
					case 0:
						GameState = GAME_RUN;
						PlayGameSound();
						break;
					case 1:
						GameState = GAME_HELP;
						delete gameMenu;
						gameMenu = NULL;
						LoadGameMenu(GAME_HELP);
						break;
					case 2:
						GameState = GAME_ABOUT;
						delete gameMenu;
						gameMenu = NULL;
						LoadGameMenu(GAME_ABOUT);
						break;
					case 3:
						SendMessage(m_hWnd, WM_SYSCOMMAND, SC_CLOSE, 0);
						break;
					}
				}
			}
		}
		if(GameState == GAME_UPGRADE || GameState == GAME_WIN || GameState == GAME_OVER)
		{
			if(gameMenu != NULL)
			{
				int index = gameMenu->MenuMouseClick(x, y);
				if(index >=0)
				{
					switch(index)
					{
					case 0:
						GameState = GAME_RUN;
						PlayGameSound();
						break;
					case 1:
						SendMessage(m_hWnd, WM_SYSCOMMAND, SC_CLOSE, 0);
						break;
					}
				}
			}

		}
		if(GameState == GAME_HELP || GameState == GAME_ABOUT)
		{
			int index = gameMenu->MenuMouseClick(x, y);
			if(index >=0)
			{
				switch(index)
				{
				case 0:
					GameState = GAME_START;
					delete gameMenu;
					gameMenu = NULL;
					LoadGameMenu(GAME_START);
					break;
				case 1:
					SendMessage(m_hWnd, WM_SYSCOMMAND, SC_CLOSE, 0);
					break;

				}
			}
		}
	}
}

//������������������������������������������������������������������������������
// ����Դ������صĹ��ܺ��� 
//������������������������������������������������������������������������������
// ������Ϸ��ҽ�ɫ
void TankGame::LoadPlayer()
{
	GAMELAYER gameLayer;
	SPRITEINFO tank_Info;
	player = new T_Sprite(L".\\res\\sprite\\playertank.png", 40, 40);
	if(superPlayerImg == NULL) superPlayerImg = new T_Graph(L".\\res\\sprite\\playersuper.png");

	tank_Info.Active = false;
	tank_Info.Dead = false;
	tank_Info.Dir = DIR_UP;
	tank_Info.Rotation = TRANS_NONE;
	tank_Info.Ratio =1.0f;
	tank_Info.Level = 0;
	tank_Info.Score = 0;
	tank_Info.Speed = 3;
	tank_Info.Alpha = 220;
	tank_Info.X = wnd_width/5;
	tank_Info.Y = (wnd_height-player->GetHeight())/2;
	tank_Info.Visible = true;
	player->Initiate(tank_Info);
	player->SetSequence(TANK_FRAME_UP, 4);
	player->SetLayerTypeID(LAYER_PLY);

	gameLayer.layer = player;
	gameLayer.type_id = LAYER_PLY;
	gameLayer.z_order = t_scene->getSceneLayers()->size()+1;
	gameLayer.layer->setZorder(gameLayer.z_order);
	t_scene->Append(gameLayer);
	player->SetStartTime(GetTickCount());
	isDelayCollision = true;
}

// ����NPC��ɫ
void TankGame::LoadNpc(int total, int iTime)
{
	if(NpcLoadNum < total)
	{
		long iCount = GetTickCount();

		if(iCount-NpcLoadTime>iTime)
		{
			NpcLoadTime = iCount;

			GAMELAYER gameLayer;
			SPRITEINFO tank_Info;

			tank_Info.Active = true;
			tank_Info.Dead = false;
			tank_Info.Rotation = TRANS_NONE;
			tank_Info.Ratio = 1.0f;
			tank_Info.Speed = 2;
			tank_Info.Alpha = 230;
			tank_Info.Visible = true;
			tank_Info.Level = GameLevel;
			tank_Info.Score = 0;

			int sp_width = 40;
			int sp_height = 40;
			//��ȡ�������(NPC��ɫ�ڵ�ͼ���Ľ�����)
			int sdr = rand()%4;
			int d = rand()%2;
			switch(sdr)
			{
			case 0://���Ͻ�
				if(d==0)  tank_Info.Dir = DIR_RIGHT;
				if(d==1)  tank_Info.Dir = DIR_DOWN;
				tank_Info.X = t_scene->getSceneX();
				tank_Info.Y =  t_scene->getSceneY();
				break;
			case 1://���Ͻ�
				if(d==0)  tank_Info.Dir = DIR_LEFT;
				if(d==1)  tank_Info.Dir = DIR_DOWN;
				tank_Info.X = t_scene->getSceneX() + scn_width - sp_width;
				tank_Info.Y = t_scene->getSceneY();
				break;
			case 2://���½�
				if(d==0)  tank_Info.Dir = DIR_RIGHT;
				if(d==1)  tank_Info.Dir = DIR_UP;
				tank_Info.X = t_scene->getSceneX();
				tank_Info.Y = t_scene->getSceneY() + scn_height - sp_height;
				break;
			case 3://���½�
				if(d==0)  tank_Info.Dir = DIR_LEFT;
				if(d==1)  tank_Info.Dir = DIR_UP;
				tank_Info.X =  t_scene->getSceneX() + scn_width - sp_width;
				tank_Info.Y = t_scene->getSceneY() + scn_height - sp_height;
				break;
			}

			//��NPC�б��������µ���Ŀ
			npc_set.push_back(new T_Sprite(L".\\res\\sprite\\npctank.png",  40, 40));

			//��ʼ�������ӵ���Ŀ
			T_Sprite *sp = npc_set.back();
			sp->Initiate(tank_Info);
			switch(tank_Info.Dir)
			{
			case DIR_LEFT:
				sp->SetSequence(TANK_FRAME_LEFT, 4);
				break;

			case DIR_RIGHT:
				sp->SetSequence(TANK_FRAME_RIGHT, 4);
				break;
			case DIR_UP:
				sp->SetSequence(TANK_FRAME_UP, 4);
				break;
			case DIR_DOWN:
				sp->SetSequence(TANK_FRAME_DOWN, 4);
				break;
			}
			sp->SetLayerTypeID(LAYER_NPC); 

			gameLayer.layer = sp;
			gameLayer.type_id = LAYER_NPC;
			gameLayer.z_order = t_scene->getSceneLayers()->size()+1;
			gameLayer.layer->setZorder(gameLayer.z_order);
			t_scene->Append(gameLayer);

			NpcLoadNum = NpcLoadNum + 1;
			NpcNumber = NpcNumber + 1; 
			sp = NULL;
		}
	}
}

// ������Ϸ��ͼ����
void TankGame::LoadMap()
{
	t_scene->LoadTxtMap("res\\game\\tankmap.txt");
	scn_width = t_scene->getSceneLayers()->back().layer->GetWidth();
	scn_height =t_scene->getSceneLayers()->back().layer->GetHeight();
	
	// ��ͼ��ʼλ���Ե�ͼ��Ϊ����
	int scn_x = (wnd_width-scn_width)/2;
	int scn_y = (wnd_height-scn_height)/2;
	// ����Ϸ��ͼ��ʼ��Ϊ��Ļ����λ��
	t_scene->InitScene(scn_x, scn_y, scn_width, scn_height, wnd_width, wnd_height);

	// �����е�ͼͼ�㶨λ����Ļ����
	SCENE_LAYERS::iterator p;
	for (p = t_scene->getSceneLayers()->begin(); p != t_scene->getSceneLayers()->end(); p++) 
	{
		if(p->layer->ClassName() == "T_Map" ) p->layer->SetPosition(scn_x, scn_y);
	}
}

// ������ϷͼƬ��Դ
void TankGame::LoadImageRes()
{
	if(npcNumImg == NULL)	npcNumImg = new T_Graph(L".\\res\\game\\npc.png");
	if(lifeImg == NULL)		lifeImg = new T_Graph(L".\\res\\game\\life.png");
	if(flagImg == NULL)		flagImg = new T_Graph(L".\\res\\game\\flag.png");
	if(bmbImg == NULL)		bmbImg = new T_Graph(L".\\res\\game\\charge.png");
}

// ���ر�ʾ�����Ķ���(type=0: �ڵ�; type=1:����)
void TankGame::LoadBonus()
{
	long iCount = GetTickCount();
	if(iCount-BonusLoadTime>BONUS_TIME)
	{
		BonusLoadTime = iCount;

		GAMELAYER gameLayer;
		SPRITEINFO bonusInfo;

		bonusInfo.Active = true;
		bonusInfo.Dead = false;
		bonusInfo.Visible = true;
		bonusInfo.Rotation = TRANS_NONE;
		bonusInfo.Ratio = 1.0f;
		bonusInfo.Speed = 0;
		bonusInfo.Alpha = 255;
		bonusInfo.Level = 0;
		bonusInfo.Score = 0;
		bonusInfo.Dir = 0;

		// �ڵ�ͼ�ϲ��ҷ��ϰ����λ��
		POINT bPT = t_scene->getRandomFreeCell();
		bonusInfo.X = bPT.x;
		bonusInfo.Y = bPT.y;

		int type = rand()%2;

		if(type==0)
		{
			//��NPC�б��������µ���Ŀ
			bonus_bomb_set.push_back(new T_Sprite(L".\\res\\sprite\\bonusammo.png", 40, 40));

			//��ʼ�������ӵ���Ŀ
			T_Sprite *sp = bonus_bomb_set.back();
			sp->Initiate(bonusInfo);
			sp->SetSequence(BONUS_FRAME, 3);
			sp->SetLayerTypeID(LAYER_BONUS); 
			sp->SetStartTime(GetTickCount());

			gameLayer.layer = sp;
			gameLayer.type_id = LAYER_BONUS;
			gameLayer.z_order = t_scene->getBarrier()->getZorder()+1;
			gameLayer.layer->setZorder(gameLayer.z_order);
			t_scene->Append(gameLayer);

			sp = NULL;
		}

		if(type==1)
		{
			//��NPC�б��������µ���Ŀ
			bonus_life_set.push_back(new T_Sprite(L".\\res\\sprite\\bonuslife.png", 40, 40));

			//��ʼ�������ӵ���Ŀ
			T_Sprite *sp = bonus_life_set.back();
			sp->Initiate(bonusInfo);
			sp->SetSequence(BONUS_FRAME, 3);
			sp->SetLayerTypeID(LAYER_BONUS); 
			sp->SetStartTime(GetTickCount());

			gameLayer.layer = sp;
			gameLayer.type_id = LAYER_BONUS;
			gameLayer.z_order = t_scene->getBarrier()->getZorder()+1;
			gameLayer.layer->setZorder(gameLayer.z_order);
			t_scene->Append(gameLayer);

			sp = NULL;
		}
	}
}

// ��Ϸ�˵�ֵ�趨����
void TankGame::SetMenuPara(wstring* menuItems, int itemSize, int m_w, int m_h, int posType)
{
	int y = 0, x = 0;
	for(int i=0; i<itemSize; i++)
	{
		switch(posType)
		{
		case 0:
			x =(wndWidth - 296)-100;
			y = i*(m_h+MENU_SPACE)+200;
			break;
		case 1:
			x =i*(m_w+MENU_SPACE)+(wndWidth - 2*m_w - MENU_SPACE)/2;
			y = (wndHeight - m_h)/2;
			break;
		case 2:
			x =i*(m_w+MENU_SPACE)+(wndWidth - m_w*itemSize - (itemSize-1)*MENU_SPACE)/2;
			y = wndHeight-3*m_h;
			break;
		}

		MENUITEM mItem;
		mItem.pos.x = x;
		mItem.pos.y = y;
		mItem.ItemName = menuItems[i];
		gameMenu->AddMenuItem(mItem);
	}
	gameMenu->SetMoveSound(mouseOverSound);
	gameMenu->SetClickSound(mouseDownSound);

	MENU_INFO menuInfo;
	menuInfo.align = 1;
	menuInfo.space = MENU_SPACE;
	menuInfo.width = m_w;
	menuInfo.height = m_h;
	menuInfo.fontName = L"����";
	menuInfo.isBold = true;
	menuInfo.normalTextColor = Color::White;
	menuInfo.focusTextColor = Color::Red;
	gameMenu->SetMenuInfo(menuInfo);
}

// ������Ϸ�˵�
void TankGame::LoadGameMenu(int type)
{
	if(gameMenu == NULL) gameMenu = new T_Menu();
	gameMenu->SetWinSize(wnd_width, wnd_height);
	gameMenu->SetMenuIndex(-1);
	int btnwidth = 200, btnheight = 80;
	if(type == GAME_START)
	{
		wstring menuItems[] = {L"��ʼ��Ϸ", L"��Ϸ����", L"������Ϸ", L"�˳���Ϸ"};
		gameMenu->SetMenuBkgColor(RGB(50, 250, 50));
		gameMenu->SetMenuBkg(L".\\res\\game\\tanksplash.jpg", 0, 0, 180);
		gameMenu->SetBtnBmp(L".\\res\\game\\btn.png", 280, 60);
		SetMenuPara(menuItems, 4, 280, 60, 0);
	}
	if(type == GAME_UPGRADE)
	{
		wstring menuItems[] = {L"��ʼ��Ϸ", L"�˳���Ϸ"};
		gameMenu->SetMenuBkgColor(RGB(50, 97, 55), 0, 0, 230);
		gameMenu->SetBtnBmp(L".\\res\\game\\btn2.png", btnwidth, btnheight);
		SetMenuPara(menuItems, 2, btnwidth, btnheight, 1);
	}

	if(type == GAME_WIN || type == GAME_OVER)
	{
		wstring menuItems[] = {L"���¿�ʼ", L"�˳���Ϸ"};
		gameMenu->SetMenuBkgColor(RGB(50, 97, 55), 0, 0, 230);
		gameMenu->SetBtnBmp(L".\\res\\game\\btn2.png", btnwidth, btnheight);
		SetMenuPara(menuItems, 2, btnwidth, btnheight, 1);
	}

	if(type == GAME_HELP || type == GAME_ABOUT)
	{
		wstring menuItems[] = {L"���ز˵�", L"�˳���Ϸ"};
		gameMenu->SetMenuBkgColor(RGB(50, 97, 55), 0, 0, 230);
		gameMenu->SetBtnBmp(L".\\res\\game\\btn2.png", btnwidth, btnheight);
		SetMenuPara(menuItems, 2, btnwidth, btnheight, 2);
	}
}

// ������Ϸ������Դ
void TankGame::LoadSound(HWND hwnd)
{
	gameOverSound = new AudioDXBuffer;
	gameWinSound = new AudioDXBuffer;
	menuMusic = new AudioDXBuffer;
	bkgMusic = new AudioDXBuffer;
	mouseOverSound = new AudioDXBuffer;
	mouseDownSound = new AudioDXBuffer;
	explosionSound = new AudioDXBuffer;
	powerupSound = new AudioDXBuffer;

	if(!dxSnd.CreateDS(hwnd))  return;
	gameOverSound->LoadWave(dxSnd, L".\\res\\sound\\gameover.wav");
	gameWinSound->LoadWave(dxSnd, L".\\res\\sound\\victory.wav");
	menuMusic->LoadWave(dxSnd, L".\\res\\sound\\menumusic.wav");
	bkgMusic->LoadWave(dxSnd, L".\\res\\sound\\backmusic.wav");
	mouseOverSound->LoadWave(dxSnd, L".\\res\\sound\\mouseover.wav");
	mouseDownSound->LoadWave(dxSnd, L".\\res\\sound\\mousedown.wav");
	explosionSound->LoadWave(dxSnd, L".\\res\\sound\\blast.wav");
	powerupSound->LoadWave(dxSnd, L".\\res\\sound\\powerup.wav");
}

// ������Ϸ��ըЧ��
void TankGame::LoadExplosion(int x, int y)
{
	SPRITEINFO explInfo;
	GAMELAYER gameLayer;

	explosion_set.push_back(new T_Sprite(L".\\res\\sprite\\explosion.png", 66, 66));
	// ��ʼ�������ӵ���Ŀ
	T_Sprite *sp_explosion = explosion_set.back();

	explInfo.Active = true;
	explInfo.Dead = false;
	explInfo.Dir = 0;
	explInfo.Rotation = TRANS_NONE;
	explInfo.Ratio =1;
	explInfo.Level = 0;
	explInfo.Score = 0;
	explInfo.Speed = 5;
	explInfo.Alpha = 255;
	explInfo.X = x;
	explInfo.Y =  y;
	explInfo.Visible = true;

	sp_explosion->Initiate(explInfo);
	sp_explosion->SetSequence(EXPLOSION_FRAME, 8);
	sp_explosion->SetLayerTypeID(LAYER_EXPLOSION);

	gameLayer.layer = sp_explosion;
	gameLayer.type_id = LAYER_EXPLOSION;
	gameLayer.z_order = t_scene->getSceneLayers()->size()+1;
	gameLayer.layer->setZorder(gameLayer.z_order);
	t_scene->Append(gameLayer);
}

// �����ڵ�����(sp:�����ڵ��Ľ�ɫ; bombSet:�ڵ�����; iTime:�����ڵ��ļ��ʱ��)
void TankGame::LoadBomb(T_Sprite* sp, vSpriteSet& bombSet, int iTime)
{
	sp->SetEndTime(GetTickCount());
	if(sp->GetEndTime() - sp->GetStartTime() >= (DWORD)iTime)
	{
		sp->SetStartTime(sp->GetEndTime());

		GAMELAYER gameLayer;
		SPRITEINFO bombInfo;
		int m_dir = sp->GetDir();

		wchar_t path[256];
		wsprintf(path, L".\\res\\sprite\\missle%d.bmp", m_dir);
		bombSet.push_back(new T_Sprite(path));

		//��ʼ�������ӵ���Ŀ
		T_Sprite *bomb = bombSet.back();
		bombInfo.Active = true;
		bombInfo.Dead = false;
		bombInfo.Dir = m_dir;
		bombInfo.Rotation = TRANS_NONE;
		bombInfo.Ratio =1;
		bombInfo.Level = 0;
		bombInfo.Score = 0;
		bombInfo.Speed = 5;
		bombInfo.Alpha = 255;

		switch(m_dir)
		{
		case DIR_LEFT:
			bombInfo.X = sp->GetX() - 16;
			bombInfo.Y =  sp->GetY()+sp->GetRatioSize().cy/2 - 12;
			break;
		case DIR_RIGHT:
			bombInfo.X = sp->GetX()+sp->GetRatioSize().cx;
			bombInfo.Y = sp->GetY()+sp->GetRatioSize().cy/2 - 12;
			break;
		case DIR_UP:
			bombInfo.X = sp->GetX()+sp->GetRatioSize().cx/2 - 2;
			bombInfo.Y = sp->GetY() - 16;
			break;
		case DIR_DOWN:
			bombInfo.X = sp->GetX()+sp->GetRatioSize().cx/2 - 2;
			bombInfo.Y = sp->GetY()+sp->GetRatioSize().cy - 25;
			break;
		}

		bombInfo.Visible = true;
		bomb->Initiate(bombInfo);

		gameLayer.layer = bomb;
		if(bombSet == player_bomb_set)
		{
			gameLayer.type_id = LAYER_PLY_BOMB;
			gameLayer.layer->SetLayerTypeID(LAYER_PLY_BOMB);
		}
		if(bombSet == npc_bomb_set)
		{
			gameLayer.type_id = LAYER_NPC_BOMB;
			gameLayer.layer->SetLayerTypeID(LAYER_NPC_BOMB);
		}		
		gameLayer.z_order = t_scene->GetTotalLayers()+1;
		gameLayer.layer->setZorder(gameLayer.z_order);
		t_scene->Append(gameLayer);
	}
}

// ������Ϸ��Ӫ��־
void TankGame::LoadNest()
{
	GAMELAYER gameLayer;
	SPRITEINFO Info;

	nest = new T_Sprite(L".\\res\\game\\nest.png");
	Info.Active = false;
	Info.Dead = false;
	Info.Dir = 0;
	Info.Rotation = TRANS_NONE;
	Info.Ratio = 1.0f;
	Info.Level = 0;
	Info.Score = 0;
	Info.Speed = 0;
	Info.Alpha = 255;

	int centerX =(wnd_width-nest->GetWidth())/2;
	int centerY =(wnd_height-(int)(2.5*nest->GetHeight()))/2;

	Info.X = centerX;
	Info.Y = centerY;
	Info.Visible = true;

	nest->Initiate(Info);
	nest->SetLayerTypeID(LAYER_TARGET);

	gameLayer.layer = nest;
	gameLayer.type_id = LAYER_TARGET;
	gameLayer.z_order = LAYER_MAX+1;
	gameLayer.layer->setZorder(LAYER_MAX+1);
	t_scene->Append(gameLayer);
}

// ������ҽ�ɫ��λ��
void TankGame::UpdatePlayerPos()
{
	if(player==NULL) return;

	int nextStepX, nextStepY;
	int SpeedX=0, SpeedY=0;

	if(player->IsDead() == false && player->IsVisible()==true &&player->IsActive() == true)
	{
		switch(player->GetDir())
		{
		case DIR_LEFT:
			SpeedX = -player->GetSpeed();
			SpeedY = 0;
			//������һ���ƶ��Ƿ񳬹��߽�
			nextStepX = player->GetX()-player->GetSpeed();
			if(nextStepX <= 0) 
			{
				SpeedX = 0 - player->GetX();
			}

			break;

		case DIR_RIGHT:
			SpeedX =player->GetSpeed();
			SpeedY = 0;
			//������һ���ƶ��Ƿ񳬹��߽�
			nextStepX = player->GetX()+ player->GetRatioSize().cx + player->GetSpeed();
			if(nextStepX >= wnd_width) 
			{
				SpeedX = wnd_width - player->GetRatioSize().cx - player->GetX();
			}

			break;

		case DIR_UP:
			SpeedX = 0;
			SpeedY = -player->GetSpeed();
			//������һ���ƶ��Ƿ񳬹��߽�
			nextStepY = player->GetY()-player->GetSpeed();
			if(nextStepY <= 0) 
			{
				SpeedY = 0 - player->GetY();
			}

			break;

		case DIR_DOWN:
			SpeedX = 0;
			SpeedY = player->GetSpeed();
			//������һ���ƶ��Ƿ񳬹��߽�
			nextStepY = player->GetY() + player->GetRatioSize().cy + player->GetSpeed();
			if(nextStepY >= wnd_height) 
			{
				SpeedY = wnd_height - player->GetRatioSize().cy - player->GetY();
			}

			break;
		}

		int x = player->GetX();// ����ƶ�ǰ��xλ��
		int y = player->GetY();// ����ƶ�ǰ��yλ��

		if(!player->CollideWith(t_scene->getBarrier())) 
		{
			player->Move(SpeedX, SpeedY);	// �����ƶ�
		}
		if(player->CollideWith(t_scene->getBarrier()))
		{
			player->SetPosition(x, y);		// ��ԭ�ƶ�ǰ��λ��
		}

		// ������NPC��ײ
		vSpriteSet::iterator p;
		for (p = npc_set.begin(); p != npc_set.end(); p++) 
		{
			if(player->CollideWith((*p)) && !(*p)->IsDead() && (*p)->IsVisible())
			{
				(*p)->SetActive(false);
				(*p)->SetVisible(false);
				(*p)->SetDead(true);

				//NPC��Ŀ����
				NpcNumber = NpcNumber - 1;
				LoadExplosion(player->GetX(), player->GetY());
				explosionSound->Play(false);
				if(isDelayCollision == false)
				{
					updateLifeCount = true;
					looseBlood = true;
					return;  //�˴����뷵��
				}
			}
		}

		//��Ϸ������������
		if(NpcNumber == 0)
		{ 
			GameState = GAME_UPGRADE;
			upgradeGameLevel = true;
		}

		t_scene->ScrollScene(player);//��������
	}
}

// ������ҵ�����ֵ
void TankGame::UpdatePlayerLife()
{
	LifeCount = LifeCount - 1;
	player->SetStartTime(GetTickCount());
	isDelayCollision = true;
	updateLifeCount = false;
	int mapCenterX =(wnd_width-scn_width)/2;
	int mapCenterY =(wnd_height-scn_height)/2;
	int mapOffsetX= abs(t_scene->getlastSceneX())-abs(mapCenterX);
	int mapOffsetY= abs(t_scene->getlastSceneY())-abs(mapCenterY);
	int startX = wnd_width/5;
	int startY = (wnd_height-player->GetHeight())/2;

	t_scene->SetScenePos(mapCenterX, mapCenterY);
	player->SetPosition(startX-mapOffsetX, startY-mapOffsetY);
	player->SetSequence(TANK_FRAME_UP, 4);
	player->SetDir(DIR_UP);
	player->SetVisible(true);
	player->SetActive(false);
	if(LifeCount == 0)
	{
		if(bkgMusic->IsAlive())	 bkgMusic->Release();
		player->SetActive(false);
		player->SetVisible(false);
		player->SetDead(true);
		gameOverSound->Play(false);
		GameState = GAME_OVER;
		ClearGameLevel();
		GameLevel = 0;
		LoadGameLevel(GameLevel);
	}
}

// ����NPC��λ��
void TankGame::UpdateNpcPos()
{
	if(npc_set.size()==0) return;
	
	vSpriteSet::iterator p;
	T_AI* spAi = new T_AI(4);
	for (p = npc_set.begin(); p != npc_set.end(); p++) 
	{
		if((*p)->IsActive()==true && (*p)->IsVisible()==true)
		{
			int SpeedX=0, SpeedY=0;
			int npc_dir=0;

			spAi->Evade((*p), player);
			spAi->CheckOverlay((*p), npc_set);

			npc_dir = (*p)->GetDir();
			switch(npc_dir)
			{
			case DIR_LEFT:
				(*p)->SetSequence(TANK_FRAME_LEFT, 4);
				break;

			case DIR_RIGHT:
				(*p)->SetSequence(TANK_FRAME_RIGHT, 4);
				break;

			case DIR_UP:
				(*p)->SetSequence(TANK_FRAME_UP, 4);
				break;

			case DIR_DOWN:
				(*p)->SetSequence(TANK_FRAME_DOWN, 4);
				break;
			}
			
			// �����ײ����
			if( (*p)->CollideWith(player) && (*p)->IsDead() == false && (*p)->IsVisible())
			{
				(*p)->SetVisible(false);
				(*p)->SetDead(true);
				(*p)->SetActive(false);;

				//NPC��Ŀ����
				NpcNumber = NpcNumber - 1;
				LoadExplosion(player->GetX(), player->GetY());
				explosionSound->Play(false);
				
				if(isDelayCollision == false)
				{
					updateLifeCount = true;
					return;
				}				
			}

			spAi->Wander((*p), t_scene->getBarrier());

			//�����ڵ�
			LoadBomb((*p), npc_bomb_set, 2000);
		}
	}
	delete spAi;
}


// ���½�ɫ����֡���к�
void TankGame::UpdateAnimation()
{
	vSpriteSet::iterator p;

	if(npc_set.size()>0)
	{
		for (p = npc_set.begin(); p != npc_set.end(); p++) 
		{
			(*p)->LoopFrame();
		}
	}
		
	if(player != NULL)
	{
		if(player->IsVisible()==true && player->IsActive()==true)
		{
			player->LoopFrame();
		}
	}
	
	if(explosion_set.size()>0)
	{
		vSpriteSet::iterator e = explosion_set.begin();
		for(; e != explosion_set.end();)
		{
			if((*e)->LoopFrameOnce() == true)
			{
				// ɾ���������Ѿ�ʧЧ�ı�ը����	
				SCENE_LAYERS::iterator lp;
				for (lp = t_scene->getSceneLayers()->begin(); lp != t_scene->getSceneLayers()->end(); lp++) 
				{
					if((*lp).layer == (*e))
					{
						lp = t_scene->getSceneLayers()->erase(lp);
						break;
					}
				}
				delete (*e);
				e = explosion_set.erase(e);
				continue;
			}
			else
			{
				++e;
			}
		}
	}
	
}

// �����ڵ���λ��
void TankGame::UpdateBombPos(vSpriteSet* bombSet)
{
	if(bombSet->size()==0) return;

	vSpriteSet::iterator itp = bombSet->begin();
	for(; itp != bombSet->end();)
	{
		if((*itp)->IsVisible()==true && (*itp)->IsActive()==true)
		{
			int SpeedX=0, SpeedY=0;
			switch((*itp)->GetDir())
			{
			case DIR_LEFT:
				SpeedX = -(*itp)->GetSpeed();
				SpeedY = 0;
				break;

			case DIR_RIGHT:
				SpeedX =(*itp)->GetSpeed();
				SpeedY = 0;
				break;

			case DIR_UP:
				SpeedX = 0;
				SpeedY = -(*itp)->GetSpeed();
				break;

			case DIR_DOWN:
				SpeedX = 0;
				SpeedY = (*itp)->GetSpeed();
				break;
			}
			(*itp)->Move(SpeedX, SpeedY);

			//����ڵ��Ƿ����Ŀ��
			BombCollide((*itp));
			if(GameState != GAME_RUN) return;
		}

		if((*itp)->IsVisible() == false ||
			(*itp)->GetY() < 0 || (*itp)->GetX() < 0 ||
			(*itp)->GetY() > wnd_height || (*itp)->GetX() > wnd_width )
		{
			//ɾ�������еĶ���	
			SCENE_LAYERS::iterator p;
			for (p = t_scene->getSceneLayers()->begin(); 
				p != t_scene->getSceneLayers()->end(); p++) 
			{
				if((*p).layer == (*itp))
				{
					p = t_scene->getSceneLayers()->erase(p);
					break;
				}
			}
			delete (*itp);
			itp = bombSet->erase(itp);
		}

		if(itp == bombSet->end())
		{
			break;
		}

		itp++;
	}
}

// ���½��������״̬
void TankGame::UpdateBonus(vSpriteSet& bonusSet)
{
	if(bonusSet.size()==0) return;

	long tm = GetTickCount();
	vSpriteSet::iterator itp;
	if(bonusSet.size()>0) 
	{
		itp = bonusSet.begin();
		for(; itp != bonusSet.end();)
		{
			if((tm-(*itp)->GetStartTime()) > (BONUS_TIME-2000))
			{
				(*itp)->LoopFrame(); // ������ʧ���涯��
			}

			bool isPicked = player->CollideWith(*itp);
			if(isPicked || ((tm-(*itp)->GetStartTime()) > BONUS_TIME))
			{
				if(isPicked)
				{
					if(bonusSet == bonus_bomb_set)
					{
						ChargeCount = ChargeCount + BONUS_CHARGE;
						powerupSound->Play();
					}
					if(bonusSet == bonus_life_set)
					{
						LifeCount = LifeCount + 1;
						powerupSound->Play();
					}
				}
				//ɾ�������еĶ���	
				SCENE_LAYERS::iterator p;
				for (p = t_scene->getSceneLayers()->begin(); 
					p != t_scene->getSceneLayers()->end(); p++) 
				{
					if((*p).layer == (*itp))
					{
						p = t_scene->getSceneLayers()->erase(p);
						break;
					}
				}
				delete (*itp);
				itp = bonusSet.erase(itp);
			}

			if(itp == bonusSet.end())
			{
				break;
			}
			itp++;
		}
	}
}

//������������������������������������������������������������������������������
// ��Ϸ�ؿ���صĹ��ܺ��� 
//������������������������������������������������������������������������������
// ���عؿ�����
void TankGame::LoadGameLevel(int level)
{
	isDelayCollision = true;
	upgradeGameLevel = false;
	looseBlood = false;
	updateLifeCount = false;
	LifeCount = PLAYER_LIFE;
	ChargeCount = PLAYER_CHARGE;
	NpcNumber = 0;
	NpcLoadNum = 0;
	NpcLoadTime = GetTickCount();
	BonusLoadTime = GetTickCount();
	superPlayerImg = NULL;
	npcNumImg = NULL;
	lifeImg = NULL;
	flagImg = NULL;
	bmbImg = NULL;
	gameMenu = NULL;
	t_scene = NULL;

	if(t_scene == NULL) t_scene = new T_Scene();
	if(gameMenu == NULL) gameMenu = new T_Menu();
	LoadMap();
	LoadPlayer();
	LoadNest();
	LoadSound(m_hWnd);
	LoadImageRes();
	SetFrame(FRAME_SPEED);
	LoadGameMenu(GameState);
	PlayGameSound();
}

// ����ؿ�����
void TankGame::ClearGameLevel()
{
	delete npcNumImg;		npcNumImg = NULL;
	delete lifeImg;			lifeImg = NULL;
	delete superPlayerImg;	superPlayerImg = NULL;
	delete flagImg;		flagImg = NULL;
	delete bmbImg;			bmbImg = NULL;
	delete t_scene;			t_scene = NULL;
	delete gameMenu;		gameMenu = NULL;

	npc_set.clear();			npc_set.swap(vector<T_Sprite*>()) ;
	player_bomb_set.clear();	player_bomb_set.swap(vector<T_Sprite*>());
	npc_bomb_set.clear();		npc_bomb_set.swap(vector<T_Sprite*>());
	explosion_set.clear();		explosion_set.swap(vector<T_Sprite*>());
	bonus_life_set.clear();		bonus_life_set.swap(vector<T_Sprite*>());
	bonus_bomb_set.clear();		bonus_bomb_set.swap(vector<T_Sprite*>());

	ReleaseDXBuffer(gameOverSound);
	ReleaseDXBuffer(gameWinSound);
	ReleaseDXBuffer(menuMusic);
	ReleaseDXBuffer(bkgMusic);
	ReleaseDXBuffer(mouseOverSound);
	ReleaseDXBuffer(mouseDownSound);
	ReleaseDXBuffer(explosionSound);
	ReleaseDXBuffer(powerupSound);
	dxSnd.ReleaseAll();
}


//������������������������������������������������������������������������������
// ��Ϸϸ�ڴ�����
//������������������������������������������������������������������������������
// �����������
bool TankGame::UpgradePlayerLevel()
{
	bool breakThrough = false;

	if(upgradeGameLevel)
	{
		if(GameLevel <= TOTAL_LEVEL -1) 
		{
			player->SetLevel(player->GetLevel()+1);
			GameLevel = GameLevel + 1; //��Ϸ��һ��
			upgradeGameLevel = false;
			breakThrough = false;
			GameState = GAME_UPGRADE;
			ClearGameLevel();
			LoadGameLevel(GameLevel);
		}

		if(GameLevel > TOTAL_LEVEL - 1)
		{
			if(bkgMusic->IsAlive()) bkgMusic->Release();
			GameLevel = 0;
			upgradeGameLevel = false;
			gameWinSound->Play(false);
			breakThrough = true;
			GameState = GAME_WIN;
			ClearGameLevel();
			LoadGameLevel(GameLevel);
		}
	}

	return breakThrough;
}

// ������Ϸ�еĸ�����������
void TankGame::PlayGameSound()
{
	menuMusic->Terminate();			// �˵�����
	bkgMusic->Terminate();			// ��Ϸ��������
	gameOverSound->Terminate();		// ��Ϸ��������
	gameWinSound->Terminate();		// ��Ϸʤ������
	
	switch(GameState)
	{
	case GAME_START:
		menuMusic->Play(true);
		break;
	case GAME_RUN:
		bkgMusic->Play(false);
		break;
	case GAME_WIN:
		gameWinSound->Play(false);
		break;
	case GAME_OVER:
		gameOverSound->Play(false);
		break;
	case GAME_UPGRADE:
		gameWinSound->Play(false);
		break;
	}
}

// ��Ϸ��Ϣ��ʾ
void TankGame::DisplayInfo(HDC hdc, int game_state)
{
	int FontHeight = 0;//�ֺ�
	Gdiplus::RectF rect;
	wstring Content = L"";
	switch(game_state)
	{
	case GAME_START:
		{
			wchar_t* GameName = L"��̹�˴�ս";
			wchar_t* Version = L" �汾: 1.0.0 ";
			wchar_t* CopyRight = L" (C) 2013 ������(Wan Lizhong)";
			wchar_t* Notice = L" ����ϷΪTinyEngine����ʹ�÷���";

			rect.X = 50.00;
			rect.Y = 80.00;
			rect.Width = 768.00;
			rect.Height = 100.00;

			FontHeight = 32;//�ֺ�

			T_Graph::PaintText(hdc, rect, GameName, (REAL)FontHeight, L"����", 
				Color::White, FontStyleBold, StringAlignmentNear);

			rect.X = 50.00;
			rect.Y = 170.00;
			rect.Width = 768.00;
			rect.Height = 30.00;

			FontHeight = 16;//�ֺ�
			T_Graph::PaintText(hdc, rect, Version, (REAL)FontHeight, L"����",
				Color::White, FontStyleBold, StringAlignmentNear);

			rect.X = 50.00;
			rect.Y = 200.00;
			rect.Width = 768.00;
			rect.Height = 30.00;

			FontHeight = 14;//����ÿ���ֵ����ؼ����ֺ�
			T_Graph::PaintText(hdc, rect, CopyRight, (REAL)FontHeight, L"����",
				Color::White, FontStyleBold, StringAlignmentNear);

			rect.X = 50.00;
			rect.Y = 250.00;
			rect.Width = 768.00;
			rect.Height = 30.00;

			FontHeight = 14;//�ֺ�
			T_Graph::PaintText(hdc, rect, Notice, (REAL)FontHeight, L"����",
				Color::White, FontStyleBold, StringAlignmentNear);
		}
		break;
	case GAME_RUN:
		{
			int i=0;
			int x = 25;
			int y = 12;
			int fy = 15;
			int iconW = 30;
			int textW = 150;
			int textH = 30;
			FontHeight = 18; // �ֺ�

			//������Ϸ״̬��Ϣ
			if(GameLevel <= TOTAL_LEVEL)
			{
				npcNumImg->PaintRegion(
					npcNumImg->GetBmpHandle(), hdc, x, y, 0, 0, 
					npcNumImg->GetImageWidth(), npcNumImg->GetImageHeight(), 1);

				x = x + iconW;
				wstring EnemyNum = L"����: ";
				EnemyNum.append(T_Util::int_to_wstring(NpcNumber));
				rect.X = (float)x;		
				rect.Y = (float)fy;
				rect.Width = (float)textW;				
				rect.Height = (float)textH;
				T_Graph::PaintText(hdc, rect, EnemyNum.c_str(),(REAL)FontHeight, 
					L"����", Color(255, 255, 255, 0), 
					FontStyleBold, StringAlignmentNear);

				x = x+textW;
				lifeImg->PaintRegion(
					lifeImg->GetBmpHandle(), hdc, x, y, 0, 0, 
					lifeImg->GetImageWidth(), lifeImg->GetImageHeight(), 1);

				wstring lifeleft = L"����: ";
				lifeleft.append(T_Util::int_to_wstring(LifeCount));
				x = x + iconW;
				rect.X = (float)x;		
				rect.Y = (float)fy;
				rect.Width = (float)textW;				
				rect.Height = (float)textH;
				T_Graph::PaintText(
					hdc, rect, lifeleft.c_str(),(REAL)FontHeight, 
					L"����", Color(255, 255, 255, 0), 
					FontStyleBold, StringAlignmentNear);

				x = x+textW;
				bmbImg->PaintRegion(
					bmbImg->GetBmpHandle(),hdc, x, y, 0, 0, 
					bmbImg->GetImageWidth(), bmbImg->GetImageHeight(), 1);

				wstring ChargeNum = L"��ҩ: ";
				ChargeNum.append(T_Util::int_to_wstring(ChargeCount));
				x = x + iconW;
				rect.X = (float)x;		
				rect.Y = (float)fy;
				rect.Width = (float)textW;				
				rect.Height = (float)textH;
				T_Graph::PaintText(
					hdc, rect, ChargeNum.c_str(),(REAL)FontHeight, 
					L"����", Color(255, 255, 255, 0), FontStyleBold, StringAlignmentNear);


				x = x+textW;
				flagImg->PaintRegion(
					flagImg->GetBmpHandle(),hdc, x, y, 0, 0, 
					flagImg->GetImageWidth(), flagImg->GetImageHeight(), 1);

				wstring ScoreNum = L"����: ";
				ScoreNum.append(T_Util::int_to_wstring(GameLevel+1));
				x = x + iconW;
				rect.X = (float)x;		
				rect.Y = (float)fy;
				rect.Width = (float)textW;				
				rect.Height = (float)textH;
				T_Graph::PaintText(hdc, rect, ScoreNum.c_str(),(REAL)FontHeight, 
					L"����", Color(255, 255, 255, 0), FontStyleBold, StringAlignmentNear);
			}
		}
		break;

	case GAME_OVER:
		rect.X = 0.00;
		rect.Y = 0.00;
		rect.Width = float(WIN_WIDTH);
		rect.Height = float(WIN_HEIGHT/2);
		FontHeight = 48;
		T_Graph::PaintText(hdc, rect, L"��Ϸ����!", (REAL)FontHeight, L"����");
		break;
	case GAME_UPGRADE:
		rect.X = 0.00;
		rect.Y = 0.00;
		rect.Width = float(WIN_WIDTH);
		rect.Height = float(WIN_HEIGHT/2);

		FontHeight = 48;
		T_Graph::PaintText(hdc, rect, L"��һ��", (REAL)FontHeight, L"����");
		break;
	case GAME_WIN:
		rect.X = 0.00;
		rect.Y = 0.00;
		rect.Width = float(WIN_WIDTH);
		rect.Height = float(WIN_HEIGHT/2);
		FontHeight = 48;
		T_Graph::PaintText(hdc, rect, L"��ϲͨ�أ�", (REAL)FontHeight, L"����");
		
		break;
	case GAME_HELP:
		rect.X = 60.00;
		rect.Y = 130.00;
		rect.Width = float(WIN_WIDTH-120);
		rect.Height = 80;
		FontHeight = 48;
		T_Graph::PaintText(hdc, rect, L"��  ��", (REAL)FontHeight, L"����");

		rect.X = 60.00;
		rect.Y = 220.00;
		rect.Width = float(WIN_WIDTH-120);
		rect.Height = WIN_HEIGHT-450;
		FontHeight = 26;
		Content = L"    ���ϡ��¡����ҷ�����ƶ����̹�ˣ����հ׼������ڵ���";
		Content.append(L"ÿ����һ���ڵ�������ڵ�����1��");
		Content.append(L"������̹�˱��з��ڵ����У��������ֵ��1��");
		Content.append(L"���������ֵΪ0ʱ���з��ڵ����У���Ϸ������");
		Content.append(L"��Ϸ�������������ͼ�������̹�˴���������ֵ������1��");
		Content.append(L"��Ϸ��������ֵ�ҩ��ͼ�������̹�˴������ڵ���ֵ������20��");
		Content.append(L"�����𱾹�ȫ���з�̹�ˣ���Ϸ������һ�ء�");
		T_Graph::PaintText(
			hdc, rect, Content.c_str(), (REAL)FontHeight, L"����", 
			Color::White, FontStyleRegular, StringAlignmentNear);

		break;
	case GAME_ABOUT:
		rect.X = 60.00;
		rect.Y = 130.00;
		rect.Width = float(WIN_WIDTH-120);
		rect.Height = 80;
		FontHeight = 48;
		T_Graph::PaintText(hdc, rect, L"��  ��", (REAL)FontHeight, L"����");

		rect.X = 80.00;
		rect.Y = 220.00;
		rect.Width = float(WIN_WIDTH-120);
		rect.Height = WIN_HEIGHT-450;
		FontHeight = 24;
		Content = L"��̹�˴�ս\n";
		Content.append(L"�汾��1.0.0\n");
		Content.append(L"����ϷΪTinyEngine��Ϸ���永����Ϸ\n");
		Content.append(L"���ߣ�������(WanLizhong)\n");
		Content.append(L"����: www.wanlizhong.com\n");
		Content.append(L"Copyright 2007-2013 WanLizhong\n");
		Content.append(L"All Rights Reserved\n");
		Content.append(L"��������Ȩ��");
		T_Graph::PaintText(
			hdc, rect, Content.c_str(), (REAL)FontHeight, L"����", 
			Color::White, FontStyleRegular, StringAlignmentNear);
	}	
}

// �ڵ���ײ����
void TankGame::BombCollide(T_Sprite* bomb)
{
	if(bomb->IsActive()==true && bomb->IsVisible()==true)
	{
		// �ж��Ƿ�����ϳ�
		if(bomb->CollideWith(nest))
		{
			bomb->SetVisible(false);
			bomb->SetActive(false);
			if(bkgMusic->IsAlive())	bkgMusic->Release();
			explosionSound->Play(false);
			gameOverSound->Play(false);
			GameState = GAME_OVER;
			ClearGameLevel();
			GameLevel = 0;
			LoadGameLevel(GameLevel);
			return;
		}

		// ��������˵�ͼ�ϰ�������
		T_Map* map;
		bool collideBarrier = bomb->CollideWith(t_scene->getBarrier());
		bool collideMask = bomb->CollideWith(t_scene->getMask());
		if(collideBarrier || collideMask)
		{
			if(collideBarrier) map = t_scene->getBarrier();
			if(collideMask) map = t_scene->getMask();
			map->setTile(bomb->GetMapBlockPT().x, bomb->GetMapBlockPT().y, 0);
			bomb->SetVisible(false);
			bomb->SetActive(false);
			bomb->SetVisible(false);
			bomb->SetActive(false);
			int x = bomb->GetX() - 33;
			int y = bomb->GetY() - 33;
			LoadExplosion(x, y);
			explosionSound->Play(false);
		}

		// ���NPC�ڵ����������
		if(bomb->GetLayerTypeID() == LAYER_NPC_BOMB && 
			bomb->CollideWith(player) && 
			player->IsDead()==false && player->IsVisible()==true)
		{

			bomb->SetVisible(false);
			bomb->SetActive(false);

			int x = bomb->GetX()-player->GetWidth()/2;
			int y = bomb->GetY()-player->GetHeight()/2;

			LoadExplosion(x, y);
			explosionSound->Play(false);

			if(isDelayCollision == false)
			{
				looseBlood = true;
				updateLifeCount = true;
			}
		}

		// �������ڵ�������NPC
		if(bomb->GetLayerTypeID() == LAYER_PLY_BOMB)
		{
			for (vSpriteSet::iterator sp = npc_set.begin(); sp != npc_set.end(); sp++) 
			{
				if(bomb->CollideWith((*sp)) && !((*sp)->IsDead()) && 
					((*sp)->IsVisible()) && ((*sp)->IsActive()))
				{
					(*sp)->SetActive(false);
					(*sp)->SetVisible(false);
					(*sp)->SetDead(true);
					bomb->SetActive(false);
					bomb->SetVisible(false);
					//NPC��Ŀ����
					NpcNumber = NpcNumber - 1;
					LoadExplosion((*sp)->GetX(), (*sp)->GetY());
					explosionSound->Play(false);
					//��Ϸ������������
					if(NpcNumber == 0)
					{ 
						GameState = GAME_UPGRADE;
						upgradeGameLevel = true;
					}

					break;
				}

			}
		}
	}
}

// �ͷ���������
void TankGame::ReleaseDXBuffer(AudioDXBuffer* dxBuf)
{
	if(dxBuf != NULL) 
	{
		dxBuf->Release();
		delete dxBuf;
		dxBuf = NULL;
	}
}