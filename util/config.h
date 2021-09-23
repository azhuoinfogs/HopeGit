/*
 * Copyright (c) 2020 bj-Starblaze Data, Inc. <qiang.hu@Starblaze-tech.com>
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

#ifndef STARDB_CFG_H
#define STARDB_CFG_H

#ifdef __cplusplus
extern "C" {
#endif
#include<stdbool.h>

#define STAR_CFG_MAX_NUM    110
#define STAR_CFG_PRINT_LEN  23
#define STAR_CFG_OPTION_LEN 24
#define STAR_CFG_VALUE_LEN  41

#define STAR_CFG_CTYPE_B_CONFIG    1U   // can be configured from file
#define STAR_CFG_CTYPE_B_SHOW      2U   // can displayed by "show configs" commands
#define STAR_CFG_CTYPE_B_LOG       4U   // is a log type configuration
#define STAR_CFG_CTYPE_B_CLIENT    8U   // can be displayed in the client log
#define STAR_CFG_CTYPE_B_OPTION    16U  // can be configured by star_options function
#define STAR_CFG_CTYPE_B_NOT_PRINT 32U  // such as password

enum {
  STAR_CFG_CSTATUS_NONE,     // not configured
  STAR_CFG_CSTATUS_DEFAULT,  // use system default value
  STAR_CFG_CSTATUS_FILE,     // configured from file
  STAR_CFG_CSTATUS_OPTION,   // configured by star_options function
  STAR_CFG_CSTATUS_ARG,      // configured by program argument
};

enum {
  STAR_CFG_VTYPE_INT16,
  STAR_CFG_VTYPE_INT32,
  STAR_CFG_VTYPE_FLOAT,
  STAR_CFG_VTYPE_STRING,
  STAR_CFG_VTYPE_IPSTR,
  STAR_CFG_VTYPE_DIRECTORY,
};

enum {
  STAR_CFG_UTYPE_NONE,
  STAR_CFG_UTYPE_PERCENT,
  STAR_CFG_UTYPE_GB,
  STAR_CFG_UTYPE_Mb,
  STAR_CFG_UTYPE_BYTE,
  STAR_CFG_UTYPE_SECOND,
  STAR_CFG_UTYPE_MS
};

typedef struct {
  char *   option;
  void *   ptr;
  float    minValue;
  float    maxValue;
  int8_t   cfgType;
  int8_t   cfgStatus;
  int8_t   unitType;
  int8_t   valType;
  uint32_t ptrLength;
} SGlobalCfg;

extern SGlobalCfg starGlobalConfig[];
extern int32_t    starGlobalConfigNum;
extern char *     starCfgStatusStr[];

void starReadGlobalLogCfg();
bool starReadGlobalCfg();
void starPrintGlobalCfg();

void starInitConfigOption(SGlobalCfg cfg);
SGlobalCfg * starGetConfigOption(const char *option);

#ifdef __cplusplus
}
#endif

#endif  // STARDB_TGLOBALCFG_H
