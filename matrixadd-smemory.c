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
    // printf("Hello");

    int shmid1, shmid2, pid, status;
    int *shmadd, *shmdata;
    int mtx1[2][2] = {{1, 2}, {3, 4}};
    int mtx2[2][2] = {{4, 3}, {2, 1}};
    int row1 = 2, row2 = 2, col1 = 2, col2 = 2;

    int i, j, k;
    // printf("Hello");
    shmid1 = shmget(shmkey, 4096, shmmode | IPC_CREAT | 0666 | IPC_EXCL);
    // printf("Hello");
    if (shmid1 < 0)
    {
        printf("Error");
    }
    // printf("Hello");
    shmadd = (int *)shmat(shmid1, 0, 0); //attaching shared memory
    shmdata = shmadd;

    // printf("Enter the rows and columns of matrix 1");
    // scanf("%d%d",&row1,&col1);
    // printf("Enter the matrix 1: \n");
    // for(int i=0;i<row1;++i)
    //     for(int j=0;j<col1;++j)
    //         scanf("%d",&mtx1[i][j]);
    // printf("\nEnter the rows and columns of matrix 2");
    // scanf("%d%d",&row2,&col2);
    // printf("Enter the matrix 2: \n");
    // for(i=0;i<row2;++i)
    //     for(j=0;j<col2;++j)
    //         scanf("%d",&mtx2[i][j]);
    // printf("Hello");
    if (col1 != row2)
    {

        printf("\nmatrices cannot multiply\n");
        return 0;
    }
    // printf("Hello");
    for (i = 0; i < row1 / 2; i++)
        for (j = 0; j < col1; j++, shmadd += sizeof(int))
        {
            *shmadd = 0;
                *shmadd += (mtx1[i][j] + mtx2[i][j]);
        }
    pid = fork();
    if (pid == 0)
    {
        for (i = row1 / 2; i < row1; i++)
            for (j = 0; j < col2; j++, shmadd += sizeof(int))
            {
                *shmadd = 0;
                    *shmadd += (mtx1[i][j] + mtx2[i][j]);
            }
    }
    while ((pid = wait(&status)) != -1)
        ;
    shmadd = shmdata;
    printf("\nResult from pocess id number %d\n", getpid());
    for (i = 0; i < row1; ++i)
    {
        printf("\n     ");

        for (j = 0; j < col2; j++, shmadd += sizeof(int))
            printf("%d ", *shmadd);
    }
    struct shmd_ds *shmidds;
    shmdt((void *)shmadd); //detaching

    shmctl(shmid1, IPC_RMID, shmidds); //destroy the segment

    return 1;
}