// #include <iostream>
#define ll long long
// # using namespace std; 
#include <stdio.h>
#include <stdlib.h>

ll modular_pow(ll base, ll exponent, int modulus)
{
    ll result = 1;
    while (exponent > 0)
    {
        if (exponent % 2 == 1)
            result = (result * base) % modulus;
        exponent = exponent >> 1;
        base = (base * base) % modulus;
    }
    return result;
}

int main(){

    time_t t;
 srand((unsigned) time(&t));
	long X, k,M; //input

	long long Y; //output

	scanf("%ld %ld %ld", &X, &k,&M); //reading

	Y = modular_pow(X, k,M); //Square-and-Multiply modular Exponentation

	printf("%ld\n", Y);	

    long p = 131;
    long g = 2;
  
    long a =26674;
    long b= 32348;
    printf("the random value for a = %ld\n", a);	
     printf("the random value for b = %ld\n", b);	
    unsigned long long int ga = modular_pow(g,a,p);
   unsigned long long int gb = modular_pow(g,b,p)%p;
    printf("the g^a value for alice = %ld\n", ga);	

    printf("the g^b value for bob = %ld\n", gb);	
    long h = a*b;
    unsigned long long int gab =modular_pow(g,h,p);
     printf("the g^a*b value for both of them = %ld\n", gab);	
	return 0;
}