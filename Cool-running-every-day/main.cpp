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
*/
#include<iostream>
#include<stdio.h>
#include<graphics.h>
#include"tools.h"
using namespace std;

//��������
#define _CRT_SECURE_NO_DEPRECATE
#define WIN_WIDTH 1012
#define WIN_HEIGHT 396

//�����������ȫ�ֱ���
IMAGE imgBgs[3];//����ͼƬ
int bgX[3];//����ͼƬx����
int bgSpeed[3] = { 1,2,4 };//����ͼƬ�����ٶ�

//��Ϸ�ĳ�ʼ��
void init()
{
	//������Ϸ����
	initgraph(WIN_WIDTH, WIN_HEIGHT);
	//���ر�����Դ
	char name[64];
	for (int i = 0; i < 3; i++)
	{	
		// "res/bg001.png"		"res/bg002.png"		"res/bg003.png"
		sprintf_s(name,"res/bg%03d.png", i + 1);
		//�޸��ַ���Ϊʹ�ö��ֽ��ַ���
		loadimage(&imgBgs[i],name);                                    //��
		
		bgX[i] = 0;
	}
}

//��������
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

//��Ⱦ��Ϸ����
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