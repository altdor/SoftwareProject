/*
 * main.c
 *
 *  Created on: Mar 19, 2016
 *      Author: gizunterman, lironemilyg
 */

#include <stdio.h>
#include <stdbool.h>
#include "MathIsFun.h"

int main(){
	int choice,x,n,d,val;
	bool boolVal = false;

	puts("Welcome to Math Is Fun - beta version\n");
	puts("Supported operations are:\n");
	puts("1 - Power calculation\n");
	puts("2 - Prime test\n");
	puts("3 - Palindrome test\n");
	puts("Please enter operation number (1/2/3): ");
	scanf("%d",&choice);
	switch (choice){
	case 1:
		puts("Please enter three space separated numbers:");
		scanf("%d%d%d",&x,&n,&d);
		val=funPow(x,n,d);
		printf("res = %d\n",val);
		break;
	case 2:
		puts("Please enter an integer: ");
		scanf("%d",&x);
		boolVal=funPrimeCheck(x);
		printf("res = ");
		printf(boolVal ? "true\n" : "false\n");
		break;
	case 3:
		puts("Please enter an integer: ");
		scanf("%d",&x);
		boolVal=funPalindromeCheck(x);
		printf("res = ");
		printf(boolVal ? "true\n" : "false\n");
		break;
	}

}
