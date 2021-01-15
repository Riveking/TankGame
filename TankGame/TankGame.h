//*******************************************************************
// ����TinyEngine�����д̹�˴�ս��Ϸ  
// ����: ������(WanLizhong)
// ����: www.wanlizhong.com 
// ����: 2013-08-02
// ��Ȩ���� 2007-2013 ������
// (C) 2007-2013 WanLizhong All Rights Reserved
//*******************************************************************

#pragma once
#include "TinyEngine\\T_Map.h"
#include "TinyEngine\\T_Audio.h"
#include "TinyEngine\\T_Sprite.h" 
#include "TinyEngine\\T_Scene.h"
#include "TinyEngine\\T_AI.h"
#include "TinyEngine\\T_Engine.h"
#include "TinyEngine\\T_Menu.h"
#include "JRobot.h"

// ����vector�������͵���������vSpriteSet
typedef vector<T_Sprite*> vSpriteSet;

class TankGame:public T_Engine
{
private:
	//������������������������������������������������������������������������������
	// ���嶯��֡���У��뾲̬����֡ͼƬһ�£�
	//������������������������������������������������������������������������������
	static int TANK_FRAME_LEFT[4];					// ̹�������֡����
	static int TANK_FRAME_RIGHT[4];					// ̹���ҷ����֡����
	static int TANK_FRAME_UP[4];					// ̹���Ϸ����֡����
	static int TANK_FRAME_DOWN[4];					// ̹���·����֡����
	static int EXPLOSION_FRAME[8];					// ��ը֡����
	static int BONUS_FRAME[3];						// ������Ʒ����֡����
	//������������������������������������������������������������������������������
	// ��Ϸ�еĹؼ�����
	//������������������������������������������������������������������������������
	static const int TOTAL_LEVEL	= 5;			// ��Ϸ�ܹ���
	static const int NPC_NUM		= 20;			// ̹������
	static const int NPC_LOAD_TIME	= 1000;			// NPC���ؼ��ʱ��
	static const int SUPER_TIME		= 5000;			// ��ҽ�ɫ�޵�ʱ��
	static const int PLAYER_LIFE	= 5;			// ��ҳ�ʼ������
	static const int BONUS_TIME		= 15000;		// �������ڵ�ʱ��
	static const int PLAYER_CHARGE	= 100;			// ����ڵ���ʼֵ
	static const int BONUS_CHARGE	= 20;			// �����ڵ���

	//������������������������������������������������������������������������������
	// �볡����صı���
	//������������������������������������������������������������������������������
	int scn_width, scn_height;						// ��ͼ�������
	int wnd_width, wnd_height;						// ��Ϸ���ڿ��
	//������������������������������������������������������������������������������
	// ����Ϸ�߼�������صı���
	//������������������������������������������������������������������������������
	int GameLevel;									// ��Ϸ����������
	int LifeCount;									// �������������
	bool isDelayCollision;							// �Ƿ���ʱ��ײ
	bool looseBlood;								// �Ƿ�ʧѪ״̬
	bool upgradeGameLevel;							// �Ƿ��Ѿ�����
	bool updateLifeCount;							// �Ƿ�������������

	long NpcLoadTime;								// NPC���ؼ�ʱ
	long BonusLoadTime;								// �������ؼ�ʱ
	int ChargeCount;								// ����ڵ�������
	int NpcNumber;									// NPC������������
	int NpcLoadNum;									// �Ѿ����ص�NPC������
	//������������������������������������������������������������������������������
	// ��������Դ��صı���
	//������������������������������������������������������������������������������
	AudioDX			dxSnd;							// DirectSound����
	AudioDXBuffer*	gameOverSound;					// ��Ϸ��������
	AudioDXBuffer*	gameWinSound;					// ��Ϸʤ������
	AudioDXBuffer*	menuMusic;						// �˵�����
	AudioDXBuffer*	bkgMusic;						// ��Ϸ��������
	AudioDXBuffer*  explosionSound;					// ��ը����
	AudioDXBuffer*	mouseDownSound;					// ��갴������
	AudioDXBuffer*	mouseOverSound;					// ����Ƶ���ť������
	
	AudioDXBuffer*  powerupSound;					// �õ�������Ʒ������
	//������������������������������������������������������������������������������
	// ��ͼƬ��Դ��صı���
	//������������������������������������������������������������������������������
	T_Graph* superPlayerImg;						// �޵�״̬��Ҷ���ͼƬ
	T_Graph* lifeImg;								// ״̬�����̹��СͼƬ

	T_Graph* npcNumImg;								// ״̬���з�̹��СͼƬ
	T_Graph* flagImg;								// ״̬����ҵ÷�СͼƬ
	T_Graph* bmbImg;								// ״̬���ڵ���СͼƬ
	//������������������������������������������������������������������������������
	// ����Ϸ��������صı���
	//������������������������������������������������������������������������������
	T_Sprite* player;								// ��Ϸ���
	T_Scene* t_scene;								// ��Ϸ��ͼ����
	T_Menu* gameMenu;								// ��Ϸ�˵�

	T_Sprite* nest;									// ��Ϸ�е��ϳ�
	//������������������������������������������������������������������������������
	// ����Ϸ��ɫ��صļ���
	//������������������������������������������������������������������������������
	vSpriteSet npc_set;								// NPC��ɫ����
	vSpriteSet explosion_set;						// ��ըЧ������

	vSpriteSet player_bomb_set;						// ����ڵ�����
	vSpriteSet npc_bomb_set;						// NPC�ڵ�����
	vSpriteSet bonus_bomb_set;						// �����ڵ�����
	vSpriteSet bonus_life_set;						// ������������
	
private: 
	//������������������������������������������������������������������������������
	// ����Դ������صĹ��ܺ��� 
	//������������������������������������������������������������������������������
	void LoadPlayer();								// ������Ϸ��ҽ�ɫ
	void LoadNpc(int total, int iTime);				// ������ϷNPC
	void LoadMap();									// ������Ϸ��ͼ����	
	void LoadImageRes();							// ������ϷͼƬ��Դ
	
	void LoadBonus();								// ���ؽ�������
	void LoadGameMenu(int type);					// ������Ϸ�˵�
	void LoadSound(HWND hwnd);						// ������Ϸ������Դ

	void LoadExplosion(int x, int y);				// ������Ϸ��ըЧ��
	void LoadBomb(									// �����ڵ�����
		T_Sprite* sp,								// �����ڵ��Ľ�ɫ
		vSpriteSet& bombSet,						// �ڵ�����
		int iTime=0);								// �����ڵ��ļ��ʱ��

	void LoadNest();								// ������Ϸ��Ӫ��־
	//������������������������������������������������������������������������������
	// ��Ϸ״̬������صĹ��ܺ��� 
	//������������������������������������������������������������������������������
	void UpdatePlayerPos();							// ������ҽ�ɫ��λ��
	void UpdatePlayerLife();						// ������ҵ�����ֵ
	void UpdateNpcPos();							// ����NPC��λ��
	void UpdateAnimation();							// ���½�ɫ����֡���к�

	void UpdateBombPos(vSpriteSet* bombSet);		// �����ڵ���λ��
	void UpdateBonus(vSpriteSet& bonusSet);			// ���½��������״̬
	//������������������������������������������������������������������������������
	// ��Ϸ�ؿ���صĹ��ܺ��� 
	//������������������������������������������������������������������������������
	void LoadGameLevel(int level);					// ���عؿ�����
	void ClearGameLevel();							// ����ؿ�����
	//������������������������������������������������������������������������������
	// ��Ϸϸ�ڴ�����
	//������������������������������������������������������������������������������
	bool UpgradePlayerLevel();						// �����������
	void PlayGameSound();							// ������Ϸ�еĸ�����������
	void DisplayInfo(HDC hdc, int game_state);		// ��Ϸ��Ϣ��ʾ
	void ReleaseDXBuffer(AudioDXBuffer* dxBuf);		// �ͷ���������
	void BombCollide(T_Sprite* bomb);				// �ڵ���ײ����
	//������������������������������������������������������������������������������
	// ���������� 
	//������������������������������������������������������������������������������
	void SetMenuPara(wstring* menuItems,			// Ҫ���õĲ˵���
					 int itemSize,					// �˵������
					 int m_w, int m_h,				// �˵�����
					 int posType);					// ��ť���ַ�ʽ
public:
	//������������������������������������������������������������������������������
	// ��Ĺ��캯������������
	//������������������������������������������������������������������������������ 
	TankGame(HINSTANCE h_instance, LPCTSTR sz_winclass, LPCTSTR sz_title, 
			 WORD icon = NULL, WORD sm_icon = NULL, 
			 int winwidth = 1024, int winheight = 768);
	virtual ~TankGame(void);
	
	//������������������������������������������������������������������������������
	// ����T_Engine���е��麯��ʵ����Ϸ����
	//������������������������������������������������������������������������������
	void GameInit();								// ��Ϸ��ʼ��	
	void GameLogic();								// ��Ϸ�߼�����
	void GameEnd();									// ��Ϸ��������
	void GamePaint(HDC hdc);						// ��Ϸ��ʾ
	void GameKeyAction(int Action=KEY_SYS_NONE);	// ������Ϊ����	
	void GameMouseAction(int x, int y, int Action); // �����Ϊ����	
};