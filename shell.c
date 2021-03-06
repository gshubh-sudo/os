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
void remove_all_chars(char *str, char c);
int finder();
// char *path;
char* mydir="MTL4xx~$";
int main(){
	char* hist[10];
	char inpath[1500];
	char currpath[2000];
	char *printpath=(char*)malloc(1500*sizeof(char));
	if(getcwd(inpath,1500)!=NULL){
		// printf("the current dir is%s\n",inpath );
	}
	printf("MTL4xx~ $ ");
	strcpy(currpath,inpath);
	int exit=1;
	do{
		char* ins =read_from_line(currpath);
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
		// printf("%d\n",k );
		// printf("inside the do while loop\n");
		// printf("%s\n",currpath );
		printf("MTL4xx~");
		printf("%s",printpath );
		printf(" $" );	
	}while(exit);
	
	return 0;
}

char* read_from_line(char* path){
	// printf("%s $", path);
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

void remove_all_chars(char* str, char c){
	char *pr=str, *pw=str;
	while(*pr){
		*pw=*pr++;
		pw+=(*pw!=c);
	}
	*pw='\0';
	return;
}


char* change_dir(char* buffer, char* path, char* newPath, char* inpath){

	// printf("here\n");
	char* pathBuffer = (char*)malloc(1000);
	// printf("%s\n",path );
	strcpy(pathBuffer,path);
	// printf("this is pathbuffer %s\n",pathBuffer );
	// printf("this is new path %s\n", newPath);
	// printf("this is currpath %s\n",path);

	char* p=strtok(buffer," ");

	p=strtok(NULL,"\n");
	if(p==NULL){
		printf("enter more arguments\n");
		strcpy(newPath,path);
		return path;
	}

	p[strcspn(p,"\n")]=0;
	// printf("%s\n", p);

	// printf("heck\n");


	if(strcmp("..",p)==0){
		char* slash ="/";
		// printf("%s\n",path );
		p=strtok(path,"/");
		// printf("%s\n",p);
		// printf("print ho gaya\n");
		// strcat(newPath,"/");
		// printf("%s\n",newPath );
		char *q=strtok(NULL,"/");
		while(q!=NULL){
			p[strcspn(p,"\n")]=0;
			strcat(newPath,slash);
			strcat(newPath,p);
			
			p=q;
			// printf("%s\n",p );
			q=strtok(NULL,"/");
		}
		// printf("%s\n",newPath );
	}

	else if(strcmp("~",p)==0){
		// printf("brrr\n");
		// printf("%s\n",path );
		// p=strtok(path,"/");
		// printf("%s\n",p );
		strcpy(newPath,inpath);
		// printf("newPath\n");
		// strcat(newPath,p);
		// printf("%s\n", newPath);
	}

	else{
		// printf("what the fuck\n");
		// printf("%s\n",p );
		if(p[0]=='/'){
			// printf("this loop\n");
			// printf("%s\n",p );
			strcpy(newPath,p);
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
	// printf("%s\n",newPath);
	// printf("%s\n",getcwd(s,200) );
	return newPath;
}

int finder(char* inpath, char* currpath, char* printpath){
	// printf("%s\n",inpath );
	// printf("%s\n",currpath );
	// printf("%s\n",printpath );

	if(strcmp(inpath,currpath)==0){
		// printf("this?\n");
		printpath="";
		return 0;
	}
	int i=1;
	// printf("%d\n",(int)strlen(currpath) );
	// printf("%d\n",(int)strlen(inpath) );

	if(strlen(currpath)<strlen(inpath)){
		// printf("or this?\n");
		printpath=currpath;
		return -1;
	}
	
	else{
		char* s=inpath,*s1=currpath;
		// printf("%s\n",s );
		// printf("%s\n",s1 );
		int n=strlen(inpath);
		// printf("%d\n", n);
		int j=0;

		while(j<n){

			if(inpath[j]!=currpath[j]){
				// printf("%c\n",inpath[j] );
				i=0;
			}
			j++;
		}
		if(i){
			// printf("this executes\n");
			// printf("%s\n",s1+i );
			printpath=(s1+i);
			// printf("%s\n",printpath );
			return j;
		}
		else{
			printpath=currpath;
			return -1;

		}
	}



}

void executing_code(char *buffer,char *path,char* inpath){

	// printf("%s\n", path);

	// printf("you typed : %s",buffer );
	// printf("bleh\n");

	//tempbuffer stores the buffer as strtok modifies it
	// char tempBuffer = (char*)malloc(1500);
	char tempBuffer[1500];
	// printf("%d\n", (int)sizeof(buffer));
	strcpy(tempBuffer,buffer);

	//pathBuffer stores the temporary path
	char* pathBuffer = (char*)malloc(1500);
	strcpy(pathBuffer,path);

	//finds the command identifier
	char *p=strtok(buffer," ");
	// printf("%s",p );
	if(strcmp(p,"\n")==0){
		// printf("empty string\n");
		return;
	}

	p[strcspn(p,"\n")]=0;
	// printf("%s\n", p);

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
		char newPath[1500];
		// printf("this is new path xx  %s\n",newPath);
		change_dir(tempBuffer,path, newPath,inpath);
		// printf("%s\n",newPath );
		memset(path,0,sizeof(path));
		strcpy(path,newPath);
		memset(newPath,0,sizeof(newPath));
		// printf("%s\n",path );
		char s[1500];
		chdir(path);
		// printf("%s\n",getcwd(s,200) );
		return;
	}


	tempBuffer[strcspn(tempBuffer,"\n")]=0;
	strcat(tempBuffer," ");
	// printf("%s\n",tempBuffer );

	char **args=malloc(1500 * sizeof(char *));
	int pos=0;
	char *q=(char*)malloc(150*sizeof(char));

	int n=strlen(tempBuffer);
	// printf("%d\n", n);
	int i=0;
	int quotFlag=0;

	while(i<n){
		if(tempBuffer[i]=='\"'){
				quotFlag=1-quotFlag;
		}

		else if(tempBuffer[i]==' ' && quotFlag==0){
			// printf("%s\n",q );
			// char *qn=(char*)malloc(150*sizeof(char));
			args[pos]=(char*)malloc(150*sizeof(char));
			strcpy(args[pos],q);
			// printf("%s\n",args[pos] );

			pos++;
			// printf("%s\n",args[pos-1] );
			// printf("%s\n",q );
			// memset(q,0,150*sizeof(char));
			free(q);
			q=(char*)malloc(150*sizeof(char));
			// printf("%s\n",args[pos-1] );

		}

		else{
			// printf("%c\n",tempBuffer[i] );
			char p[2];
			p[0]=tempBuffer[i];
			p[1]='\0';
			// printf("%s\n",p );
			strcat(q,p);
			// printf("%s\n",q );
		}
		i++;
		
	}
	free(q);
	// printf("routine started\n");
	// while(p!=NULL){
	// 	// printf("%s\n",p );
	// 	p[strcspn(p,"\n")]=0;
	// 	args[pos]=p;
	// 	pos++;
	// 	p=strtok(NULL," ");
	// }
	args[pos]=NULL;
	process_launch(args);
	// printf("\n");
	// free(tempBuffer);
	memset(tempBuffer,0,sizeof(tempBuffer));
	free(args);
	free(pathBuffer);

}



int process_launch(char **args){
	
	pid_t rc=fork();
	
	if(rc<0){
		printf("fork failed, try again\n");
	}
	else if(rc==0){
		// printf("child process launched\n");
		execvp(args[0],args);
		printf("cannot execute the command\n");
	}
	else{
		int rc_wait=wait(&rc);
	}
	return 1;
}

