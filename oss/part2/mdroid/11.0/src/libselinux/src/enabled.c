#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <selinux/selinux_internal.h>  //STDLIBC_INTEGER
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <selinux/policy.h>  //STDLIBC_INTEGER

int is_selinux_enabled(void)
{
	/* init_selinuxmnt() gets called before this function. We
 	 * will assume that if a selinux file system is mounted, then
 	 * selinux is enabled. */
#ifdef ANDROID
	return (selinux_mnt ? 1 : 0);
#else
	return (selinux_mnt && has_selinux_config);
#endif
}

hidden_def(is_selinux_enabled)

/*
 * Function: is_selinux_mls_enabled()
 * Return:   1 on success
 *	     0 on failure
 */
int is_selinux_mls_enabled(void)
{
	char buf[20], path[PATH_MAX];
	int fd, ret, enabled = 0;

	if (!selinux_mnt)
		return enabled;

	snprintf(path, sizeof path, "%s/mls", selinux_mnt);
	fd = open(path, O_RDONLY | O_CLOEXEC);
	if (fd < 0)
		return enabled;

	memset(buf, 0, sizeof buf);

	do {
		ret = read(fd, buf, sizeof buf - 1);
	} while (ret < 0 && errno == EINTR);
	close(fd);
	if (ret < 0)
		return enabled;

	if (!strcmp(buf, "1"))
		enabled = 1;

	return enabled;
}

hidden_def(is_selinux_mls_enabled)
