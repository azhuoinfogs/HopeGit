#include "util.h"
void *nvme_alloc(size_t len, int base_size, bool *huge)
{
	void *p;
	if (!posix_memalign(&p, base_size, len)) {
		*huge = false;
		memset(p, 0, len);
		return p;
	}
	return NULL;
}
void nvme_free(void *p, bool huge)
{
	free(p);
}
void d(unsigned char *buf, int len, int width, int group)
{
	int i, offset = 0, line_done = 0;
	char ascii[32 + 1];

	assert(width < sizeof(ascii));
	printf("     ");
	for (i = 0; i <= 15; i++)
		printf("%3x", i);
	for (i = 0; i < len; i++) {
		line_done = 0;
		if (i % width == 0)
			printf( "\n%04x:", offset);
		if (i % group == 0)
			printf( " %02x", buf[i]);
		else
			printf( "%02x", buf[i]);
		ascii[i % width] = (buf[i] >= '!' && buf[i] <= '~') ? buf[i] : '.';
		if (((i + 1) % width) == 0) {
			ascii[i % width + 1] = '\0';
			printf( " \"%.*s\"", width, ascii);
			offset += width;
			line_done = 1;
		}
	}
	if (!line_done) {
		unsigned b = width - (i % width);
		ascii[i % width + 1] = '\0';
		printf( " %*s \"%.*s\"",
				2 * b + b / group + (b % group ? 1 : 0), "",
				width, ascii);
	}
	printf( "\n");
}

void d_raw(unsigned char *buf, unsigned len)
{
	unsigned i;
	for (i = 0; i < len; i++)
		putchar(*(buf+i));
}
int get_nsid(char *res) {
	int i = 0;
	int found = 0;
    while (res[i] != '\0') {
        if (res[i] >= '0' && res[i] <= '9') {
            found = 1;
            break;
        }
        i++;
    }
	if (found)
		return (res[i] - '0');
	else
		return -1;
}
int tSystem(const char * cmd) 
{ 
  FILE * fp; 
  int res; 
  char buf[1024]; 
  if (cmd == NULL) { 
    printf("tSystem cmd is NULL!\n");
    return -1;
  } 
  
  if ((fp = popen(cmd, "r") ) == NULL) { 
    printf("popen cmd:%s error: %s/n", cmd, strerror(errno)); 
    return -1; 
  } else {
    while(fgets(buf, sizeof(buf), fp))  {
    } 

    if ((res = pclose(fp)) == -1) { 
      printf("close popen file pointer fp error!\n");
    } else {
		res = get_nsid(buf);
    }
    return res;
  }
}
int create_ns(char *devname, size_t lba_count) {
  if ((strlen(devname) == 0) || (lba_count == 0)) {
    printf("create_ns not enabled");
    return -1;
  }
  char shellCmd[1024+1] = {0};
  //(void)snprintf(shellCmd, 1024, "cp -rf %s %s", srcDir, destDir);
  (void)snprintf(shellCmd, 1024, "nvme create-ns %s -s %ld -c %ld -f 0 -m 0 -d 0", devname, lba_count, lba_count);
  int nsid = tSystem(shellCmd);
  if (nsid < 0) {
	printf("create_ns failed\n");
    return -1;
  }
  memset(shellCmd, 0, sizeof(shellCmd));
  (void)snprintf(shellCmd, 1024, "nvme attach-ns %s -c 0x1200 -n %d", devname, nsid);
  nsid = tSystem(shellCmd);
  if (nsid < 0) {
	printf("create_ns failed\n");
    return -1;
  }
  return nsid;
}
int delete_ns(char *devname, int nsid) {
  if ((strlen(devname) == 0)) {
    printf("delete_ns not enabled");
    return -1;
  }
  char shellCmd[1024+1] = {0};
  (void)snprintf(shellCmd, 1024, "nvme delete-ns %s -n %d", devname, nsid);
  nsid = tSystem(shellCmd);
  if (nsid < 0) {
	printf("delete_ns failed\n");
    return -1;
  }
  return nsid;
}

char **list_ns(int *num)
{
	FILE *stream;
	char line[256] = {0};
	char cmd[PATH_MAX] = "nvme list";
	stream = popen(cmd, "r");
	int row = 0;
	char *b = "/dev/";
	char **devices = malloc(sizeof(char *) * MAX_DEVICE_COUNT);
	if (!devices) {
		printf("malloc sizeof dbinfo failed.\n");
		goto errout;
	}
	for(int i = 0; i< MAX_DEVICE_COUNT; i++ ) {
		devices[i] = (char *)calloc(1, MAX_NAME_LEN);
	}
	while (fgets(line, sizeof(line), stream)) {
		char *dev = strstr(line, b);
		if (dev) {
			strncpy(devices[row], dev, 12);
			row++;
		}
	}
	pclose(stream);
	*num = row;
	return devices;
errout:
	return NULL;
}