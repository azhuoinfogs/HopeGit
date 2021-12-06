/*
 * Copyright (c) 2020 Starblaze Data, Inc. <qiang.hu@starblaze-tech.com>
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

#include "array.h"

void* starArrayInit(size_t size, size_t elemSize) {
  assert(elemSize > 0);

  if (size < TARRAY_MIN_SIZE) {
    size = TARRAY_MIN_SIZE;
  }

  SArray* pArray = calloc(1, sizeof(SArray));
  if (pArray == NULL) {
    return NULL;
  }

  pArray->pData = calloc(size, elemSize);
  if (pArray->pData == NULL) {
    free(pArray);
    return NULL;
  }

  pArray->capacity = size;
  pArray->elemSize = elemSize;
  return pArray;
}

static int32_t starArrayResize(SArray* pArray) {
  assert(pArray->size >= pArray->capacity);

  size_t size = pArray->capacity;
  size = (size << 1u);

  void* tmp = realloc(pArray->pData, size * pArray->elemSize);
  if (tmp == NULL) {  // reallocate failed, the original buffer remains
    return -1;
  }

  pArray->pData = tmp;
  pArray->capacity = size;
  
  return 0;
}

void* starArrayPush(SArray* pArray, void* pData) {
  if (pArray == NULL || pData == NULL) {
    return NULL;
  }

  if (pArray->size >= pArray->capacity) {
    int32_t ret = starArrayResize(pArray);
    
    // failed to push data into buffer due to the failure of memory allocation
    if (ret != 0) {
      return NULL;
    }
  }

  void* dst = TARRAY_GET_ELEM(pArray, pArray->size);
  memcpy(dst, pData, pArray->elemSize);

  pArray->size += 1;
  return dst;
}

void* starArrayPop(SArray* pArray) {
  assert( pArray != NULL );

  if (pArray->size == 0) {
    return NULL;
  }
  pArray->size -= 1;
  return TARRAY_GET_ELEM(pArray, pArray->size);
}

void* starArrayGet(const SArray* pArray, size_t index) {
  assert(index < pArray->size);
  return TARRAY_GET_ELEM(pArray, index);
}

void* starArrayGetP(const SArray* pArray, size_t index) {
  assert(index < pArray->size);
  
  void* d = TARRAY_GET_ELEM(pArray, index);
  
  return *(void**)d;
}

size_t starArrayGetSize(const SArray* pArray) { return pArray->size; }

void* starArrayInsert(SArray* pArray, size_t index, void* pData) {
  if (pArray == NULL || pData == NULL) {
    return NULL;
  }

  if (index >= pArray->size) {
    return starArrayPush(pArray, pData);
  }

  if (pArray->size >= pArray->capacity) {
    int32_t ret = starArrayResize(pArray);
    
    if (ret < 0) {
      return NULL;
    }
  }

  void* dst = TARRAY_GET_ELEM(pArray, index);

  int32_t remain = pArray->size - index;
  memmove(dst + pArray->elemSize, dst, pArray->elemSize * remain);
  memcpy(dst, pData, pArray->elemSize);

  pArray->size += 1;
  
  return dst;
}

void starArrayRemove(SArray* pArray, size_t index) {
  assert(index < pArray->size);
  
  if (index == pArray->size - 1) {
    starArrayPop(pArray);
    return;
  }
  
  size_t remain = pArray->size - index - 1;
  memmove(pArray->pData + index * pArray->elemSize, pArray->pData + (index + 1) * pArray->elemSize, remain * pArray->elemSize);
  pArray->size -= 1;
}

void starArrayCopy(SArray* pDst, const SArray* pSrc) {
  assert(pSrc != NULL && pDst != NULL);
  
  if (pDst->capacity < pSrc->size) {
    void* pData = realloc(pDst->pData, pSrc->size * pSrc->elemSize);
    if (pData == NULL) { // todo handle oom
    
    } else {
      pDst->pData = pData;
      pDst->capacity = pSrc->size;
    }
  }
  
  memcpy(pDst->pData, pSrc->pData, pSrc->elemSize * pSrc->size);
  pDst->elemSize = pSrc->elemSize;
  pDst->capacity = pSrc->size;
  pDst->size = pSrc->size;
}

SArray* starArrayClone(const SArray* pSrc) {
  assert(pSrc != NULL);
  
  if (pSrc->size == 0) { // empty array list
    return starArrayInit(8, pSrc->elemSize);
  }
  
  SArray* dst = starArrayInit(pSrc->size, pSrc->elemSize);
  
  memcpy(dst->pData, pSrc->pData, pSrc->elemSize * pSrc->size);
  dst->size = pSrc->size;
  return dst;
}

void starArrayClear(SArray* pArray) {
  assert( pArray != NULL );
  pArray->size = 0;
}

void starArrayDestroy(SArray* pArray) {
  if (pArray == NULL) {
    return;
  }

  free(pArray->pData);
  free(pArray);
}

void starArraySort(SArray* pArray, int (*compar)(const void*, const void*)) {
  assert(pArray != NULL);
  assert(compar != NULL);

  qsort(pArray->pData, pArray->size, pArray->elemSize, compar);
}

void* starArraySearch(const SArray* pArray, const void* key, __compar_fn_t comparFn) {
  assert(pArray != NULL && comparFn != NULL);
  assert(key != NULL);

  return bsearch(key, pArray->pData, pArray->size, pArray->elemSize, comparFn);
}

void starArraySortString(SArray* pArray, __compar_fn_t comparFn) {
  assert(pArray != NULL);
  qsort(pArray->pData, pArray->size, pArray->elemSize, comparFn);
}

char* starArraySearchString(const SArray* pArray, const char* key, __compar_fn_t comparFn) {
  assert(pArray != NULL);
  assert(key != NULL);

  void* p = bsearch(&key, pArray->pData, pArray->size, pArray->elemSize, comparFn);
  if (p == NULL) {
    return NULL;
  }
  return *(char**)p;
}