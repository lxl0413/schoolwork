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
void SetTimeSpeed(double s); //设置速度 
void EditSpeed(double speed); // 修改排序速度 
int32_t MBPointer; //存储本步中需要移动的方块数，除了归并排序外都为2 

int32_t able_to_move;//为1时，可以读取下一步，否则为暂停状态 
int32_t continue_to_move;//为1时，自动读取，不用点击NEXT 
int32_t finish_sorting;
int32_t m_lNowStep;//记录现在的步数 

char DataType;
int32_t SortType; //记录排序的种类 
int32_t m_lN;//记录读入的数据总数 
double m_array[MAXN]; //原数据的存储数组 
double m_MergeArray[MAXN]; //用于归并排序的第二个存储数组 

//控制全局正在进行的任务 
struct task{
	double time;   //从0增加到1，到1时表示当前的任务结束 
	int32_t isMoving;   
	int32_t isComparing;
	int32_t isStop;
	int32_t Page; //0为开始界面，1为排序界面 
} Task; 

typedef struct { 
	double lHeight; 
	double lNumber; 
	
	double lX;
	double lWidth;
	int32_t lState;  
	int32_t lComparing;  //记录状态，方便选择颜色 
} Block; //存储一个柱子的状态 

Block m_blocks[MAXN];

typedef struct {
	int32_t kind; //该步骤的种类。1表示比较（部分柱子亮起），2表示移动（部分柱子移动） 
	int32_t object[2]; //用于merge之外的排序模式。记录当前的两个高亮柱子。 
	int32_t compare;//统计该步骤内产生了多少次比较 
	int32_t change;//统计该步骤内产生了多少次变换 
	int32_t LR[2][2];
	/*用于quick和merge。 
	用于quick时，仅用到LR[0][0]和LR[0][1]。表示当前排序区间的左右边界。该边界内的非高亮柱子会被涂上第三种颜色
	
	用于merge时，LR[0]的两个值表示part1的左右边界，LR[1]的两个值表示part2的左右边界。 
	动画中，需要将LR[0][0]和LR[0][1]范围内的柱子涂上第一种红色。
	LR[1][0]和LR[1][1]范围内的柱子涂上第二种红色。 
	*/				
	int32_t position[MAXN];//表示merge的移动中，每个柱子将要移动到的位置。 
} OneStep; //存储排序的【一步】的状态 

OneStep m_alSteps[MAXN*MAXN*5]; //存储排序算法的所有步骤 
int32_t m_lStepPointer; //记录步骤的总数量 

int32_t compareCount; //统计当前算法的比较次数 
int32_t changeCount;  //统计当前算法的交换次数（对归并排序来说，是移动次数） 

typedef struct {
	double Position;  //当前的位置 
	double distance;  //从起点到终点总共需要移动的距离 
	int32_t startBlock;  //从第几块方块开始移动 
	int32_t toBlock;    //到第几块方块停止移动 
} MovingBlocks; //存储移动中柱子的状态 

MovingBlocks m_MovingBlocks[MAXN]; //存储所有移动的柱子的状态 

#endif
