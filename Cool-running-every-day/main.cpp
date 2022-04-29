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
#include<iostream>
#include<stdio.h>
#include<graphics.h>
#include<conio.h>
#include"tools.h"
using namespace std;

//定义宏变量
#define _CRT_SECURE_NO_DEPRECATE
#define WIN_WIDTH 1012
#define WIN_HEIGHT 396

//背景图片
IMAGE imgBgs[3];
//背景图片x坐标
int bgX[3];
//背景图片滚动速度
int bgSpeed[3] = { 1,2,4 };

//角色图片
IMAGE imgHeros[12];
int heroX;//角色的X坐标
int heroY;//角色的Y坐标
int heroIndex;//角色动画帧序号
bool heroJump;//角色跳跃状态
int jumpHeightMax;
int jumpHeightOff;

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
		loadimage(&imgBgs[i],name);                                    //？
		
		bgX[i] = 0;
	}

	//加载角色奔跑素材
	for (int i = 0; i < 12; i++)
	{
		//"res/hero1.png"	……	"res/bg12.png"
		sprintf_s(name, "res/hero%d.png", i + 1);
		loadimage(&imgHeros[i], name);
	}

	//设置玩家的初始位置
	heroX = WIN_WIDTH * 0.5 - imgHeros[0].getwidth() * 0.5;
	heroY = 345 - imgHeros[0].getheight();
	heroIndex = 0;
	heroJump = 0;
	jumpHeightMax = 345 - imgHeros[0].getheight() - 120;
	jumpHeightOff = -13;
}

//素材的循环滚动
void circulate()
{
	//背景循环
	for (int i = 0; i < 3; i++)
	{
		bgX[i] -= bgSpeed[i];
		if (bgX[i]<-WIN_WIDTH)
		{
			bgX[i] = 0;
		}
	}

	//角色跳跃
	if (heroJump)
	{
		if (heroY < jumpHeightMax)
		{
			jumpHeightOff = 13;
		}
		heroY += jumpHeightOff;
		if (heroY >  345 - imgHeros[0].getheight())
		{
			heroJump = 0;
			jumpHeightOff = -13;
		}
	}
	else
	{
		//角色循环
		heroIndex = (heroIndex + 1) % 12;
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

int main()
{
	init();
	while (1)
	{
		keyEvent();
		BeginBatchDraw();
		updateBg();
		putimagePNG2(heroX,heroY, &imgHeros[heroIndex]);
		EndBatchDraw();
		circulate();
		Sleep(30);
	}
	
	system("pause");
	return 0;
}