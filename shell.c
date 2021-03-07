#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>

char *read_from_line();
char *change_dir();
void executing_code();
char* my_strcpy();
int process_launch();
void remove_all_chars(char *str, char c);
int finder();

char* mydir="MTL4xx~$";
int main(){
	char** hist=(char **)malloc(10*sizeof(char *));
	for(int i=0;i<10;i++){
		hist[i]=(char*)malloc(1500*sizeof(char));
	}
	int his=1;

	char inpath[1500];
	char currpath[2000];
	char *printpath=(char*)malloc(1500*sizeof(char));
	if(getcwd(inpath,1500)!=NULL){
	}
	printf("MTL4xx~ $ ");
	strcpy(currpath,inpath);
	int exit=1;
	do{
		int flag=0;
		char* ins =read_from_line(currpath);
		for(int i=0;i<strlen(ins);i++){
			if(!isspace(ins[i])){
				flag=1;
				break;
			}
		}
		if(!flag){
			printf("MTL4xx~");
			printf("%s",printpath );
			printf(" $" );	
			continue;
		}
		//the history routine

		for(int i=his-1;i>0;i--){
			memset(hist[i],0,1500*sizeof(char));
			strcpy(hist[i],hist[i-1]);
		}
		memset(hist[0],0,1500*sizeof(char));

		strcpy(hist[0],ins);

		if(strcmp(hist[0],"history\n")==0){
			for(int i=0;i<his;i++){
				printf("%s",hist[i] );
			}
			printf("MTL4xx~");
			printf("%s",printpath );
			printf(" $" );
			continue;
		}
		if(his<10){
			his++;
		}

		executing_code(ins,currpath,inpath);
		int k=finder(inpath,currpath,printpath);
		if(k==0){
			printpath="";
		}
		else if(k==-1){
			printpath=currpath;
		}
		else{
			printpath=currpath+k;	
		}
		printf("MTL4xx~");
		printf("%s",printpath );
		printf(" $" );	
	}while(exit);	
	return 0;
}

char* read_from_line(char* path){
	// printf("%s $", path);
	char *buffer=NULL;

	size_t bufsize=1500;
	size_t chars;

	buffer=(char *) malloc(bufsize * sizeof(char));
	if(buffer==NULL){
		printf("Buffer can't be allocated\n");
		return NULL;
	}

	chars=getline(&buffer,&bufsize,stdin);
	return buffer;
}

void remove_all_chars(char* str, char c){
	// printf("%s\n",str );
	char *p1=str;
	char *p2=str;

	while(*p1){
		*p2=*p1;
		*p1++;
		if(*p2==c){
			continue;
		}
		else *p2++;
	}
	*p2='\0';
	// printf("%s\n",str );
	return;
	
}

char* change_dir(char* buffer, char* path, char* newPath, char* inpath){
	// printf("here\n");

	char* pathBuffer = (char*)malloc(1000);
	memset(newPath,0,1500*sizeof(char));
	strcpy(pathBuffer,path);

	char* p=strtok(buffer," ");
	p=strtok(NULL,"\n");
	if(p==NULL){
		printf("enter more arguments\n");
		strcpy(newPath,path);
		return path;
	}

	p[strcspn(p,"\n")]=0;
	if(strcmp("..",p)==0){
		p=strtok(path,"/");
		char *q=strtok(NULL,"/");
		while(q!=NULL){
			p[strcspn(p,"\n")]=0;
			strcat(newPath,"/");
			strcat(newPath,p);
			p=q;
			q=strtok(NULL,"/");
		}
	}
	else if(strcmp("~",p)==0){
		strcpy(newPath,inpath);}
	else{
		if(p[0]=='/'){
			strcpy(newPath,p);
			newPath[strcspn(newPath,"\n")]=0;
		}

		else{
			strcpy(newPath,strcat(path,"/"));
			strcat(newPath,p);
			newPath[strcspn(newPath,"\n")]=0;
		}
	}

	char npath[1500];
	memset(npath,0,sizeof(npath));
	strcpy(npath,newPath);

	char *pixy= strchr(newPath,'/');
	remove_all_chars(pixy,'\"');
	remove_all_chars(pixy,'\'');
	DIR* dir=opendir(pixy);
	if(dir){
		closedir(dir);
	}
	else if(ENOENT==errno){
		printf("%s directory does not exist\n", pixy);
		memset(npath,0,sizeof(npath));
		strcpy(npath,pathBuffer);
	}
	else{
		printf("cant open directory\n");
	}

	strcpy(newPath,npath);
	remove_all_chars(newPath,'\"');
	remove_all_chars(pixy,'\'');
	return newPath;
}

int finder(char* inpath, char* currpath, char* printpath){
	
	if(strcmp(inpath,currpath)==0){
		// printf("this?\n");
		printpath="";
		return 0;
	}
	int i=1;
	if(strlen(currpath)<strlen(inpath)){
		// printf("or this?\n");
		printpath=currpath;
		return -1;
	}
	else{
		char* s=inpath,*s1=currpath;
		int n=strlen(inpath);
		int j=0;
		while(j<n){
			if(inpath[j]!=currpath[j]){			
				i=0;
			}
			j++;
		}
		if(i){
			printpath=(s1+i);
			return j;
		}
		else{
			printpath=currpath;
			return -1;
		}
	}
}

void executing_code(char *buffer,char *path,char* inpath){

	char tempBuffer[1500];
	memset(tempBuffer,0,1500*sizeof(char));
	strcpy(tempBuffer,buffer);

	//pathBuffer stores the temporary path
	char* pathBuffer = (char*)malloc(1500);
	memset(pathBuffer,0,1500*sizeof(char));
	strcpy(pathBuffer,path);

	char *p=strtok(buffer," ");
	if(strcmp(p,"\n")==0){
		return;
	}
	p[strcspn(p,"\n")]=0;
	if(p==NULL){
		return ;
	}
	if(strcmp("cd",p)==0){
		char newPath[1500];
		change_dir(tempBuffer,path, newPath,inpath);
		memset(path,0,sizeof(path));
		strcpy(path,newPath);
		memset(newPath,0,sizeof(newPath));
		chdir(path);
		return;
	}

	tempBuffer[strcspn(tempBuffer,"\n")]=0;
	strcat(tempBuffer," ");

	char **args=malloc(1500 * sizeof(char *));
	int pos=0;
	char *q=(char*)malloc(150*sizeof(char));

	int n=strlen(tempBuffer);
	int i=0;
	int quotFlag=0;

	while(i<n){
		if(tempBuffer[i]=='\"'){
				quotFlag=1-quotFlag;
		}
		else if(isspace(tempBuffer[i])&& quotFlag==0){
			if(strcmp(q,"")==0){
				i++;
				continue;
			}
			args[pos]=(char*)malloc(150*sizeof(char));
			strcpy(args[pos],q);
			pos++;
			free(q);
			q=(char*)malloc(150*sizeof(char));
		}
		else{
			char p[2];
			p[0]=tempBuffer[i];
			p[1]='\0';
			strcat(q,p);
		}
		i++;	
	}
	free(q);
	args[pos]=NULL;
	process_launch(args);
	memset(tempBuffer,0,sizeof(tempBuffer));
	free(args);
	free(pathBuffer);
}

int process_launch(char **args){
	// printf("process_launch\n");
	pid_t rc=fork();
	if(rc<0){
		printf("fork failed, try again\n");
	}
	else if(rc==0){
		// printf("inside the child\n");
		// printf("child process launched\n");
		if(execvp(args[0],args)<0){
			printf("cannot execute the command\n");
			exit(0);
		}
		// execvp(args[0],args);
		
	}
	else{
		int rc_wait=wait(&rc);
		// printf("this is the parent\n");
		
	}
	return 1;
}

