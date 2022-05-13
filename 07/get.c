int get(int fd, long pos, char *buf, int n)	/* read n bytes from position pos */
{
	if (lseek(fd, pos, 0) == -1)	/* get to pos */
		return -1;
	else
		return read(fd, buf, n);
}
