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
      printf("popen result:%s", buf); 
    } 

    if ((res = pclose(fp)) == -1) { 
      printf("close popen file pointer fp error!\n");
    } else { 
      printf("popen res is :%d\n", res);
    }
    return res;
  }
}