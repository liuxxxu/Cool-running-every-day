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
4.实现玩家奔跑
5.实现玩家跳跃
6.实现随机出现乌龟
7.创建障碍物结构体数据类型
8.使用障碍物结构体重新初始化
9.封装多个障碍物的显示
10.实现玩家下蹲
11.实现柱子障碍物
12.优化障碍物的出现频率
13.实现碰撞检测
14.实现音效和背景音乐
15.实现血条
16.实现结束判定
17.实现重新开始和界面
18.实现开始界面
19.优化必碰障碍
20.实现分数
*/

#include<stdio.h>
#include<iostream>
#include<graphics.h>
#include<conio.h>
#include<ctime>
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

int jumpHeightMax;//跳跃最大高度
int jumpHeightOff;//跳跃偏移量

int heroBlood;//血量
int score;//分数

//优化帧等待
int timer;//时间统计
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
	Hook1,
	Hook2,
	Hook3,
	Hook4,
	OBSTACLE_TYPE_COUNT
}obstacle_type;

vector<vector<IMAGE>>obstacleImgs;	//所有障碍物图片

typedef struct obstacle
{
	int type;//障碍物类型
	int imgIndex;//当前图片序号
	int X, Y;//障碍物坐标
	int speed;//障碍物速度
	int power;//障碍物伤害
	bool exist;//障碍物存在状态
	bool hited;//是否已经碰撞
	bool passed;//是否已被通过
}obstacle_t;

obstacle_t obstacles[OBSTACLE_COUNT];
int lastObstacleIndex;

IMAGE imgHeroDown[2];
bool heroDown;//角色下蹲状态

//游戏的初始化
void init()
{
	//创建游戏窗口
	initgraph(WIN_WIDTH, WIN_HEIGHT, EW_SHOWCONSOLE);
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

	heroBlood = 100;
	score = 0;

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

	//加载角色下蹲素材
	loadimage(&imgHeroDown[0], "res/d1.png");
	loadimage(&imgHeroDown[1], "res/d2.png");
	heroDown = 0;

	//加载柱子素材
	IMAGE imgH;
	for (int i = 0; i < 4; i++)
	{
		//"res/h1.png"	……	"res/h4.png"
		vector<IMAGE> imgHArray;
		sprintf_s(name, "res/h%d.png", i + 1);
		loadimage(&imgH, name,63,260,true);
		imgHArray.push_back(imgH);
		obstacleImgs.push_back(imgHArray); 
	}

	lastObstacleIndex = -1;
	
	//预加载音频
	preLoadSound("res/hit.mp3");

	//背景音乐
	mciSendString("play res/bg.mp3 repeat", 0, 0, 0);
}

void createObstacle()
{
	int i;
	for (i = 0; i < OBSTACLE_COUNT; i++)
	{
		if (obstacles[i].exist==0)
		{
			break;
		}
	}
	if (i>= OBSTACLE_COUNT)
	{
		return;
	}

	obstacles[i].exist = 1;
	obstacles[i].hited = 0;
	obstacles[i].imgIndex = 0;
	//obstacles[i].type = (obstacle_type)(rand() % OBSTACLE_TYPE_COUNT);
	obstacles[i].type = (obstacle_type)(rand() % 3);
	if (lastObstacleIndex >= 0 &&
			obstacles[lastObstacleIndex].type >= Hook1 &&
			obstacles[lastObstacleIndex].type <= Hook4 &&
			obstacles[i].type == LION &&
			obstacles[lastObstacleIndex].X > (WIN_WIDTH-500))
	{
		obstacles[i].type = TOR;
	}
	lastObstacleIndex = i;
	if (obstacles[i].type == Hook1)
	{
		obstacles[i].type += rand() % 4;
	}
	obstacles[i].X = WIN_WIDTH;
	obstacles[i].Y = 350 - obstacleImgs[obstacles[i].type][0].getheight();
	if (obstacles[i].type == TOR)
	{
		obstacles[i].speed = 0;
		obstacles[i].power = 10;
	}
	else if (obstacles[i].type == LION)
	{
		obstacles[i].speed = 8;
		obstacles[i].power = 15;
	}
	else if (obstacles[i].type >= Hook1 && obstacles[i].type <= Hook4)
	{
		obstacles[i].speed = 0;
		obstacles[i].power = 5;
		obstacles[i].Y = 0;
	}
	obstacles[i].passed = 0;
}

//碰撞检测
void checkHit()
{
	for (int i = 0; i < OBSTACLE_COUNT; i++)
	{
		if (obstacles[i].exist && !obstacles[i].hited)
		{
			int a1x, a1y, a2x, a2y;
			int off = 30;
			if (!heroDown)
			{
				a1x = heroX + off;
				a1y = heroY + off;
				a2x = heroX + imgHeros[heroIndex].getwidth() - off;
				a2y = heroY + imgHeros[heroIndex].getheight();
			}
			else
			{
				a1x = heroX + off;
				a1y = 345 - imgHeroDown[heroIndex].getheight();
				a2x = heroX + imgHeroDown[heroIndex].getwidth() - off;
				a2y = 345;
			}

			IMAGE img = obstacleImgs[obstacles[i].type][obstacles[i].imgIndex];
			int b1x = obstacles[i].X + off;
			int b1y = obstacles[i].Y + off;
			int b2x = obstacles[i].X + img.getwidth() - off;
			int b2y = obstacles[i].Y + img.getheight()- 10;

			if (rectIntersect(a1x,a1y,a2x,a2y,b1x,b1y,b2x,b2y))
			{
				heroBlood -= obstacles[i].power;
				cout << "剩余血量:" << heroBlood << endl;
				playSound("res/hit.mp3");
				obstacles[i].hited = 1;
			}
		}
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
	//角色下蹲
	else if (heroDown)
	{
		static int count = 0;
		int delay[2] = {2,20};
		count++;
		if (count > delay[heroIndex])
		{
			count = 0;
			heroIndex++;
			if (heroIndex >= 2)
			{
				heroIndex = 0;
				heroDown = 0;
			}
		}
	}
	//角色奔跑
	else
	{
		heroIndex = (heroIndex + 1) % 12;
	}

	//创建障碍物 
	//torIndex = (torIndex + 1) % 7;
	static int frameCount = 0;
	static int enemyFre = 50;
	frameCount++;
	if (frameCount > enemyFre)
	{
		frameCount = 0;
		enemyFre = 50 + rand() % 100;
		createObstacle();
		/*if (!torExist)
		{
			torExist = 1;
			torX = WIN_WIDTH;
			torFre = 50 + rand() % 200; 
		}*/
	}
	/*if (torExist)
	{
		torX -= 8;
		if (torX < -imgTors->getwidth())
		{
			torExist = 0;
		}
	}*/

	//更新障碍物坐标
	for (int i = 0; i < OBSTACLE_COUNT; i++)
	{
		if (obstacles[i].exist)
		{
			obstacles[i].X -= obstacles[i].speed + 6;
			if (obstacles[i].X < -obstacleImgs[obstacles[i].type][0].getwidth())
			{
				obstacles[i].exist = 0;
			}
			int len = obstacleImgs[obstacles[i].type].size();
			obstacles[i].imgIndex = (obstacles[i].imgIndex + 1) % len;
		}

	}
	
	//角色和障碍物碰撞检测
	checkHit();
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

//角色下蹲
void down()
{
	heroDown = 1;
	update = 1;
	heroIndex = 0;
}

//处理用户按键输入
void keyEvent()
{
	char ch;
	if (_kbhit())	//如果有按键输入，_kbhit返回值为true
	{
		ch = _getch();
		if (ch=='j')
		{
			jump(); 
		}
		else if (ch == 'd')
		{
			down();
		}
	}
}

//渲染乌龟
void updateEnemy()
{
	/*if (torExist)
	{
		putimagePNG2(torX,torY,WIN_WIDTH,&imgTors[torIndex]);
	}*/
	for (int i = 0; i < OBSTACLE_COUNT; i++)
	{
		if (obstacles[i].exist)
		{
			putimagePNG2(obstacles[i].X, obstacles[i].Y, WIN_WIDTH,
				&obstacleImgs[obstacles[i].type][obstacles[i].imgIndex]);
		}
	}
}

//渲染角色
void updateHero()
{
	if (!heroDown)
	{
		putimagePNG2(heroX, heroY, &imgHeros[heroIndex]);
	}
	else
	{
		int Y = 345 - imgHeroDown[heroIndex].getheight();
		putimagePNG2(heroX, Y, &imgHeroDown[heroIndex]);
	}
}

//渲染血条
void updateBloodBar()
{
	drawBloodBar(10, 10, 200, 10, 2, BLUE, DARKGRAY, RED, heroBlood / 100.0);
}

void checkOver()
{
	if (heroBlood <= 0)
	{
		loadimage(0, "res/over.png");
		FlushBatchDraw();
		mciSendString("stop res/bg.mp3", 0, 0, 0);
		system("pause");
		heroBlood = 100;
		score = 0;
		mciSendString("play res/bg.mp3 repeat", 0, 0, 0);
	}
}

//检查分数
void checkScore()
{
	for (int i = 0; i < OBSTACLE_COUNT; i++)
	{
		if (obstacles[i].exist &&
			obstacles[i].passed == 0 &&
			obstacles[i].hited == 0 &&
			obstacles[i].X + obstacleImgs[obstacles[i].type][0].getwidth() < heroX)
		{
			score++;
			obstacles[i].passed = 1;
			cout << "score:" << score << endl;
		}
	}
}

int main()
{
	init();
	loadimage(0, "res/start.png");
	system("pause");
	srand((unsigned int)(time(NULL)));
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
			//putimagePNG2(heroX, heroY, &imgHeros[heroIndex]);
			updateHero();
			updateEnemy();
			updateBloodBar();
			EndBatchDraw();
			checkOver();
			checkScore();
			circulate();
		}
		//Sleep(30);
	}
	
	system("pause");
	return 0;
}