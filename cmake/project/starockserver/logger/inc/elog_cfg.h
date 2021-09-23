#ifndef _ELOG_CFG_H_
#define _ELOG_CFG_H_

/* enable log output. default open this macro */
#define ELOG_OUTPUT_ENABLE
/* enable log write file. default open this macro */
#define ELOG_FILE_ENABLE
/* enable flush file cache. default open this macro */
#define ELOG_FILE_FLUSH_CAHCE_ENABLE
/* setting static output log level */
#define ELOG_OUTPUT_LVL                      ELOG_LVL_DEBUG
/* enable assert check */
#define ELOG_ASSERT_ENABLE
/* buffer size for every line's log */
#define ELOG_LINE_BUF_SIZE                   40960
/* output line number max length */
#define ELOG_LINE_NUM_MAX_LEN                5
/* output filter's tag max length */
#define ELOG_FILTER_TAG_MAX_LEN              16
/* output filter's keyword max length */
#define ELOG_FILTER_KW_MAX_LEN               16
/* output filter's tag level max num */
#define ELOG_FILTER_TAG_LVL_MAX_NUM          5
/* output newline sign */
#define ELOG_NEWLINE_SIGN                    "\n"
/* enable log color */
#define ELOG_COLOR_ENABLE
/* enable asynchronous output mode */
#define ELOG_ASYNC_OUTPUT_ENABLE
/* the highest output level for async mode, other level will sync output */
#define ELOG_ASYNC_OUTPUT_LVL                ELOG_LVL_VERBOSE
/* buffer size for asynchronous output mode */
#define ELOG_ASYNC_OUTPUT_BUF_SIZE           (ELOG_LINE_BUF_SIZE * 100)
/* each asynchronous output's log which must end with newline sign */
#define ELOG_ASYNC_LINE_OUTPUT
/* asynchronous output mode using POSIX pthread implementation */
#define ELOG_ASYNC_OUTPUT_USING_PTHREAD

#endif /* _ELOG_CFG_H_ */
