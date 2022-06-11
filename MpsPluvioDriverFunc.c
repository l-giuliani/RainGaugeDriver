/******************************************************************************/
/*           STS module (*)                                                   */
/*                                                                            */
/*          2084 bytes of CODE  memory                                        */
/*           512 bytes of CONST memory                                        */
/*            67 bytes of DATA  memory                                        */
/******************************************************************************/

#include "MpspluvioDriverFunc.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "serial.h"


static int sensor_address;
static boolean sensor_address_initialized = FALSE;

/******************************************************************************/

typedef struct {
    int command_code;
    boolean (*decode_function)(word, byte*,void*);
} DECODE_FUNCTIONS;

/******************************************************************************/

boolean decode_startmeasure_response(word length, byte* buffer, void* value);
boolean decode_startmeasure1_response(word length, byte* buffer, void* value);
boolean decode_readstatus_response(word length, byte* buffer, void* value);
boolean decode_readdata_response(word length, byte* buffer, void* value);
boolean decode_readdata1_response(word length, byte* buffer, void* value);
boolean decode_mpsinit_response(word length, byte* buffer, void* value);

DECODE_FUNCTIONS decode_function[] = {
    {START_MEASURE_ID, &decode_startmeasure_response},
    {START_MEASURE1_ID, &decode_startmeasure1_response},
    {MPS_READ_STATUS_ID, &decode_readstatus_response},
    {MPS_READ_DATA_ID, &decode_readdata_response},
    {MPS_READ_DATA1_ID, &decode_readdata1_response},
    {MPS_INIT_ID, &decode_mpsinit_response}
};

#define DECODE_FUNCTIONS_LEN sizeof(decode_function)/sizeof(DECODE_FUNCTIONS)

boolean execute_decode_function(int command_code, word length, byte* responseBuffer, void* value)
{
    int i;

    for(i=0; i < DECODE_FUNCTIONS_LEN; i++)
    {
        if(decode_function[i].command_code == command_code)
        {
            if(decode_function[i].decode_function != NULL)
            {
                return decode_function[i].decode_function(length, responseBuffer, value);
            }
        }
    }
    return TRUE;
}

boolean decode_mpsinit_response(word length, byte* buffer, void* value)
{
    char* pch;
    char tmpbuf[16];

    pch = strchr((char*)buffer, CR);
    if(pch == NULL)
    {
        return FALSE;
    }
    memset(tmpbuf, 0, sizeof(tmpbuf));
    memcpy(tmpbuf, buffer, pch - ((char*)buffer));
    sensor_address = atoi(tmpbuf);
    sensor_address_initialized = TRUE;

    return TRUE;
}

boolean decode_startmeasure_response(word length, byte* buffer, void* value)
{
    return (length >= MPSPLUVIO_MIN_STARTMEAS);
}

boolean decode_startmeasure1_response(word length, byte* buffer, void* value)
{
    return (length >= MPSPLUVIO_MIN_STARTMEAS);
}

static boolean decode_data(word length, byte* buffer, float* value)
{
    char* pch;
    char* pchprec;
    byte tmpbuf[16];
    char keys[] = "+-";

    if(length < MPSPLUVIO_MIN_STARTMEAS)
    {
        return FALSE;
    }

    pchprec = strpbrk((char*)buffer, keys);
    if(pchprec == NULL)
    {
        return FALSE;
    }
    pch = strpbrk(pchprec+1, keys);
    if(pch == NULL)
    {        
        return FALSE;        
    }
    memset(tmpbuf, 0, sizeof(tmpbuf));
    memcpy(tmpbuf, pchprec, pch - pchprec);
    *value++ = atof(tmpbuf);
    pchprec = pch;

    pch = strpbrk(pchprec+1, keys);
    if(pch == NULL)
    {
        return FALSE;
    }
    memset(tmpbuf, 0, sizeof(tmpbuf));
    memcpy(tmpbuf, pchprec, pch - pchprec);
    *value++ = atof(tmpbuf);
    pchprec = pch;

    pch = strchr(pchprec+1, CR);
    if(pch == NULL)
    {
        return FALSE;
    }
    memset(tmpbuf, 0, sizeof(tmpbuf));
    memcpy(tmpbuf, pchprec, pch - pchprec);
    *value = atof(tmpbuf);
    
    return TRUE;
}

boolean decode_readdata_response(word length, byte* buffer, void* value)
{
    return decode_data(length, buffer, (float*)value);
}

boolean decode_readdata1_response(word length, byte* buffer, void* value)
{
    return decode_data(length, buffer, (float*)value);
}

boolean decode_readstatus_response(word length, byte* buffer, void* value)
{
    char* pch;
    char* pchprec;
    char tmpbuf[16];

    pchprec = strpbrk((char*)buffer, "+-");
    if(pchprec == NULL)
    {
        return FALSE;
    }
    pch = strchr(pchprec+1, CR);
    if(pch == NULL)
    {        
        return FALSE;        
    }
    memset(tmpbuf, 0, sizeof(tmpbuf));
    memcpy(tmpbuf, pchprec, pch - pchprec);
    *((int*)value) = atoi(tmpbuf);
    pchprec = pch;

    return TRUE;
}

int setCommand(char* buffer, int bufferLen, int command_code, const char* command) 
{
    int len = 0;
    if(buffer == NULL && command == NULL)
    {
        return -1;
    }
    memset(buffer, 0, bufferLen);
    if(command_code == MPS_INIT_ID)
    {
        len = sprintf(buffer, "%s", command);
    }
    else
    {
        len = sprintf(buffer, "%d%s", sensor_address, command);
    }

    return len;
}
