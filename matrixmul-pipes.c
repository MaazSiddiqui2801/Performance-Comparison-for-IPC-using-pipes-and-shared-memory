#include<unistd.h>
#include<stdio.h>
#include<sys/stat.h>
#define P(X) for(l=0;l<2;l++){\
                 for(m=0;m<2;m++)\
                        printf("%d ",X[l][m]); printf("\n");}\

#define S(X) for(l=0;l<2;l++){\
                 for(m=0;m<2;m++)\
                        scanf("%d",&X[l][m]);}\

int main()
{
    int l,m,n;
    int pid;
    int p1[2],p2[2];
    pipe(p1);
    pipe(p2);

            if((pid=fork())==0)
                   {
              
                           int mat1[2][2]={1,2,3,4};
                           int mat2[2][2]={{1,2},{2,3}};
                            P(mat1);
                         P(mat2);
                 
                           int ans[2][2];
    
                           write(p1[1],mat1,2*2*sizeof(int));
                           write(p2[1],mat2,2*2*sizeof(int));
       
       
                           read(p1[0],ans,2*2*sizeof(int));
                           P(ans);
            
                   }
                else
                   {
                           int mul[2][2]={0},m1[2][2],m2[2][2],i,j,k;
 
                           read(p1[0],m1,2*2*sizeof(int));
                           read(p2[0],m2,2*2*sizeof(int));
                           P(m1);
                           P(m2);
                           for(i=0;i<2;i++)
                                 for(j=0;j<2;j++)
                                          for(k=0;k<2;k++)
                                              mul[i][j]+=m1[i][k]*m2[k][j];
           
                           P(mul);
                           write(p1[1],mul,2*2*sizeof(int));
             
            
                    }
return 0;
}