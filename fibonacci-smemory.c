# include <stdlib.h>
# include <stdio.h>
# include <sys/shm.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <unistd.h>
# include <sys/wait.h>
# include <errno.h>
# include<time.h>

typedef struct{
    long fib_sequence[10];
    int sequence_size;
} shared_data;

int main()
{
    int a, b, m = 10, n, i;
    a = 0; b = 1;
    printf("Enter the number of a Fibonacci Sequence:\n");
    // Always check whether input conversion worked

    /* the identifier for the shared memory segment */
    int segment_id;
    size_t segment_size = sizeof(shared_data);

    segment_id = shmget(IPC_PRIVATE, segment_size, S_IRUSR | S_IWUSR);

    if (segment_id == -1) {
        perror("shmget failed");
        return EXIT_FAILURE;
    }

    shared_data *shared_memory = shmat(segment_id, NULL, 0);

    if ((void*)shared_memory == (void*)-1) {
        perror("shmat failed");
        goto destroy; // clean up
    }
    printf("\nshared memory segment %d attached at address %p\n", segment_id, (void*)shared_memory);

    shared_memory->sequence_size = m;
    pid_t pid;
    pid = fork();
    if (pid == 0){
        clock_t ct = clock();
        printf("Child is producing the Fibonacci Sequence...\n");
        shared_memory->fib_sequence[0] = a;
        shared_memory->fib_sequence[1] = b;
        for (i = 2; i < shared_memory->sequence_size; i++){
            n = a+b;
            shared_memory->fib_sequence[i] = n;
            a = b;
            b = n;
        }
        printf("\nChild ends\n");
        ct = clock() - ct;
        double time_taken = (double)ct / CLOCKS_PER_SEC;
        printf("\nChild process time: %f\n", time_taken);
    }
    else{
        clock_t pt = clock();
        printf("Parent is waiting for child to complete...\n");
        wait(NULL);
        printf("Parent ends\n");
        for(i = 0; i < shared_memory->sequence_size; i++) {
            printf("%ld ", shared_memory->fib_sequence[i]);
        }
        printf("\n");
        pt = clock() - pt;
        double time_taken = (double)pt / CLOCKS_PER_SEC;
        printf("\nParent process time: %f\n", time_taken);
    }

    if (shmdt(shared_memory) == -1) {
        fprintf(stderr, "Unable to detach\n");
    }
    
    destroy:
    shmctl(segment_id, IPC_RMID, NULL); 

    return 0;
}
