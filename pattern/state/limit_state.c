#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
 
/*
	简单状态机模式实现
	有一个灯，按下开按钮，就会开灯，按下关按钮就会关灯。
	这就是一个很典型的简单的有限状态机。简单的描述有2个状态，关灯[STATE_OFF]，亮[STATE_LIGHT_ON] 。
	有两个事件，开和关按钮。这两个事件促使状态机间的转换。
*/
 
#define STATE_OFF 0
#define STATE_LIGHT_ON 1
#define STATE_MAX 2
#define EVENT_BTN_OFF  0
#define EVENT_BTN_ON   1
#define EVENT_MAX      2
 
#if 0
//this is for linux define
int light_fsm_simple_table[STATE_MAX][EVENT_MAX] =
{
	[STATE_OFF][EVENT_BTN_OFF] = STATE_OFF,
	[STATE_OFF][EVENT_BTN_ON] = STATE_LIGHT_ON,
	[STATE_LIGHT_ON][EVENT_BTN_OFF] = STATE_OFF,
	[STATE_LIGHT_ON][EVENT_BTN_ON] = STATE_LIGHT_ON
};
#endif
int light_fsm_simple_table[STATE_MAX][EVENT_MAX] = {};

void init_table()
{
    light_fsm_simple_table[STATE_OFF][EVENT_BTN_OFF] = STATE_OFF;
    light_fsm_simple_table[STATE_OFF][EVENT_BTN_ON] = STATE_LIGHT_ON;
    light_fsm_simple_table[STATE_LIGHT_ON][EVENT_BTN_OFF] = STATE_OFF;
    light_fsm_simple_table[STATE_LIGHT_ON][EVENT_BTN_ON] = STATE_LIGHT_ON;
}
 
int light_fsm_event(int cur_stat, int event)
{
    int next_state;
    next_state = light_fsm_simple_table[cur_stat][event];
    return next_state;
}
 
int main()
{
    int light_state = STATE_OFF;
    int eve1 = EVENT_BTN_OFF;
    int eve2 = EVENT_BTN_ON;
    init_table();
    light_state = light_fsm_event(light_state, eve1);
    printf("now light state is %d\n", light_state);
    light_state = light_fsm_event(light_state, eve2);
    printf("now light state is %d\n", light_state);
    return 1;
}