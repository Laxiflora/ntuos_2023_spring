#include "kernel/types.h"
#include "kernel/fs.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

#define STDOUT 1
#define PATH_QUANTITY 50
#define PATH_LENGTH 500

char paths[PATH_QUANTITY][PATH_LENGTH];
int path_num = 0;


int readLine(int fd, char* buffer){
    char temp=0;
    int i=0;
    for(i=0 ;read(fd,&temp ,1) != 0 && temp != '\n';i++){
        buffer[i] = temp;
    }
    buffer[i] = '\0';
    return i;
}

int countChar(char* line, char key){
    int count = 0;
    for(int i=0;i<strlen(line);i++){
        if(line[i] == key){
            count++;
        }
    }
    return count;
}

int findCharIndex(char* line, char key){
    for(int i=0;i<strlen(line);i++){
        if(line[i] == key){
            return i;
        }
    }
    return -1;
}

char* substr(char* toSplit, int head){
    char* newStr = malloc(sizeof(char)*(strlen(toSplit)-head)+1);
    memcpy(newStr,toSplit+head,strlen(toSplit)-head);
    return newStr;
}

void strcat(char* head, char* tail){
    char* newStr = malloc(sizeof(char)*strlen(head)+sizeof(char)*strlen(tail)+1);
    memcpy(newStr, head, strlen(head));
    memcpy(newStr+strlen(head),tail,strlen(tail));
    newStr[strlen(head)+strlen(tail)]='\0';
    strcpy(head, newStr);
}

void printPath(char* argv){
    for(int i=0;i<path_num-1;i++){
        int counter = countChar(paths[i], argv[0]);
        printf("%s %d\n",paths[i],counter);
    }
    printf("\n%s\n",paths[path_num-1]);
    exit(0);
}


void makePath(int fd){
    char line[PATH_LENGTH];
    char prefix[5][PATH_LENGTH];
    for(int i=0;readLine(fd, line) !=0;i++){
        if(i==0){;
            if(findCharIndex(line, '[') != -1){
                printf("%s\n",line);
                readLine(fd, line);
                readLine(fd, line);
                printf("\n%s\n",line);
                exit(0);
            } 
            else{
                strcpy(prefix[0], line);
                strcpy(paths[path_num++], line);
                continue;
            }
        }
        if(i %2 ==0){
            int level = (findCharIndex(line, '+')- ((countChar(line, '|')>0?countChar(line, '|'):0)))/3+1;
            int index = findCharIndex(line, '+')+4;
            strcpy(line, substr(line,index));
            char path[PATH_LENGTH] = {'\0'};
            strcpy(prefix[level], line);
            for(int j=0;j<level;j++){
                strcat(path, prefix[j]);
                strcat(path,"/");
            }
            strcat(path, line);
            strcpy(paths[path_num++], path);
        }

    }
    readLine(fd, line);
    strcpy(paths[path_num++],line);
    return;
}


int main(int argc, char *argv[]) {
    int fds[2] = {0,0};
    int pid = -1;
    if (argc < 2) {
        printf("tree: missing argv[1]\n");
        exit(-1);
    }

    // Create pipes
    if (pipe(fds) < 0) {
        printf("tree: pipe failed\n");
        exit(-1);
    }

    // Create child process
    if ((pid=fork()) == 0) { // Child
        close(STDOUT);
        dup(fds[1]);
        close(fds[0]);
        char* arguments[2];
        arguments[0] = "count";
        arguments[1] = argv[1]; 
        exec("count", arguments);
        exit(0);

    } 
    else{
        wait(&pid);
        close(fds[1]);
        makePath(fds[0]);
        printPath(argv[2]);
        return 0;
    }
    return 0;
}