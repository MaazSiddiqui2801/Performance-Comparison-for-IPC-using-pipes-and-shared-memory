#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main()
{
    int l, m, n;
    int pid;
    int p1[2], p2[2];
    pipe(p1);
    pipe(p2);

    if ((pid = fork()) == 0)
    {

        int mat1[2][2] = {{1, 2}, {3, 4}};
        int mat2[2][2] = {{4, 3}, {2, 1}};
        int ans[2][2];
        int i, j;
        printf("Matrix 1:\n");
        for (i = 0; i < 2; i++)
        {
            for (j = 0; j < 2; j++)
            {
                // printf("Value %d %d : ", i + 1, j + 1);
                // scanf("%d", &mat1[i][j]);
                printf("%d ", mat1[i][j]);
            }
            printf("\n");
        }
        printf("Matrix 2:\n");
        for (i = 0; i < 2; i++)
        {
            for (j = 0; j < 2; j++)
            {
                // printf("Value %d %d : ", i + 1, j + 1);
                // scanf("%d", &mat2[i][j]);
                printf("%d ", mat2[i][j]);
            }
            printf("\n");
        }

        write(p1[1], mat1, 2 * 2 * sizeof(int));
        write(p2[1], mat2, 2 * 2 * sizeof(int));

        sleep(1);

        read(p1[0], ans, 2 * 2 * sizeof(int));
        printf("Added Matrix: \n%d %d \n%d %d \n", ans[0][0], ans[0][1], ans[1][0], ans[1][1]);
    }
    else
    {
        int mul[2][2] = {0}, m1[2][2], m2[2][2], i, j, k;

        read(p1[0], m1, 2 * 2 * sizeof(int));
        read(p2[0], m2, 2 * 2 * sizeof(int));
        for (i = 0; i < 2; i++)
        {
            for (j = 0; j < 2; j++)
            {
                mul[i][j] = m1[i][j] + m2[i][j];
            }
        }
        write(p1[1], mul, 2 * 2 * sizeof(int));
        wait(NULL);
    }
    return 0;
}