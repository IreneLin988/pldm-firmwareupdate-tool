# PLDM Firmware Update
Install json-c library:
    
    sudo apt-get install libjson0 libjson0-dev

Build Command:
    
    gcc main.c libpldm.a -lz -ljson-c -o test
BIN file name: PLDM_FW_PKG_Header.bin  

libpldm.a
---------------------
    
    $ ar -t libpldm.a
    compo.o
    pkg_info.o
    FD_ID.o
    data_trans_fxn.o

libparse.a
---------------------

    $ ar -t libparse.a
    parse_PLDM_json.o

Refer to PLDM for Firmware Update Specification DSP0267_1.0.0.pdf
https://www.dmtf.org/documents/pmci/pldm-firmware-update-specification-100
