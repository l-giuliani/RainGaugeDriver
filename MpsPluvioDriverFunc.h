#ifndef _MPS_PLUVIO_DRIVER_FUNC_H
#define _MPS_PLUVIO_DRIVER_FUNC_H

#include "types.h"

#define MPS_START_MEASURE   /*start measure command*/""
#define MPS_START_MEASURE1  /*start measure command*/""
#define MPS_READ_STATUS     /*read status command*/ ""
#define MPS_READ_DATA       /*read data command*/""
#define MPS_INIT            /*mps init command*/""

#define START_MEASURE_ID    0
#define START_MEASURE1_ID   1
#define MPS_READ_STATUS_ID  2
#define MPS_READ_DATA_ID    3
#define MPS_READ_DATA1_ID   4
#define MPS_INIT_ID         5

#define	MPSPLUVIO_MIN_STARTMEAS	6		// Min response length start meas
#define	MPSPLUVIO_MIN_POLL_SIZE	32		// Min response length in POLL

#define RX_MESSAGE_TIMEOUT		    1000							  /* msec */

#define MPS_MAX_RETRIES_NUMBER			2

#define CR      0x0D
#define LF      0x0A

#define MAX_RESPONSE_LEN        32

#define DATA_NOT_VALID  -9999999

typedef struct {
    float PR;
	float WAVG; 
	float PRTOT; 
	float PRLAST;
	float RIINST;
	float RI;
	int STATUS;
} MPSPLUVIO_DATA;

int setCommand(char* buffer, int bufferLen, int command_code, const char* command);
boolean execute_decode_function(int command_code, word length, byte* responseBuffer, void* value);

#endif
