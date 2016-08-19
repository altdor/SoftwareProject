/*
 * main.c
 *
 *  Created on: 2 αιεμ 2016
 *      Author: DorAlt
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct String{
	char* str;
}string;
typedef struct Tree_type{
	string* data;
	struct Tree_type* left;
	struct Tree_type* right;
}tree_type;
tree_type* CreateRoot(string *str){
	tree_type* tree = (tree_type*)malloc(sizeof(tree)*sizeof(str));
	if(tree== NULL){
		return NULL;
	}
	tree->left =NULL;
	tree->right = NULL;
	tree->data = str;
	return tree;
}
void FreeTree(tree_type* tree){
	tree_type* left;
	tree_type* right;
	if(tree == NULL)
		return;
	left = tree->left;
	right = tree->right;
	free(tree->data->str);
	free(tree->data);
	free(tree);
	FreeTree(left);
	FreeTree(right);
}
int InsertToTree(tree_type* tree, string* str){
	tree_type* node = CreateRoot(str);
	tree_type* current;
	tree_type* back = tree;;
	current = tree;
	if(node == NULL){
		return 0;
	}
	if(tree == NULL){
		tree = node;
		return 1;
	}
	tree = realloc(tree, sizeof(tree)+sizeof(node));
	while(!current){
		back = current;
		if(strcmp(back->data->str,str->str)>0){
			current = current ->right;
		}
		else{
			current = current ->left;
		}
	}
	if(strcmp(back->data->str,str->str)>0){
		back->right=node;
	}
	else{
		back->left=node;
	}
	return 1;
}/*
#define N 30
int main(){
  char str[N], new_str[N];
  int i,len;
  printf("please enter 2-0 lettes\n");
  scanf("%s",str);
  len = strlen(str);
  for(i=0; i<len;i++){
    new_str[i] = str[len-i-1];
  }
  printf("the result is: %s\n",new_str);
}*/

struct t{
	char* name;
	int age;
};
void foo(struct t tmp){
	char st[] = "here";
	strcpy(tmp.name,st);
	tmp.age=0;
}
int main(){
	struct t tmp;
	tmp.name = (char*)malloc(20);
	strcpy(tmp.name,"there");
	tmp.age=10;
	foo(tmp);
	printf("%s,%d",tmp.name,tmp.age);
	return 0;
}
