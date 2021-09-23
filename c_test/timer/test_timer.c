#include <stdio.h>
#include "timer.h"
 
typedef struct Argument{
 int a;
 int b;
 int ret;
}Arg;
 
//任务1，打印语句
TASK_START(test1, arg)
 printf("This is a test1\n");
TASK_END
 
//任务2，计算arg中两个数的和，打印语句
TASK_START(test2, arg)
 
 Arg *temp = (Arg*)arg;
 temp->ret = temp->a + temp->b;
 printf("This is a test2\n");
 
TASK_END
 
//任务3，打印语句
TASK_START(test3, arg)
 printf("This is a test3\n");
TASK_END
 
void main(){
 
 Arg arg;
 
    //设置tick 为 500ms
 setTick(500 * 1000);
 
    //添加任务1到任务列表，设置定时器时间为2.5s，重复执行，无参数
 addTimerTask(test1, 5, NORESET, NULL);
 arg.a = 2; arg.b = 3;
 //添加任务2到任务列表，设置定时器时间为0.5s，不重复执行，参数为arg
 addTimerTask(test2, 1, AUTORESET, &arg);
 //添加任务3到任务列表，设置定时器时间为1s，重复执行，无参数
 addTimerTask(test3, 2, NORESET, NULL);
 
    //启动定时器
 StartTimer();
 printf("Timer is started\n");
 //程序等待5秒
 sleep(5);
 //停止定时器
 TimerStop();
 //等待所有任务执行完毕
 TimerWait();
 //打印任务二的计算结果
 printf("%d\n", arg.ret);
 
}