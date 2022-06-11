#include "MpsPluvioDriver.h"
#include "stdio.h"

int main(void)
{
    MPSPLUVIO_DATA MPS_pluvio_data;
    boolean res;

    res = MpsPluvio(&MPS_pluvio_data);
    if(res)
    {
        printf("PR: %f\n", MPS_pluvio_data.PR);
        printf("WAVG: %f\n", MPS_pluvio_data.WAVG);
        printf("PRTOT: %f\n", MPS_pluvio_data.PRTOT);
        printf("PRLAST: %f\n", MPS_pluvio_data.PRLAST);
        printf("RIINST: %f\n", MPS_pluvio_data.RIINST);
        printf("RI: %f\n", MPS_pluvio_data.RI);
        printf("STATUS: %d\n", MPS_pluvio_data.STATUS);
    }
    else
    {
        printf("Error Mps Pluvio poll\n");
    }

    return 0;
}