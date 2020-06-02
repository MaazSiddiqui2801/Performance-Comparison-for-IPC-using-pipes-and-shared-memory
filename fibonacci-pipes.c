//Fibonacci Sequence using pipes
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<time.h>


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
        clock_t ct = clock();
        close(fd1[0]);
        write(fd1[1], &n, 4);
        close(fd2[1]);
        read(fd2[0], buffer, 30*sizeof(int));
        printf("Fibonacci Sequence of first %d digits:", n);
        for(int i = 0; i < n; i++)
            printf("%d, ", buffer[i]);
        ct = clock() - ct;
        double time_taken = (double)ct / CLOCKS_PER_SEC;
        printf("\nChild process time: %f\n", time_taken);
    }
    else{
        clock_t pt = clock();
        close(fd1[1]);
        read(fd1[0],&limit,4);
    
        prev=0,curr=1;
        ar[0]=0;
        ar[1]=1;
        int i;
        printf("Parent calculating fibonacci...\n");
        for(int i=2;i < limit;i++)
        {
            int next = curr + prev;
            prev = curr;
            curr = next;
            ar[i]=next;
        }
        close(fd2[0]);
        write(fd2[1],ar,30*sizeof(int));
        pt = clock() - pt;
        double time_taken = (double)pt / CLOCKS_PER_SEC;
        printf("\nParent process time: %f\n", time_taken);
    }
    printf("\n");
}