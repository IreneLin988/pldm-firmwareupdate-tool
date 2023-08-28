

# PLDM Firmware package Header Generator

This project offers the capability to parse and generate the PLDM firmware update header. Primarily, it decodes the PLDM.json file to produce a binary firmware update header file named pldm_update_header.bin. This binary file is an encapsulated firmware update header. Subsequently, the system calculates the CRC32 for the entire PLDM firmware update header and appends this CRC32 to the end of the header section within the package. Finally, the content from the image_payload.bin file is appended to the created pldm_update_header.bin, forming a complete PLDM firmware update package

# Overview
Starting with a source file named PLDM.json, this generator will:
1. Parse the JSON content to understand the structure and data it contains.
2. Generate an intermediary binary file, PLDM_FW_PKG_Header.bin, representing the firmware package header from the parsed data.
3. If an image_payload.bin file is present, its contents will be appended to pldm_update_header.bin
4 Finally, the utility generates Pldm Firmware Update Package file "pldm_update_pkg.bin ".

# Key Features
### Structured Input: 
The program anticipates a standardized JSON format in PLDM.json, enabling users to easily modify or expand their firmware data.

### Binary Generation: 
It skillfully crafts a binary representation of the package header, optimized for firmware applications.

### Checksum Calculation: 
To ensure the integrity of the data, a checksum is calculated for the entire package header and subsequently appended to the pldm_update_header.bin file.

### Payload Integration: 
Upon detecting an image_payload.bin, the utility effortlessly incorporates its content to the resulting binary header.

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
-> pldm_update_header.bin: This is the binary representation of the firmware header, inclusive of the appended checksum.
-> pldm_update_pkg.bin: This is the binary representation of the entire firmware package, inclusive of the appended image_payload.bin.


## Usage
### Installation
For the json-c library:
    
    sudo apt install libjson-c-dev

### Encoding 
To generate the PLDM_FW_PKG_Header.bin:
    
    make all
    make encode

![image](https://github.com/quanta-Irenelin/PLDM_FW_UPDATE/assets/85274528/3168a588-b750-4157-8a1a-c09a56324a77)
![image](https://github.com/quanta-Irenelin/PLDM_FW_UPDATE/assets/85274528/bf9a6505-9c90-46b1-a966-17715edf0ff9)


### Decoding:
To decode the generated file:


    make decode

![image](https://github.com/quanta-Irenelin/PLDM_FW_UPDATE/assets/85274528/01490ea7-1c65-4ba3-87b0-20c350b4496c)


 ### Pldm Firmware Update Package file name: pldm_update_pkg.bin  
![image](https://github.com/quanta-Irenelin/PLDM_FW_UPDATE/assets/85274528/ae13fd83-fa27-45e1-8870-36d22cf1d68c)

### clear:
To clean the generated file:

        make clean
![image](https://github.com/quanta-Irenelin/PLDM_FW_UPDATE/assets/85274528/093990a6-d5fd-4d96-b5e1-354e8d0c6f6c)


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




