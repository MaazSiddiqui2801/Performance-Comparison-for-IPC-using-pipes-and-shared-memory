#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <time.h>
#define shmmode (SHM_R | SHM_W)
#define shmkey (key_t)31

int main()
{
    int shmid1, shmid2, pid, status;
    int *shmadd, *shmdata;
    int mtx1[2][2] = {{1, 2}, {3, 4}};
    int mtx2[2][2] = {{4, 3}, {2, 1}};
    int row1 = 2, row2 = 2, col1 = 2, col2 = 2;

    int i, j, k;
    shmid1 = shmget(shmkey, 4096, shmmode | IPC_CREAT | 0666 | IPC_EXCL);
    if (shmid1 < 0)
    {
        printf("Error");
    }
    shmadd = (int *)shmat(shmid1, 0, 0); //attaching shared memory
    shmdata = shmadd;
    if (col1 != row2)
    {

        printf("\nmatrices cannot multiply\n");
        return 0;
    }
    clock_t pt = clock();
    for (i = 0; i < row1 / 2; i++)
    {
        for (j = 0; j < col1; j++, shmadd += sizeof(int))
        {
            *shmadd = 0;
            for (int k = 0; k < col1; k++)
                *shmadd += (mtx1[i][k] * mtx2[k][j]);
        }
    }
    pt = clock() - pt;
    double time_taken = (double)pt / CLOCKS_PER_SEC;
    printf("\nParent process time: %f\n", time_taken);
    pid = fork();
    if (pid == 0)
    {
        clock_t ct = clock();
        for (i = 0; i < row1 / 2; i++)
            for (j = 0; j < col1; j++, shmadd += sizeof(int))
            {
                *shmadd = 0;
                for (int k = 0; k < col1; k++)
                    *shmadd += (mtx1[i][k] * mtx2[k][j]);
            }
        ct = clock() - ct;
        double time_taken = (double)ct / CLOCKS_PER_SEC;
        printf("\nParent process time: %f\n", time_taken);
    }
    while ((pid = wait(&status)) != -1)
        ;
    shmadd = shmdata;
    printf("\nResult from pocess id number %d\n", getpid());
    for (i = 0; i < row1; ++i)
    {
        for (j = 0; j < col2; j++, shmadd += sizeof(int))
            printf("%d ", *shmadd);
        printf("\n");
    }
    struct shmd_ds *shmidds;
    shmdt((void *)shmadd); //detaching

    shmctl(shmid1, IPC_RMID, shmidds); //destroy the segment

    return 0;
}
