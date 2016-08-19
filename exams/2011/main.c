/*
 * main.c
 *
 *  Created on: 2 αιεμ 2016
 *      Author: DorAlt
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
typedef struct listNode{
	unsigned char data;
	struct listNode* next;
}ListNode;
typedef struct list{
	ListNode* head;
	ListNode* tail;
}List;

char* SumLists(List arr[], int n){
	char* str;
	int count = 0;
	for(int i = 0; i<n;i++){
		ListNode* head = arr[i].head;
		while(!head){
			unsigned char ch=head->data;
			count++;
			str = (char*)realloc(str, sizeof(char)*4*count);
			for(int i=0; i<8;i+=2){
				int mask = 3;
				switch (ch&mask){
				case (0):
					strcat(str,"a");
					break;
				case (1):
					strcat(str,"b");
					break;
				case (2):
					strcat(str,"c");
					break;
				case (3):
					strcat(str,"d");
					break;

				}
				ch=ch>>2;
			}
			head = head->next;
		}
	}
	return str;
}
