/*
���������Ŀ������־��Դ��Bվ��ţ��̣�

1.������Ŀ
2.�����ز�
3.������Ϸ����
	1)������Ϸ����
	2)�����Ϸ����
		a.���ر�����ͬ�ٶ�ͬʱ�ƶ�
		b.����ѭ������
	3)ʵ����Ϸ����
		a.���ر�����Դ
		b.��Ⱦ��Ϸ����
			���֪ʶ������ΪͼƬ���Ͻǵ��λ��
			�������⣺png��ʽͼƬ͸��������ʾΪ��ɫ		��������ʹ��tools
4.ʵ����ұ���
*/
#include<iostream>
#include<stdio.h>
#include<graphics.h>
#include<conio.h>
#include"tools.h"
using namespace std;

//��������
#define _CRT_SECURE_NO_DEPRECATE
#define WIN_WIDTH 1012
#define WIN_HEIGHT 396

//����ͼƬ
IMAGE imgBgs[3];
//����ͼƬx����
int bgX[3];
//����ͼƬ�����ٶ�
int bgSpeed[3] = { 1,2,4 };

//��ɫͼƬ
IMAGE imgHeros[12];
int heroX;//��ɫ��X����
int heroY;//��ɫ��Y����
int heroIndex;//��ɫ����֡���
bool heroJump;//��ɫ��Ծ״̬
int jumpHeightMax;
int jumpHeightOff;

//��Ϸ�ĳ�ʼ��
void init()
{
	//������Ϸ����
	initgraph(WIN_WIDTH, WIN_HEIGHT);
	//���ر����ز�
	char name[64];
	for (int i = 0; i < 3; i++)
	{	
		// "res/bg001.png"		"res/bg002.png"		"res/bg003.png"
		sprintf_s(name,"res/bg%03d.png", i + 1);
		//�޸��ַ���Ϊʹ�ö��ֽ��ַ���
		loadimage(&imgBgs[i],name);                                    //��
		
		bgX[i] = 0;
	}

	//���ؽ�ɫ�����ز�
	for (int i = 0; i < 12; i++)
	{
		//"res/hero1.png"	����	"res/bg12.png"
		sprintf_s(name, "res/hero%d.png", i + 1);
		loadimage(&imgHeros[i], name);
	}

	//������ҵĳ�ʼλ��
	heroX = WIN_WIDTH * 0.5 - imgHeros[0].getwidth() * 0.5;
	heroY = 345 - imgHeros[0].getheight();
	heroIndex = 0;
	heroJump = 0;
	jumpHeightMax = 345 - imgHeros[0].getheight() - 120;
	jumpHeightOff = -13;
}

//�زĵ�ѭ������
void circulate()
{
	//����ѭ��
	for (int i = 0; i < 3; i++)
	{
		bgX[i] -= bgSpeed[i];
		if (bgX[i]<-WIN_WIDTH)
		{
			bgX[i] = 0;
		}
	}

	//��ɫ��Ծ
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
		//��ɫѭ��
		heroIndex = (heroIndex + 1) % 12;
	}
}

//��Ⱦ��Ϸ����
void updateBg()
{
	putimagePNG2(bgX[0], 0, &imgBgs[0]);
	putimagePNG2(bgX[1],119, & imgBgs[1]);
	putimagePNG2(bgX[2],330, & imgBgs[2]);
}

//��ɫ��Ծ
void jump()
{
	heroJump = 1;
}

//�����û���������
void keyEvent()
{
	char ch;
	if (_kbhit())	//����а������룬_kbhit����ֵΪtrue
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