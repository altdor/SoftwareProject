/*
 * main.c
 *
 *  Created on: 3 αιεμ 2016
 *      Author: DorAlt
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
/*
//bb
typedef struct stack_node{
	char data;
	struct stack_node* next;
}Node;
typedef struct stack{
	Node* top;
	int count;
}Stack;
void delete(Stack* stck){
	Node* current = stck->top;
	while(current!=NULL || !current){
		Node* next = current->next;
		free(current);
		current = next;
	}
}
int push(Stack* stack, char ch){
	Node* node = (Node*)malloc(sizeof(node)*ch);
	if(node == NULL)
		return 0;
	node->data = ch;
	node->next= stack->top;
	stack->top = node;
	return 1;
}
int create(Stack* stack){
	char ch;
	while((scanf("%c",&ch))!=EOF){
		stack->count++;
		stack = (Stack*)realloc(stack,(stack->count*sizeof(stack)));
		if(stack == NULL){
			return 0;
		}
		if(push(stack,ch)==0){
			delete(stack);
			return 0;
		}

	}

	return 1;
}
char pop(Stack* stack){
	char ch = stack->top->data;
	Node* current = stack->top;
	stack->top = stack->top->next;
	stack->count--;
	free(current);
	return ch;
}
*/
//aa
int sum(int num){
	int sum = 0;
	while (num!=0){
		int temp = num%10;
		num = num/10;
		sum +=temp;
	}
	return sum;
}
int compareint(const void *pa, const void *pb){
	return (sum(*(int*)pa)-sum(*(int*)pb));
}
int main(int argc, char** argv){
	assert(argc==3);
	FILE *fpr, *fpw;
	int num,i;
	int* nums;
	fpr= fopen(argv[1],"r");
	if(fpr == NULL)
		return 1;
	fpw= fopen(argv[2],"w");
	if (fpw == NULL){
		fclose(fpr);
		return 1;
	}
	while(feof(fpr)==0){
		fscanf(fpr,"%d",&num);
		i++;
		nums = (int*)realloc(nums,((i)*sizeof(int)));
		nums[i] = num;
	}
	qsort(nums,i, sizeof(int),compareint);
	for(int j=0;j<i;j++)
		fprintf(fpw,"%d ",nums[i]);
	free(nums);
	fclose(fpr);
	fclose(fpw);
}
