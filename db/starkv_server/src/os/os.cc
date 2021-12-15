#include "os.h"
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/detail/classification.hpp>
using std::string;
using std::vector;
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
bool Split(string& splited_str, const string& split_ch, vector<string>& split_vec) {
    vector<string> split_tmp_vec;
    split_vec.clear();
    boost::split(split_tmp_vec, splited_str, boost::is_any_of(split_ch));
    for (vector<string>::iterator cur_iter = split_tmp_vec.begin();
         cur_iter != split_tmp_vec.end();
         ++cur_iter) {
        string& cur_str = *cur_iter;
        if (" " == cur_str || "" == cur_str) {
            continue;
        }
        cur_str.erase(remove(cur_str.begin(), cur_str.end(), ' '), cur_str.end());
        split_vec.push_back(cur_str);
    }
    return true;
}


bool Split(const char* splited_chs, const string& split_ch, vector<string>& split_vec) {
    string splited_str = splited_chs;
    return Split(splited_str, split_ch, split_vec);
}