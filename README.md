

# PLDM Firmware package Header Generator

This project provides functionality for parsing and generating a PLDM firmware package header. It primarily parses a PLDM.json and creates a binary firmware package header file named PLDM_FW_PKG_Header.bin. In addition, it calculates the checksum for the header and appends it to the end of this binary file.

To run this program, make sure you have the following dependencies installed on your system:
1. C compiler (e.g., GCC)
2. json-c

Input
The program expects a file named PLDM.json in the same directory to serve as input. Additionally, there should be a image_payload.bin file, the content of which will be appended to the generated firmware package header.

Output
The program will produce PLDM_FW_PKG_Header.bin, which is the encoded firmware package header with the checksum appended at the end.

Notes
Ensure PLDM.json and image_payload.bin are located in the same directory.
Install all necessary dependencies before running the program.

# How to Run
Install json-c library:
    
    sudo apt install libjson-c-dev

Encode Command:
Upon execution, the program will produce PLDM_FW_PKG_Header.bin.
    
    make all
    make encode

Decode Command:
    
    make decode

BIN file name: PLDM_FW_PKG_Header.bin  

libpldm.a
---------------------
    
    $ ar -t libpldm.a
    compo.o
    pkg_info.o
    FD_ID.o
    data_trans_fxn.o
    parse_PLDM_json.o


Refer to PLDM for Firmware Update Specification DSP0267_1.0.0.pdf
https://www.dmtf.org/documents/pmci/pldm-firmware-update-specification-100

# Result:
----------------------------------------------------------------------------
![image](https://github.com/quanta-Irenelin/PLDM_FW_UPDATE/assets/85274528/efabeefe-a06a-4c7d-8226-fdb00c48f08c)
![image](https://github.com/quanta-Irenelin/PLDM_FW_UPDATE/assets/85274528/5684b49a-4f46-42e6-b527-284087eb03cb)
![image](https://github.com/quanta-Irenelin/PLDM_FW_UPDATE/assets/85274528/6d1ffb13-6928-40af-9a38-19f222421cb4)
![image](https://github.com/quanta-Irenelin/PLDM_FW_UPDATE/assets/85274528/8bff0b1f-9b17-45b0-8477-f73388bec31f)
![image](https://github.com/quanta-Irenelin/PLDM_FW_UPDATE/assets/85274528/dcac495f-5226-466a-818c-a74867a09b6f)



