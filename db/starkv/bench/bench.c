#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <sys/time.h>
#include <unistd.h>
#include "../include/starkv.h"

#define TOLOG   (1)
#define KSIZE	16
#define VSIZE	80
#define V       "1.8"
#define LINE    "+---------------------------+----------------+-----------------------------+--------------------+\n"
#define LINE1    "+---------------------------+----------------+-----------------------------+--------------------+\n"

void _print_header(int count)
{
	double index_size = (double)((double)(KSIZE + 8 + 1) * count) / 1048576.0;
	double data_size = (double)((double)(VSIZE + 4) * count) / 1048576.0;
	printf("Keys:	   %d bytes each\n", KSIZE);
	printf("Values:	   %d bytes each\n", VSIZE);
	printf("Entries:   %d\n", count);
	printf("IndexSize: %.1f MB (estimated)\n", index_size);
	printf("DataSize:  %.1f MB (estimated)\n", data_size);
	printf(LINE1);
}

void _print_environment()
{
	time_t now = time(NULL);
	printf("Starkv:    version %s(LSM-Tree storage engine)\n", V);
	printf("Date:      %s", (char*)ctime(&now));
	
	int num_cpus = 0;
	char cpu_type[256] = {0};
	char cache_size[256] = {0};

	FILE* cpuinfo = fopen("/proc/cpuinfo", "r");
	if (cpuinfo) {
		char line[1024] = {0};
		while (fgets(line, sizeof line, cpuinfo) != NULL) {
			const char* sep = strchr(line, ':');
			if (sep == NULL || strlen(sep) < 10)
				continue;

			char key[1024] = {0};
			char value[1024] = {0};
			strncpy(key, line, sep-1-line);
			strncpy(value, sep+1, strlen(sep)-1);
			if (strcmp("model name", key) == 0) {
				num_cpus++;
				strcpy(cpu_type, value);
			} else if (strcmp("cache size", key) == 0) {
				strncpy(cache_size, value+1, strlen(value) - 1);
			}
		}
		fclose(cpuinfo);
		printf("CPU:       %d * %s", num_cpus, cpu_type);
		printf("CPUCache:  %s\n", cache_size);
	}
}

long long _ustime(void)
{
	struct timeval tv;
	long long ust;
	gettimeofday(&tv, NULL);
	ust = ((long long)tv.tv_sec)*1000000;
	ust += tv.tv_usec;
	return ust / 1000000;
}

void _random_key(char* key, int length) 
{
	char salt[36] = "abcdefghijklmnopqrstuvwxyz0123456789";
	int i;
	memset(key, 0, length);
	for(i = 0; i < length; i++)
		key[i] = salt[rand() % 36];
}

void _write_test(starkv_t *idx, long int count)
{
	int i;
	double cost;
	long long start, end;

	char key[KSIZE];
	char val[VSIZE];
	unsigned char *sv = NULL;
	size_t val_len = 0;
	char *err = NULL;
	printf("count : %ld\n", count);
	start = _ustime();
	for (i = 0; i < count; i++) {
		if (i < 1000) {
			snprintf(key, KSIZE, "key:%d", i);
			snprintf(val, VSIZE, "val:%d", i);
		} else {
			_random_key(key, KSIZE);
			_random_key(val, VSIZE);
		}
		unsigned char *keybuf =  calloc(1,strlen(key));
		memcpy(keybuf, key, strlen(key));
		size_t keybuf_len = strlen(key);

		unsigned char *valbuf =  calloc(1,strlen(val));
		memcpy(valbuf, val, strlen(val));
		size_t valbuf_len = strlen(val);

		starkv_put(idx, keybuf, keybuf_len, valbuf, valbuf_len, &err);
		int ret = starkv_get(idx, keybuf, keybuf_len, &sv, &val_len, &err);
		if(sv) {
			// printf("%s--%d\n", sv, val_len);
			free(sv);
			sv = NULL;
		} else {
			printf("%s is not found\n", keybuf);
		}
		free(keybuf);
		free(valbuf);
	}
	end = _ustime();
	cost = end - start;
	printf(LINE);
	printf("|Random-Write	(done:%ld): %.6f sec/op; %.1f writes/sec(estimates); cost:%.3f(sec)\n",
			count,
			(double)(cost / count),
			(double)(count / cost),
			cost);
}

void _read_test(starkv_t *idx,long int count)
{
	int hint_count = 0;
	int i;
	double cost;
	char key[KSIZE];
	long long start, end;
	size_t val_len = 0;
	char *err = NULL;
	unsigned char *sv;
	int ret;
	start = _ustime();
	for(i = 0; i < count; i++) {
		snprintf(key, KSIZE, "key:%d", i);
		unsigned char *keybuf =  calloc(1, strlen(key));
		size_t keybuf_len = strlen(key);
		memcpy(keybuf, key, strlen(key));
		int ret = starkv_get(idx, keybuf, keybuf_len, &sv, &val_len, &err);
		if(sv) {
			printf("%s-%s", key, sv);
			free(sv);
			sv = NULL;
		}
		free(keybuf);
	}
	end = _ustime();
	cost = end - start;
	printf(LINE);
	printf("|Random-Read  (done:%ld): %.6f sec/op; %.1f reads/sec(estimated); cost:%.3f(sec)\n",
			count,
			(double)(cost/count),
			(double)(count/cost),
			cost);
	printf("hint_count : %d\n", hint_count );
}

void _check_test(starkv_t *dev, int count)
{
	unsigned char *sv;
	size_t val_len = 0;
	char *err = NULL;
	int ret = 0;
	char key[KSIZE];
	char val[VSIZE];
	for (int i = 0; i < count; i++) {
		_random_key(key, KSIZE);
		_random_key(val, VSIZE);
		unsigned char *keybuf =  calloc(1,strlen(key));
		memcpy(keybuf, key, strlen(key));
		size_t keybuf_len = strlen(key);
		unsigned char *valbuf =  calloc(1,strlen(val));
		memcpy(valbuf, val, strlen(val));
		size_t valbuf_len = strlen(val);
		starkv_put(dev, keybuf, keybuf_len, valbuf, valbuf_len, &err);
		ret = starkv_get(dev, keybuf,keybuf_len, &sv, &val_len, &err);
		if (ret==0 && val_len > 0) {
			// printf("Get Key:<%s>--->value is :<%s>", key, sv);
			free(sv);
		} else {
			printf("Get Key:<%s>, but value is NULL", key);
		}
		free(keybuf);
		free(valbuf);
	}
}

int main(int argc, char* argv[])
{
	long int count;
	_print_environment();
	srand(time(NULL));
	if(argc != 3) {
		fprintf(stderr, "Usage: bench <op: write | read> <count>\n");
		exit(1);
	}
	starkv_t *idx;
	int create_as_new = 1;
	idx = starkv_open("/dev/nvme0n1");
	starkv_init(idx);
	if (strcmp(argv[1], "write") == 0) {
		count = atoi(argv[2]);
		_print_header(count);
		_write_test(idx, count);
	} else if (strcmp(argv[1], "read") == 0) {
		count = atoi(argv[2]);
		_print_header(count);
		_read_test(idx, count);
	} else if (strcmp(argv[1], "check") == 0) {
		count = atoi(argv[2]);
		_check_test(idx, count);
	} else {
		fprintf(stderr, "Usage: bench <op: write | read> <count>\n");
		exit(1);
	}
	starkv_close(idx);
	starkv_cleanup(idx);
	return 1;
}
