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
#define STAR_DATA_TYPE_NULL       0     // 1 bytes
#define STAR_DATA_TYPE_BOOL       1     // 1 bytes
#define STAR_DATA_TYPE_TINYINT    2     // 1 byte
#define STAR_DATA_TYPE_SMALLINT   3     // 2 bytes
#define STAR_DATA_TYPE_INT        4     // 4 bytes
#define STAR_DATA_TYPE_BIGINT     5     // 8 bytes
#define STAR_DATA_TYPE_FLOAT      6     // 4 bytes
#define STAR_DATA_TYPE_DOUBLE     7     // 8 bytes
#define STAR_DATA_TYPE_BINARY     8     // string
#define STAR_DATA_TYPE_TIMESTAMP  9     // 8 bytes
#define STAR_DATA_TYPE_NCHAR      10    // unicode string

#ifdef _TD_GO_DLL_
  #define DLL_EXPORT    __declspec(dllexport)
#else
  #define DLL_EXPORT 
#endif
DLL_EXPORT starkv_t *starkv_open(char *dev);
DLL_EXPORT int starkv_init(starkv_t *dev, int create_as_new);
DLL_EXPORT void starkv_cleanup(starkv_t *handle);
DLL_EXPORT int starkv_close(starkv_t *handle);
DLL_EXPORT int starkv_get(starkv_t *dev, unsigned char *key, size_t keylen, unsigned char **buf, size_t *buflen, char **errptr);
DLL_EXPORT int starkv_put(starkv_t *dev, unsigned char *key, size_t keylen,unsigned char *data, size_t datalen, char **errptr);
#ifdef __cplusplus
}
#endif

#endif
