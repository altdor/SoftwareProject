#include "MathIsFun.h"
#include<stdio.h>
/*
 * Calculates the largest integer less or equal than the square root of x.
 * funSqrt(10) = 3
 * funSqrt(16) = 4
 * @param x - An integer for which the function applies
 * @return the value of |_sqrt(x)_|, if x is negative then the function
 * returns -1.
 */
int funSqrt(int x); // ==> Declaration is always in the beginning of the file.

int funPow(int x, int n, int d) {
	//Your implementation
	//Declaration + Variabl initializing at the beginning of the function
	int temp=0;
	if(n==0)
		return 1;
	if(n%2==0){
		temp = funPow(x,n/2,d);
		temp=(temp*temp)%d;
		return temp;
	}
	else{
		temp=(x % d *funPow(x,(n-1),d))%d;
		return temp;
	}
}

int funSqrt(int x) {
	//Your implementation
	//Declaration + Variabl initializing at the beginning of the function
	if (x<0)
		return -1;
	int left=0, right=x, mid;
	while (right-left>1){
		mid=(left+right)/2;
		if ((mid^2)==x)
			return mid;
		if((mid^2)<x)
			left=mid;
		else
			right=mid;
	}
	return left;
}

bool funPrimeCheck(int x) {
	//Your implementation
	//Declaration + Variabl initializing at the beginning of the function
	int sqrtX=funSqrt(x);
	for(int i=2;i<sqrtX;i++){
		if(x % i == 0)
			return false;
	}
	return true;
}

bool funPalindromeCheck(int x) {
	//Your implementation
	//Declaration + Variabl initializing at the beginning of the function
	int reverseX=0,temp,copyX=x;
	while(copyX>0){
		temp=copyX%10;
		copyX=(int)copyX/10;
		reverseX=reverseX*10+temp;
	}
	if(x==reverseX)
		return true;
	else
		return false;
}
