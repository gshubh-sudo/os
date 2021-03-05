#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>


char *read_from_line();
char *change_dir();
void executing_code();
char* my_strcpy();
int process_launch();

// char *path;
int main(){
	char* hist[10];
	char path[150];
	strcpy(path,"$MTL4xx:/home/shubh");
	// printf("%s ",path);
	
	char* ins =read_from_line(path);

	// printf("you typed : %s",ins );
	executing_code(ins,path);
	printf("not prbls\n");
	return 0;
}

char* read_from_line(char* path){
	printf("%s $", path);
	char *buffer=NULL;

	size_t bufsize=32;
	size_t chars;

	buffer=(char *) malloc(bufsize * sizeof(char));
	if(buffer==NULL){
		printf("Buffer can't be allocated\n");
		return NULL;
	}

	chars=getline(&buffer,&bufsize,stdin);
	return buffer;
}

char* change_dir(char* buffer, char* path, char* newPath){


	// char newPath[(strlen(buffer)+strlen(path))*2];
	// printf("%d\n",(int)sizeof(newPath) );
	// printf("%d\n", (int)strlen(path));
	// printf("%d\n", (int)strlen(buffer));

	char* pathBuffer = (char*)malloc(sizeof(path));
	// printf("%s\n",path );
	strcpy(pathBuffer,path);
	// printf("this is pathbuffer %s\n",pathBuffer );

	char* p=strtok(buffer," ");

	p=strtok(NULL," ");
	if(p==NULL){
		printf("enter more arguments\n");
		return path;
	}

	p[strcspn(p,"\n")]=0;
	printf("%s\n", p);

	// printf("heck\n");


	if(strcmp("..",p)==0){
		// printf("no problem with the slash?\n");
		char* slash ="/";
		// printf("problem not here\n");
		p=strtok(path,"/");
		// printf("%s\n",p);

		char *q=strtok(NULL,"/");
		while(q!=NULL){
			p[strcspn(p,"\n")]=0;
			strcat(newPath,p);
			strcat(newPath,slash);
			p=q;
			// printf("%s\n",p );
			q=strtok(NULL,"/");
		}

	}

	else if(strcmp(".",p)==0){
		// printf("brrr\n");
		p=strtok(path,"/");
		strcpy(newPath,p);
		strcat(newPath,"/");
		// printf("%s\n", newPath);
	}

	else{
		// printf("what the fuck\n");
		// printf("%s\n",p );
		if(p[0]=='/'){
			// printf("this loop\n");
			char* q=strtok(path,"/");
			strcpy(newPath,strcat(q,p));
			newPath[strcspn(newPath,"\n")]=0;
			// printf("%s\n",newPath );
		}

		else{
			// printf("that loop\n");
			strcpy(newPath,strcat(path,"/"));
			// printf("%s\n", newPath);
			// printf("%d\n",(int)strlen(p) );
			strcat(newPath,p);
			newPath[strcspn(newPath,"\n")]=0;
			// printf("%s\n", newPath);
		}
	}

	char npath[strlen(newPath)+50];
	memset(npath,0,sizeof(npath));
	strcpy(npath,newPath);
	

	char *pixy= strchr(newPath,'/');
	char *pixy1=pixy;
	if(pixy!=NULL){
		pixy++;
	}

	// printf("%s\n",pixy );
	if(strcmp(newPath,"$MTL4xx:/")!=0){
		DIR* dir=opendir(pixy);
		DIR* dir1=opendir(pixy1);
		if(dir){
			closedir(dir);
		}
		else if(dir1){
			closedir(dir1);
		}
		else if(ENOENT==errno){
			printf("directory does not exist\n");

			// printf("%d\n",(int)sizeof(npath) );
			// printf("%d\n", (int)sizeof(pathBuffer));
			memset(npath,0,sizeof(npath));
			strcpy(npath,pathBuffer);
			// printf("%s\n",npath );
		}
		else{
			printf("cant open directory\n");
		}

	}
	strcpy(newPath,npath);
	printf("%s\n",newPath);
	// printf("problem is not here\n");
	return newPath;
}


void executing_code(char *buffer,char *path){

	printf("%s\n", path);

	printf("you typed : %s",buffer );
	// printf("bleh\n");

	//tempbuffer stores the buffer as strtok modifies it
	char* tempBuffer = (char*)malloc(sizeof(buffer));
	// printf("%d\n", (int)sizeof(buffer));
	strcpy(tempBuffer,buffer);

	//pathBuffer stores the temporary path
	char* pathBuffer = (char*)malloc(sizeof(path));
	strcpy(pathBuffer,path);

	//finds the command identifier
	char *p=strtok(buffer," ");
	p[strcspn(p,"\n")]=0;
	printf("%s\n", p);

	// printf("bleh\n");

	if(p==NULL){
		return ;
	}
	// printf("this is chill\n");

	//this is the echo command routine
	// if(strcmp("echo",p)==0){
	// 	p=strtok(NULL," ");
		

	// 	while(p!=NULL){
	// 		p[strcspn(p,"\n")]=0;
	// 		printf("%s ",p );
	// 		p=strtok(NULL," ");
	// 	}
	// 	printf("\n");
	// 	return;
	// }
	// printf("this is chill\n");


	if(strcmp("cd",p)==0){
		//we store the newpath in newPath
		// char* newPath=(char*)malloc(120*sizeof(char));
		char newPath[150];
		change_dir(tempBuffer,path, newPath);
		printf("%s\n",newPath );
		memset(path,0,sizeof(path));
		strcpy(path,newPath);
		return;
	}


	char **args=malloc(150 * sizeof(char *));
	int pos=0;
	printf("routine started\n");
	while(p!=NULL){
		printf("%s\n",p );
		p[strcspn(p,"\n")]=0;
		args[pos]=p;
		pos++;
		p=strtok(NULL," ");
	}
	args[pos]=NULL;
	process_launch(args);
	// printf("\n");

}

int process_launch(char **args){
	pid_t p1,wpid;

	p1=fork();
	int status;

	if(p1<0){
		printf("fork failed\n");
	}
	else if(p1==0){
		printf("child process launched\n");
		execvp(args[0],args);
		printf("this shouldn't print\n");
	}
	else{
		do {
      	wpid = waitpid(p1, &status, WUNTRACED);
    	}while(!WIFEXITED(status) && !WIFSIGNALED(status));
	}
	return 1;
}

