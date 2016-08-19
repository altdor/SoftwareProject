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
	if(x<0){
		x = x+d;
	}
	if(n==0){
		return 1;
	}
	if(n==1)
		return x%d;
	else if(n%2==0){
		return (funPow(x,n/2,d) * funPow(x,n/2,d))%d;
	}
	else{
		return (funPow(x,(n-1),d) * funPow(x,1,d))%d;
	}
}


int funSqrt(int x) {
	int i = x/2;
	int max = x;
	int min = 0;
	if(x<0){
		return -1;
	}
	if(x<2)
		return true;
	while (!(x>=i*i && x<(i+1)*(i+1))){
		if (i*i>x){
			max = i;
			i=(i+min)/2;
		} else {
			min = i;
			i = (i+max)/2;
		}
	}
	return i;

}

bool funPrimeCheck(int x) {
	int a = funSqrt(x);
	int b = 2;
	while (b<=a){
		if (x%b==0)
			return false;
		b++;
	}
	return true;
}

bool funPalindromeCheck(int x) {
	int a = x;
	int b = 0;
	if (x<0)
		return false;
	while (a!=0){
		b=(b*10)+(a%10);
		a/=10;
	}
	while (x!=0){
		if (x%10!=b%10)
			return false;
		x/=10;
		b/=10;
	}
	return true;
}
