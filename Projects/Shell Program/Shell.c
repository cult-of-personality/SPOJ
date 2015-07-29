/*  For single pipes only yet.
    Assume atleast one space on either side of a comma.
    Total no of pipes & pipes that have come.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define COMLEN 1999
#define READ 0
#define WRITE 1

int currentPipe[4];
int totalPipe[4];
int p[2][2];
int commandStatus[2];
pid_t cPid; //pid of child which will execute the next command.
char *pathVar;
char **envVar;

void countPipes(char *command)
{
    int i,len;
    for(i=0;i<4;i++)
        totalPipe[i] = 0;
    len = strlen(command);
    i=0;
    while(i<len)
    {
        if(command[i]!='|')
            i++;
        else if(i<len-1 && command[i]=='|' && command[i+1]!='|'){
            totalPipe[1]++;
            i++;
        }
        else if(i<len-2 && command[i]=='|' && command[i+1]=='|' && command[i+2]!='|'){
            totalPipe[2]++;
            i+=2;
        }
        else if(i<len-3 && command[i]=='|' && command[i+1]=='|' && command[i+2]=='|' && command[i+3]!='|'){
            totalPipe[3]++;
            i+=3;
        }
    }
}

int searchInPath(char *command,char fullpath[])
{
    char *token;
    char tempPath[250];
    token = strtok(pathVar,":");
    while(token!=NULL){
        strcpy(tempPath,token);
        strcat(tempPath,"/");
        strcat(tempPath,command);
        if(access(tempPath,F_OK)==0){   //file exists
            if(access(tempPath,X_OK)==0){    //is executable
                strcpy(fullpath,tempPath);
                return 1;
            }
            else{
                strcpy(fullpath,tempPath);
                return 2;
            }
        }
        token = strtok(NULL,":");
    }
    return -1;
}

void executeCommand(char *tokens[],int noTokens,int flag,int in,int out,int app,char *input,char *output)
{
    /*printf("Inside executeCommand\n");
    printf("Tokens:-\n");*/
    int i;
    /*for(i=0;i<noTokens;i++)
        printf("Token %d: %s\n",i,tokens[i]);
    printf("Flag = %d\nInFlag = %d\nOutFlag = %d\nAppFlag = %d\n",flag,in,out,app);*/
    close(commandStatus[READ]);
    for(i=0;i<4;i++){
        if(currentPipe[i]!=0) //this is the first command, keep stdin
            break;  }
    if(i!=4){   //not the first command.
        //printf("%s not first\n",tokens[0]);
        if(in==0){  // not file redirected.
            close(p[flag%2][WRITE]);
            dup2(p[flag%2][READ],READ);
            close(p[flag%2][READ]);
            //printf("Stdin to pipe %d\n",flag%2);
        }
        else if(in==1){ //input redirection.
            int fd = open(input,O_RDONLY,0600);
            dup2(fd,READ);
            close(fd);
            //printf("Stdin to file\n");
            char str[1000];
            int len = 1000;
            close(p[flag%2][WRITE]);
            while(read(p[flag%2][READ],str,len) > 0);//also flush p[flag%2]
        }
    }
    else if(in==1){ //first command.
        int fd = open(input,O_RDONLY,0600);
        dup2(fd,READ);
        //printf("Stdin to file\n");
    }
    
    for(i=0;i<4;i++){
        if(currentPipe[i] != totalPipe[i])//for all i, this is the last command, keep stdout
            break;  
    }
    if(i!=4){   // not the last command.
        //printf("%s Not last\n",tokens[0]);
        if(out==0 && app==0){   //no output redirection 
            close(p[(flag+1)%2][READ]);
            dup2(p[(flag+1)%2][WRITE],WRITE);
            close(p[(flag+1)%2][WRITE]);
            //printf("Stdout to pipe %d\n",(flag+1)%2);
        }
        else if(app==1){
            int fd = open(output,O_CREAT|O_APPEND|O_WRONLY,0600);
            if(fd==-1)
                fd = open(output,O_CREAT|O_RDONLY,0600);
            dup2(fd,WRITE);
            close(fd);
            //printf("Stdout to file\n");
        }
        else if(out==1){
            int fd = open(output,O_CREAT|O_WRONLY,0600);
            dup2(fd,WRITE);
            close(fd);
            //printf("Stdout to file\n");
        }
    }
    else if(app==1){   //last command, output redirected.
        int fd = open(output,O_CREAT|O_APPEND|O_WRONLY,0600);
        if(fd==-1)
            fd = open(output,O_CREAT|O_RDONLY,0600);
        dup2(fd,WRITE);
        close(fd);
        //printf("Stdout to file\n");
    }
    else if(out==1){
        int fd = open(output,O_CREAT|O_WRONLY,0600);
        dup2(fd,WRITE);
        close(fd);
        //printf("Stdout to file\n");
    }
    char **args = tokens;  //separate arguments from redirections.
    char error[100];
    //printf("%s Reached execution stage\n",args[0]);
    if(args[0][0]!='.'){    //not in current directory
        char fullPath[250];
        int ret = searchInPath(args[0],fullPath);
        if(ret==-1){//command does not exist.
            sprintf(error,"Command %s does not exist\n",args[0]);
            write(commandStatus[WRITE],error,strlen(error));
            return;
        }
        else if(ret==1){
            execve(fullPath,args,envVar);
            sprintf(error,"Command %s could not execute\n",args[0]);
            write(commandStatus[WRITE],error,strlen(error));
            return;
        }
        else if(ret==2){
            sprintf(error,"Command %s is not executable\n",args[0]);
            write(commandStatus[WRITE],error,strlen(error));
            return;
        }
    }
    else{
        execve(args[0],args,envVar);
        sprintf(error,"Command %s could not execute\n",args[0]);
        write(commandStatus[WRITE],error,strlen(error));
        return;
    }
}

void correct(char *arr[],int size)
{
    char *copy[size];
    int i,j=0;
    for(i=0;i<size;i++){
        if(arr[i])
            copy[j++] = arr[i];
        arr[i] = NULL;
    }
    for(i=0;i<j;i++)
        arr[i] = copy[i];
}

void executeSingle(char *tokens[],int count,int flag)
{
    //printf("Inside executeSingle\n");
    int inFlag=0,outFlag=0,appFlag=0,i,inIdx=-1,outIdx=-1,appIdx=-1;
    char in[] = "<\0";
    char out[] = ">\0";
    char app[] = ">>\0";
    char input[100];
    char output[100];
    char *inSend = NULL,*outSend = NULL;
    for(i=0;i<count;i++){
        if(strcmp(tokens[i],in)==0){
            inFlag = 1;
            inIdx = i;
        }
        else if(strcmp(tokens[i],out)==0){
            outFlag = 1;
            outIdx = i;
        }
        else if(strcmp(tokens[i],app)==0){
            appFlag = 1;
            appIdx = i;
        }
    }
    if(inFlag){
        if(inIdx!=-1){
            strcpy(input,tokens[inIdx+1]);
            inSend = input;
            tokens[inIdx] = NULL;
            tokens[inIdx+1] = NULL;
            correct(tokens,count);
            count-=2;
        }
        else inFlag = 0;
    }
    if(outFlag){
        if(outIdx!=-1){
            if(inIdx!=-1){  //input redirection was also present
                for(i=0;i<count;i++){
                    if(strcmp(tokens[i],out)==0){
                        outIdx = i;
                        break;  }
                }
            }
            strcpy(output,tokens[outIdx+1]);
            outSend = output;
            tokens[outIdx] = NULL;
            tokens[outIdx+1] = NULL;
            correct(tokens,count);
            count-=2;
        }
        else outFlag = 0;
    }
    if(appFlag){
        if(appIdx!=-1){
            if(inIdx!=-1 || outIdx!=-1){
                for(i=0;i<count;i++){
                    if(strcmp(tokens[i],app)==0){
                        appIdx = i;
                        break;  }
                }
            }
            strcpy(output,tokens[appIdx+1]);
            outSend = output;
            tokens[appIdx] = NULL;
            tokens[appIdx+1] = NULL;
            correct(tokens,count);
            count-=2;
        }
        else appFlag = 0;
    }
    executeCommand(tokens,count,flag,inFlag,outFlag,appFlag,inSend,outSend);
}

void executeDouble(char *tokens[],int count,int flag)
{
    int status,i,j; //need to fork twice again.
    int cIdx=-1;   //position of comma.
    pid_t a;
    char comma[] = ",\0";
    for(i=0;i<count;i++){
        if(strcmp(tokens[i],comma)==0){
            cIdx = i;
            break;
        }
    }
    char input[4100];
    int bytes = read(p[flag%2][READ],input,4100);   //store common input for both.
    pipe(p[flag%2]);
    write(p[flag%2][WRITE],input,bytes);
    close(p[flag%2][WRITE]);
    //fork again.
    if((a = fork())==0){
        char *c[75] = {NULL};
        for(i=0;i<cIdx;i++)
            c[i] = tokens[i];
        executeSingle(c,cIdx,flag);
    }
    wait(&status);
    //check status pipe
    pipe(p[flag%2]);
    write(p[flag%2][WRITE],input,bytes);
    close(p[flag%2][WRITE]);
    if((a = fork())==0){
        char *c[75] = {NULL};
        j=0;
        for(i=cIdx+1;i<count;i++)
            c[j++] = tokens[i];
        executeSingle(c,j,flag);
    }
    wait(&status);
    //check status pipe.
}

void executeTriple(char *tokens[],int count,int flag)
{
    int status,i,j; //need to fork thrice again.
    int x=-1,y=-1;   //position of comma.
    pid_t a;
    char comma[] = ",\0";
    for(i=0;i<count;i++){
        if(strcmp(tokens[i],comma)==0){
            if(x==-1)
                x = i;
            else{
                y = i;
                break;
            }
        }
    }
    char input[4100];
    int bytes = read(p[flag%2][READ],input,4100);   //store common input for both.
    pipe(p[flag%2]);
    write(p[flag%2][WRITE],input,bytes);
    close(p[flag%2][WRITE]);
    //fork again.
    if((a = fork())==0){
        char *c[75] = {NULL};
        for(i=0;i<x;i++)
            c[i] = tokens[i];
        executeSingle(c,x,flag);
    }
    wait(&status);
    pipe(p[flag%2]);
    write(p[flag%2][WRITE],input,bytes);
    close(p[flag%2][WRITE]);
    if((a = fork())==0){
        char *c[75] = {NULL};
        j=0;
        for(i=x+1;i<y;i++)
            c[j++] = tokens[i];
        executeSingle(c,j,flag);
    }
    wait(&status);
    pipe(p[flag%2]);
    write(p[flag%2][WRITE],input,bytes);
    close(p[flag%2][WRITE]);
    if((a = fork())==0){
        char *c[75] = {NULL};
        j=0;
        for(i=y+1;i<count;i++)
            c[j++] = tokens[i];
        executeSingle(c,j,flag);
    }
    wait(&status);
}

int main(int argc,char *argv[],char **env)
{
    envVar = env;
    pathVar = getenv("PATH");
    int sFlag,dFlag,tFlag,tkidx,i;
    //int inFlag,outFlag,appFlag;
    int status,flag,count;
    char command[2000];
    char *tkcom[100];
    char *token;
    char delim[] = " \t";
    char singlePipe[] = "|\0";
    char doublePipe[] = "||\0";
    char triplePipe[] = "|||\0";
    char in[] = "<\0";
    char out[] = ">\0";
    char app[] = ">>\0";
    char error[500];
    //char comma[] = ",\0";
    while(1)
    {
        printf("$ ");
        if(fgets(command,COMLEN,stdin)==NULL)    //take input
            break;
        command[strlen(command)-1] = '\0';
        //printf("Command is: %s\n",command);
        tkidx = -1;
        count = 0;
        countPipes(command);
        pipe(p[0]);
        pipe(p[1]);
        pipe(commandStatus);
        close(commandStatus[WRITE]);    //you can only read a command's status.
        //printf("Pipes:-\n");
        for(i=0;i<4;i++){
            currentPipe[i] = 0;
            //printf("%d: %d\n",i,totalPipe[i]);
        }
        sFlag =  dFlag = tFlag = 0;
        flag=0;
        token = strtok(command,delim);
        while(1){  //loop condition???
            tkidx++;
            tkcom[tkidx] = token;
            if(tkcom[tkidx]==NULL){     //last command.
                //execute the last command. & exit this loop
                //printf("Last Command\n");
                if((cPid = fork())==0){
                    //check flags: sFlag,dFlag,tFlag
                    if(sFlag==0 && dFlag==0 && tFlag==0)//first & last command
                        executeSingle(tkcom,tkidx,flag);
                    else if(sFlag==1)
                        executeSingle(tkcom,tkidx,flag);
                    else if(dFlag==1)
                        executeDouble(tkcom,tkidx,flag);
                    else if(tFlag==1)
                        executeTriple(tkcom,tkidx,flag);
                    exit(0);
                }
                else wait(&status);
                if(read(commandStatus[READ],error,500)>0){
                    printf("Process %d Error: %s",cPid,error);
                    break;
                }
                //inFlag = outFlag = appFlag = 0;
                break;
            }
            if(strcmp(tkcom[tkidx],singlePipe)==0){
                tkcom[tkidx] = NULL;
                if((cPid = fork())==0){
                    //execute command,inside child.
                    //check flags, and execute accordingly.
                    if(sFlag==0 && dFlag==0 && tFlag==0)//first
                        executeSingle(tkcom,tkidx,flag);
                    else if(sFlag==1)
                        executeSingle(tkcom,tkidx,flag);
                    else if(dFlag==1)
                        executeDouble(tkcom,tkidx,flag);
                    else if(tFlag==1)
                        executeTriple(tkcom,tkidx,flag);
                    exit(0);
                }
                wait(&status);
                close(p[(flag+1)%2][WRITE]);
                pipe(p[flag%2]);
                if(read(commandStatus[READ],error,500)>0){
                    printf("Process %d Error: %s",cPid,error);
                    break;
                }
                flag++;
                //inFlag = outFlag = appFlag = 0;
                currentPipe[1]++;
                sFlag = 1;
                dFlag = tFlag = 0;
                //check status pipe. print child's pid & status.
                tkidx = -1;
            }
            else if(strcmp(tkcom[tkidx],doublePipe)==0){   //double
                tkcom[tkidx] = NULL;
                if((cPid = fork())==0){
                    //execute command,inside child.
                    //check flags, and execute accordingly.
                    if(sFlag==0 && dFlag==0 && tFlag==0)//first
                        executeSingle(tkcom,tkidx,flag);
                    else if(sFlag==1)
                        executeSingle(tkcom,tkidx,flag);
                    else if(dFlag==1)
                        executeDouble(tkcom,tkidx,flag);
                    else if(tFlag==1)
                        executeTriple(tkcom,tkidx,flag);
                    exit(0);
                }
                wait(&status);
                close(p[(flag+1)%2][WRITE]);
                pipe(p[flag%2]);
                if(read(commandStatus[READ],error,500)>0){
                    printf("Process %d Error: %s",cPid,error);
                    break;
                }
                flag++;
                //inFlag = outFlag = appFlag = 0;
                currentPipe[2]++;
                dFlag = 1;
                sFlag = tFlag = 0;
                //check status pipe. print child's pid & status.
                tkidx = -1;
            }
            else if(strcmp(tkcom[tkidx],triplePipe)==0){   //triple
                tkcom[tkidx] = NULL;
                if((cPid = fork())==0){
                    //execute command,inside child.
                    //check flags, and execute accordingly.
                    if(sFlag==0 && dFlag==0 && tFlag==0)//first
                        executeSingle(tkcom,tkidx,flag);
                    else if(sFlag==1)
                        executeSingle(tkcom,tkidx,flag);
                    else if(dFlag==1)
                        executeDouble(tkcom,tkidx,flag);
                    else if(tFlag==1)
                        executeTriple(tkcom,tkidx,flag);
                    exit(0);

                }
                wait(&status);
                close(p[(flag+1)%2][WRITE]);
                pipe(p[flag%2]);
                if(read(commandStatus[READ],error,500)>0){
                    printf("Process %d Error: %s",cPid,error);
                    break;
                }
                //inFlag = outFlag = appFlag = 0;
                currentPipe[3]++;
                flag++;
                tFlag = 1;
                dFlag = sFlag = 0;
                //check status pipe. print child's pid & status.
                tkidx = -1;
            }
            token = strtok(NULL,delim);
        }
        /*close(p[0][READ]);
        close(p[0][WRITE]);
        close(p[1][READ]);
        close(p[1][WRITE]);*/
        close(commandStatus[READ]);
    }
    printf("\n");
    return 0;
}
