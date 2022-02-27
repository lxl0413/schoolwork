#ifndef _HEADER_H
#define _HEADER_H

#define MAXN (100)
#define SELECTION 0
#define BUBBLE 1
#define MERGE 2
#define QUICK 3 

#define INT 'i'
#define FLOAT 'f'
#define DOUBLE 'd' 

typedef signed int int32_t;

void DisplaySort();
void Display();
void Init(int32_t flag);

int32_t next_step(int direction);
void endOneStep();
void endOneStepMerge();
void OneStepMerge(int32_t direction);
double GetEditSpeed();
double GetTimeSpeed();
void SetTimeSpeed(double s); //�����ٶ� 
void EditSpeed(double speed); // �޸������ٶ� 
int32_t MBPointer; //�洢��������Ҫ�ƶ��ķ����������˹鲢�����ⶼΪ2 

int32_t able_to_move;//Ϊ1ʱ�����Զ�ȡ��һ��������Ϊ��ͣ״̬ 
int32_t continue_to_move;//Ϊ1ʱ���Զ���ȡ�����õ��NEXT 
int32_t finish_sorting;
int32_t m_lNowStep;//��¼���ڵĲ��� 

char DataType;
int32_t SortType; //��¼��������� 
int32_t m_lN;//��¼������������� 
double m_array[MAXN]; //ԭ���ݵĴ洢���� 
double m_MergeArray[MAXN]; //���ڹ鲢����ĵڶ����洢���� 

//����ȫ�����ڽ��е����� 
struct task{
	double time;   //��0���ӵ�1����1ʱ��ʾ��ǰ��������� 
	int32_t isMoving;   
	int32_t isComparing;
	int32_t isStop;
	int32_t Page; //0Ϊ��ʼ���棬1Ϊ������� 
} Task; 

typedef struct { 
	double lHeight; 
	double lNumber; 
	
	double lX;
	double lWidth;
	int32_t lState;  
	int32_t lComparing;  //��¼״̬������ѡ����ɫ 
} Block; //�洢һ�����ӵ�״̬ 

Block m_blocks[MAXN];

typedef struct {
	int32_t kind; //�ò�������ࡣ1��ʾ�Ƚϣ������������𣩣�2��ʾ�ƶ������������ƶ��� 
	int32_t object[2]; //����merge֮�������ģʽ����¼��ǰ�������������ӡ� 
	int32_t compare;//ͳ�Ƹò����ڲ����˶��ٴαȽ� 
	int32_t change;//ͳ�Ƹò����ڲ����˶��ٴα任 
	int32_t LR[2][2];
	/*����quick��merge�� 
	����quickʱ�����õ�LR[0][0]��LR[0][1]����ʾ��ǰ������������ұ߽硣�ñ߽��ڵķǸ������ӻᱻͿ�ϵ�������ɫ
	
	����mergeʱ��LR[0]������ֵ��ʾpart1�����ұ߽磬LR[1]������ֵ��ʾpart2�����ұ߽硣 
	�����У���Ҫ��LR[0][0]��LR[0][1]��Χ�ڵ�����Ϳ�ϵ�һ�ֺ�ɫ��
	LR[1][0]��LR[1][1]��Χ�ڵ�����Ϳ�ϵڶ��ֺ�ɫ�� 
	*/				
	int32_t position[MAXN];//��ʾmerge���ƶ��У�ÿ�����ӽ�Ҫ�ƶ�����λ�á� 
} OneStep; //�洢����ġ�һ������״̬ 

OneStep m_alSteps[MAXN*MAXN*5]; //�洢�����㷨�����в��� 
int32_t m_lStepPointer; //��¼����������� 

int32_t compareCount; //ͳ�Ƶ�ǰ�㷨�ıȽϴ��� 
int32_t changeCount;  //ͳ�Ƶ�ǰ�㷨�Ľ����������Թ鲢������˵�����ƶ������� 

typedef struct {
	double Position;  //��ǰ��λ�� 
	double distance;  //����㵽�յ��ܹ���Ҫ�ƶ��ľ��� 
	int32_t startBlock;  //�ӵڼ��鷽�鿪ʼ�ƶ� 
	int32_t toBlock;    //���ڼ��鷽��ֹͣ�ƶ� 
} MovingBlocks; //�洢�ƶ������ӵ�״̬ 

MovingBlocks m_MovingBlocks[MAXN]; //�洢�����ƶ������ӵ�״̬ 

#endif
