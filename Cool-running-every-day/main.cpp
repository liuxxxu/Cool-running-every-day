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
*/
#include<iostream>
#include<stdio.h>
#include<graphics.h>
#include"tools.h"
using namespace std;

//定义宏变量
#define _CRT_SECURE_NO_DEPRECATE
#define WIN_WIDTH 1012
#define WIN_HEIGHT 396

//创建背景相关全局变量
IMAGE imgBgs[3];//背景图片
int bgX[3];//背景图片x坐标
int bgSpeed[3] = { 1,2,4 };//背景图片滚动速度

//游戏的初始化
void init()
{
	//创建游戏窗口
	initgraph(WIN_WIDTH, WIN_HEIGHT);
	//加载背景资源
	char name[64];
	for (int i = 0; i < 3; i++)
	{	
		// "res/bg001.png"		"res/bg002.png"		"res/bg003.png"
		sprintf_s(name,"res/bg%03d.png", i + 1);
		//修改字符集为使用多字节字符集
		loadimage(&imgBgs[i],name);                                    //？
		
		bgX[i] = 0;
	}
}

//背景滚动
void fly()
{
	for (int i = 0; i < 3; i++)
	{
		bgX[i] -= bgSpeed[i];
		if (bgX[i]<-WIN_WIDTH)
		{
			bgX[i] = 0;
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


int main()
{
	init();
	while (1)
	{
		BeginBatchDraw();
		updateBg();
		EndBatchDraw();
		fly();
		Sleep(30);
	}
	
	system("pause");
	return 0;
}