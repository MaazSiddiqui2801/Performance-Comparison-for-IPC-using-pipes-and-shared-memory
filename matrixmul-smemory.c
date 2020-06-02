#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/wait.h>
#define shmmode (SHM_R | SHM_W)
#define shmkey (key_t)31

int main()
{
    int shmid1, shmid2, pid, status;
    int *shmadd, *shmdata;
    int mtx1[2][2] = {{1, 2}, {3, 4}};
    int mtx2[2][2] = {{4, 3}, {2, 1}};
    int row1 = 2, row2 = 2, col1 = 2, col2 = 2;

    printf("Matrix 1: \n");
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            printf("%d ", mtx1[i][j]);
        }
        printf("\n");
    }
    printf("Matrix 2: \n");
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            printf("%d ", mtx2[i][j]);
        }
        printf("\n");
    }

    int i, j, k;
    shmid1 = shmget(shmkey, 4096, shmmode | IPC_CREAT | 0666 | IPC_EXCL);
    if (shmid1 < 0)
    {
        printf("Error");
    }
    shmadd = (int *)shmat(shmid1, 0, 0);
    shmdata = shmadd;
    if (col1 != row2)
    {

        printf("\nmatrices cannot multiply\n");
        return 0;
    }
    for (i = 0; i < row1 / 2; i++)
        for (j = 0; j < col1; j++, shmadd += sizeof(int))
        {
            *shmadd = 0;
            for (int k = 0; k < col1; k++)
                *shmadd += (mtx1[i][k] * mtx2[k][j]);
        }
    pid = fork();
    if (pid == 0)
    {
        for (i = row1 / 2; i < row1; i++){
            for (j = 0; j < col2; j++, shmadd += sizeof(int))
            {
                *shmadd = 0;
                for (k = 0; k < col1; k++)
                {
                    *shmadd += (mtx1[i][k] * mtx2[k][j]);
                }
            }
        }
    }
}
while ((pid = wait(&status)) != -1)
    ;
shmadd = shmdata;
printf("\nResult from pocess id number %d\n", getpid());
for (i = 0; i < row1; ++i)
{
    for (j = 0; j < col2; j++, shmadd += sizeof(int))
    {
        printf("%d ", *shmadd);
    }
}
struct shmd_ds *shmidds;
shmdt((void *)shmadd);

shmctl(shmid1, IPC_RMID, shmidds);

return 1;
}