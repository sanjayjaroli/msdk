# This file can be used to set build configuration
# variables.  These variables are defined in a file called 
# "Makefile" that is located next to this one.

# For instructions on how to use this system, see
# https://analogdevicesinc.github.io/msdk/USERGUIDE/#build-system

#BOARD=MAX32520FTHR
# ^ For example, you can uncomment this line to make the 
# project build for the "MAX32520FTHR" board.

# **********************************************************

# Add your config here!

# Set hardware floating-point acceleration
MFLOAT_ABI = hard

# Include the CMSIS-DSP library
LIB_CMSIS_DSP = 1

