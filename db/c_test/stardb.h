/*
 * Copyright (c) 2019 TAOS Data, Inc. <jhtao@taosdata.com>
 *
 * This program is free software: you can use, redistribute, and/or modify
 * it under the terms of the GNU Affero General Public License, version 3
 * or later ("AGPL"), as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TDENGINE_SDB_H
#define TDENGINE_SDB_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void    SDB;
typedef void**  SDB_ROW;
typedef void    SDB_RES;

#define TSDB_DATA_TYPE_TIMESTAMP  9     // 8 bytes
#define TSDB_DATA_TYPE_INT        4     // 4 bytes
#define TSDB_DATA_TYPE_BINARY     8     // string

#define SUCCESS 0
#define FAILURE -1

typedef enum {
  SDB_MAX_OPTIONS
} SDB_OPTION;

typedef struct sdbField {
  char  name[64];
  short bytes;
  char  type;
} SDB_FIELD;

#ifdef _TD_GO_DLL_
  #define DLL_EXPORT    __declspec(dllexport)
#else
  #define DLL_EXPORT 
#endif

DLL_EXPORT void  stardb_init();
DLL_EXPORT int   stardb_options(SDB_OPTION option, const void *arg, ...);
DLL_EXPORT SDB *stardb_connect(const char *ip, const char *user, const char *pass, const char *db, uint16_t port);
DLL_EXPORT void  stardb_close(SDB *taos);
DLL_EXPORT SDB_RES *stardb_get_result(SDB *taos);
DLL_EXPORT void stardb_free_result(SDB_RES *res);
DLL_EXPORT SDB_ROW stardb_decode_result(SDB_RES *res);
DLL_EXPORT char *stardb_get_client_info();
DLL_EXPORT char *stardb_errstr(SDB *taos);
DLL_EXPORT int stardb_errno(SDB *taos);
DLL_EXPORT char *stardb_get_server_info(SDB *taos);
DLL_EXPORT int stardb_create_kv_database(SDB *taos, SDB_OPTION *option, char *name);
DLL_EXPORT int stardb_put_kv(SDB *taos, char *name, char *time, char *key, char *value);
DLL_EXPORT int stardb_get_key_by_value(SDB *taos, char *name, char *value, char **key_data, int *key_len);
DLL_EXPORT int stardb_get_value_by_key(SDB *taos, char *name, char *key, char **value_data, int *value_len );
DLL_EXPORT int stardb_get_data_by_time(SDB *taos, char *name, char *start, char *end);
DLL_EXPORT int stardb_open_database(SDB *taos, char *name);
DLL_EXPORT int stardb_destroy_database(SDB *taos, char *name);
DLL_EXPORT int stardb_create_database(SDB *taos, SDB_OPTION *option, char *name);
DLL_EXPORT int stardb_create_table_with_column(SDB *taos, char *tablename, int num_column, ...);
DLL_EXPORT int stardb_query(SDB *taos, const char *sql);
DLL_EXPORT int stardb_field_count(SDB *taos);
DLL_EXPORT int stardb_affected_rows(SDB *taos);
#ifdef __cplusplus
}
#endif

#endif
