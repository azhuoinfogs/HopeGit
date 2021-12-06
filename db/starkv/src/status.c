
#include "kv_store.h"

const char *status_to_string(uint32_t status)
{
	switch (status & 0x3ff) {
	case KV_STORE_SUCCESS:
		return "SUCCESS: The command completed successfully";
	case KV_STORE_ERROR:
		return "Failed: Malloc failed";
	case KV_STORE_INVALID_DEV:
		return "Failed: The dev is invalid";
	case KV_STORE_INVALID_PARAMETER:
		return "Failed: The parameter is not valid";
	case KV_STORE_INVALID_MALLOC:
		return "Failed: Malloc failed";
	case KV_STORE_ERROR_SKIPLIST:
		return "Failed: Error while using skiplist";
	case KV_STORE_INVALID_DB:
		return "Failed: The DB is invalid";
	case KV_STORE_ERROR_WRITE:
		return "Failed: write error";
	case KV_STORE_ERROR_READ:
		return "Failed: write error";
	case KV_STORE_INVALID_KEY:
		return "Failed: The key is invalid";
	case KV_STORE_INVALID_BLOCK:
		return "Failed: The Block is invalid";
	case KV_STORE_ERROR_BITMAP:
		return "Failed: The Bitmap is invalid";
	default:
		return "Unknown Error";
	}
}
bool SaveError(char** errptr, uint32_t status) {
  if ((status & 0x3ff) == KV_STORE_SUCCESS) {
    return false;
  } else if (*errptr == NULL) {
    *errptr = strdup(status_to_string(status));
  } else {
    free(*errptr);
    *errptr = strdup(status_to_string(status));
  }
  return true;
}