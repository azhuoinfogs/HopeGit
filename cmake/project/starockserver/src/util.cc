#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <sys/time.h>
#include <regex.h>
#include <elog_file_cfg.h>
#include <elog.h>
#include "util.h"

#define LOG_TAG      "util.cc"

static void split_whole_name(const char *whole_name, char *fname, char *ext)
{
	const char *p_ext;
 
	p_ext = rindex(whole_name, '.');
	if (NULL != p_ext)
	{
		strcpy(ext, p_ext);
		snprintf(fname, p_ext - whole_name + 1, "%s", whole_name);
	}
	else
	{
		ext[0] = '\0';
		strcpy(fname, whole_name);
	}
}

void splitpath(const char *path, char *drive, char *dir, char *fname, char *ext)
{
	const char *p_whole_name;
 
	drive[0] = '\0';
	if (NULL == path)
	{
		dir[0] = '\0';
		fname[0] = '\0';
		ext[0] = '\0';
		return;
	}
 
	if ('/' == path[strlen(path)])
	{
		strcpy(dir, path);
		fname[0] = '\0';
		ext[0] = '\0';
		return;
	}
 
	p_whole_name = rindex(path, '/');
	if (NULL != p_whole_name)
	{
		p_whole_name++;
		split_whole_name(p_whole_name, fname, ext);
 
		snprintf(dir, p_whole_name - path, "%s", path);
	}
	else
	{
		split_whole_name(path, fname, ext);
		dir[0] = '\0';
	}
}

void elogInit(void)
{
    /* close printf buffer */
    // setbuf(stdout, NULL);
    /* initialize EasyLogger */
    elog_init();
    /* set EasyLogger log format */
    elog_set_fmt(ELOG_LVL_ASSERT, ELOG_FMT_ALL);
    elog_set_fmt(ELOG_LVL_ERROR, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
    elog_set_fmt(ELOG_LVL_WARN, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
    elog_set_fmt(ELOG_LVL_INFO, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
    elog_set_fmt(ELOG_LVL_DEBUG, ELOG_FMT_ALL & ~ELOG_FMT_FUNC);
    elog_set_fmt(ELOG_LVL_VERBOSE, ELOG_FMT_ALL & ~ELOG_FMT_FUNC);
    elog_set_text_color_enabled(true);
	// elog_async_enabled(true);

    char drive[128] = {0}, dir[128] = {0}, fname[128] = {0}, ext[128] = {0};
    splitpath(ELOG_FILE_NAME, drive, dir, fname, ext);
    if(-1 == access(dir, F_OK)){
        printf("The path '%s' does not exist, please create the directory manually\n", dir);
        exit(0);
    }
    /* start EasyLogger */
    elog_start();
}

//@return timestamp in millisecond
int64_t starGetTimestampMs() {
  struct timeval systemTime;
  gettimeofday(&systemTime, NULL);
  return (int64_t)systemTime.tv_sec * 1000L + (uint64_t)systemTime.tv_usec / 1000;
}

int regmatch(const char *pattern, const char *buf){
  int status;
  int flag=REG_EXTENDED;
  regmatch_t pmatch[1];
  const size_t nmatch=1;
  regex_t reg;
  //编译正则模式
  regcomp(&reg, pattern, flag);
  //执行正则表达式和缓存的比较
  status = regexec(&reg, buf, nmatch, pmatch, 0);
  //打印匹配的字符串
//   for(i=pmatch[0].rm_so;i<pmatch[0].rm_eo;++i){
//     putchar(buf[i]);
//   }
//   printf("\n");
  regfree(&reg);
  return status;
}

void del_char(char str[], char c)
{
   int i = 0, j = 0;
   while(str[i] != '\0')
   {
       if(str[i] != c){
            str[j++] = str[i++];
       }
       else{
            i++;
        }
   }
   str[j] = '\0';
}