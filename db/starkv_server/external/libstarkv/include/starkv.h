/*
 * Copyright (c) 2019 Starblaze Data, Inc. <qiang.hu@starblaze-tech.com>
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

#ifndef STAR_H
#define STAR_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void starkv_options_t;
typedef void starkv_writeoptions_t;
typedef void starkv_readoptions_t;
typedef void starkv_iterator_t;
typedef void starkv_t;
// Data type definition
#ifdef _TD_GO_DLL_
  #define DLL_EXPORT    __declspec(dllexport)
#else
  #define DLL_EXPORT 
#endif

typedef struct starkv_info{
	int  db_id;
	char dbname[32];
	char devname[32];
}starkv_info_t;

DLL_EXPORT int starkv_get(starkv_t *dev,  char *key, size_t keylen,  char **buf, size_t *buflen, char **errptr);
DLL_EXPORT int starkv_put(starkv_t *dev,  char *key, size_t keylen, char *data, size_t datalen, char **errptr);
DLL_EXPORT int starkv_delete(starkv_t *dev,  char *key, size_t keylen, char **errptr);
DLL_EXPORT starkv_iterator_t* starkv_create_iterator(starkv_t* db);
DLL_EXPORT void starkv_iter_destroy(starkv_iterator_t* iter);
DLL_EXPORT bool starkv_iter_next(starkv_iterator_t* iter);
DLL_EXPORT char* starkv_iter_key(starkv_iterator_t *iter, size_t* klen);
DLL_EXPORT char* starkv_iter_value(starkv_iterator_t *iter, size_t* vlen);
DLL_EXPORT int starkv_create_database(char *dev, char *dbname, starkv_options_t *op);
DLL_EXPORT starkv_t *starkv_open_database(char *dev, char *dbname, int nsid);
DLL_EXPORT void starkv_cleanup_database(starkv_t *dev);
DLL_EXPORT int starkv_close_database(starkv_t *dev);
DLL_EXPORT starkv_info_t **starkv_list_dbinfo(size_t* lencfs, char** errptr);
DLL_EXPORT void starkv_array_free(void **infoArray, int num);
DLL_EXPORT starkv_options_t *starkv_options_create(size_t dbsize, int min_key_size, int max_value_size);
DLL_EXPORT void starkv_options_destroy(starkv_options_t* v);

#ifdef __cplusplus
}
#endif

#endif
