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

#include<stdio.h>
#include<graphics.h>
#include<conio.h>
#include<vector>
#include"tools.h"
using namespace std;

//��������
#define WIN_WIDTH 1012
#define WIN_HEIGHT 396
#define OBSTACLE_COUNT 10

//����ͼƬ
IMAGE imgBgs[3];
//����ͼƬx����
int bgX[3];
//����ͼƬ�����ٶ�
int bgSpeed[3] = { 1,3,6 };

//��ɫͼƬ
IMAGE imgHeros[12];
int heroX;//��ɫ��X����
int heroY;//��ɫ��Y����
int heroIndex;//��ɫ����֡���
bool heroJump;//��ɫ��Ծ״̬
int jumpHeightMax;
int jumpHeightOff;

//�Ż�֡�ȴ�
int timer = 0;
bool update;//��ʾ�Ƿ�������Ҫˢ�»���

/*
//�ϰ���ͼƬ
IMAGE imgTors[7];//�ڹ�
int torX;//�ڹ��X����
int torY;//�ڹ��Y����
bool torExist;//��ǰ�����ڹ����״̬
int torIndex;//�ڹ궯��֡���
*/

typedef enum
{
	TOR,//�ڹ� 0
	LION,//ʨ�� 1
	OBSTACLE_TYPE_COUNT//2
}obstacle_type;

vector<vector<IMAGE>>obstacleImgs;	//�൱��IMAGE obstacleImgs[3][12];	�ϰ���ͼƬ

typedef struct obstacle
{
	obstacle_type type;//�ϰ�������
	int imgIndex;//��ǰͼƬ���
	int X, Y;//�ϰ�������
	int speed;//�ϰ����ٶ�
	int power;//�ϰ����˺�
	bool exist;//�ϰ������״̬

}obstacle_t;

obstacle_t obstacles[OBSTACLE_COUNT];

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
		loadimage(&imgBgs[i],name);
		
		bgX[i] = 0;
	}

	//���ؽ�ɫ�����ز�
	for (int i = 0; i < 12; i++)
	{
		//"res/hero1.png"	����	"res/hero12.png"
		sprintf_s(name, "res/hero%d.png", i + 1);
		loadimage(&imgHeros[i], name);
	}

	//������ҵĳ�ʼλ��
	heroX = WIN_WIDTH * 0.3 - imgHeros[0].getwidth() * 0.5;
	heroY = 345 - imgHeros[0].getheight();
	heroIndex = 0;
	heroJump = 0;
	jumpHeightMax = 345 - imgHeros[0].getheight() - 120;
	jumpHeightOff = -12;

	timer = 0;
	update = 1;

	/*
	//�����ڹ��ز�
	for (int i = 0; i < 7; i++)
	{
		//"res/t1.png"	����	"res/t7.png"
		sprintf_s(name, "res/t%d.png", i + 1);
		loadimage(&imgTors[i], name);
	}
	torExist = 0;
	torY = 350 - imgTors->getheight();
	*/

	//�����ڹ��ز�
	IMAGE imgTor;
	vector<IMAGE> imgTorArray;
	for (int i = 0; i < 7; i++)
	{
		//"res/t1.png"	����	"res/t7.png"
		sprintf_s(name, "res/t%d.png", i + 1);
		loadimage(&imgTor, name);
		imgTorArray.push_back(imgTor);
	}
	obstacleImgs.push_back(imgTorArray);

	//����ʨ���ز�
	IMAGE imgLion;
	vector<IMAGE> imgLionArray;
	for (int i = 0; i < 6; i++)
	{
		//"res/p1.png"	����	"res/p6.png"
		sprintf_s(name, "res/p%d.png", i + 1);
		loadimage(&imgLion, name);
		imgLionArray.push_back(imgLion);
	}
	obstacleImgs.push_back(imgLionArray);

	//��ʼ���ϰ����
	for (int i = 0; i < OBSTACLE_COUNT; i++)
	{
		obstacles[i].exist = 0;
	}
}

//�زĵ�ѭ������
void circulate()
{
	//����ѭ��
	for (int i = 0; i < 3; i++)
	{
		bgX[i] -= bgSpeed[i];
		if (bgX[i] < -WIN_WIDTH)
		{
			bgX[i] = 0;
		}
	}

	//��ɫ��Ծ
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
		//����״̬��ɫѭ��
		heroIndex = (heroIndex + 1) % 12;
	}

	//�����ڹ� 
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
	update = 1;
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

//��Ⱦ�ڹ�
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