#ifndef LIST_H
#define LIST_H

const int maxlen = 255;

struct flag_s
{
	bool r,a,l,h,m;
	int l_arg,h_arg,m_arg;
}flag;

/*
l_arg:minimum the file's size
h_arg:maximum the file's size
m_arg(=n):the latest update time must be in n days
*/

void get_opt(int argc, char *argv[]);
int print(char *path);
int get_file_type(char *path);
int myls(char *path);

#endif