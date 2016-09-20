/*
 * SPConfig.c
 *
 *  Created on: 21 баев 2016
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
#define SPID "spimagesdirectory"
#define SPIP "spimagesprefix"
#define SPIS "spimagesuffix"
#define SPCADIM "sppcadimension"
#define SPNOI "spnumofimages"
#define SPFN "sppcafilename"
#define SPNOF "spnumoffeatures"
#define SPLITM "spkdtreesplitmethod"
#define MSPREAD "max_spread"
#define RAND "random"
#define INC "incremental"
#define KNN "spknn"
#define MINGUI "spminimalgui"
#define SPLL "sploggerlevel"
#define SPLFN "sploggerfilename"
#define SPNOSI "spnumofsimilarimages"
#define SPXMODE "spextractionmode"
#define PCA "pca.yml"
#define STDOUT "stdout"
#define FILE_NAME "File: %s\n"
#define LINE "Line: %d\n"
#define MSG "Message: %s\n"
#define SSDS "%s%s%d%s"
#define SSD "%s%s%d"
#define SS "%s%s"
#define FALSE "false"
#define TRUE "true"
#define READ "r"
#define SIGN '#'
#define EQ '='
#define ENDOFSTR '\0'
#define WSPACE ' '
#define DEF_DIM 20
#define DEF_FEAT 100
#define DEF_NOSI 1
#define DEF_INPUT 1
#define DEF_KNN 1
#define DEF_LL 3
#define MINDIM 10
#define MAXDIM 28
#define MINLL 1
#define MAXLL 4
#define INIT 0
#define FIRST 0
#define EQUAL 0
#define ABORT -1

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
    int linenum=INIT;
	int n=INIT;
	if(cnfg==NULL)
		return NULL;
	cnfg->spPCADimension = DEF_DIM;
	cnfg->spNumOfFeatures = DEF_FEAT;
	cnfg->spExtractionMode = true;
	cnfg->spNumOfSimilarImages = DEF_NOSI;
	cnfg->splitMethod = MAX_SPREAD;
	cnfg->spKNN = DEF_KNN;
	cnfg->spMinimalGUI = false;
	cnfg->spLoggerLevel = DEF_LL;

	if(filename == NULL){
		spConfigDestroy(cnfg);
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return NULL;
	}
    fp = fopen(filename,READ);
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
    	if (string[FIRST]!=SIGN){
    		if(strchr(string,SIGN) != NULL){
    			printError(filename,linenum, MSG1);
    			free(string);
    			free(input);
    			fclose(fp);
    			*msg = SP_CONFIG_INVALID_ARGUMENT;
    			return NULL;
    		}
    		strlwr(string);
    		start = strchr(string,EQ);
    		start++;
    		len = n-strlen(start);
    		if(strstr(string,SPID)){
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
    			input[0]=DEF_INPUT;
    			if(len==0 || strchr(cnfg->spImagesDirectory,WSPACE)!=NULL){
    				spConfigDestroy(cnfg);
    				free(string);
    				free(input);
    				fclose(fp);
    				printError(filename,linenum,NODIR);
    				*msg= SP_CONFIG_INVALID_STRING;
    				return NULL;
    			}
    		}
    		else if(strstr(string,SPIP)){
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
    			input[1]=DEF_INPUT;
    			if(len==0||strchr(cnfg->spImagesPrefix,WSPACE)!=NULL){
    				spConfigDestroy(cnfg);
    				free(string);
    				free(input);
    				fclose(fp);
    				printError(filename,linenum,NOPRE);
    			    *msg = SP_CONFIG_INVALID_STRING;
    			    return NULL;
    			}
    		}
    		else if(strstr(string,SPIS)){
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
    			if(len==0||strchr(cnfg->spImagesSuffix,WSPACE)!=NULL){
    				spConfigDestroy(cnfg);
    				free(string);
    				free(input);
    				fclose(fp);
    				printError(filename,linenum,NOSUF);
    				*msg = SP_CONFIG_INVALID_STRING;
    				return NULL;
    			}
    			input[2]=DEF_INPUT;
    		}
    		else if(strstr(string,SPNOI)){
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
    			input[3]=DEF_INPUT;
    		}
    		else if(strstr(string,SPCADIM)){
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
    			if(len ==0 || (cnfg->spPCADimension)<MINDIM || (cnfg->spPCADimension)>MAXDIM){
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
    		else if(strstr(string,SPFN)){
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
    			if(strchr(cnfg->spPCAFileName,WSPACE)!=NULL){
    				spConfigDestroy(cnfg);
    				free(string);
    				free(input);
    				fclose(fp);
    				printError(filename,linenum,MSG1);
    			    *msg =SP_CONFIG_INVALID_STRING;
    			    return NULL;
    			}
    		}
    		else if(strstr(string,SPNOF)){
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
    		else if(strstr(string,SPLITM)){
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
    			if(strcmp(temp,MSPREAD)==EQUAL){
    				cnfg->splitMethod = MAX_SPREAD;
    			}
    			else if(strcmp(temp,RAND)==EQUAL){
    				cnfg->splitMethod = RANDOM;
    			}
    			else if(strcmp(temp,INC)==EQUAL){
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
    		else if(strstr(string,KNN)){
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
    		else if(strstr(string,MINGUI)){
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
    			if(strcmp(temp,FALSE)==EQUAL){
    			 	cnfg->spMinimalGUI = false;
    			}
    			else if(strcmp(temp,TRUE)==EQUAL){
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
    		else if(strstr(string,SPLL)){
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
    			if(len ==0 || cnfg->spLoggerLevel<MINLL || cnfg->spLoggerLevel>MAXLL){
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
    		else if(strstr(string,SPLFN)){
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
       			if((strchr(cnfg->spLoggerFilename,WSPACE)!=NULL)){
       				spConfigDestroy(cnfg);
       				free(string);
       				free(input);
       				fclose(fp);
       				printError(filename,linenum,MSG1);
       			    *msg = SP_CONFIG_INVALID_STRING;
       			    return NULL;
       			}
    		}
    		else if(strstr(string,SPNOSI)){
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
    		else if(strstr(string,SPXMODE)){
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
    			if(strcmp(temp,FALSE)==EQUAL){
    				cnfg->spExtractionMode = false;
    			}
    			else if(strcmp(temp,TRUE)==EQUAL){
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
    	cnfg->spPCAFileName = PCA;
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
    	cnfg->spLoggerFilename = STDOUT;
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
	if(input[0] == INIT){
		printError(filename,linenum,NODIR);
		return SP_CONFIG_MISSING_DIR;
	}
	if(input[1]==INIT){
		printError(filename,linenum,NOPRE);
		return SP_CONFIG_MISSING_PREFIX;
	}
	if(input[2]==INIT){
		printError(filename,linenum,NOSUF);
		return SP_CONFIG_MISSING_SUFFIX;
	}
	if(input[3]==INIT){
		printError(filename,linenum,NONUM);
		return SP_CONFIG_MISSING_NUM_IMAGES;
	}
	return SP_CONFIG_SUCCESS;
}
void trim(char* string){
    int dest;
    int src=INIT;
    int len = strlen(string);
    while(isspace(string[src])) src++;
    for(dest=0; src<len; dest++, src++){
        string[dest] = string[src];
    }
    string[dest] = ENDOFSTR;
    for(dest=len-1; isspace(string[dest]); --dest){
    	string[dest] = ENDOFSTR;
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
	printf(FILE_NAME,filename);
	fflush(NULL);
	printf(LINE,line);
	fflush(NULL);
	printf(MSG,msg);
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
		return ABORT;
	}
	if (config->spNumOfImages<=0){
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return ABORT;
	}
	*msg = SP_CONFIG_SUCCESS;
	return config->spNumOfImages;
}
int spConfigGetNumOfFeatures(const SPConfig config, SP_CONFIG_MSG* msg){
	assert(msg!=NULL);
	if (config==NULL){
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return ABORT;
	}
	if (config->spNumOfFeatures<=0){
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return ABORT;
	}
	*msg = SP_CONFIG_SUCCESS;
	return config->spNumOfFeatures;
}
int spConfigGetPCADim(const SPConfig config, SP_CONFIG_MSG* msg){
	assert(msg!=NULL);
	if (config==NULL){
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return ABORT;
	}
	if (config->spPCADimension<=0){
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return ABORT;
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
	sprintf(path,SSDS, config->spImagesDirectory, config->spImagesPrefix, ind, config->spImagesSuffix);
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
	sprintf(path,SSD, config->spImagesDirectory, config->spImagesPrefix,ind);
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
	sprintf(path,SS, config->spImagesDirectory, config->spPCAFileName);
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
		return ABORT;
	return config->spNumOfSimilarImages;
}
SP_KDTREE_SPLIT_METHOD GetSplitMethod(SPConfig config){
	if(config ==NULL)
		return 4;
	return config->splitMethod;
}
int GetSpKNN(SPConfig config){
	if(config ==NULL)
		return ABORT;
	return config->spKNN;
}
