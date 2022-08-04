#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <json-c/json.h>
#include <time.h>
#include "parse_fw_update.h"

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
    for(j = i-1; j >= 0; j--)
      printf("%02x", arr[j]);
}

uint8_t str_type(uint8_t type)
{
  switch(type){
    case 0:
      printf("Unknown\n");
      break;
    case 1:
      printf("ASCII\n");
      break;
    case 2:
      printf("UTF-8\n");
      break;
    case 3:
      printf("UTF-16\n");
      break;
    case 4:
      printf("UTF-16LE\n");
      break;
    case 5:
      printf("UTF-16BE\n");
      break;
    }

    return 0;
}

uint16_t descriptor_id(uint8_t buf_zero, uint8_t buf_one)
{
    uint16_t id =  buf_zero | buf_one << 8;
    switch(id)
    {
      case 0x0000:
        printf("PCI vender ID\n");
        break;
      case 0x0001:
        printf("IANA Enterprise ID\n");
        break;
      case 0x0002:
        printf("UUID\n");
        break;
      case 0x0003:
        printf("PnP Vendor ID\n");
        break;
      case 0x0004:
        printf("ACPI Vendor ID\n");
        break;
      case 0x0100:
        printf("PCI Device ID\n");
        break;
      case 0x0101:
        printf("PCI Subsystem Vendor ID\n");
        break;
      case 0x0102:
        printf("PCI Subsystem ID\n");
        break;
      case 0x0103:
        printf("PCI Revision ID\n");
        break;
      case 0x0104:
        printf("PnP Product Identifier\n");
        break;
      case 0x0105:
        printf("ACPI Product Identifier\n");
        break;
    }
    return 0;
}

uint16_t compo_class(uint8_t buf_zero, uint8_t buf_one)
{
    uint16_t comp_class =  buf_zero | buf_one << 8;
    switch(comp_class)
    {
      case 0x0000:
        printf("Unknown\n");
        break;
      case 0x0001:
        printf("Other\n");
        break;
      case 0x0002:
        printf("Driver\n");
        break;
      case 0x0003:
        printf("Configuration Software\n");
        break;
      case 0x0004:
        printf("Application Software\n");
        break;
      case 0x0005:
        printf("Instrumentation\n");
        break;
      case 0x0006:
        printf("Firmware/BIOS\n");
        break;
      case 0x0007:
        printf("Diagnostic Software\n");
        break;
      case 0x0008:
        printf("Operating System\n");
        break;
      case 0x0009:
        printf("Middleware\n");
        break;
      case 0x000A:
        printf("Firmware\n");
        break;
      case 0x000B:
        printf("BIOS/FCode\n");
        break;
      case 0x000C:
        printf("Support/Service Pack\n");
        break;
      case 0x000D:
        printf("Software Bundle\n");
        break;
    }

    return 0;
}

int GetPowerNum(int num)
{
    if(!num)
      return -1;
    int power = 0;
    while (num != 1) {
      num /= 2;
      power++;
    }
    return power;
}
