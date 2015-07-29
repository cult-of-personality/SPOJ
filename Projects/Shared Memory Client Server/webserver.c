#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <pthread.h>
#include <fcntl.h>
#include <netinet/in.h>
#define BACKLOG 15
#define MAXEVENTS 100
#define MAXBUF 1000
#define MAXQUERY 500
#define FILELEN 2000
enum state{START,READING_REQUEST,HEADER_PARSING,READING_DISKFILE,WRITING_HEADER,WRITING_BODY,DONE};

struct message{     //for message queues.
    long type;
    int fd;
    enum state st;
};

struct hashEntry{
    int fd;
    enum state st;
    int qLen,filefd,filelen;
    char request[MAXQUERY];
    char type[10];
    char filename[100];
    char header[500];
    char file[FILELEN];
    char toSend[FILELEN+500];
};

int epfd;
struct epoll_event e;
int sockfd, msqid=-1;
struct hashEntry ds[100];
//pthread_mutex_t a = PTHREAD_MUTEX_INITIALIZER,b = PTHREAD_MUTEX_INITIALIZER;
struct epoll_event *eventList;

void getFileType(char name[],char type[])
{
    int i=0,j=0;
    while(name[i]!='.' && name[i]!='\0')
        i++;
    i++;
    while(name[i]!='\0')
        type[j++] = name[i++];
    type[j] = '\0';
}

void processQuery(int fd)   // sets filename
{
    int i=0,j=0,len = ds[fd].qLen;
    while(i<len && ds[fd].request[i]!='/')
        i++;
    //i++;
    while(i<len && ds[fd].request[i]!='?')
        ds[fd].filename[j++] = ds[fd].request[i++];
    ds[fd].filename[j] = '\0';
}

void readFile(int desc)
{
    ds[desc].filefd = open(ds[desc].filename,O_RDONLY);
    if(ds[desc].filefd==-1)
        return;    // file missing.
    int flags = fcntl(ds[desc].filefd,F_GETFL,0);
    fcntl(ds[desc].filefd,F_SETFL,flags|O_NONBLOCK);
    ds[desc].filelen = read(ds[desc].filefd,ds[desc].file,FILELEN);
}

void sigIntHandler(int signo)
{
    if(msqid!=-1)
        msgctl(msqid,IPC_RMID,NULL);
    exit(0);
}

int writeHeader(int desc)
{
    if(ds[desc].filefd==-1){
        sprintf(ds[desc].header,"HTTP/1.1 404 Not Found\r\n");
        strcat(ds[desc].header,"Server: localhost\r\n");
        strcat(ds[desc].header,"Content-Type: text/html\r\n\r\n");
        strcat(ds[desc].header,"<html>\r\n");
        strcat(ds[desc].header,"<head><h1>404 Error</h1><br></head>\r\n");
        strcat(ds[desc].header,"<body>Requested file not found</body>\r\n");
        strcat(ds[desc].header,"</html>\r\n");
        return -1;
    }
    sprintf(ds[desc].header,"HTTP/1.1 200 OK\r\n");
    strcat(ds[desc].header,"Server: localhost\r\n");
    if(strcmp(ds[desc].type,"html")==0)
        strcat(ds[desc].header,"Content-Type: text/html\r\n");
    else if(strcmp(ds[desc].type,"txt")==0)
        strcat(ds[desc].header,"Content-Type: text/plain\r\n");
    char conLen[30];
    sprintf(conLen,"Content-Length: %d\r\n\r\n",ds[desc].filelen);
    strcat(ds[desc].header,conLen);
    return 1;
}

void writeBody(int desc)
{
    if(ds[desc].filefd==-1){
        strcpy(ds[desc].toSend,ds[desc].header);
        close(ds[desc].filefd);
        return;
    }
    close(ds[desc].filefd);
    strcpy(ds[desc].toSend,ds[desc].header);
    strcat(ds[desc].toSend,ds[desc].file);
}

void* processThread(void *arg){     //where should mutexes and condition variables be set?
    pthread_detach(pthread_self());
    struct message m;
    int desc,state;
    while(1){
        msgrcv(msqid,&m,sizeof(struct message),0,0);    //return first message on the queue.
        desc = m.fd;
        if(desc<0 || m.type >100)
            continue;
        if(ds[desc].fd==-1 && m.st == START){  // new connection.
            ds[desc].fd = m.fd;
            ds[desc].st = START;
            msgsnd(msqid,&m,sizeof(struct message),0);
        }
        else{
            if(m.st!=ds[desc].st){
                //went wrong somewhere.
                if(m.st < ds[desc].st)
                    state = m.st;
                else state = ds[desc].st;
            }
            else state = m.st;
            switch(state){
                case START: m.type = desc;
                            m.fd = desc;
                            m.st = READING_REQUEST;
                            ds[desc].st = READING_REQUEST;   //empty state.
                            msgsnd(msqid,&m,sizeof(struct message),IPC_NOWAIT);
                            break;
                case READING_REQUEST: m.type = desc;
                                      m.fd = desc;
                                      m.st = HEADER_PARSING;
                                      ds[desc].qLen = recv(desc,ds[desc].request,MAXQUERY,0); //reading request.
                                      if(ds[desc].qLen!=-1)
                                        ds[desc].st = HEADER_PARSING;
                                      if(ds[desc].qLen==0){     //EOF from client;
                                        close(desc);
                                        ds[desc].fd = -1;
                                        break;
                                      }
                                      msgsnd(msqid,&m,sizeof(struct message),IPC_NOWAIT);
                                      break;
                case HEADER_PARSING: m.type = desc;
                                     m.fd = desc;
                                     m.st = READING_DISKFILE;
                                     processQuery(desc);
                                     getFileType(ds[desc].filename,ds[desc].type);
                                     ds[desc].st = READING_DISKFILE;
                                     msgsnd(msqid,&m,sizeof(struct message),IPC_NOWAIT);
                                     break;
                case READING_DISKFILE: m.type = desc;
                                       m.fd = desc;
                                       m.st = WRITING_HEADER;
                                       readFile(desc);
                                       ds[desc].st = WRITING_HEADER;
                                       msgsnd(msqid,&m,sizeof(struct message),IPC_NOWAIT);
                                       break;
                case WRITING_HEADER: m.type = desc;
                                     m.fd = desc;
                                     m.st = WRITING_BODY;
                                     writeHeader(desc);
                                     ds[desc].st = WRITING_BODY;
                                     msgsnd(msqid,&m,sizeof(struct message),IPC_NOWAIT);
                                     break;
                case WRITING_BODY:  m.type = desc;
                                    m.fd = desc;
                                    m.st = DONE;
                                    writeBody(desc);
                                    ds[desc].st = DONE;
                                    msgsnd(msqid,&m,sizeof(struct message),IPC_NOWAIT);
                                    break;
                case DONE: send(desc,ds[desc].toSend,strlen(ds[desc].toSend),0);
                           ds[desc].st = START;
                           break;
            }
        }
    }
    return NULL;
}

void exitWithError(char error[])
{
    perror("%s error: ");
    if(msqid!=-1)
        msgctl(msqid,IPC_RMID,NULL);
    exit(0);
}

int main(int argc,char *argv[])
{
    if((epfd = epoll_create(100))==-1)
        exitWithError("epoll_create");
    printf("Epoll fd = %d\n",epfd);
    key_t mkey = ftok("./webserver.c",1);
    msqid = msgget(mkey,IPC_CREAT|IPC_EXCL);
    pthread_t ptid;
    pthread_create(&ptid,NULL,&processThread,NULL);     //only one process thread.
    pthread_detach(ptid);
    if((sockfd = socket(AF_INET,SOCK_STREAM,0))<0)
        exitWithError("socket");
    struct sockaddr_in servAddr;
    memset(&servAddr,'\0',sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(7000);    //default port chosen as 7000
    if(bind(sockfd,(struct sockaddr*)&servAddr,sizeof(struct sockaddr_in)) < 0)
        exitWithError("bind");
    if(listen(sockfd,BACKLOG) < 0)
        exitWithError("listen");
    e.events = EPOLLIN|EPOLLET; // add to list of interesting fds.
    e.data.fd = sockfd;
    if(epoll_ctl(epfd,EPOLL_CTL_ADD,sockfd,&e) == -1)
        exitWithError("epoll_ctl");
    printf("Server has started\n");
    signal(SIGINT,sigIntHandler);
    eventList = calloc(MAXEVENTS,sizeof(e));
    int ready,i,connfd,clientLen;
    struct sockaddr_in client;

    for(i=0;i<100;i++)
        ds[i].fd = -1;   //no active connection with this fd.

    for(;;){
        ready = epoll_wait(epfd,eventList,MAXEVENTS,-1);
        if(ready==-1){
            if(errno == EINTR)
                continue;
            exitWithError("epoll_wait");
        }
        for(i=0;i<ready;i++){
            if(eventList[i].events & EPOLLIN){  //readable fd.
                if(eventList[i].data.fd == sockfd){ //new connection(s).
                    connfd = 0;
                    while(connfd!=-1){
                        connfd = accept(sockfd,(struct sockaddr*)&client,&clientLen);
                        if(connfd == -1)
                            break;  // all new connections accepted.
                        int flags = fcntl(connfd,F_GETFL,0);
                        if(fcntl(connfd,F_SETFL,flags|O_NONBLOCK)==-1)
                            exitWithError("fcntl");
                        e.events = EPOLLIN|EPOLLET; // edge triggered.
                        e.data.fd = connfd;
                        if(epoll_ctl(epfd,EPOLL_CTL_ADD,connfd,&e) == -1)
                            exitWithError("epoll_ctl");
                        printf("New connection %d\n",connfd);
                        struct message m;
                        m.type = connfd;
                        m.fd = connfd;
                        m.st = START;
                        msgsnd(msqid,&m,sizeof(m),IPC_NOWAIT);
                    }
                }
                else{   //connection responding.
                    struct message m;
                    m.type = eventList[i].data.fd;
                    m.fd = eventList[i].data.fd;
                    m.st = START;
                    if(msgsnd(msqid,&m,sizeof(m),IPC_NOWAIT)==-1){
                        if(errno == EAGAIN)
                            exitWithError("msgsnd EAGAIN");
                        exitWithError("msgsnd");
                    }
                    printf("Change in socket %d\n",eventList[i].data.fd);
                }
            }
            else if((eventList[i].events&EPOLLERR)||(eventList[i].events&EPOLLHUP)){
                exitWithError("epoll notification");
            }
        }
    }
    return 0;
}
