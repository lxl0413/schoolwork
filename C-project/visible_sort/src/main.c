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

// 清屏函数，在libgraphics中 
void DisplayClear(void); 
// 计时器启动函数，在libgraphics中 
void startTimer(int id,int timeinterval);
//返回得到速度 
double GetTimeSpeed();

// 用户的鼠标事件响应函数
void myMouseEvent (int x, int y, int button, int event) {
	uiGetMouse(x, y, button, event); //GUI获取鼠标
	Display(); //刷新显示 
}

// 用户的键盘事件响应函数
void KeyboardEventProcess(int key, int event)
{
	uiGetKeyboard(key,event); // GUI获取键盘
	Display(); // 刷新显示
}

// 用户的字符事件响应函数
void CharEventProcess(char ch)
{
	uiGetChar(ch); // GUI字符输入
	Display(); //刷新显示
}

//用户的计时器响应函数 
void myTimerEvent (int timerID) 
{
	if(timerID == Sort && ( able_to_move || Task.isMoving == 1 || Task.isComparing == 1 || Task.isStop == 1) ) 
	{
		//控制运动状态下的任务状态 
		if(Task.isMoving == 1) 
		{ 
			//完成最将近结束的动画 
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
				Task.isStop = 1;    //完成动画后暂停一会儿，方便看清 
				MBPointer = 0;
			} 
			
			//完成正常状态下的动画 
			else 
			{	
				//除了归并排序外，其他都是两块交换 
				if(SortType != MERGE) 
				{	double speed = GetTimeSpeed();
					Task.time += speed;
					double dt = m_MovingBlocks[0].distance;		
					m_MovingBlocks[0].Position += speed * dt;
					m_MovingBlocks[1].Position -= speed * dt;	
				} 
				//归并排序块运动关联性不强，故单独处理，利用for循环完成位置更新 
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
		//控制非运动状态下的任务状态 
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
		//控制其他情况下的任务状态 
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
	//初始化部分 
	SetWindowSize(winwidth, winheight);
	InitGraphics();
	Init(1);
	SortType = SELECTION;  
	//默认排序算法为选择排序 
	
	//注册各种响应函数 
	registerTimerEvent(myTimerEvent);
	registerMouseEvent(myMouseEvent);
	registerCharEvent(CharEventProcess);        
	registerKeyboardEvent(KeyboardEventProcess);
 
	startTimer(Sort, 20);
}

//根据当前的界面不同，控制显示的函数 
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

//调用和修改速度的函数 
static double m_timeSpeed = 0.2;
void SetTimeSpeed(double s) 
{
	m_timeSpeed = s;
}

//获取当前速度的函数 
double GetTimeSpeed()
{
	return m_timeSpeed; 
}

