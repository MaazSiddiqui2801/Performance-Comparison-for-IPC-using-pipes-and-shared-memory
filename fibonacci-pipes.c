//Fibonacci Sequence using pipes
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>


void main()
{
    int fd1[2], fd2[2];
    pipe(fd1);
    pipe(fd2);
    int n = 10; //fibonacci sequence till 10
    int limit, ar[30], buffer[30];
    int prev, curr;
    int pid= fork();
    if(!pid){
        close(fd1[0]);
        write(fd1[1], &n, 4);
        close(fd2[1]);
        read(fd2[0], buffer, 30*sizeof(int));
        printf("Fibonacci Sequence:");
        for(int i = 0; i < n; i++)
            printf("%d, ", buffer[i]);
    }
    else{
        close(fd1[1]);
        read(fd1[0],&limit,4);
    
        prev=0,curr=1;
        ar[0]=0;
        ar[1]=1;
        int i;
        for(int i=2;i < limit;i++)
        {
            int next = curr + prev;
            prev = curr;
            curr = next;
            ar[i]=next;
        }
        close(fd2[0]);
        write(fd2[1],ar,30*sizeof(int));
    }
    printf("\n");
}