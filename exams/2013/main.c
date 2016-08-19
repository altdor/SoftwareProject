/*
 * main.c
 *
 *  Created on: 1 αιεμ 2016
 *      Author: DorAlt
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#define WORD_SIZE 11
int countWords(FILE* fl){
	int count = 0;
	char str[WORD_SIZE];
	fgets(str,WORD_SIZE,fl);
	fgetc(fl);
	while(strlen(str)!='\0'){
		count++;
		memset(str,0,WORD_SIZE);
		fgets(str,WORD_SIZE,fl);
		fgetc(fl);
	}
	printf("%d\n", count);
			fflush(NULL);
	return count;
}
void* wordsToArray(FILE* fl, int count){
	char** array = (char**)malloc(sizeof(char*)*WORD_SIZE*count);
	fseek(fl,0,SEEK_SET);
	for(int i =0; i<count;i++){
		array[i] = (char*)malloc(sizeof(char)*WORD_SIZE);
		fgets(array[i],WORD_SIZE,fl);
		fseek(fl,1,SEEK_CUR);
		printf("%s ", array[i]);
		fflush(NULL);
	}
	printf("\n");
	return array;
}

int compare(const void* pa,const void*pb){
	return strcmp(*(char**)pa,*(char**)pb);
}

int main(int argc, char** argv){
	assert(argc==2);
	void** array;
	int count = 0;
	FILE* fpr = fopen(argv[1],"r");
	if(fpr == NULL){

		return 1;
	}
	count =  countWords(fpr);
	array = wordsToArray(fpr,count);
	qsort(array, count, sizeof(char*),compare);
	for(int i =0;i<count;i++){
		printf("%s ", array[i]);
		fflush(NULL);
	}
	for(int i =0;i<count;i++){
		free(array[i]);
	}
	free(array);
	fclose(fpr);
}
