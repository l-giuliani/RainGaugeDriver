## Unofficial driver for Rain Gauge

This is a simple driver that can read some params from a commercial Rain Gauge based on weight measurement.
The driver of the rain gauge uses a simple ascii protocol over serial communication.

serial read and write functions are stub functions.
MpsPluvioTest.c is an example program that uses the driver.

The driver allows to read the following params:

- PR : rain in ml since last data request
- WAVG : weight of water inside the rain gauge
- PRTOT : total rain in ml since the rain gauge is on 
- PRLAST : rain in ml since last rain event 
- RIINST : instantaneous rain intensity 
- RI : rain intensity
- STATUS : rain gauge status