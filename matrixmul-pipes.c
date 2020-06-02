#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<time.h>

void main()
{
    int fd1[2], fd2[2];
    pipe(fd1);
    pipe(fd2);
    int pid = fork();
    if(pid == 0){
        clock_t ct = clock();
        int mat1[2][2] = {{1, 2}, {3, 4}};
        int mat2[2][2] = {{4, 3}, {2, 1}};

        printf("Matrix1:\n");
        for(int i = 0; i < 2; i++){
            for(int j = 0; j < 2; j++){
                printf("%d ", mat1[i][j]);
            }
            printf("\n");
        }
        printf("Matrix2:\n");
        for(int i = 0; i < 2; i++){
            for(int j = 0; j < 2; j++){
                printf("%d ", mat2[i][j]);
            }
            printf("\n");
        }
        write(fd1[1], mat1, sizeof(int) *2 *2);
        write(fd2[1], mat2, sizeof(int) *2 *2);

        sleep(1);
        
        int result[2][2];
        read(fd1[0], result, sizeof(int) *2 *2);
        for(int i = 0; i < 2; i++){
            for(int j = 0; j < 2; j++){
                printf("%d ", result[i][j]);
            }
            printf("\n");
        }
        ct = clock() - ct;
        double time_taken = (double)ct / CLOCKS_PER_SEC;
        printf("\nChild process time: %f\n", time_taken);
    }
    else{
        clock_t pt = clock();
        int matrix1[2][2], matrix2[2][2];
        read(fd1[0], matrix1, sizeof(int) *2 *2);
        read(fd2[0], matrix2, sizeof(int) *2 *2);

        int cal[2][2] = {0};
        printf("\nCalculating answer\n\n");
        for(int i = 0; i < 2; i++){
            for(int j = 0; j < 2; j++){
                for(int k = 0; k < 2; k++){
                    cal[i][j] += (matrix1[i][k] * matrix2[k][j]);
                }
            }
        }
        write(fd1[1], cal, sizeof(int) *2 *2);
        wait(NULL);
        pt = clock() - pt;
        double time_taken = (double)pt / CLOCKS_PER_SEC;
        printf("\nParent process time: %f\n", time_taken);
    }
    return;
}