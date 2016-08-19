#include<stdio.h>

#include "MathIsFun.h"

int main(){
	int p;
	int val;
	printf("Welcome to Math Is Fun - beta version\n");
	printf("Supported operations are:\n");
	printf("1 - Power Calculation\n");
	printf("2 - Prime Check\n");
	printf("3 - Palindrome Check\n");
	printf("Please enter operation number (1/2/3): \n");
	scanf("%d",&p);
	if (p==1){
		int x,n,d;
		printf("Please enter three space separated numbers: \n");
		scanf("%d %d %d",&x,&n,&d);
		val = funPow(x,n,d);
		printf("res = %d\n", val);
	}
	else {
		int x;
		printf("Please enter an integer: \n");
		scanf("%d",&x);
		if(p==2){
			val = funPrimeCheck(x);
		}
		else if (p==3){
			val = funPalindromeCheck(x);
		}
		if(val!=0){
			printf("res = true\n");
		}
		else{
			printf("res = false\n");
		}

		}
	return 0;
}
