#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <json-c/json.h>
#include <time.h>
#include "data_trans_fxn.h"
#define str_type_num 6

/*
  - uint8_t str_type(const char *input)
  - Name: string type
  - Description: Transforming the string type into value.
    (Refer to DSP0267_1.0.0 Table 20-String Type values)
  - Input:
      * input: string of string type
  - Return:
      * -value of string type
      * -1, get error(check input string)
*/
uint8_t str_type(const char *input)
{
    if(!input)
      return -1;
    const char *str[str_type_num] = {"Unknown","ASCII","UTF-8","UTF-16","UTF-16LE","UTF-16BE"};

    uint8_t str_type_val;
    int i = 0;
    while(i < str_type_num){
      if(strcmp(input, str[i]) == 0){
        str_type_val = i;
      }
      i++;
    }

    return str_type_val;
}

int bit32_ctrl_1 (uint32_t *input, int bit)
{
    if(!input)
      return -1;
    *input |= (1 << bit);

    return 0;
}

int bit16_ctrl_1 (uint16_t *input, int bit)
{
    if(!input)
      return -1;
    *input |= (1 << bit);

    return 0;
}

int bit8_ctrl_1 (uint8_t *input, int bit)
{
    if(!input)
      return -1;
    *input |= (1 << bit);

    return 0;
}


int GetMsbPos(int num)
{
    if(!num)
      return -1;

    int pos = 0;
    while (num) {
        pos++;
        num = num >> 1;
    }

    return pos - 1;
}

/*
  - uint32_t update_opt_val(const char *input)
  - Name: Device Update Option Flags
  - Description:
  - Input:
      * input: string of Option
  - Return:
      * 1 Default :Continue component updates after failure
*/


uint32_t update_opt_val(const char *input)
{
    if(!input)
      return 1;

		const char *str1 = "Continue component updates after failure";
		uint32_t opt_val;
    if(strcmp(str1, input) == 0)
			bit32_ctrl_1(&opt_val, 0);

		return opt_val;
}

/*
  - int init_des_type(const char *input, uint16_t *output)
  - Name: Descriptor Data
  - Description: Transforming the string of descriptors into value.
    (Refer to DSP0267_1.0.0 Table 7- Descriptor identifier table)
  - Input:
      * input: string of descriptors
  - Return:
      * -string type value in buf[0] and string length in buf[1]
      * -1, get error(check input string)
*/

int init_des_type(const char *input, uint16_t *output)
{
  if(!input || !output){
      return -1;
  }
	const char *str[11] = { "PCI Vendor ID", "IANA Enterprise ID", "UUID", 
	                        "PnP Vendor ID", "ACPI Vendor ID", "PCI Device ID",
	                        "PCI Subsystem Vendor ID", "PCI Subsystem ID", "PCI Revision ID",
	                        "PnP Product Identifier", "ACPI Product Identifier",
	                      };
	uint16_t str_val_len[11][2] = { {0x0000,2}, {0x0001,4}, {0x0002,16}, 
	                                {0x0003,3}, {0x0004,4}, {0x0100,2}, 
	                                {0x0101,2}, {0x0102,2}, {0x0103,1}, 
	                                {0x0104,4}, {0x0105,4}
                                };
  for(int i=0; i < 11; i++){
      if(strcmp(str[i], input) == 0){
            output[0] =  str_val_len[i][0];
            output[1] =  str_val_len[i][1];
      }
  }

  return 0;
}

/*
  -int hexstr_tobin(char *input, uint8_t len, uint8_t *output, uint8_t size)
  - Name: Tranfer hex-string to binary data
  - Description:
    e.g. Input: string "1FF0" => uint8_t *output = 0x1FF0
  - Input:
      * input: string of hex char
  - Return:
      * -1, get error(check input string)
*/

int hexstr_tobin(char *input, uint8_t len, uint8_t *output, uint8_t size){
  char str[2];
  uint8_t j = 0;
  if(!input || !output){
    return -1;
  }

  if(size > (len/2)){
    return -1;
  }

  for (int i = 0; i < len; i+=2){
    memcpy(str, input + i, 2);
    output[j] = strtol(str,NULL,16);
    j++;
  }
  return 0;
}

/*
  - uint16_t compo_class_val(const char *input)
  - Name: Component Classification Value
  - Description: Transforming the string of Classification into value.
    (Refer to DSP0267_1.0.0 Table 19-ComponentClassification values)
  - Input:
      * input: string of Component Classification
  - Return:
      * -string type value
      * -1, get error(check input string)
*/


uint16_t compo_class_val(const char *input)
{
    if(!input)
      return -1;
    const char *str[14] = { "Unknown","Other","Driver",
                            "Configuration Software","Application Software",
                            "Instrumentation","Firmware/BIOS","Diagnostic Software",
                            "Operating System","Middleware",
                            "Firmware","BIOS/FCode","Support/Service Pack","Software Bundle"
                            };

    uint8_t str_type_val;
    int i = 0;
    while(i < 14){
        if(strcmp(input, str[i]) == 0){
          str_type_val = i;
        }
        i++;
    }

    return str_type_val;
}


/*
  - uint16_t compo_opt_val(const char *input)
  - Name: Component Options
  - Description: Set val_buf[0] or val_buf[1] from input string
  - Input:
      * input: string of the Component Options
      * compo_opt_val: bitfield of the Component Options
  - Return:
      * -1, get error(check input string)
*/

uint16_t compo_opt_val(const char *input)
{
    if(!input){
      return -1;
    }
		const char *str[2] = {"Force Update","Use Component Comparison Stamp"};
		uint16_t compo_opt_val;
    if(strcmp(str[0], input) == 0)
			bit16_ctrl_1(&compo_opt_val, 0);
    else if(strcmp(str[1], input) == 0)
			bit16_ctrl_1(&compo_opt_val, 1);

		return compo_opt_val;
}

/*
  - uint16_t act_meth_val(const char *input, uint16_t bit_val)
  - Name: Component Activation Method
  - Description: Get 16 bits data of the method.
  - Input:
      * input: string of the method
      * bit_val: Accumulation bitfield of the various methods
  - Return:
      * -1, get error(check input string)
*/

uint16_t act_meth_val(const char *input, uint16_t bit_val)
{
    if(!input)
      return -1;

    const char *str[6] = {"Automatic","Self-Contained","Medium-specific reset","System reboot","DC power cycle","AC power cycle"};
    for(int i = 0; i < 6; i++){
      if(strcmp(str[i], input) == 0){
        bit16_ctrl_1(&bit_val, i);
      }
    }

    return bit_val;
}

/*
  - int str_tobin(char *input, uint8_t len, uint8_t *output, uint8_t size)
  - Name:
  - Description: .
  - Input:
      * input:
      * bit_val:
  - Return:
      * -1, get error(check input string)
*/

int str_tobin(char *input, uint8_t len, uint8_t *output, uint8_t size)
{
  char str[2];
  uint8_t j = 0;
  if(!input || !output)
    return -1;
  if(size < (len/2))
    return -1;
  for (int i = 0; i < len; i+=2)
  {
    memcpy(str, input + i, 2);
    output[j] = atoi(str);
    j++;
  }
  return 0;
}

uint8_t *timestamp104(){
  static uint8_t timestamp104[13];
  time_t rawtime;
  struct tm *release_time;
  uint8_t len;
  char t1[30];
  char t1_y[5];
  uint8_t t2[5];
  uint8_t year[2];
  uint8_t tm[5] = {0};
  uint8_t t_UTC = 0x06;
  time(&rawtime);
  release_time = localtime(&rawtime);
  strftime(t1,sizeof(t1),"%S%M%H%e%m", release_time);
  strftime(t1_y,sizeof(t1_y),"%Y", release_time);
  len = strlen(t1);
  str_tobin(t1,len,t2, sizeof(t2));
  year[0] = atoi(t1_y);
  year[1] = atoi(t1_y) >> 8;
  // printf("time: %s\n",t1);
  memcpy(timestamp104, &tm,  sizeof(tm));
  memcpy(timestamp104 + 5, &t2,  sizeof(t2));
  memcpy(timestamp104 + 10, &year,  sizeof(year));
  memcpy(timestamp104 + 12, &t_UTC,  sizeof(t_UTC));

  return timestamp104;
}

int power(int num){
  if(!num)
    return -1;

  int result = 1;
  if(num == 0)
    return result;
  if(num > 0)
  {
    for(num; num > 0; num--)
      result = 2 * result;
  }
  return result;
}
