/*
 * SPConfig.c
 *
 *  Created on: 21 ���� 2016
 *      Author: DorAlt
 */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include "SPLogger.h"
#include "SPConfig.h"

struct sp_config_t{
	char* spImagesDirectory;
	char* spImagesPrefix;
	char* spImagesSuffix;
	int spNumOfImages;
	int spPCADimension;
	char* spPCAFileName;
	int spNumOfFeatures;
	bool spExtractionMode;
	int spNumOfSimilarImages;
	int spKNN;
	bool spMinimalGUI;
	int spLoggerLevel;
	char* spLoggerFilename;
	SP_KDTREE_SPLIT_METHOD splitMethod;
};

#define BUFSIZE 1024
#define MSG1 "Invalid configuration line"
#define MSG2 "Invalid value - constraint not met"
#define NODIR "Parameter spImagesDirectory is not set"
#define NOPRE "Parameter spImagesPrefix is not set"
#define NOSUF "Parameter spImagesSuffix is not set"
#define NONUM "Parameter spNumOfImages is not set"

char* strlwr (char *str){
  unsigned char *p = (unsigned char *)str;
  while (*p) {
     *p = tolower(*p);
      p++;
  }
  return str;
}

SPConfig spConfigCreate(const char* filename, SP_CONFIG_MSG* msg){
	assert(msg!=NULL);
	SPConfig cnfg = (SPConfig)malloc(sizeof(*cnfg));
	int* input;
	int len;
	char* string;
    FILE* fp;
    int linenum=0;
	int n=0;
	if(cnfg==NULL)
		return NULL;
	cnfg->spPCADimension = 20;
	cnfg->spNumOfFeatures = 100;
	cnfg->spExtractionMode = true;
	cnfg->spNumOfSimilarImages = 1;
	cnfg->splitMethod = MAX_SPREAD;
	cnfg->spKNN = 1;
	cnfg->spMinimalGUI = false;
	cnfg->spLoggerLevel = 3;

	if(filename == NULL){
		spConfigDestroy(cnfg);
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return NULL;
	}
    fp = fopen(filename,"r");
    if(fp==NULL){
    	spConfigDestroy(cnfg);
    	*msg= SP_CONFIG_CANNOT_OPEN_FILE;
    	return NULL;
    }
    string = (char*)calloc(BUFSIZE,1);
    if(string == NULL){
    	spConfigDestroy(cnfg);
    	fclose(fp);
    	*msg = SP_CONFIG_ALLOC_FAIL;
    	return NULL;
    }
    input = (int*)malloc(4*sizeof(int));
    if(input==0){
    	*msg = SP_CONFIG_ALLOC_FAIL;
    	spConfigDestroy(cnfg);
    	fclose(fp);
    	free(string);
    }
    while(fgets(string,BUFSIZE,fp)!= NULL){
    	char* start;
    	n=strlen(string);
    	linenum++;
    	trim(string);
    	if (string[0]!='#'){
    		if(strchr(string,'#') != NULL){
    			printError(filename,linenum, MSG1);
    			free(string);
    			free(input);
    			fclose(fp);
    			*msg = SP_CONFIG_INVALID_ARGUMENT;
    			return NULL;
    		}
    		strlwr(string);
    		start = strchr(string,'=');
    		start++;
    		len = n-strlen(start);
    		if(strstr(string,"spimagesdirectory")){
    			cnfg->spImagesDirectory = (char*)malloc(len*sizeof(char));
    			if(cnfg->spImagesDirectory==NULL){
    			   	spConfigDestroy(cnfg);
    			   	free(string);
    			   	free(input);
    			   	fclose(fp);
    			   	*msg = SP_CONFIG_ALLOC_FAIL;
    			   	return NULL;
    			}
    			strncpy(cnfg->spImagesDirectory, start, len);
    			trim(cnfg->spImagesDirectory);
    			input[0]=1;
    			if(len==0 || strchr(cnfg->spImagesDirectory,' ')!=NULL){
    				spConfigDestroy(cnfg);
    				free(string);
    				free(input);
    				fclose(fp);
    				printError(filename,linenum,NODIR);
    				*msg= SP_CONFIG_INVALID_STRING;
    				return NULL;
    			}
    		}
    		else if(strstr(string,"spimagesprefix")){
    			cnfg->spImagesPrefix = (char*)malloc(len*sizeof(char));
    			if(cnfg->spImagesPrefix==NULL){
    				spConfigDestroy(cnfg);
    				free(string);
    				free(input);
    				fclose(fp);
    				*msg = SP_CONFIG_ALLOC_FAIL;
    				return NULL;
    			}
    			strncpy(cnfg->spImagesPrefix, start, len);
    			trim(cnfg->spImagesPrefix);
    			input[1]=1;
    			if(len==0||strchr(cnfg->spImagesPrefix,' ')!=NULL){
    				spConfigDestroy(cnfg);
    				free(string);
    				free(input);
    				fclose(fp);
    				printError(filename,linenum,NOPRE);
    			    *msg = SP_CONFIG_INVALID_STRING;
    			    return NULL;
    			}
    		}
    		else if(strstr(string,"spimagessuffix")){
    			cnfg->spImagesSuffix = (char*)malloc(len*sizeof(char));
    			if(cnfg->spImagesSuffix==NULL){
    			  spConfigDestroy(cnfg);
    			  free(string);
    			  fclose(fp);
    			  free(input);
    			  *msg = SP_CONFIG_ALLOC_FAIL;
    			  return NULL;
    			}
    			strncpy(cnfg->spImagesSuffix, start, len);
    			trim(cnfg->spImagesSuffix);
    			if(len==0||strchr(cnfg->spImagesSuffix,' ')!=NULL){
    				spConfigDestroy(cnfg);
    				free(string);
    				free(input);
    				fclose(fp);
    				printError(filename,linenum,NOSUF);
    				*msg = SP_CONFIG_INVALID_STRING;
    				return NULL;
    			}
    			input[2]=1;
    		}
    		else if(strstr(string,"spnumofimages")){
    			char* temp = (char*)malloc(len*sizeof(char));
    			if(temp==NULL){
    				free(string);
    				fclose(fp);
    				free(input);
    			   	spConfigDestroy(cnfg);
    			   	*msg = SP_CONFIG_ALLOC_FAIL;
    			   	return NULL;
    			}
    			strncpy(temp, start, len);
    			trim(temp);
    			cnfg->spNumOfImages = atoi(temp);
    			if(len ==0 || cnfg->spNumOfImages<=0){
    				spConfigDestroy(cnfg);
    				free(temp);
    				free(string);
    				fclose(fp);
    				free(input);
    				printError(filename,linenum,NONUM);
    			    *msg = SP_CONFIG_INVALID_INTEGER;
    			    return NULL;
    			 }
    			free(temp);
    			input[3]=1;
    		}
    		else if(strstr(string,"sppcadimension")){
    			char* temp = (char*)malloc(len*sizeof(char));
    			if(temp==NULL){
    				free(string);
    				fclose(fp);
    				free(input);
    			   spConfigDestroy(cnfg);
    			   *msg = SP_CONFIG_ALLOC_FAIL;
    			   return NULL;
    			}
    			strncpy(temp, start, len);
    			trim(temp);
    			cnfg->spPCADimension = atoi(temp);
    			if(len ==0 || (cnfg->spPCADimension)<10 || (cnfg->spPCADimension)>28){
    				free(temp);
    				free(string);
    				fclose(fp);
    				free(input);
    			    spConfigDestroy(cnfg);
    			    printError(filename,linenum,MSG2);
    				*msg = SP_CONFIG_INVALID_INTEGER;
    				return NULL;
    			}
    			free(temp);
    		}
    		else if(strstr(string,"sppcafilename")){
    			cnfg->spPCAFileName = (char*)malloc(len*sizeof(char));
    			if(cnfg->spPCAFileName==NULL){
    			   spConfigDestroy(cnfg);
    			   free(string);
    			   fclose(fp);
    			   free(input);
    			   *msg = SP_CONFIG_ALLOC_FAIL;
    			   return NULL;
    			}
    			strncpy(cnfg->spPCAFileName, start, len);
    			trim(cnfg->spPCAFileName);
    			if(len == 0){
    			    spConfigDestroy(cnfg);
    			    free(string);
    			    free(input);
    			    fclose(fp);
    			    printError(filename,linenum,MSG2);
    			    *msg= SP_CONFIG_INVALID_STRING;
    			    return NULL;
    			}
    			if(strchr(cnfg->spPCAFileName,' ')!=NULL){
    				spConfigDestroy(cnfg);
    				free(string);
    				free(input);
    				fclose(fp);
    				printError(filename,linenum,MSG1);
    			    *msg =SP_CONFIG_INVALID_STRING;
    			    return NULL;
    			}
    		}
    		else if(strstr(string,"spnumoffeatures")){
    			char* temp = (char*)malloc(len*sizeof(char));
    			if(temp==NULL){
    			   spConfigDestroy(cnfg);
    			   free(string);
    			   free(input);
    			   fclose(fp);
    			   *msg = SP_CONFIG_ALLOC_FAIL;
    			   return NULL;
    			}
    			strncpy(temp, start, len);
    			trim(temp);
    			cnfg->spNumOfFeatures = atoi(temp);
    			if(len ==0 || cnfg->spNumOfFeatures<=0){
    				spConfigDestroy(cnfg);
    				printError(filename,linenum,MSG2);
    				free(temp);
    				free(string);
    				fclose(fp);
    				free(input);
    			   	*msg = SP_CONFIG_INVALID_INTEGER;
    			   	return NULL;
    			}
    			free(temp);
    		}
    		else if(strstr(string,"spkdtreesplitmethod")){
    			char* temp = (char*)malloc(len*sizeof(char));
    			if(temp==NULL){
    			  spConfigDestroy(cnfg);
    			  free(string);
    			  free(input);
    			  fclose(fp);
    			  *msg = SP_CONFIG_ALLOC_FAIL;
    			  return NULL;
    			}
    			strncpy(temp, start, len);
    			trim(temp);
    			if(strcmp(temp,"max_spread")==0){
    				cnfg->splitMethod = MAX_SPREAD;
    			}
    			else if(strcmp(temp,"random")==0){
    				cnfg->splitMethod = RANDOM;
    			}
    			else if(strcmp(temp,"incremental")==0){
    				cnfg->splitMethod = INCREMENTAL;
    			}
    			else{
    				spConfigDestroy(cnfg);
    				free(temp);
    				free(string);
    				fclose(fp);
    				free(input);
    				printError(filename,linenum,MSG2);
    				*msg = SP_CONFIG_INVALID_STRING;
    				return NULL;
    			}
    			free(temp);
    		}
    		else if(strstr(string,"spknn")){
    			char* temp = (char*)malloc(len*sizeof(char));
    			if(temp==NULL){
    				free(string);
    				free(input);
    				fclose(fp);
    			   spConfigDestroy(cnfg);
    			   *msg = SP_CONFIG_ALLOC_FAIL;
    			   return NULL;
    			}
    			strncpy(temp, start, len);
    			trim(temp);
    			cnfg->spKNN = atoi(temp);
    			if(len ==0 || cnfg->spKNN<=0){
    				spConfigDestroy(cnfg);
    				free(temp);
    				free(string);
    				free(input);
    				fclose(fp);
    				printError(filename,linenum,MSG2);
    			 	*msg = SP_CONFIG_INVALID_INTEGER;
    			 	return NULL;
    			}
    			free(temp);
    		}
    		else if(strstr(string,"spminimalgui")){
    			char* temp = (char*)malloc(len*sizeof(char));
    			if(temp==NULL){
    			    spConfigDestroy(cnfg);
    			    free(string);
    			    fclose(fp);
    			    free(input);
    			    *msg = SP_CONFIG_ALLOC_FAIL;
    			    return NULL;
    			}
    			strncpy(temp, start, len);
    			trim(temp);
    			if(strcmp(temp,"false")==0){
    			 	cnfg->spMinimalGUI = false;
    			}
    			else if(strcmp(temp,"true")==0){
    				cnfg->spMinimalGUI = true;
    			}
    			else{
    				spConfigDestroy(cnfg);
    			 	free(temp);
    			 	fclose(fp);
    			 	printError(filename,linenum,MSG2);
    			 	*msg = SP_CONFIG_INVALID_STRING;
    			 	return NULL;
    			 }
       			free(temp);

    		}
    		else if(strstr(string,"sploggerlevel")){
    			char* temp = (char*)malloc(len*sizeof(char));
    			if(temp==NULL){
    			    spConfigDestroy(cnfg);
    			    free(string);
    			    fclose(fp);
    			    free(input);
    			    *msg = SP_CONFIG_ALLOC_FAIL;
    			    return NULL;
    			}
       			strncpy(temp, start, len);
       			trim(temp);
    			cnfg->spLoggerLevel = atoi(temp);
    			if(len ==0 || cnfg->spLoggerLevel<1 || cnfg->spLoggerLevel>4){
    				spConfigDestroy(cnfg);
    				free(temp);
    				fclose(fp);
    				free(string);
    				free(input);
    				printError(filename,linenum,MSG2);
   			    	*msg = SP_CONFIG_INVALID_INTEGER;
   			    	return NULL;
    			}
    			free(temp);
    		}
    		else if(strstr(string,"sploggerfilename")){
    			cnfg->spLoggerFilename = (char*)malloc(len*sizeof(char));
    			if(cnfg->spLoggerFilename==NULL){
    			   spConfigDestroy(cnfg);
    			   free(string);
    			   free(input);
    			   fclose(fp);
    			   *msg = SP_CONFIG_ALLOC_FAIL;
    			   return NULL;
    			}
       			strncpy(cnfg->spLoggerFilename, start, len);
       			trim(cnfg->spLoggerFilename);
       			if(len == 0){
       			    spConfigDestroy(cnfg);
       			    free(string);
       			    fclose(fp);
       			    free(input);
       			    printError(filename,linenum,MSG2);
       			    *msg= SP_CONFIG_INVALID_STRING;
       			    return NULL;
       			}
       			if((strchr(cnfg->spLoggerFilename,' ')!=NULL)){
       				spConfigDestroy(cnfg);
       				free(string);
       				free(input);
       				fclose(fp);
       				printError(filename,linenum,MSG1);
       			    *msg = SP_CONFIG_INVALID_STRING;
       			    return NULL;
       			}
    		}
    		else if(strstr(string,"spnumofsimilarimages")){
    			char* temp = (char*)malloc(len*sizeof(char)+1);
    			if(temp==NULL){
    			   spConfigDestroy(cnfg);
    			   free(string);
    			   fclose(fp);
    			   free(input);
    			   *msg = SP_CONFIG_ALLOC_FAIL;
    			   return NULL;
    			}
       			strncpy(temp, start, len);
       			trim(temp);
       			cnfg->spNumOfSimilarImages = atoi(temp);
       			if(len ==0 || cnfg->spNumOfSimilarImages<=0){
       				spConfigDestroy(cnfg);
       				free(temp);
       				free(string);
       				free(input);
       				fclose(fp);
       			 	*msg= SP_CONFIG_INVALID_INTEGER;
       			 	return NULL;
       			}
       			free(temp);
    		}
    		else if(strstr(string,"spextractionmode")){
    			char* temp = (char*)malloc(len*sizeof(char)+1);
    			if(temp==NULL){
    			    spConfigDestroy(cnfg);
    			    free(string);
    			    fclose(fp);
    			    free(input);
    			    *msg = SP_CONFIG_ALLOC_FAIL;
    			    return NULL;
    			}
    			strncpy(temp, start, len);
    			trim(temp);
    			if(strcmp(temp,"false")==0){
    				cnfg->spExtractionMode = false;
    			}
    			else if(strcmp(temp,"true")==0){
  	  				cnfg->spExtractionMode = true;
       			}
    			else{
    				spConfigDestroy(cnfg);
    				free(temp);
    				fclose(fp);
    				printError(filename,linenum,MSG2);
    				*msg= SP_CONFIG_INVALID_STRING;
    				return NULL;
       			}
       			free(temp);
    		}
    		else{
    			spConfigDestroy(cnfg);
    			*msg = SP_CONFIG_INVALID_STRING;
    			return NULL;
    		}
    	}
    }
    if(cnfg->spPCAFileName==NULL){
    	cnfg->spPCAFileName = (char*)malloc((BUFSIZE)*sizeof(char));
    	if(cnfg->spPCAFileName==NULL){
    		 spConfigDestroy(cnfg);
    		 free(string);
    		 fclose(fp);
    		 free(input);
    		 return NULL;
    	}
    	cnfg->spPCAFileName = "pca.yml";
    }
    if(cnfg->spLoggerFilename==NULL){
    	cnfg->spLoggerFilename = (char*)malloc((BUFSIZE)*sizeof(char));
    	if(cnfg->spPCAFileName==NULL){
    	     spConfigDestroy(cnfg);
    		 free(string);
    		 fclose(fp);
    		 free(input);
    		 return NULL;
    	}
    	cnfg->spLoggerFilename = "stdout";
    }
    *msg = checkinput(filename, input, linenum);
    free(string);
    free(input);
    fclose(fp);
    if(*msg == SP_CONFIG_SUCCESS){
    	return cnfg;
    }
    spConfigDestroy(cnfg);
    return NULL;
}
SP_CONFIG_MSG checkinput(const char* filename, int* input, int linenum){
	if(input[0] == 0){
		printError(filename,linenum,NODIR);
		return SP_CONFIG_MISSING_DIR;
	}
	if(input[1]==0){
		printError(filename,linenum,NOPRE);
		return SP_CONFIG_MISSING_PREFIX;
	}
	if(input[2]==0){
		printError(filename,linenum,NOSUF);
		return SP_CONFIG_MISSING_SUFFIX;
	}
	if(input[3]==0){
		printError(filename,linenum,NONUM);
		return SP_CONFIG_MISSING_NUM_IMAGES;
	}
	return SP_CONFIG_SUCCESS;
}
void trim(char* string){
    int dest;
    int src=0;
    int len = strlen(string);
    while(isspace(string[src])) src++;
    for(dest=0; src<len; dest++, src++)
    {
        string[dest] = string[src];
    }
    string[dest] = '\0';
    for(dest=len-1; isspace(string[dest]); --dest)
    {
    	string[dest] = '\0';
    }
}

void spConfigDestroy(SPConfig cnfg){
	free(cnfg->spImagesDirectory);
	free(cnfg->spImagesPrefix);
	free(cnfg->spImagesSuffix);
	free(cnfg->spPCAFileName);
	free(cnfg->spLoggerFilename);
	free(cnfg->spImagesPrefix);
	free(cnfg);
}
void printError(const char* filename, int line, char* msg){
	printf("File: %s\n",filename);
	fflush(NULL);
	printf("Line: %d\n",line);
	fflush(NULL);
	printf("Message: %s\n",msg);
	fflush(NULL);
}

bool spConfigIsExtractionMode(const SPConfig config, SP_CONFIG_MSG* msg){
	assert(msg!=NULL);
	if (config==NULL){
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return NULL;
	}
	*msg = SP_CONFIG_SUCCESS;
	return config->spExtractionMode;
}
bool spConfigMinimalGui(const SPConfig config, SP_CONFIG_MSG* msg){
	assert(msg!=NULL);
	if (config==NULL){
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return NULL;
	}
	*msg = SP_CONFIG_SUCCESS;
	return config->spMinimalGUI;
}
int spConfigGetNumOfImages(const SPConfig config, SP_CONFIG_MSG* msg){

	assert(msg!=NULL);
	if (config==NULL){
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return -1;
	}
	if (config->spNumOfImages<=0){
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return -1;
	}
	*msg = SP_CONFIG_SUCCESS;
	return config->spNumOfImages;
}
int spConfigGetNumOfFeatures(const SPConfig config, SP_CONFIG_MSG* msg){
	assert(msg!=NULL);
	if (config==NULL){
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return -1;
	}
	if (config->spNumOfFeatures<=0){
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return -1;
	}
	*msg = SP_CONFIG_SUCCESS;
	return config->spNumOfFeatures;
}
int spConfigGetPCADim(const SPConfig config, SP_CONFIG_MSG* msg){
	assert(msg!=NULL);
	if (config==NULL){
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return -1;
	}
	if (config->spPCADimension<=0){
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return -1;
	}
	*msg = SP_CONFIG_SUCCESS;
	return config->spPCADimension;
}


SP_CONFIG_MSG spConfigGetImagePath(char* imagePath, const SPConfig config,
		int ind){
	char* path;
	int size;
	if(config == NULL || imagePath==NULL){
		return SP_CONFIG_INVALID_ARGUMENT;
	}
	if(config->spNumOfImages < ind)
		return SP_CONFIG_INDEX_OUT_OF_RANGE;
	size = strlen(config->spImagesPrefix)+strlen(config->spImagesSuffix)+strlen(config->spImagesDirectory)+sizeof(ind);
	path = (char*)malloc(size*sizeof(char)+1);
	sprintf(path,"%s%s%d%s", config->spImagesDirectory, config->spImagesPrefix, ind, config->spImagesSuffix);
	if(sizeof(path)<=sizeof(imagePath)){
		strcpy(imagePath,path);
		free(path);
		return SP_CONFIG_SUCCESS;
	}
	free(path);
	return SP_CONFIG_INVALID_ARGUMENT;
}
SP_CONFIG_MSG spConfigGetImagePathWithoutSuffix(char* imagePath, const SPConfig config,
		int ind){
	char* path;
	int size;
	if(config == NULL || imagePath==NULL){
		return SP_CONFIG_INVALID_ARGUMENT;
	}
	if(config->spNumOfImages < ind)
		return SP_CONFIG_INDEX_OUT_OF_RANGE;
	size = strlen(config->spImagesPrefix)+strlen(config->spImagesSuffix)+strlen(config->spImagesDirectory)+sizeof(ind);
	path = (char*)malloc(size*sizeof(char)+1);
	sprintf(path,"%s%s%d", config->spImagesDirectory, config->spImagesPrefix,ind);
	if(sizeof(path)<=sizeof(imagePath)){
		strcpy(imagePath,path);
		free(path);
		return SP_CONFIG_SUCCESS;
	}
	free(path);
	return SP_CONFIG_INVALID_ARGUMENT;
}
SP_CONFIG_MSG spConfigGetPCAPath(char* pcaPath, const SPConfig config){
	char* path;
	int size;
	if(config == NULL || pcaPath==NULL){
		return SP_CONFIG_INVALID_ARGUMENT;
	}
	size = strlen(config->spImagesDirectory)+strlen(config->spPCAFileName);
	path = (char*)malloc(size*sizeof(char)+1);
	sprintf(path,"%s%s", config->spImagesDirectory, config->spPCAFileName);
	strcpy(pcaPath,path);
	free(path);
	return SP_CONFIG_SUCCESS;
}

SP_LOGGER_LEVEL GetSpLoggerLevel(SPConfig config){
	if(config == NULL){
			return SP_CONFIG_INVALID_ARGUMENT;
	}
	switch(config->spLoggerLevel){
	case 1:
		return SP_LOGGER_ERROR_LEVEL;
		break;
	case 2:
		return SP_LOGGER_WARNING_ERROR_LEVEL;
		break;
	case 3:
		return SP_LOGGER_INFO_WARNING_ERROR_LEVEL;
		break;
	case 4:
		return SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL;
		break;
	}
	return SP_LOGGER_ERROR_LEVEL;
}
int GetSpLoggerLevelNum(SPConfig config){
	if(config == NULL){
		return SP_CONFIG_INVALID_ARGUMENT;
	}
	return config->spLoggerLevel;
}
char* GetSpLoggerFilename(SPConfig config){
	if(config == NULL){
		return NULL;
	}
	return config->spLoggerFilename;
}
int GetspNumOfSimilarImages(SPConfig config){
	if(config ==NULL)
		return -1;
	return config->spNumOfSimilarImages;
}
SP_KDTREE_SPLIT_METHOD GetSplitMethod(SPConfig config){
	if(config ==NULL)
		return 4;
	return config->splitMethod;
}
int GetSpKNN(SPConfig config){
	if(config ==NULL)
		return -1;
	return config->spKNN;
}
