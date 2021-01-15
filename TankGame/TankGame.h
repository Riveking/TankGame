//*******************************************************************
// 利用TinyEngine引擎编写坦克大战游戏  
// 作者: 万立中(WanLizhong)
// 博客: www.wanlizhong.com 
// 日期: 2013-08-02
// 版权所有 2007-2013 万立中
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

// 定义vector容器类型的数据类型vSpriteSet
typedef vector<T_Sprite*> vSpriteSet;

class TankGame:public T_Engine
{
private:
	//………………………………………………………………………………………………………
	// 定义动画帧序列（与静态动画帧图片一致）
	//………………………………………………………………………………………………………
	static int TANK_FRAME_LEFT[4];					// 坦克左方向的帧动画
	static int TANK_FRAME_RIGHT[4];					// 坦克右方向的帧动画
	static int TANK_FRAME_UP[4];					// 坦克上方向的帧动画
	static int TANK_FRAME_DOWN[4];					// 坦克下方向的帧动画
	static int EXPLOSION_FRAME[8];					// 爆炸帧动画
	static int BONUS_FRAME[3];						// 奖励物品闪动帧动画
	//………………………………………………………………………………………………………
	// 游戏中的关键数量
	//………………………………………………………………………………………………………
	static const int TOTAL_LEVEL	= 5;			// 游戏总关数
	static const int NPC_NUM		= 20;			// 坦克总数
	static const int NPC_LOAD_TIME	= 1000;			// NPC加载间隔时间
	static const int SUPER_TIME		= 5000;			// 玩家角色无敌时间
	static const int PLAYER_LIFE	= 5;			// 玩家初始生命数
	static const int BONUS_TIME		= 15000;		// 奖励存在的时间
	static const int PLAYER_CHARGE	= 100;			// 玩家炮弹初始值
	static const int BONUS_CHARGE	= 20;			// 奖励炮弹数

	//………………………………………………………………………………………………………
	// 与场景相关的变量
	//………………………………………………………………………………………………………
	int scn_width, scn_height;						// 地图场景宽高
	int wnd_width, wnd_height;						// 游戏窗口宽高
	//………………………………………………………………………………………………………
	// 与游戏逻辑控制相关的变量
	//………………………………………………………………………………………………………
	int GameLevel;									// 游戏关数计数器
	int LifeCount;									// 玩家生命计数器
	bool isDelayCollision;							// 是否延时碰撞
	bool looseBlood;								// 是否失血状态
	bool upgradeGameLevel;							// 是否已经升级
	bool updateLifeCount;							// 是否更新玩家生命数

	long NpcLoadTime;								// NPC加载计时
	long BonusLoadTime;								// 奖励加载计时
	int ChargeCount;								// 玩家炮弹计数器
	int NpcNumber;									// NPC消灭数计数器
	int NpcLoadNum;									// 已经加载的NPC计数器
	//………………………………………………………………………………………………………
	// 与声音资源相关的变量
	//………………………………………………………………………………………………………
	AudioDX			dxSnd;							// DirectSound对象
	AudioDXBuffer*	gameOverSound;					// 游戏结束声音
	AudioDXBuffer*	gameWinSound;					// 游戏胜利声音
	AudioDXBuffer*	menuMusic;						// 菜单声音
	AudioDXBuffer*	bkgMusic;						// 游戏背景音乐
	AudioDXBuffer*  explosionSound;					// 爆炸声音
	AudioDXBuffer*	mouseDownSound;					// 鼠标按下声音
	AudioDXBuffer*	mouseOverSound;					// 鼠标移到按钮的声音
	
	AudioDXBuffer*  powerupSound;					// 得到奖励物品的声音
	//………………………………………………………………………………………………………
	// 与图片资源相关的变量
	//………………………………………………………………………………………………………
	T_Graph* superPlayerImg;						// 无敌状态玩家动画图片
	T_Graph* lifeImg;								// 状态栏玩家坦克小图片

	T_Graph* npcNumImg;								// 状态栏敌方坦克小图片
	T_Graph* flagImg;								// 状态栏玩家得分小图片
	T_Graph* bmbImg;								// 状态栏炮弹数小图片
	//………………………………………………………………………………………………………
	// 与游戏对象类相关的变量
	//………………………………………………………………………………………………………
	T_Sprite* player;								// 游戏玩家
	T_Scene* t_scene;								// 游戏地图场景
	T_Menu* gameMenu;								// 游戏菜单

	T_Sprite* nest;									// 游戏中的老巢
	//………………………………………………………………………………………………………
	// 与游戏角色相关的集合
	//………………………………………………………………………………………………………
	vSpriteSet npc_set;								// NPC角色集合
	vSpriteSet explosion_set;						// 爆炸效果集合

	vSpriteSet player_bomb_set;						// 玩家炮弹集合
	vSpriteSet npc_bomb_set;						// NPC炮弹集合
	vSpriteSet bonus_bomb_set;						// 奖励炮弹集合
	vSpriteSet bonus_life_set;						// 奖励生命集合
	
private: 
	//………………………………………………………………………………………………………
	// 与资源加载相关的功能函数 
	//………………………………………………………………………………………………………
	void LoadPlayer();								// 加载游戏玩家角色
	void LoadNpc(int total, int iTime);				// 加载游戏NPC
	void LoadMap();									// 加载游戏地图场景	
	void LoadImageRes();							// 加载游戏图片资源
	
	void LoadBonus();								// 加载奖励对象
	void LoadGameMenu(int type);					// 加载游戏菜单
	void LoadSound(HWND hwnd);						// 加载游戏声音资源

	void LoadExplosion(int x, int y);				// 加载游戏爆炸效果
	void LoadBomb(									// 发射炮弹处理
		T_Sprite* sp,								// 发射炮弹的角色
		vSpriteSet& bombSet,						// 炮弹集合
		int iTime=0);								// 发射炮弹的间隔时间

	void LoadNest();								// 加载游戏大本营标志
	//………………………………………………………………………………………………………
	// 游戏状态更新相关的功能函数 
	//………………………………………………………………………………………………………
	void UpdatePlayerPos();							// 更新玩家角色的位置
	void UpdatePlayerLife();						// 更新玩家的生命值
	void UpdateNpcPos();							// 更新NPC的位置
	void UpdateAnimation();							// 更新角色动画帧序列号

	void UpdateBombPos(vSpriteSet* bombSet);		// 更新炮弹的位置
	void UpdateBonus(vSpriteSet& bonusSet);			// 更新奖励对象的状态
	//………………………………………………………………………………………………………
	// 游戏关卡相关的功能函数 
	//………………………………………………………………………………………………………
	void LoadGameLevel(int level);					// 加载关卡数据
	void ClearGameLevel();							// 清除关卡数据
	//………………………………………………………………………………………………………
	// 游戏细节处理函数
	//………………………………………………………………………………………………………
	bool UpgradePlayerLevel();						// 玩家升级处理
	void PlayGameSound();							// 管理游戏中的各种声音播放
	void DisplayInfo(HDC hdc, int game_state);		// 游戏信息显示
	void ReleaseDXBuffer(AudioDXBuffer* dxBuf);		// 释放声音缓冲
	void BombCollide(T_Sprite* bomb);				// 炮弹碰撞处理
	//………………………………………………………………………………………………………
	// 其它处理函数 
	//………………………………………………………………………………………………………
	void SetMenuPara(wstring* menuItems,			// 要设置的菜单项
					 int itemSize,					// 菜单项个数
					 int m_w, int m_h,				// 菜单项宽高
					 int posType);					// 按钮布局方式
public:
	//………………………………………………………………………………………………………
	// 类的构造函数与析构函数
	//……………………………………………………………………………………………………… 
	TankGame(HINSTANCE h_instance, LPCTSTR sz_winclass, LPCTSTR sz_title, 
			 WORD icon = NULL, WORD sm_icon = NULL, 
			 int winwidth = 1024, int winheight = 768);
	virtual ~TankGame(void);
	
	//………………………………………………………………………………………………………
	// 重载T_Engine类中的虚函数实现游戏功能
	//………………………………………………………………………………………………………
	void GameInit();								// 游戏初始化	
	void GameLogic();								// 游戏逻辑处理
	void GameEnd();									// 游戏结束处理
	void GamePaint(HDC hdc);						// 游戏显示
	void GameKeyAction(int Action=KEY_SYS_NONE);	// 按键行为处理	
	void GameMouseAction(int x, int y, int Action); // 鼠标行为处理	
};