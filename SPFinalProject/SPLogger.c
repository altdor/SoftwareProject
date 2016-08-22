#include "SPLogger.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <math.h>

//File open mode
#define SP_LOGGER_OPEN_MODE "w"

// Global variable holding the logger
SPLogger logger = NULL;

struct sp_logger_t {
	FILE* outputChannel; //The logger file
	bool isStdOut; //Indicates if the logger is stdout
	SP_LOGGER_LEVEL level; //Indicates the level
};

SP_LOGGER_MSG spLoggerCreate(const char* filename, SP_LOGGER_LEVEL level) {
	if (logger != NULL) { //Already defined
		return SP_LOGGER_DEFINED;
	}
	logger = (SPLogger) malloc(sizeof(*logger));
	if (logger == NULL) { //Allocation failure
		return SP_LOGGER_OUT_OF_MEMORY;
	}
	logger->level = level; //Set the level of the logger
	if (filename == NULL) { //In case the filename is not set use stdout
		logger->outputChannel = stdout;
		logger->isStdOut = true;
	} else { //Otherwise open the file in write mode
		logger->outputChannel = fopen(filename, SP_LOGGER_OPEN_MODE);
		if (logger->outputChannel == NULL) { //Open failed
			free(logger);
			logger = NULL;
			return SP_LOGGER_CANNOT_OPEN_FILE;
		}
		logger->isStdOut = false;
	}
	return SP_LOGGER_SUCCESS;
}

void spLoggerDestroy() {
	if (!logger) {
		return;
	}
	if (!logger->isStdOut) {//Close file only if not stdout
		fclose(logger->outputChannel);
	}
	free(logger);//free allocation
	logger = NULL;
}


SP_LOGGER_MSG spLoggerPrintError(const char* msg, const char* file,
		const char* function, const int line){
	int num;
	if (msg==NULL || file==NULL || function==NULL || line<0)
		return SP_LOGGER_INVAlID_ARGUMENT;
	if (logger == NULL)
		return SP_LOGGER_UNDIFINED ;
	num = fprintf(logger->outputChannel, "%s\n%s%s\n%s%s\n%s%d\n%s%s\n", "---ERROR---","- file: ", file,"- function: " , function, "- line: " , line,"- message: " , msg);//write the details to the log file
	if (num<0)
		return SP_LOGGER_WRITE_FAIL;
	return SP_LOGGER_SUCCESS;
}



SP_LOGGER_MSG spLoggerPrintWarning(const char* msg, const char* file,
		const char* function, const int line){
	int num;
	if (msg==NULL || file==NULL || function==NULL || line<0)
		return SP_LOGGER_INVAlID_ARGUMENT;
	if (logger == NULL)
		return SP_LOGGER_UNDIFINED ;
	if (logger->level>0){
		num = fprintf(logger->outputChannel, "%s\n%s%s\n%s%s\n%s%d\n%s%s\n", "---WARNING---","- file: ", file,"- function: " , function, "- line: " , line,"- message: " , msg);//write the details to the log file
		if (num<0)
			return SP_LOGGER_WRITE_FAIL;
	}
	return SP_LOGGER_SUCCESS;
}

SP_LOGGER_MSG spLoggerPrintInfo(const char* msg){
	int num;
	if (msg==NULL)
		return SP_LOGGER_INVAlID_ARGUMENT;
	if (logger == NULL)
		return SP_LOGGER_UNDIFINED ;
	if (logger->level>1){
		num = fprintf(logger->outputChannel, "%s\n%s%s\n","---INFO---","- message: ",msg);//write the details to the log file
		if (num<0)
			return SP_LOGGER_WRITE_FAIL;
	}
	return SP_LOGGER_SUCCESS;
}

SP_LOGGER_MSG spLoggerPrintDebug(const char* msg, const char* file,
		const char* function, const int line){
	int num;
	if (msg==NULL || file==NULL || function==NULL || line<0)
		return SP_LOGGER_INVAlID_ARGUMENT;
	if (logger == NULL)
		return SP_LOGGER_UNDIFINED ;
	if (logger->level>2){
		num = fprintf(logger->outputChannel, "%s\n%s%s\n%s%s\n%s%d\n%s%s\n","---DEBUG---","- file: ",file,"- function: ",function,"- line: ",line,"- message: ",msg);//write the details to the log file
		if (num<0)
			return SP_LOGGER_WRITE_FAIL;
	}
	return SP_LOGGER_SUCCESS;
}

SP_LOGGER_MSG spLoggerPrintMsg(const char* msg){
	int num;
	if (msg==NULL)
		return SP_LOGGER_INVAlID_ARGUMENT;
	if (logger == NULL)
		return SP_LOGGER_UNDIFINED ;
	num = fprintf(logger->outputChannel, "%s\n",msg); //write the message to the log file
	if (num<0)
		return SP_LOGGER_WRITE_FAIL;
	return SP_LOGGER_SUCCESS;
}
