#include "MpsPluvioDriver.h"
#include "MpsPluvioDriverFunc.h"


static boolean execute_command(int command_code, const char* command_str, void* value)
{
    char command[32];
    char response_buffer[32];
    int commandLen;
    int length;
    boolean res;

    commandLen = setCommand(command, sizeof(command), command_code, command_str);
    uart_tx_data((byte*)command, commandLen);
    length = uart_rx_data((byte*)response_buffer);
    if(length <= 0)
    {
        return FALSE;
    }
    res = execute_decode_function(command_code, length, response_buffer, value); 
    if(!res)
    {
        return FALSE;
    }
}

boolean MpsPluvio (MPSPLUVIO_DATA* MPS_pluvio_data)
{
    boolean command_res;

    command_res = execute_command(MPS_INIT_ID, MPS_INIT, NULL);
    if(!command_res)
    {
        return FALSE;
    }

    command_res = execute_command(START_MEASURE_ID, MPS_START_MEASURE, NULL);
    if(!command_res)
    {
        return FALSE;
    }

    command_res = execute_command(MPS_READ_DATA_ID, MPS_READ_DATA, (void*)&MPS_pluvio_data->PR);
    if(!command_res)
    {
        return FALSE;
    }
    
    command_res = execute_command(START_MEASURE1_ID, MPS_START_MEASURE1, NULL);
    if(!command_res)
    {
        return FALSE;
    }

    command_res = execute_command(MPS_READ_DATA1_ID, MPS_READ_DATA, (void*)&MPS_pluvio_data->PRLAST);
    if(!command_res)
    {
        return FALSE;
    }

    command_res = execute_command(MPS_READ_STATUS_ID, MPS_READ_STATUS, (void*)&MPS_pluvio_data->STATUS);
    if(!command_res)
    {
        return FALSE;
    }

    return TRUE;

}