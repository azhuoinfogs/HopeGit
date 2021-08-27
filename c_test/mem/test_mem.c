#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <string.h>
#include <assert.h>

void* my_memcpy(void* dest, const void* src, unsigned int count)
{
	assert(dest && src);//断言
	void* temp = dest;//temp保存dest的起始地址
	while (count--)
	{
		*(char*)dest = *(char*)src;//复制src的内容到dest
        (char*)++dest;//下一个字节的拷贝
        (char*)++src;
	}

	return temp;//返回dest起始地址
}
int my_memcmp(void* dest, const void* src, unsigned int count)
{
	assert(dest && src);//断言
	if (!count)
	{
		return 0;
	}
	while (--count && *(char*)dest == *(char*)src)
	{
		(char*)++dest;
		(char*)++src;
	}
	return *(char*)dest - *(char*)src;
}

void* my_memset(void* dest, int a, unsigned int count)
{
	assert(dest);//断言
	void* temp = dest;//记录dest的首地址

	while (count--)
	{
		*(char*)dest = a;
		(char*)++dest;
	}

	return temp;//返回dest的首地址
}
int main()
{
	char arr1[20] = { 0 };
	char arr2[] = "hello world!";
	int arr3[10] = { 0 };
	int arr4[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	int i = 0;

	memcpy(arr1, arr2, 12);
	memcpy(arr3, arr4, 16);
	printf("%s\n", arr1);

	for (i = 0; i < 10; i++)
	{
		printf("%d ", arr3[i]);
	}
	printf("\n", arr1);
    int arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	i = 0;
	memmove(arr + 2, arr, 16);
	for (i = 0; i < 10; i++)
	{
		printf("%d ", arr[i]);
	}
	printf("\n");
	int _n = 13;
	if (_n&(sizeof(long)-1))
		printf("%d\n", (_n&(sizeof(long)-1)));
		_n += sizeof(long)-(_n&(sizeof(long)-1));
	printf("%d--%d--%d\n",_n, sizeof(long), sizeof(int));
	return 0;
}