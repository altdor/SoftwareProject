/*
 * main.c
 *
 *  Created on: 3 αιεμ 2016
 *      Author: DorAlt
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
typedef struct listNode{
	char data;
	struct listNode* next;
}ListNode;
typedef struct list{
	ListNode* head;
	int size;
}List;

int insert(List* lst, char a, char b){
	ListNode* node = (ListNode*)malloc(sizeof(node));
	ListNode* afterplace;
	ListNode* current = lst->head;
	if(node ==NULL)
		return 0;
	node->data = b;
	lst->size++;
	while(current->next!=NULL){
		if(current->data == a)
			afterplace = current;
		current = current->next;
	}
	if(afterplace!=NULL){
		node->next = afterplace->next;
		afterplace->next = node->next;
		return 1;
	}
	current->next = node;
	return 1;
}



int remove2(List* lst, char a){
	ListNode* place;
		ListNode* current = lst->head;
		while(current->next!=NULL){
			if(current->next->data == a)
				place = current;
			current->next = current->next->next;
			free(place);
			lst->size--;
		}
		if(lst->head->data == a){
			place = lst->head;
			lst->head = lst->head->next;
			free(place);
			lst->size--;
		}
		return 1;
}
int create(char* str, List* lst){
	char c;
	FILE* fl = fopen(str,"r");
	if(fl == NULL)
		return 0;
	lst = (List*)malloc(sizeof(lst));
	if(lst ==NULL)
		return 0;
	lst->size = 0;
	while(feof(fl)){
		lst->size++;
		ListNode* node = (ListNode*)malloc(sizeof(node));
		if(node ==NULL)
			return 0;
		c = fgetc(fl);
		node->data = c;
		lst = (List*)realloc(lst,(sizeof(lst)*lst->size));
		if(lst ==NULL)
			return 0;
		node->next =lst-> head;
		lst->head = node;
	}
	return 1;
}
int delete(List* lst){
	ListNode* next;
	ListNode* current;
	if(lst == NULL){
		return 1;
	}
	current = lst->head;
	while(current!=NULL){
		next = current->next;
		free(current);
		current = next;
	}
	free(lst);
	return 1;
}

int save(List* lst, char* str){
	ListNode* current = lst->head;
	FILE* fl = fopen(str,"w");
	if(fl == NULL)
		return 0;
	while(current != NULL){
		char ch = current->data;
		if(fputc(ch,fl)<0)
			return 0;
		current = current->next;
	}
	fclose(fl);
	return 1;
}

