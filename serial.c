/*****************************************************
Serial functions are stubbed
*****************************************************/

#include "serial.h"
#include "string.h" 
#include "stdio.h"

typedef struct {
    char* request;
    char* prec_command;
    char * response;
} REQUEST_RESPONSE_MAP;

REQUEST_RESPONSE_MAP rr_map[] = {
    {"PI", "", "0\r\n"},
    {"K$", "", "00005\r\n"},
    {"DR$", "K$", "0+0.2+120+0.8\r\n"},
    {"K1$", "", "00005\r\n"},
    {"DR$", "K1$", "0+0.42+2-1\r\n"},
    {"SR$", "", "0+1\r\n"}
};
#define RR_MAP_LEN sizeof(rr_map)/sizeof(REQUEST_RESPONSE_MAP)

static int prec_len;
static byte prec_command[32];
static byte current_command[32];

void uart_tx_data(byte* command, int commandLen)
{
    if(prec_len != 0)
    {
        memset(prec_command, 0, sizeof(prec_command));
        memcpy(prec_command, current_command, prec_len);
    }

    prec_len = command[0]=='0'?commandLen-1:commandLen;
    memset(current_command, 0, sizeof(current_command));
    memcpy(current_command, command[0]=='0'?command+1:command, prec_len);
}

int uart_rx_data(byte* response_buffer)
{
    int i;
    int len;

    for(i=0; i<RR_MAP_LEN; i++)
    {
        if(strcmp(rr_map[i].request, current_command) == 0)
        {
            if(strcmp(rr_map[i].request, "DR$") == 0)
            {
                if(strcmp(prec_command, "K$") == 0 && strcmp(rr_map[i].prec_command, "K$") == 0)
                {
                    len = sprintf(response_buffer, "%s", rr_map[i].response);
                    return len;
                }
                else if(strcmp(prec_command, "K1$") == 0 && strcmp(rr_map[i].prec_command, "K1$") == 0)
                {
                    len = sprintf(response_buffer, "%s", rr_map[i].response);
                    return len;
                }
            }
            else 
            {
                len = sprintf(response_buffer, "%s", rr_map[i].response);
                return len;
            }
        }
    }
    
}
