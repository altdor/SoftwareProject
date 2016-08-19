#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "unit_test_util.h"
#include "../SPLogger.h"


static bool basicLoggerTest()
{
	ASSERT_TRUE(spLoggerPrintError("A","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_UNDIFINED);
	spLoggerDestroy();
	return true;
}


static bool basicLoggerErrorTest() 
{
	
	const char* testFile = "basicLoggerErrorTest.log";
	ASSERT_TRUE(spLoggerCreate(testFile,SP_LOGGER_ERROR_LEVEL) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintError("MSGA","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintWarning("MSGB","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintInfo("MSGC") == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintDebug("MSGD","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	spLoggerDestroy();
	
	return true;
}


static bool basicLoggerDebugTest() 
{
	
	const char* testFile = "basicLoggerDebugTest.log";
	ASSERT_TRUE(spLoggerCreate(testFile,SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintError("MSGA","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintWarning("MSGB","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintInfo("MSGC") == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintDebug("MSGD","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	spLoggerDestroy();
	
	return true;
}


static bool basicLoggerWarningTest()
{
	
	const char* testFile = "basicLoggerWarningTest.log";
	ASSERT_TRUE(spLoggerCreate(testFile,SP_LOGGER_WARNING_ERROR_LEVEL) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintError("MSGA","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintWarning("MSGB","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintInfo("MSGC") == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintDebug("MSGD","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	spLoggerDestroy();
	
	return true;

}


static bool basicLoggerInfoTest()
{
	
	const char* testFile = "basicLoggerInfoTest.log";
	ASSERT_TRUE(spLoggerCreate(testFile,SP_LOGGER_INFO_WARNING_ERROR_LEVEL) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintError("MSGA","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintWarning("MSGB","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintInfo("MSGC") == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintDebug("MSGD","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	spLoggerDestroy();
	
	return true;

}


static bool basicLoggerMsgTest() 
{
	
	const char* testFile = "basicLoggerMsgTest.log";
	ASSERT_TRUE(spLoggerCreate(testFile,SP_LOGGER_INFO_WARNING_ERROR_LEVEL) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintMsg("MSG") == SP_LOGGER_SUCCESS);
	spLoggerDestroy();
	
	return true;
}

static bool basicLoggerArgumentTest() 
{
	const char* testFile = "basicLoggerFailTest.log";
	ASSERT_TRUE(spLoggerCreate(testFile,SP_LOGGER_INFO_WARNING_ERROR_LEVEL) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintError(0,"sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_INVAlID_ARGUMENT); 
	ASSERT_TRUE(spLoggerPrintError("MSGA","sp_logger_unit_test.c",__func__,-10) == SP_LOGGER_INVAlID_ARGUMENT); 
	ASSERT_TRUE(spLoggerPrintError("MSGB","sp_logger_unit_test.c",0,__LINE__) == SP_LOGGER_INVAlID_ARGUMENT); 
	spLoggerDestroy();

	return true;
}

static bool basicLoggerDestroyUndefinedTest() 
{
	const char* testFile = "basicLoggerFailTest.log";
	ASSERT_TRUE(spLoggerCreate(testFile,SP_LOGGER_INFO_WARNING_ERROR_LEVEL) == SP_LOGGER_SUCCESS);
	spLoggerDestroy();
	ASSERT_TRUE(spLoggerPrintError("MSGC","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_UNDIFINED);
	return true;
}

int main()
{
	RUN_TEST(basicLoggerTest);
	RUN_TEST(basicLoggerErrorTest);
	RUN_TEST(basicLoggerDebugTest);
	RUN_TEST(basicLoggerWarningTest);
	RUN_TEST(basicLoggerInfoTest);
	RUN_TEST(basicLoggerMsgTest);
	RUN_TEST(basicLoggerArgumentTest);
	RUN_TEST(basicLoggerDestroyUndefinedTest);

	return 0;
}


