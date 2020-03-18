#include<stdio.h>
#include<stdlib.h>
#include "stardb.h"
int main() {

  SDB *sdb;
  SDB_OPTION opt;
  char *dbname = "test";
  char *kvname = "kv_db";
  char *table_name = "test_table";
  char *column_name = "test_table_column1";
  int ret = 0;

  stardb_init();
  sdb = stardb_connect("192.168.90.119", "root", "taosdata", NULL, 0);
  if (sdb == NULL) {
    printf("failed connect database \n");
    return ret;
 }
 ret = stardb_create_database(sdb, &opt, dbname);
 if (ret < 0) {
    printf("failed create database \n");
    return ret;
 }
 /*
  struct sdbField column1 = {"column1", 8, TSDB_DATA_TYPE_TIMESTAMP};
  struct sdbField column2 = {"column2", 32, TSDB_DATA_TYPE_INT};
  struct sdbField column3 = {"column3",  40, TSDB_DATA_TYPE_INT};
  ret = stardb_create_table_with_column(sdb, table_name, 3, &column2, &column1, &column3);
  if (ret < 0) {
    printf("failed create table\n");
    return ret;
  }
  ret = stardb_create_table(sdb, table_name, column_name, TYPE_INT);
  if (ret < 0) {
      printf("failed create table\n");
      return ret;
  }
  ret = stardb_put_int_table(sdb, table_name, column_name, TYPE_INT);
  if (ret < 0) {
      printf("failed put int data in table\n");
      return ret;
 ret = stardb_create_kv_database(sdb, &opt, kvname);
 if (ret < 0) {
    printf("failed create database \n");
    return ret;
 }
 ret = stardb_put_kv(sdb, kvname, "2020-01-01 00:00:00.000", "key1", "value1");
  if (ret < 0) {
      printf("failed put int default data in table\n");
      return ret;
  }
  sleep(1);
  ret = stardb_put_kv(sdb, kvname,"2020-01-01 00:01:00.000", "key2", "value2");
  if (ret < 0) {
      printf("failed put int default data in table\n");
      return ret;
  }
  sleep(1);
  ret = stardb_put_kv(sdb, kvname,"2020-01-01 00:02:00.000", "key3", "value3");
  if (ret < 0) {
      printf("failed put int default data in table\n");
      return ret;
  }
  sleep(1);
  ret = stardb_put_kv(sdb, kvname,"2020-01-01 00:03:00.000", "key4", "value4");
  if (ret < 0) {
      printf("failed put int default data in table\n");
      return ret;
  }
 
  char buf[1024];
  int len = 0;
  ret = stardb_get_value_by_key(sdb, kvname, "key1", &buf, &len);
  if (ret < 0) {
      printf("failed get value int default data in table\n");
      return ret;
  }
  printf("get value:%s--%d\n", buf, len);
  
  char buf1[1024];
  int len1 = 0;
  ret = stardb_get_key_by_value(sdb, kvname, "value2", &buf1, &len1);
  if (ret < 0) {
      printf("failed get key int default data in table\n");
      return ret;
  }
  printf("get key:%s--%d\n", buf1, len1);
  
  ret = stardb_get_data_by_time(sdb, kvname, "2020-01-01 00:00:00.000", "2020-01-01 00:03:00.000");
  if (ret < 0) {
      printf("failed get time int default data in table\n");
      return ret;
  }
  SDB_RES *result = stardb_get_result(sdb);
  if (!result) {
      printf("failed get result of data int default table\n");
      return ret;
  }
  int data_count = 0;
  int get_len = 0;
  SDB_ROW data;
  while ((data = stardb_decode_result(result))) {
    printf("time= %ld\n", data[0]);
    printf("key= %s\n", data[1]);
    printf("value= %s\n", data[2]);
    data_count++; 
  }
  printf("count :%d\n", data_count);
  */
  return 0;
}
