#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>
#include<stdbool.h>
#include<stdint.h>
#include "fsm.h"
/** @enum FSM_STATE_E
 *  @brief 状态机运行状态
 *  
 */
typedef enum
{
    RUNNING = 0x00, // 运行态
    FAULT,          // 故障态
}FSM_STATE_E;
 
/** @enum TRIG_EVENT_E
 *  @brief 状态机触发事件
 *  
 */
typedef enum
{       
    SENSOR_FAULT,       // 传感器故障事件
    SENSOR_RESUME,      // 传感器故障恢复事件
}TRIG_EVENT_E, *PTRIG_EVENT_E;
 
 
/** @struct MSG_EVENT_TRIG_T
 *  @brief 事件触发消息结构
 *  
 */
typedef struct
{
    uint8_t eDevPort;    			/* 事件触发传感器端口 */
    TRIG_EVENT_E eEventType;        /* 事件触发类型 */
    
}MSG_EVENT_TRIG_T, *PMSG_EVENT_TRIG_T;
 
/** @struct DEV_DET_T
 *  @brief  探测器
 *  
 */
typedef struct
{
    SQ_QUEUE stEventQue;                /* 事件触发队列 */
    MSG_EVENT_TRIG_T eEventQueBuf[8];  /* 事件触发缓存 */
    
}DEV_DET_T, *PDEV_DET_T;
 
 
/* 创建监控探测器实例 */
static DEV_DET_T g_stDetIns;
 
 
/* 状态机变量 */
static FSM_T g_stFsm;
 
/* 动作函数 */
static void ActFun_FaultEvent(void *parm);
static void ActFun_FaultResumeEvent(void *parm);
 
/* 状态迁移表 */
static FsmTable_T g_stFsmTable[] = 
{
    /* 触发事件         初态            动作函数             次态  */	
	{SENSOR_FAULT,      RUNNING,     ActFun_FaultEvent,             FAULT},
	{SENSOR_RESUME,     FAULT,       ActFun_FaultResumeEvent,       RUNNING},
};
 
/* 计算状态迁移表长度 */
#define FSM_TABLE_MAX_NUM (sizeof(g_stFsmTable)/sizeof(FsmTable_T))
 
 
/*==================================================================
* Function  : APP_Init
* Description : 创建消息队列，初始化状态机
* Input Para  : 
* Output Para : 
* Return Value: 
==================================================================*/
void APP_Init(void)
{   
    /* 创建事件触发队列 */
    if (!Que_InitQueue(&g_stDetIns.stEventQue,\
        sizeof(g_stDetIns.eEventQueBuf)/sizeof(g_stDetIns.eEventQueBuf[0]),\
        sizeof(g_stDetIns.eEventQueBuf[0]), 1, (uint8_t *)g_stDetIns.eEventQueBuf))
    {
        USER_DEBUG("ELEC: electrical fire detector tigger event queue create fail!\r\n");
    }
 
    /* 初始化状态机 */
    FSM_Init(&g_stFsm, g_stFsmTable, FSM_TABLE_MAX_NUM, RUNNING);
    
}
 
/*==================================================================
* Function  : ActFun_FaultEvent
* Description : 触发故障事件动作函数
* Input Para  : LocalActiveStatus
* Output Para : 
* Return Value: 
==================================================================*/
static void ActFun_FaultEvent(void *parm)
{
	/* 执行故障相关的动作 */	
}
 
/*==================================================================
* Function  : ActFun_FaultResumeEvent
* Description : 故障恢复事件动作函数
* Input Para  : 
* Output Para : 
* Return Value: 
==================================================================*/
static void ActFun_FaultResumeEvent(void *parm)
{
	/* 执行故障恢复相关的动作 */
}
 
/*==================================================================
* Function  : FsmEventHandleTask
* Description : 在定时器中定时轮询，避免动作函数中含有长任务函数
* Input Para  : 
* Output Para : 
* Return Value: 
==================================================================*/
void FsmEventHandleTask(void)
{       
    uint8_t* p_EventBuf = NULL;
    PMSG_EVENT_TRIG_T p_TrigMsg = NULL;
    
    /* 取出触发事件队列中的事件 */
    if (Que_DeQueue(&g_stDetIns.stEventQue, &p_EventBuf))
    {   
        p_TrigMsg = (PMSG_EVENT_TRIG_T)p_EventBuf;
        
        /* 在其它模块中改变触发事件，即可完成相应动作的执行 */
        FSM_EventHandle(&g_stElecFsm, p_TrigMsg->eEventType, (void *)&p_TrigMsg->eDevPort);
    }    
}
 
/*==================================================================
* Function  : Elec_UpdateEvent
* Description : 更新触发事件
* Input Para  : 
* Output Para : 
* Return Value: 
==================================================================*/
void Elec_UpdateEvent(MSG_EVENT_TRIG_T stTrigEventMsg)
{    
    /* 触发事件入队 */
    if (!Que_EnQueue(&g_stDetIns.stEventQue, (uint8_t*)&stTrigEventMsg, sizeof(g_stDetIns.eEventQueBuf[0])))
    {
        USER_DEBUG(PRINT_SYS, "ELEC: electrical fire detector tigger event entry queue fail!\r\n");
    }
}
 
/*==================================================================
* Function  : TestEvent
* Description : 测试事件函数
* Input Para  : 
* Output Para : 
* Return Value: 
==================================================================*/
void TestEvent(void)
{
	MSG_EVENT_TRIG_T stTrigEventMsg;
	
	/* 触发故障事件 */
	stTrigEventMsg.eEventType = SENSOR_FAULT; 
	Elec_UpdateEvent(stTrigEventMsg);
}