#include<stdio.h>
typedef unsigned int uint32_t;
typedef unsigned char uint8_t;
union bit32_data
{
    uint32_t data;
    struct
    {
        uint8_t byte0;
        uint8_t byte1;
        uint8_t byte2;
        uint8_t byte3;
    }byte;
};
#define GET_LOW_BYTE0(x)?((x?>>??0)?&?0x000000ff)
#define GET_LOW_BYTE1(x)?((x?>>??8)?&?0x000000ff)
#define GET_LOW_BYTE2(x)?((x?>>?16)?&?0x000000ff)
#define GET_LOW_BYTE3(x)?((x?>>?24)?&?0x000000ff)

int main(void)
{
    union bit32_data num;
    num.data = 0x12345678;
    printf("byte0=0x%x\n",num.byte.byte0);
    printf("byte1=0x%x\n",num.byte.byte1);
    printf("byte2=0x%x\n",num.byte.byte2);
    printf("byte3=0x%x\n",num.byte.byte3);
    return 0;
}