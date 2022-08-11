#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <json-c/json.h>
#include <time.h>
#include "data_trans_fxn.h"

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
		const char *str1 = "ASCII";
		uint8_t str_type_val;
    if(strcmp(str1, input) == 0)
			str_type_val = 0x01;

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
    if(!input || !output)
      return -1;
		const char *str1 = "PCI Vendor ID";
    const char *str2 = "PCI Device ID";
    const char *str3 = "PCI Subsystem Vendor ID";
    const char *str4 = "PCI Subsystem ID";
		uint16_t str_type_val;
    uint16_t str_type_len;
    if(strcmp(str1, input) == 0){
			str_type_val = 0x0000;
      str_type_len = 0x0002;
    }else if(strcmp(str2, input) == 0){
			str_type_val = 0x0100;
      str_type_len = 0x0002;
    }else if(strcmp(str3, input) == 0){
			str_type_val = 0x0101;
      str_type_len = 0x0002;
    }else if(strcmp(str4, input) == 0){
			str_type_val = 0x0102;
      str_type_len = 0x0002;
    }
		output[0] = str_type_val;
    output[1] = str_type_len;

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
  if(!input || !output)
    return -1;
  if(size > (len/2))
    return -1;
  for (int i = 0; i < len; i+=2)
  {
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
		const char *str1 = "Firmware";
    const char *str2 = "Driver";
		uint16_t str_type_val;
    if(strcmp(str1, input) == 0)
			str_type_val = 0x000A;
    else if(strcmp(str2, input) == 0)
			str_type_val = 0x0002;

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
    if(!input)
      return -1;
		const char *str1 = "Force Update";
    const char *str2 = "Use Component Comparison Stamp";
		uint16_t compo_opt_val;
    if(strcmp(str1, input) == 0)
			bit16_ctrl_1(&compo_opt_val, 0);
    else if(strcmp(str2, input) == 0)
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

    const char *str1 = "Self-Contained";
    const char *str2 = "Medium-specific reset";
		const char *str3 = "System reboot";
    const char *str4 = "DC power cycle";
    const char *str5 = "AC power cycle";
    if(strcmp(str1, input) == 0)
			bit16_ctrl_1(&bit_val, 1);
    else if(strcmp(str2, input) == 0)
			bit16_ctrl_1(&bit_val, 2);
    else if(strcmp(str3, input) == 0)
			bit16_ctrl_1(&bit_val, 3);
    else if(strcmp(str4, input) == 0)
			bit16_ctrl_1(&bit_val, 4);
    else if(strcmp(str5, input) == 0)
			bit16_ctrl_1(&bit_val, 5);

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
  printf("time: %s\n",t1);
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
