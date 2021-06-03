void fCallBack(int a)       // 应用者增加的函数，此函数会在A中被执行
{
    //do something
    printf("a = %d\n",a);
    printf("fCallBack print! \n");
}


int main(void)
{
    SetCallBackFun(4,fCallBack);

    return 0;
}
