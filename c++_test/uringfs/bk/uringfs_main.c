#define FUSE_USE_VERSION 26

#include <fuse.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/time.h>
#include <assert.h>
#include <libgen.h>
#include "uringfs.h"
static struct options {
	const char *filename;
	const char *contents;
	int show_help;
} options;

static void show_help(const char *progname)
{
	printf("usage: %s [options] <mountpoint>\n\n", progname);
	printf("File-system specific options:\n"
	       "    --name=<s>          Name of the \"hello\" file\n"
	       "                        (default: \"hello\")\n"
	       "    --contents=<s>      Contents \"hello\" file\n"
	       "                        (default \"Hello, World!\\n\")\n"
	       "\n");
};

struct fuse_operations ufs_oper = {
  .getattr      = ufs_getattr,
  .readlink     = ufs_readlink,
  .readdir      = ufs_readdir,  
  .getdir		= ufs_getdir,
  .mknod        = ufs_mknod,
  .mkdir        = ufs_mkdir,
  .symlink      = ufs_symlink,
  .unlink       = ufs_unlink,
  .rmdir        = ufs_rmdir,
  .rename       = ufs_rename,
  .link         = ufs_link,
  .chmod        = ufs_chmod,
  .chown        = ufs_chown,
  .truncate     = ufs_truncate,
  .utimens      = ufs_utimens,
  .open         = ufs_open,
  .read         = ufs_read,
  .write        = ufs_write,
  .release      = ufs_release
};
int main(int argc, char *argv[]) {
	// Initialize meta
//   Initialize root directory
	int ret;
	struct fuse_args args = FUSE_ARGS_INIT(argc, argv);

	/* Set defaults -- we have to use strdup so that
	   fuse_opt_parse can free the defaults if other
	   values are specified */
	options.filename = strdup("hello");
	options.contents = strdup("Hello World!\n");

	/* Parse options */
	if (fuse_opt_parse(&args, &options, NULL, NULL) == -1)
		return 1;

	/* When --help is specified, first print our own file-system
	   specific help text, then signal fuse_main to show
	   additional help (by adding `--help` to the options again)
	   without usage: line (by setting argv[0] to the empty
	   string) */
	if (options.show_help) {
		show_help(argv[0]);
		assert(fuse_opt_add_arg(&args, "--help") == 0);
		args.argv[0][0] = '\0';
	}
	ufs_create();
	ret = fuse_main(args.argc, args.argv, &ufs_oper, NULL);
	fuse_opt_free_args(&args);
}
