*** settings ***
Library    String
Library    ../resources/serialPortReader.py
Suite Setup        Open Ports    ${SERIAL_PORT_1}    ${SERIAL_PORT_2}
Suite Teardown     Close Ports 

*** Variables ***
${SERIAL_PORT_1}  /dev/ttyUSB0
${SERIAL_PORT_2}  /dev/ttyUSB1

*** test cases ***
Original Firmware Test
    [Timeout]    30s
    Sleep     5s
    Expect And Timeout    btn 2 m\n    FW_VERSION: 1     5     ${SERIAL_PORT_2}

File Discovery Test
    [Timeout]    30s
    sleep    2
    Expect And Timeout    btn 2 s\n    >>> File discovery complete <<<    5    ${SERIAL_PORT_1}

File Transfer Test
    [Timeout]    60s
    sleep    2
    Expect And Timeout    btn 2 m\n    >>> File transfer complete    20    ${SERIAL_PORT_1}

File Verification Test
    [Timeout]    30S
    sleep    2
    Expect And Timeout    btn 2 l\n    >>> Verify complete status: 0 <<<    2    ${SERIAL_PORT_1}
   
Peer Device Reset Test
    [Timeout]    30s
    sleep    2
    Expect And Timeout   btn 2 x\n    >>> Scanning started <<<    5    ${SERIAL_PORT_1}

Firmware Update Verification Test
    [Timeout]    60s     
    Read All    FW_VERSION: 2     15     ${SERIAL_PORT_2}


Firmware Reconnect Succesful Test
    [Timeout]    60s
    sleep     10
    Expect And Timeout    btn 2 s\n    >>> File discovery complete <<<    15    ${SERIAL_PORT_1}
    

    

