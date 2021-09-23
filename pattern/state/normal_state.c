#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
 
/*
网址上的 示例2:
	有一个灯，按下开按钮，就会开灯，按下关按钮就会关灯。
	和一般等不同的是，两次开之间的灯的明暗不一样。
	也就是说，第一次开的时候，是高亮，关灯后，再开是低亮，下次再开是高亮，循环往复。
    这就是一个很典型的简单的有限状态机。
    简单的描述有3个状态，关灯[STATE_OFF]，
    高亮[STATE_HIGH_LIGHT]，低亮[STATE_LOW_LIGHT]。
大型一点的项目，比如复杂协议的实现，
一个状态转移到下一个状态的情况是比较复杂的，
无法用当前状态和事件简单确定，所以一般需要函数。
*/
 
#define STATE_DEPEND 4

#define STATE_OFF        0
#define STATE_HIGH_LIGHT 1
#define STATE_LOW_LIGHT  2
#define STATE_MAX        3

#define EVENT_BTN_OFF    0
#define EVENT_BTN_ON     1
#define EVENT_MAX        2

int last_state = STATE_LOW_LIGHT;
int last_light_state = STATE_LOW_LIGHT;

int lfsm_ignore(int cur_stat, int event);
int lfsm_btn_on(int cur_stat, int event);
int lfsm_btn_off(int cur_stat, int event);

struct light_fsm {
    int(*func) (int cur_stat, int event);
    int next_state;
};

struct light_fsm light_fsm[STATE_MAX][EVENT_MAX] =
        {
                //STATE_OFF
                {
                        { lfsm_ignore, STATE_MAX }, /*EVETN_BTN_OFF*/
                        { lfsm_btn_on, STATE_DEPEND }, /*EVETN_BTN_ON*/
                },
 
                //STATE_HIGH_LIGHT
                {
                        { lfsm_btn_off, STATE_OFF }, /*EVETN_BTN_OFF*/
                        { lfsm_ignore, STATE_MAX }, /*EVETN_BTN_ON*/
                },
 
                //STATE_LOW_LIGHT
                {
                        { lfsm_btn_off, STATE_OFF }, /*EVETN_BTN_OFF*/
                        { lfsm_ignore, STATE_MAX }, /*EVETN_BTN_ON*/
                },
 
        };
int lfsm_ignore(int cur_stat, int event)
{
    printf("invalid state or event\n");
    return 0;
}
int lfsm_btn_on(int cur_stat, int event)
{
    printf("lfsm_btn_on event\n");
    if (last_light_state == STATE_HIGH_LIGHT)
    {
        return STATE_LOW_LIGHT;
    }
    else if (last_light_state == STATE_LOW_LIGHT)
    {
        return STATE_HIGH_LIGHT;
    }
    else
    {
        printf("invalid state.\n");
        return STATE_MAX;
    }
}
int lfsm_btn_off(int cur_stat, int event)
{
    printf("lfsm_btn_off event\n");
    //last_light_state = cur_stat;
    return 0;
}
int light_change_state(int cur_stat, int next_state, int event)
{
    //if light on has special handling
    if (next_state == STATE_HIGH_LIGHT)
    {
        last_light_state = next_state;
        printf("rejoice,now bright light-high\n");
    }
    else if (next_state == STATE_LOW_LIGHT)
    {
        last_light_state = next_state;
        printf("rejoice,now bright light-low\n");
    }
 
    //other state change related handlings, maybe use curent state and next
    //next state,or event type
    last_state = cur_stat;
    cur_stat = next_state;
    return 0;
}
 
int light_event_happen(int event)
{
    //if light on has special handling
    if (event == EVENT_BTN_OFF)
    {
        printf("someone turn off light\n");
    }
    //other event type related handlings
    return 0;
}
int simple2_light_fsm_event(int cur_stat, int event)
{
    int next_state, next_state_tmp;
    next_state_tmp = light_fsm[cur_stat][event].func(cur_stat, event);
    if (next_state_tmp == STATE_MAX)
    {
        printf("fsm error\n");
        return -1;
    }
 
    if (light_fsm[cur_stat][event].next_state == STATE_DEPEND)
    {
        next_state = next_state_tmp;
    }
    else
    {
        next_state = light_fsm[cur_stat][event].next_state;
    }
    light_change_state(cur_stat, next_state, event);
    light_event_happen(event);
    return 0;
}
int main()
{
    int light_state = STATE_OFF;

    last_light_state = STATE_HIGH_LIGHT;
    last_state = STATE_HIGH_LIGHT;
 
    // printf("light_state=STATE_OFF, event=EVENT_BTN_OFF.\n");
    // simple2_light_fsm_event(light_state, EVENT_BTN_OFF);
    // printf("\n");
 
    printf("light_state=STATE_OFF, event=EVENT_BTN_ON.\n");
    simple2_light_fsm_event(light_state, EVENT_BTN_ON);
    printf("\n");
 
    // printf("light_state=STATE_OFF, event=EVENT_BTN_OFF.\n");
    // simple2_light_fsm_event(light_state, EVENT_BTN_OFF);
    // printf("\n");
 
    // printf("light_state=STATE_OFF, event=EVENT_BTN_ON.\n");
    // simple2_light_fsm_event(light_state, EVENT_BTN_ON);
    // printf("\n");
 
    // printf("light_state=STATE_OFF, event=EVENT_BTN_ON.\n");
    // simple2_light_fsm_event(light_state, EVENT_BTN_ON);
    // printf("\n");
 
    // getchar();
    return 1;
}