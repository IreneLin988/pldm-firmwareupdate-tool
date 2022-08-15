#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <json-c/json.h>
#include <time.h>
#include "parse_fw_update.h"
#define id(k, v) [v] = #k,

const char  *const des_name_table[] = { id(PCI Vendor ID, 0x0000) id(IANA Enterprise ID, 0x0001) id(UUID, 0x0002) 
                                        id(PnP Vendor ID, 0x0003) id(ACPI Vendor ID, 0x0004) id(PCI Device ID, 0x0100) 
                                        id(PCI Subsystem Vendor ID, 0x0101) id(PCI Subsystem ID, 0x0102) id(PCI Revision ID, 0x0103) 
                                        id(PnP Product Identifie, 0x0104) id(ACPI Product Identifier, 0x0105)
                                      };

static char *compo_class_table[] = {  "Unknown","Other","Driver",
                                      "Configuration Software","Application Software",
                                      "Instrumentation","Firmware/BIOS","Diagnostic Software",
                                      "Operating System","Middleware",
                                      "Firmware","BIOS/FCode","Support/Service Pack","Software Bundle"
                                    };

static char *str_type_table[] = {"Unknown","ASCII","UTF-8","UTF-16","UTF-16LE","UTF-16BE"};                                   

uint16_t buf_ex_bitfield16(uint8_t buf_zero, uint8_t buf_one)
{
  return  buf_zero | buf_one << 8;
}

uint32_t buf_ex_bitfield32(uint8_t buf_zero, uint8_t buf_one, uint8_t buf_two, uint8_t buf_three)
{
  return  buf_zero | (buf_one << 8) | (buf_two << 16) | (buf_three << 24);
}

void reverse(uint8_t arr[])
{
    uint8_t i = 0;
    uint8_t j = 0;
    while(arr[i]!='\0'){
      i++;
    }
    for(j = i-1; j >= 0; j--){
      printf("%02x", arr[j]);
    }
}

uint8_t str_type(uint8_t type)
{ 
    printf("%s\n",str_type_table[type]);
    
    return 0;
}
   
uint16_t descriptor_id(uint8_t buf_zero, uint8_t buf_one)
{   
    uint16_t des_id = buf_zero | buf_one << 8;
    printf("%s\n",des_name_table[des_id]);

    return 0;
}

uint16_t compo_class(uint8_t buf_zero, uint8_t buf_one)
{
    uint16_t compo_class =  buf_zero | buf_one << 8;
    printf("%s\n",compo_class_table[compo_class]);

    return 0;
}

int GetPowerNum(int num)
{
    if(!num){
      return -1;
    }
    int power = 0;
    while (num != 1) {
      num /= 2;
      power++;
    }
    return power;
}
