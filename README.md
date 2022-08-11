# PLDM Firmware Update
Install json-c library:
    
    sudo apt-get install libjson0 libjson0-dev

Build Command:
    
    gcc main.c libpldm.a -lz -ljson-c -o test
BIN file name: PLDM_FW_PKG_Header.bin  

libpldm.a
---------------------
This is a library which encoding PLDM messages from the user's json file.

libparse.a
---------------------
This is a library which decoding PLDM messages from the user's bin file.

Refer to PLDM for Firmware Update Specification DSP0267_1.0.0.pdf

https://www.dmtf.org/documents/pmci/pldm-firmware-update-specification-100