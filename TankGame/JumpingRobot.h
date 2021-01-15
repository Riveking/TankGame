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

	int scn_width, scn_height;						// ��ͼ�������
	int wnd_width, wnd_height;						// ��Ϸ���ڿ��

	JRobot* player;									// ��Ϸ���
	T_Scene* t_scene;								// ��Ϸ��ͼ����
	T_Graph back;

public:
	CJumpingRobot(HINSTANCE h_instance, LPCTSTR sz_winclass, LPCTSTR sz_title,
		                        WORD icon = NULL, WORD sm_icon = NULL,
		                        int winwidth = 1024, int winheight = 768);
	virtual ~CJumpingRobot();

	void LoadPlayer();								// ������Ϸ��ҽ�ɫ
	void LoadMap();									// ������Ϸ��ͼ����	

	void UpdatePlayerPos();							// ������ҽ�ɫ��λ��
	void UpdateAnimation();							// ���½�ɫ����֡���к�
	//������������������������������������������������������������������������������
	// ����T_Engine���е��麯��ʵ����Ϸ����
	//������������������������������������������������������������������������������
	void GameInit();								// ��Ϸ��ʼ��	
	void GameLogic();								// ��Ϸ�߼�����
	void GameEnd();									// ��Ϸ��������
	void GamePaint(HDC hdc);						// ��Ϸ��ʾ
	void GameKeyAction(int Action = KEY_SYS_NONE);	// ������Ϊ����	
	void GameMouseAction(int x, int y, int Action); // �����Ϊ����	
};

