# This file can be used to set build configuration
# variables.  These variables are defined in a file called 
# "Makefile" that is located next to this one.

# For instructions on how to use this system, see
# https://analogdevicesinc.github.io/msdk/USERGUIDE/#build-system

# **********************************************************

# Enable Cordio library
LIB_CORDIO = 1

# Cordio library options
INIT_PERIPHERAL = 0
INIT_CENTRAL = 1

# TRACE option
# Set to 0 to disable
# Set to 1 to enable serial port trace messages
# Set to 2 to enable verbose messages
TRACE = 1

# set ADVTISEMENT name you want to connect
ADV_NAME?=DATS
PROJ_CFLAGS += -DADV_NAME=\"$(ADV_NAME)\"

### CONFIGURE security
# /*! TRUE to initiate security upon connection*/
PROJ_CFLAGS += -DINIT_SECURITY=TRUE