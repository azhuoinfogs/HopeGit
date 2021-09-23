#ifndef _STROCKS_SERVER_H
#define _STROCKS_SERVER_H

#include <stardb_app.h>

#include "httplib.h"
#include "elog.h"
#include "cJSON.h"
#include "util.h"

#define STAROCKS_DB_PATH "./starocks"

#define SUCCESS 0
#define FAILURE -1

typedef struct _PersonInfo{
    char name[51];     //张三
    char gender[7];    //Male / Female
    char phone[21];    //phone number
    char email[51];    //email address
    char address[101]; //住址
    char cardId[21];   //证件编号
    float imgVec[512];  //人脸特征
    uint8_t age;       //30
    uint8_t type;      //0普客, 1vip, 2 黑名单
    uint8_t cardType;  //证件类型
    uint8_t reserved[15];  //预留信息
    uint32_t imgLen;       //图片长度
    char img64[];          //图片, base64
} PersonInfo;

typedef struct _PersonFieldInfo{
    char name[20];
    /* The type of the field, as above. */
    int type;
    char typeName[10];

    bool isRequired;
} FieldInfo;

typedef struct _SnapsInfo
{
    char time[31];      //format time string
    char skey[51];      //sid
    uint64_t timestamp; //time stamp ms
    bool isFaceMask;   //是否佩戴口罩
    bool isSmile;      //是否微笑
    uint8_t type;      //0普客, 1vip, 2 黑名单
    uint8_t reserved[15];  //预留信息
    float imgVec[512];  //人脸特征
    uint32_t imgLen;       //图片长度
    char img64[];          //图片, base64
} SnapsInfo;

//util
void personInfoArrayFree(PersonInfo **infoArray);
void snapsInfoArrayFree(SnapsInfo **infoArray);

//api
int starocks_api_person(stardb_handle_t *db, const httplib::Request &req, httplib::Response &res);
int starocks_api_persons_info(stardb_handle_t *db, const httplib::Request &req, httplib::Response &res);
int starocks_api_face_snaps(stardb_handle_t *db, const httplib::Request &req, httplib::Response &res);
int starocks_api_temp_data(stardb_handle_t *db, const httplib::Request &req, httplib::Response &res);

#endif