//*******************************************************************
// 利用TinyEngine引擎编写坦克大战游戏  
// 作者: 万立中(WanLizhong)
// 博客: www.wanlizhong.com 
// 日期: 2013-08-02
// 版权所有 2007-2013 万立中
// (C) 2007-2013 WanLizhong All Rights Reserved
//*******************************************************************

#include "TankGame.h"
#include "resource.h"

// WinMain函数
//
int WINAPI WinMain(HINSTANCE h_instance, HINSTANCE hPrevInstance, 
				   PSTR szCmdLine, int iCmdShow)
{
	LPCTSTR WinTitle = L"新坦克大战(版本1.0.0) (C)2013 WanLizhong";
	TankGame* mygame = new TankGame(h_instance,WIN_CLASS,WinTitle, 
		                            IDI_TANKNEW, IDI_TANKNEW, WIN_WIDTH, WIN_HEIGHT);
	T_Engine::pEngine = mygame;
	mygame->StartEngine();

	return TRUE;
}
