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

#ifndef STARDB_TAOSARRAY_H
#define STARDB_TAOSARRAY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "os.h"

#define TARRAY_MIN_SIZE 8
#define TARRAY_GET_ELEM(array, index) ((void*)((char*)((array)->pData) + (index) * (array)->elemSize))

typedef struct SArray {
  size_t size;
  size_t capacity;
  size_t elemSize;
  void*  pData;
} SArray;

/**
 *
 * @param size
 * @param elemSize
 * @return
 */
void* starArrayInit(size_t size, size_t elemSize);

/**
 *
 * @param pArray
 * @param pData
 * @return
 */
void* starArrayPush(SArray* pArray, void* pData);

/**
 *
 * @param pArray
 */
void* starArrayPop(SArray* pArray);

/**
 * get the data from array
 * @param pArray
 * @param index
 * @return
 */
void* starArrayGet(const SArray* pArray, size_t index);

/**
 * get the pointer data from the array
 * @param pArray
 * @param index
 * @return
 */
void* starArrayGetP(const SArray* pArray, size_t index);

/**
 * return the size of array
 * @param pArray
 * @return
 */
size_t starArrayGetSize(const SArray* pArray);

/**
 * insert data into array
 * @param pArray
 * @param index
 * @param pData
 */
void* starArrayInsert(SArray* pArray, size_t index, void* pData);

/**
 * remove data entry of the given index
 * @param pArray
 * @param index
 */
void starArrayRemove(SArray* pArray, size_t index);

/**
 * copy the whole array from source to destination
 * @param pDst
 * @param pSrc
 */
void starArrayCopy(SArray* pDst, const SArray* pSrc);

/**
 * clone a new array
 * @param pSrc
 */
SArray* starArrayClone(const SArray* pSrc);

/**
 * clear the array (remove all element)
 * @param pArray
 */
void starArrayClear(SArray* pArray);

/**
 * destroy array list
 * @param pArray
 */
void starArrayDestroy(SArray* pArray);

/**
 * sort the array
 * @param pArray
 * @param compar
 */
void starArraySort(SArray* pArray, int (*compar)(const void*, const void*));

/**
 * sort string array
 * @param pArray
 */
void starArraySortString(SArray* pArray, __compar_fn_t comparFn);

/**
 * search the array
 * @param pArray
 * @param compar
 * @param key
 */
void* starArraySearch(const SArray* pArray, const void* key, __compar_fn_t comparFn);

/**
 * search the array
 * @param pArray
 * @param key
 */
char* starArraySearchString(const SArray* pArray, const char* key, __compar_fn_t comparFn);

#ifdef __cplusplus
}
#endif

#endif  // STARDB_TAOSARRAY_H
