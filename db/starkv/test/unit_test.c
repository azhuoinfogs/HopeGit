#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <starkv.h>
//当前测试项编号枚举 
enum TEST_ITEM
{
    TEST_1 = 1,
    TEST_2,
    TEST_3,
    TEST_4,
    TEST_5,
    TEST_6
};
int  Get_User_input(void) ;
void Run_User_Test_App(int KeyCode);

void TEST_1_APP(void)
{
    printf("测试项目一 list database info\n");
    char *err;
	size_t num = 0;
	starkv_info_t **infos = starkv_list_column_families(&num, &err);
	for (int i = 0; i < num; i++) {
		printf("%s--%s--%d\n", infos[i]->devname, infos[i]->dbname, infos[i]->db_id);
	}
	starkv_array_free((void **)infos, num);
    printf("TEST_1_APP pass !!!\n");
}

void TEST_2_APP(void)
{
    printf("测试项目二 create database and clean \n");
    char *devname = "/dev/nvme0"; 
    starkv_options_t *op = starkv_options_create(1073741824, 1, 100);
    int dbindex = starkv_create_database(devname, "test.db", op);
    if (dbindex < 0) {
        exit(-1);
    }
    starkv_options_destroy(op);
    printf("TEST_2_APP pass !!!\n");
	// // int dbindex = 2;
}

void TEST_3_APP(void)
{
    printf("测试项目三 starkv_open and close database\n");
    char *devname = "/dev/nvme0"; //执行完test2 成功后， 至少dbindex 1 的database 存在。
    int dbindex = 1;
	starkv_t *dev = starkv_open_database(devname, "test.db", dbindex);
    if (!dev) {
        exit(-1);
    }
    int ret =  starkv_close_database(dev);
    if (ret != 0) {
        exit(-1);
    }
    printf("TEST_3_APP pass !!!\n");
}

void TEST_4_APP(void)
{
    printf("测试项目四 starkv_put get and delete \n");
    char *devname = "/dev/nvme0"; //执行完test2 成功后， 至少dbindex 1 的database 存在。
    int dbindex = 1;
	starkv_t *dev = starkv_open_database(devname, "test.db", dbindex);
    if (!dev) {
        exit(-1);
    }
    char *err =NULL;
    char *keybuf= "key";
    char *valbuf= "value";
    unsigned char *sv;
    int ret;
    size_t val_len;
    size_t keybuf_len = strlen(keybuf);
    size_t valbuf_len = strlen(valbuf);
    {
    	starkv_put(dev, keybuf, keybuf_len, valbuf, valbuf_len, &err);
		if (err) {
			printf("starkv_put failed %s\n", err);
            exit(-1);
		}
        ret = starkv_get(dev, keybuf, keybuf_len, &sv, &val_len, &err);
		if(sv) {
			if ((memcmp(sv, valbuf, val_len) != 0 ))
				printf("get wrong value:%s--%s\n", sv, valbuf);
			free(sv);
			sv = NULL;
		} else {
			printf("get:%s  failed: %s\n", keybuf, err);
            exit(-1);
		}
        ret = starkv_delete(dev, keybuf, keybuf_len, &err);
        if (ret != 0) {
            printf("starkv_delete failed %s\n", err);
            exit(-1);
        }
        ret = starkv_get(dev, keybuf, keybuf_len, &sv, &val_len, &err);
		if(sv) {
			printf("get wrong delete value:%s--%s--%s\n", keybuf, sv, valbuf);
			free(sv);
			sv = NULL;
            exit(-1);
		}
    }
    ret =  starkv_close_database(dev);
    if (ret != 0) {
        exit(-1);
    }
    printf("TEST_4_APP pass !!!\n");
}

void TEST_5_APP(void)
{
    printf("测试项目五 starkv_iterate \n");
    char *devname = "/dev/nvme0"; //执行完test2 成功后， 至少dbindex 1 的database 存在。
    int dbindex = 1;
    int ret;
	starkv_t *dev = starkv_open_database(devname, "test.db", dbindex);
    if (!dev) {
        exit(-1);
    }
	starkv_iterator_t *sIter = starkv_create_iterator(dev);
	int scount;
    while (starkv_iter_next(sIter))
    {
		size_t key_len;
        char *key = starkv_iter_key(sIter, &key_len);
		if (key)
        	printf("data:%s--%ld\n", key, key_len);
		size_t val_len;
        char *val = starkv_iter_value(sIter, &val_len);
		if (val)
        	printf("data:%s--%ld\n", val, val_len);
        /* code */
		scount ++;
    }
	starkv_iter_destroy(sIter);
    ret =  starkv_close_database(dev);
    if (ret != 0) {
        exit(-1);
    }
    printf("TEST_5_APP pass !!!\n");
}

void TEST_6_APP(void)
{
    printf("测试项目六\n");
}

//执行用户测试APP 
void Run_User_Test_App(int KeyCode)
{
    switch(KeyCode)
    {
        case TEST_1:
            TEST_1_APP();
            break ;
        case TEST_2:
            TEST_2_APP();
            break ;
        case TEST_3:
            TEST_3_APP();
            break ;
        case TEST_4:
            TEST_4_APP();
            break ;
        case TEST_5:
            TEST_5_APP();
            break ;
        case TEST_6:
            TEST_6_APP();
            break ;
        default:
            break ;
    }   
}

int Get_User_input(void) 
{
    int KeyCode = 0;
    scanf("%d",&KeyCode);
    return KeyCode ;
}
int main(void)
{
    int KeyCode = 0 ;
    Run_User_Test_App(1);
    Run_User_Test_App(2);
    Run_User_Test_App(3);
    Run_User_Test_App(4);
    Run_User_Test_App(5);
    return 0 ;
}