#define LINUX_FILEOPT
#include"file.h"

int creatdir(char *dir)
{
	int i=0;
	int iRet;
	int iLen;
	char *pszDir;

	if(NULL==dir){
		return 0;
	}

	pszDir=strdup(dir);
	iLen=strlen(pszDir);

	for(i=0;i<iLen;i++){
		if(pszDir[i]=='\\'||pszDir[i]=='/'){
			pszDir[i]='\0';
			iRet=ACCESS(pszDir,0);
			if(iRet!=0){
				iRet=MKDIR(pszDir);
				if(iRet!=0)
		            return -1;
			}
			pszDir[i]='/';
		}
	}
	iRet=MKDIR(pszDir);

	return iRet;
}
#ifdef LINUX_FILEOPT
int L_operation_dir(char *str)
{
	DIR *dir;
	struct dirent *ptr;
	char filename[BUFSIZ]={0};

	dir=opendir(str);

	if(dir==NULL)
         creatdir(str);
	else{
        while((ptr=readdir(dir))!=NULL){
           if((!strcmp(".",ptr->d_name))||(!strcmp("..",ptr->d_name)))
			     continue;
		   else{
			printf("last data:%s\n",ptr->d_name);
            sprintf(filename,"%s/%s",str,ptr->d_name);
			printf("file:%s\n",filename);
            if(unlink(filename)<0){
				printf("delete %s failed\n",ptr->d_name);
				exit(1);
			  }
		   }
	    }
       closedir(dir);
	}
}

int L_display(char *str)
{
	int fd;
	char filename[BUFSIZ]={0};
	int i=0;
        while(1){    
		    sprintf(filename,"%s/img_%d.bmp",str,i);
            fd=open(filename,O_RDONLY);
			if(fd>0){
			    printf("%s\n",filename);
			}
			else
				break;
			++i;
		}
}

#elif WINDOWS_FILEOPT
int W_operation_dir(char *str)
{
	char cmd[30]={0};
	struct _finddata_t fa;
	long handle;

	if((handle = _findfirst(strcat(str,"*"),&fa))==-1L){
		printf("path does not exist...\n");
		creatdir(str);
		return 0;
	}
	else{
        sprintf(cmd,"del /s /q /f %s/*.bmp ",str);
		system(cmd);
	}
	_findclose(handle);
	return 0;
}
int W_display(char *str)
{
	int fd;
	char filename[BUFSIZ]={0};
    while(1){
	 sprintf(filename,"%s/img_%d.bmp",str,i);
	 fd=_open(filename,_O_RDONLY);
	 if(fd>0){
		 printf("%s\n",filename);
	 }
	 else
		 break;
	++i;
  }
	
}
#endif

