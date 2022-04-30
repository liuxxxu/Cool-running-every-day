/*
天天酷跑项目开发日志（源自B站奇牛编程）

1.创建项目
2.导入素材
3.创建游戏界面
	1)创建游戏窗口
	2)设计游戏背景
		a.三重背景不同速度同时移动
		b.背景循环滚动
	3)实现游戏背景
		a.加载背景资源
		b.渲染游戏背景
			相关知识：坐标为图片左上角点的位置
			遇到问题：png格式图片透明部分显示为黑色		解决：添加使用tools
4.实现玩家奔跑
*/

#include<stdio.h>
#include<graphics.h>
#include<conio.h>
#include<vector>
#include"tools.h"
using namespace std;

//定义宏变量
#define WIN_WIDTH 1012
#define WIN_HEIGHT 396
#define OBSTACLE_COUNT 10

//背景图片
IMAGE imgBgs[3];
//背景图片x坐标
int bgX[3];
//背景图片滚动速度
int bgSpeed[3] = { 1,3,6 };

//角色图片
IMAGE imgHeros[12];
int heroX;//角色的X坐标
int heroY;//角色的Y坐标
int heroIndex;//角色动画帧序号
bool heroJump;//角色跳跃状态
int jumpHeightMax;
int jumpHeightOff;

//优化帧等待
int timer = 0;
bool update;//表示是否马上需要刷新画面

/*
//障碍物图片
IMAGE imgTors[7];//乌龟
int torX;//乌龟的X坐标
int torY;//乌龟的Y坐标
bool torExist;//当前窗口乌龟存在状态
int torIndex;//乌龟动画帧序号
*/

typedef enum
{
	TOR,//乌龟 0
	LION,//狮子 1
	OBSTACLE_TYPE_COUNT//2
}obstacle_type;

vector<vector<IMAGE>>obstacleImgs;	//相当于IMAGE obstacleImgs[3][12];	障碍物图片

typedef struct obstacle
{
	obstacle_type type;//障碍物类型
	int imgIndex;//当前图片序号
	int X, Y;//障碍物坐标
	int speed;//障碍物速度
	int power;//障碍物伤害
	bool exist;//障碍物存在状态

}obstacle_t;

obstacle_t obstacles[OBSTACLE_COUNT];

//游戏的初始化
void init()
{
	//创建游戏窗口
	initgraph(WIN_WIDTH, WIN_HEIGHT);
	//加载背景素材
	char name[64];
	for (int i = 0; i < 3; i++)
	{	
		// "res/bg001.png"		"res/bg002.png"		"res/bg003.png"
		sprintf_s(name,"res/bg%03d.png", i + 1);
		//修改字符集为使用多字节字符集
		loadimage(&imgBgs[i],name);
		
		bgX[i] = 0;
	}

	//加载角色奔跑素材
	for (int i = 0; i < 12; i++)
	{
		//"res/hero1.png"	……	"res/hero12.png"
		sprintf_s(name, "res/hero%d.png", i + 1);
		loadimage(&imgHeros[i], name);
	}

	//设置玩家的初始位置
	heroX = WIN_WIDTH * 0.3 - imgHeros[0].getwidth() * 0.5;
	heroY = 345 - imgHeros[0].getheight();
	heroIndex = 0;
	heroJump = 0;
	jumpHeightMax = 345 - imgHeros[0].getheight() - 120;
	jumpHeightOff = -12;

	timer = 0;
	update = 1;

	/*
	//加载乌龟素材
	for (int i = 0; i < 7; i++)
	{
		//"res/t1.png"	……	"res/t7.png"
		sprintf_s(name, "res/t%d.png", i + 1);
		loadimage(&imgTors[i], name);
	}
	torExist = 0;
	torY = 350 - imgTors->getheight();
	*/

	//加载乌龟素材
	IMAGE imgTor;
	vector<IMAGE> imgTorArray;
	for (int i = 0; i < 7; i++)
	{
		//"res/t1.png"	……	"res/t7.png"
		sprintf_s(name, "res/t%d.png", i + 1);
		loadimage(&imgTor, name);
		imgTorArray.push_back(imgTor);
	}
	obstacleImgs.push_back(imgTorArray);

	//加载狮子素材
	IMAGE imgLion;
	vector<IMAGE> imgLionArray;
	for (int i = 0; i < 6; i++)
	{
		//"res/p1.png"	……	"res/p6.png"
		sprintf_s(name, "res/p%d.png", i + 1);
		loadimage(&imgLion, name);
		imgLionArray.push_back(imgLion);
	}
	obstacleImgs.push_back(imgLionArray);

	//初始化障碍物池
	for (int i = 0; i < OBSTACLE_COUNT; i++)
	{
		obstacles[i].exist = 0;
	}
}

//素材的循环滚动
void circulate()
{
	//背景循环
	for (int i = 0; i < 3; i++)
	{
		bgX[i] -= bgSpeed[i];
		if (bgX[i] < -WIN_WIDTH)
		{
			bgX[i] = 0;
		}
	}

	//角色跳跃
	if (heroJump)
	{
		if (heroY < jumpHeightMax)
		{
			jumpHeightOff = 12;
		}
		heroY += jumpHeightOff;
		if (heroY >  345 - imgHeros[0].getheight())
		{
			heroJump = 0;
			jumpHeightOff = -12;
		}
	}
	else
	{
		//奔跑状态角色循环
		heroIndex = (heroIndex + 1) % 12;
	}

	//创建乌龟 
	torIndex = (torIndex + 1) % 7;
	static int frameCount = 0;
	static int torFre = 50;
	frameCount++;
	if (frameCount > torFre)
	{
		frameCount = 0;
		if (!torExist)
		{
			torExist = 1;
			torX = WIN_WIDTH;
			torFre = 50 + rand() % 200; 
		}
	}
	if (torExist)
	{
		torX -= 8;
		if (torX < -imgTors->getwidth())
		{
			torExist = 0;
		}
	}
	
}

//渲染游戏背景
void updateBg()
{
	putimagePNG2(bgX[0], 0, &imgBgs[0]);
	putimagePNG2(bgX[1],119, & imgBgs[1]);
	putimagePNG2(bgX[2],330, & imgBgs[2]);
}

//角色跳跃
void jump()
{
	heroJump = 1;
	update = 1;
}

//处理用户按键输入
void keyEvent()
{
	char ch;
	if (_kbhit())	//如果有按键输入，_kbhit返回值为true
	{
		ch = _getch();
		if (ch==' ')
		{
			jump(); 
		}
	}
}

//渲染乌龟
void updateEnemy()
{
	if (torExist)
	{
		putimagePNG2(torX,torY,WIN_WIDTH,&imgTors[torIndex]);
	}
}

int main()
{
	init();
	while (1)
	{
		keyEvent();
		timer += getDelay();
		if (timer>30)
		{
			timer = 0;
			update = 1;
		}
		if (update)
		{
			update = 0;
			BeginBatchDraw();
			updateBg();
			putimagePNG2(heroX, heroY, &imgHeros[heroIndex]);
			updateEnemy();
			EndBatchDraw();
			circulate();
		}
		//Sleep(30);
	}
	
	system("pause");
	return 0;
}