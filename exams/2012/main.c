/*
 * main.c

 *
 *  Created on: 1 αιεμ 2016
 *      Author: DorAlt
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>/*
#define MAX 1024
typedef struct list_node{
	char* data;
	struct list_node* next;
	struct list_node* prev;
}Node;

Node* createNodeAfter(char* data, Node* pre){
	 Node* current = (Node*)malloc(sizeof(Node));
	 assert(current);
	 current->data = (char*)malloc(sizeof(char)*(strlen(data)+1));
	 assert(current->data);
	 strcpy(current->data,data);

	 current->prev = pre;

	 if(pre!=NULL){
		 pre->next =current;
	 }
	 return current;
}
void destroynode(Node* node){
	free(node->data);
	free(node);
}

void destroyList(Node* head){
	while(head!=NULL){
		Node* node = head->next;
		destroynode(head);
		head=node;
	}
}
int main(int argc, char** argv){
	assert(argc == 2);
	char str[MAX];
	char* strr;
	Node* head=NULL;
	FILE* fl = fopen(argv[1],"r");
	if(fl == NULL)
		return 1;



	while(fscanf(fl,"%s",str)!=EOF){
		head = createNodeAfter(str,head);
		memset(str,0,strlen(str));
	}
	fclose(fl);
	if(head==NULL){
		return 0;
	}
	while(head->prev != NULL){
		strr = head->data;
		for(int i= strlen(strr)-1;i>=0;i--){
			putchar(strr[i]);
		}
		head = head->prev;
	}
	strr = head->data;
	for(int i= strlen(strr)-1;i>=0;i--){
		putchar(strr[i]);
	}
	destroyList(head);
	return 0;
}*/
int main(void){
  char c,ch=0;
  int counter = 1;
  if(scanf("%c",&c)!=1){
    return -1;
  }
  while (ch!='\n'){
    scanf("%c",&ch);
    if (ch==c){
      counter++;}
  }
      printf("%d\n",counter);
      return 0;
}
