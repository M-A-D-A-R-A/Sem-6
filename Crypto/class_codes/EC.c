#include <stdbool.h>
#define a =1
#define b =6
#define M =13

int add_inv(int x){
    return M-x;
}

int modInverse(int y)
{
    for (int x = 1; x < M; x++)
        if (((y%M) * (x%M)) % M == 1)
            return x;
}

 
int main (){
    

    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < m; j++)
        {
           if(isECL(i,j,m)){
               printf("(%d,%d)\n",i ,j);
           }
        }
        
    }
    
}

