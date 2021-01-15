//*******************************************************************
// ����TinyEngine�����д̹�˴�ս��Ϸ  
// ����: ������(WanLizhong)
// ����: www.wanlizhong.com 
// ����: 2013-08-02
// ��Ȩ���� 2007-2013 ������
// (C) 2007-2013 WanLizhong All Rights Reserved
//*******************************************************************

#include "TankGame.h"
#include "resource.h"

// WinMain����
//
int WINAPI WinMain(HINSTANCE h_instance, HINSTANCE hPrevInstance, 
				   PSTR szCmdLine, int iCmdShow)
{
	LPCTSTR WinTitle = L"��̹�˴�ս(�汾1.0.0) (C)2013 WanLizhong";
	TankGame* mygame = new TankGame(h_instance,WIN_CLASS,WinTitle, 
		                            IDI_TANKNEW, IDI_TANKNEW, WIN_WIDTH, WIN_HEIGHT);
	T_Engine::pEngine = mygame;
	mygame->StartEngine();

	return TRUE;
}
