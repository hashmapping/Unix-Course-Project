#include <bits/stdc++.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include "list.h"
using namespace std;


int main( int argc, char *argv[])
{

	get_opt(argc,argv);
	get_file_type(argv[optind]);
	/*
	//	
	cout<<argc<<" ";
	for( int i=0; i<argc; i++ ){
		cout<<argv[i]<<endl;
	}*/
	
	/*
	//how to use the function chdir
	char path[maxlen];
	getcwd(path,maxlen);
	printf("current path is : %s\n",path);
	chdir( "/home/marco/django_pro" );
	getcwd(path,maxlen);
	printf("current path is : %s\n",path);
	*/
	
	return 0;
}  
  

void get_opt( int argc, char *argv[])
{
	char ch;
	while( (ch = getopt(argc,argv,"ral:h:m:")) != -1 ){
		switch(ch){
			case 'r':	flag.r = true; 
					//	printf("optopt=%c\n",optopt);
						break;
			case 'a':	flag.a = true; break;
			case 'l':	flag.l = true; 
						flag.l_arg = atol(optarg);	break;
			case 'h':	flag.h = true;
						flag.h_arg = atol(optarg);	break;
			case 'm':	flag.m = true;
						flag.m_arg = atol(optarg);	
			//			printf("optarg=%d\n",flag.m_arg);
						break;
			default	:	printf("unknown option\n"); break;
		}
	}
}


int print(char *path)
{

	struct stat st;
	time_t tp;
	char *filename = NULL;
	time(&tp);

	if( stat(path,&st) ){
		perror("lsat error");
		return -1;
	}

	if( (filename = strrchr(path,'/'))!=NULL ){
		filename++; //name shouldn't include'/'
	}else{
		filename = path;
	}

	if( S_ISDIR(st.st_mode) || S_ISREG(st.st_mode) ){
		if( ( !flag.l || (flag.l && (st.st_size>=flag.l_arg)) )&&
			( !flag.h || (flag.h && (st.st_size<=flag.h_arg)) )&&
				( !flag.m || (flag.m && (tp-st.st_mtime<=flag.m_arg*24*3600)))&&
					( !flag.a || (flag.a && filename[0]=='.')) ) {
						printf("%s %10ld %s\n",(S_ISDIR(st.st_mode))?"DIR":"FILE",st.st_size,filename);
					}
	}

	


	return 0;
}

int get_file_type(char *path)
{
	char tmp[maxlen];
	if ( path == NULL || path[0]=='-' ){  //not a path but a arguement, so get current path
		path = tmp;
		getcwd(path,maxlen);
	}

	struct stat st;
	if( stat(path,&st) == -1 ){
		perror("stat error");
		return -1;  
	}

	if( S_ISDIR(st.st_mode) ){  //directory,then ls
		myls(path);
	}else if( S_ISREG(st.st_mode) ){	//regular,then print
		print(path);
	}else{
		printf("not directory or regular file\n");
	}
	return 0;
}

int myls(char *path)
{
	//printf("current path is : %s\n",path);
	print(path);
	DIR *dp = NULL;
	struct dirent *dirp = NULL;

	if( (dp=opendir(path))==NULL ){
		//err_sys("Error: can't open directory",path);
		fprintf(stderr, "Error: can't open directory %s\n",path );
		return -1;
	}
	//chdir(path):change current  direcroty,return a dir struct
	//readdir()
	chdir(path);
	if(!flag.r){
		while( (dirp=readdir(dp))!=NULL ){
			print(dirp->d_name);
		}
	}else{
		rewinddir(dp);
		while( (dirp = readdir(dp))!=NULL ){
			if( !strcmp( dirp->d_name,"." ) )continue;
			get_file_type(dirp->d_name);
		}
	}

	if( closedir(dp) ){
		fprintf(stderr, "Error:can't close the directory%s\n", path);
		return -1;
	}
	chdir("..");

	return 0;
}


