#include<stdio.h>
#include<string.h>
#include <windows.h>
#include <winuser.h>
#include"graphics.h"
#include"extgraph.h"
#include"imgui.h"
#include"header.h"

#define winwidth 10.0
#define winheight 6.50

enum {Sort=1}; 

// ������������libgraphics�� 
void DisplayClear(void); 
// ��ʱ��������������libgraphics�� 
void startTimer(int id,int timeinterval);
//���صõ��ٶ� 
double GetTimeSpeed();

// �û�������¼���Ӧ����
void myMouseEvent (int x, int y, int button, int event) {
	uiGetMouse(x, y, button, event); //GUI��ȡ���
	Display(); //ˢ����ʾ 
}

// �û��ļ����¼���Ӧ����
void KeyboardEventProcess(int key, int event)
{
	uiGetKeyboard(key,event); // GUI��ȡ����
	Display(); // ˢ����ʾ
}

// �û����ַ��¼���Ӧ����
void CharEventProcess(char ch)
{
	uiGetChar(ch); // GUI�ַ�����
	Display(); //ˢ����ʾ
}

//�û��ļ�ʱ����Ӧ���� 
void myTimerEvent (int timerID) 
{
	if(timerID == Sort && ( able_to_move || Task.isMoving == 1 || Task.isComparing == 1 || Task.isStop == 1) ) 
	{
		//�����˶�״̬�µ�����״̬ 
		if(Task.isMoving == 1) 
		{ 
			//�����������Ķ��� 
			if(1 - Task.time < GetTimeSpeed()) 
			{
				Task.isMoving = 0;
				Task.time = 0;
				if(SortType != MERGE) 
				{
					endOneStep();
				} 
				else 
				{
					endOneStepMerge();	
				}
				Task.isStop = 1;    //��ɶ�������ͣһ��������㿴�� 
				MBPointer = 0;
			} 
			
			//�������״̬�µĶ��� 
			else 
			{	
				//���˹鲢�����⣬�����������齻�� 
				if(SortType != MERGE) 
				{	double speed = GetTimeSpeed();
					Task.time += speed;
					double dt = m_MovingBlocks[0].distance;		
					m_MovingBlocks[0].Position += speed * dt;
					m_MovingBlocks[1].Position -= speed * dt;	
				} 
				//�鲢������˶������Բ�ǿ���ʵ�����������forѭ�����λ�ø��� 
				else 
				{
					double speed = GetTimeSpeed();
					Task.time += speed;
					int32_t rep;
					for(rep = 0; rep < MBPointer; rep++) 
					{
						double dt = m_MovingBlocks[rep].distance;
						m_MovingBlocks[rep].Position += speed * dt; 
					}
				}
			}
		}
		//���Ʒ��˶�״̬�µ�����״̬ 
		else if (Task.isComparing == 1) 
		{
			if(Task.time >= 1)
			{
				Task.isComparing = 0;
				Task.time = 0;
			}
			else 
			{
				Task.time += GetTimeSpeed() * 2;
			}
		}
		//������������µ�����״̬ 
		else if (Task.isStop == 1) 
		{
			if(Task.time >= 1) 
			{
				Task.isStop = 0;
				Task.time = 0;
			}
			else
			{
				Task.time += GetTimeSpeed() * 2;
			}
		}	
		else 
		{
			next_step(able_to_move);
			if(continue_to_move == 0) able_to_move = 0;
		}
	}
	Display();
}

void Main()
{
	//��ʼ������ 
	SetWindowSize(winwidth, winheight);
	InitGraphics();
	Init(1);
	SortType = SELECTION;  
	//Ĭ�������㷨Ϊѡ������ 
	
	//ע�������Ӧ���� 
	registerTimerEvent(myTimerEvent);
	registerMouseEvent(myMouseEvent);
	registerCharEvent(CharEventProcess);        
	registerKeyboardEvent(KeyboardEventProcess);
 
	startTimer(Sort, 20);
}

//���ݵ�ǰ�Ľ��治ͬ��������ʾ�ĺ��� 
void Display() {
	DisplayClear();
	if(Task.Page == 0) 
	{
		DrawUI();
	}	
	else 
	{
		DrawUI();
		DisplaySort();
	}		
}

//���ú��޸��ٶȵĺ��� 
static double m_timeSpeed = 0.2;
void SetTimeSpeed(double s) 
{
	m_timeSpeed = s;
}

//��ȡ��ǰ�ٶȵĺ��� 
double GetTimeSpeed()
{
	return m_timeSpeed; 
}

