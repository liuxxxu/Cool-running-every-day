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
4.ʵ����ұ���
5.ʵ�������Ծ
6.ʵ����������ڹ�
7.�����ϰ���ṹ����������
8.ʹ���ϰ���ṹ�����³�ʼ��
9.��װ����ϰ������ʾ
10.ʵ������¶�
11.ʵ�������ϰ���
12.�Ż��ϰ���ĳ���Ƶ��
13.ʵ����ײ���
14.ʵ����Ч�ͱ�������
15.ʵ��Ѫ��
16.ʵ�ֽ����ж�
17.ʵ�����¿�ʼ�ͽ���
18.ʵ�ֿ�ʼ����
19.�Ż������ϰ�
20.ʵ�ַ���
*/

#include<stdio.h>
#include<iostream>
#include<graphics.h>
#include<conio.h>
#include<ctime>
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

int jumpHeightMax;//��Ծ���߶�
int jumpHeightOff;//��Ծƫ����

int heroBlood;//Ѫ��
int score;//����

//�Ż�֡�ȴ�
int timer;//ʱ��ͳ��
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
	Hook1,
	Hook2,
	Hook3,
	Hook4,
	OBSTACLE_TYPE_COUNT
}obstacle_type;

vector<vector<IMAGE>>obstacleImgs;	//�����ϰ���ͼƬ

typedef struct obstacle
{
	int type;//�ϰ�������
	int imgIndex;//��ǰͼƬ���
	int X, Y;//�ϰ�������
	int speed;//�ϰ����ٶ�
	int power;//�ϰ����˺�
	bool exist;//�ϰ������״̬
	bool hited;//�Ƿ��Ѿ���ײ
	bool passed;//�Ƿ��ѱ�ͨ��
}obstacle_t;

obstacle_t obstacles[OBSTACLE_COUNT];
int lastObstacleIndex;

IMAGE imgHeroDown[2];
bool heroDown;//��ɫ�¶�״̬

//��Ϸ�ĳ�ʼ��
void init()
{
	//������Ϸ����
	initgraph(WIN_WIDTH, WIN_HEIGHT, EW_SHOWCONSOLE);
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

	heroBlood = 100;
	score = 0;

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

	//���ؽ�ɫ�¶��ز�
	loadimage(&imgHeroDown[0], "res/d1.png");
	loadimage(&imgHeroDown[1], "res/d2.png");
	heroDown = 0;

	//���������ز�
	IMAGE imgH;
	for (int i = 0; i < 4; i++)
	{
		//"res/h1.png"	����	"res/h4.png"
		vector<IMAGE> imgHArray;
		sprintf_s(name, "res/h%d.png", i + 1);
		loadimage(&imgH, name,63,260,true);
		imgHArray.push_back(imgH);
		obstacleImgs.push_back(imgHArray); 
	}

	lastObstacleIndex = -1;
	
	//Ԥ������Ƶ
	preLoadSound("res/hit.mp3");

	//��������
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

//��ײ���
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
				cout << "ʣ��Ѫ��:" << heroBlood << endl;
				playSound("res/hit.mp3");
				obstacles[i].hited = 1;
			}
		}
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
	//��ɫ�¶�
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
	//��ɫ����
	else
	{
		heroIndex = (heroIndex + 1) % 12;
	}

	//�����ϰ��� 
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

	//�����ϰ�������
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
	
	//��ɫ���ϰ�����ײ���
	checkHit();
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

//��ɫ�¶�
void down()
{
	heroDown = 1;
	update = 1;
	heroIndex = 0;
}

//�����û���������
void keyEvent()
{
	char ch;
	if (_kbhit())	//����а������룬_kbhit����ֵΪtrue
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

//��Ⱦ�ڹ�
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

//��Ⱦ��ɫ
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

//��ȾѪ��
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

//������
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