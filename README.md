

# PLDM Firmware package Header Generator

This project offers the capability to parse and generate the PLDM firmware update header. Primarily, it decodes the PLDM.json file to produce a binary firmware update header file named PLDM_FW_PKG_Header.bin. This binary file is an encapsulated firmware update header. Furthermore, the content from the image_payload.bin file is appended to the created PLDM_FW_PKG_Header.bin, forming a complete PLDM firmware update package. Subsequently, the system calculates the CRC32 for the entire PLDM firmware update package and appends this CRC32 to the end of the header section within the package.

# Overview
Starting with a source file named PLDM.json, this generator will:
1. Parse the JSON content to understand the structure and data it contains.
2. Generate an intermediary binary file, PLDM_FW_PKG_Header.bin, representing the firmware package header from the parsed data.
3. If an image_payload.bin file is present, its contents will be appended to PLDM_FW_PKG_Header.bin.
4 Finally, the utility calculates the checksum for the entire firmware update package, ensuring data integrity and appends it to the end of PLDM_FW_PKG_Header.bin.

# Key Features
### Structured Input: 
The program anticipates a standardized JSON format in PLDM.json, enabling users to easily modify or expand their firmware data.

### Binary Generation: 
It skillfully crafts a binary representation of the package header, optimized for firmware applications.

### Payload Integration: 
Upon detecting an image_payload.bin, the utility effortlessly incorporates its content to the resulting binary header.

### Checksum Calculation: 
To ensure the integrity of the data, a checksum is calculated for the entire package header and subsequently appended to the PLDM_FW_PKG_Header.bin file.

## How to Use
1. Place your PLDM.json and optionally, image_payload.bin, in the project's directory.
2. Run the utility.
3. Once completed, retrieve the PLDM_FW_PKG_Header.bin with the appended checksum.
This streamlined process ensures quick turnaround times, allowing firmware developers to focus on their core functionalities while trusting the integrity and accuracy of their package headers.

## Prerequisites
Ensure the following are set up on your system before proceeding:

(1) C compiler: For instance, GCC.

(2) json-c: A JSON library for C.

(3) Files:
        (i) PLDM.json: This should be present in the same directory as the program, serving as the input.
        (ii) image_payload.bin: This file's content will be appended to the generated firmware package header.
## Important Notes
Ensure PLDM.json and image_payload.bin are located in the same directory.
Install all necessary dependencies before running the program.

## Input
The program expects a file named PLDM.json in the same directory to serve as input. Additionally, there should be a image_payload.bin file, the content of which will be appended to the generated firmware package header.        

## Output
Upon successful execution, the utility generates:
-> PLDM_FW_PKG_Header.bin: This is the binary representation of the firmware package header, inclusive of the appended checksum.


## Usage
### Installation
For the json-c library:
    
    sudo apt install libjson-c-dev

### Encoding 
To generate the PLDM_FW_PKG_Header.bin:
    
    make all
    make encode

### Decoding:
To decode the generated file:
    make decode

BIN file name: PLDM_FW_PKG_Header.bin  

### Details of the libpldm.a Library
You can view the object files within the libpldm.a static library using:
    
    $ ar -t libpldm.a

#### Contained objects:
1. compo.o
2. pkg_info.o
3. FD_ID.o
4. data_trans_fxn.o
5. parse_PLDM_json.o


## Additional Resources
For more information on the PLDM firmware update specification, refer to the document: 
PLDM for Firmware Update Specification DSP0267_1.0.0.pdf:

https://www.dmtf.org/sites/default/files/standards/documents/DSP0267_1.0.0.pdf


## Result:
----------------------------------------------------------------------------
![image](https://github.com/quanta-Irenelin/PLDM_FW_UPDATE/assets/85274528/efabeefe-a06a-4c7d-8226-fdb00c48f08c)
![image](https://github.com/quanta-Irenelin/PLDM_FW_UPDATE/assets/85274528/5684b49a-4f46-42e6-b527-284087eb03cb)
![image](https://github.com/quanta-Irenelin/PLDM_FW_UPDATE/assets/85274528/6d1ffb13-6928-40af-9a38-19f222421cb4)
![image](https://github.com/quanta-Irenelin/PLDM_FW_UPDATE/assets/85274528/8bff0b1f-9b17-45b0-8477-f73388bec31f)
![image](https://github.com/quanta-Irenelin/PLDM_FW_UPDATE/assets/85274528/dcac495f-5226-466a-818c-a74867a09b6f)



