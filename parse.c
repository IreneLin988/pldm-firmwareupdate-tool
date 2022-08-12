// PLDM package header//

/*----------------------
  pkg_header_info
------------------------
  fw_device_id_area
------------------------
  compo_info_id_area
------------------------
      checksum
------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/stat.h>
#include "parse_fw_update.h"


int main(){

  uint16_t pkg_header_size;

  FILE *b_file;
	b_file = fopen("PLDM_FW_PKG_Header.bin","rb");
  fseek(b_file, 17, SEEK_SET);
  fread(&pkg_header_size, sizeof(pkg_header_size), 1, b_file);
  fseek(b_file, 0L, SEEK_SET);

  uint8_t data[pkg_header_size];
  fread(data, sizeof(data), 1, b_file);
  fclose(b_file);

  int index = 0;
  int i = 0;
  header_info *p_header_info;
  p_header_info = (header_info *) &data[index];
  index += sizeof(header_info);
  uint16_t bitmap_len_byte = buf_ex_bitfield16(p_header_info -> compo_bitmap_len[0], p_header_info -> compo_bitmap_len[1])/8;

  printf("-----------------------------------\n");
  printf("Package Header Information List:\n");
  printf("-----------------------------------\n");

  uint8_t *p_pkg_ver_str;
  p_pkg_ver_str = &data[index];
  index += p_header_info -> pkg_ver_str_len[0];

  printf("Package Header Identifier (UUID):");
  for (i = 0; i < HEADER_ID_LENTH ; i++){
    printf("%02x", p_header_info -> pkg_header_id[i]);
  }
  printf("\n");
  printf("PLDM Package Header Revision: %02x\n",p_header_info -> pkg_header_revision[0]);
  printf("Package Header Size: %02d bytes\n", p_header_info -> pkg_header_size[0] | p_header_info -> pkg_header_size[1] << 8);

  uint16_t year = buf_ex_bitfield16(p_header_info -> yr[0], p_header_info -> yr[1]);
  printf("Package Release Date Time:");
  printf("( UTC and Time resolution: %02x ), %02d/%02d/%02d ", p_header_info -> UTC[0], year, p_header_info -> mon[0], p_header_info -> day[0]);
  printf("%02d:%02d:%02d\n", p_header_info -> hr[0], p_header_info -> min[0], p_header_info -> sec[0]);

  uint16_t bit_len = p_header_info -> compo_bitmap_len[0] | p_header_info -> compo_bitmap_len[1]<<8;
  printf("Component Bitmap Bit Length: %02d Bits (%02d Bytes)\n",bit_len, bit_len/8 );

  printf("Package Version String Type: ");
  str_type(p_header_info -> pkg_ver_str_type[0]);
  printf("Package Version String Length: %d Bytes \n", p_header_info -> pkg_ver_str_len[0]);
  printf("Package Version String: ");

  for(i = 0; i < p_header_info -> pkg_ver_str_len[0]; i++){
    printf("%c" , p_pkg_ver_str[i]);
  }
  printf("\n\n");

/*------------------------------------------------------------------------------------------------------------------------
                                  fw_device_id_area
--------------------------------------------------------------------------------------------------------------------------*/

printf("-----------------------------------\n");
printf("Firmware Device ID Record List:\n");
printf("-----------------------------------\n");
dev_id_rec_count[0] = data[index];
printf("Device ID Record Count: %d\n\n", dev_id_rec_count[0]);
index += 1;
for(int j =0 ; j < dev_id_rec_count[0]; j++)
{
      device_id *p_device_id;
      p_device_id = (device_id *) &data[index];
      index += sizeof(device_id);
      uint16_t id_rec_len = (p_device_id -> rec_len[0]) |((p_device_id->rec_len[1]<<8));
      printf("\33[4mFirmware Device ID Record #%d\33[0m\n", j);
      printf("Record Length: %02d Bytes\n", id_rec_len);
      printf("Device Update Option Flags:0x");
      for (i = 3; i >= 0 ; i--){
        printf("%02x", p_device_id -> dev_update_opt_flag[i]);
      }
      printf("\n");
      
      printf("Component Image Set Version String Type: ");
      str_type(p_device_id -> compo_image_setver_str_type[0]);
      printf("Component Image Set Version String Length: %02d bytes\n", p_device_id -> compo_image_setver_str_len[0]);

      uint16_t fw_pkg_data_len = buf_ex_bitfield16(p_device_id -> fw_pkg_data_len[0], p_device_id -> fw_pkg_data_len[1]);
      uint8_t *p_applicable_compo;
      p_applicable_compo = &data[index];
      index +=  bitmap_len_byte;
      uint8_t *p_compo_image_setver_str = malloc(p_device_id -> compo_image_setver_str_len[0]);
      memcpy(p_compo_image_setver_str, &data[index], p_device_id -> compo_image_setver_str_len[0]);
      index += p_device_id -> compo_image_setver_str_len[0];
      printf("Component Image Set Version String: ");
      for(i = 0; i < p_device_id -> compo_image_setver_str_len[0]; i++)
        printf("%c" , p_compo_image_setver_str[i]);
      printf("\n");
      free(p_compo_image_setver_str);
      printf("Applicable component:\n");
      for(i = 0; i < bitmap_len_byte ; i++)
      { 
        int bit = p_applicable_compo[i]; //Get the smallest component image map 
        while(bit)
        { 
          int dev = bit & -bit; //Get the smallest component image location by finding LSB
          bit = bit ^ dev; //delete the seleted component image location
          dev = GetPowerNum(dev);
          printf("    #%d Image in the PLDM payload \n", (dev + (8*i)));
        }
      }
      printf("The number of descriptors: %02d descriptors\n", p_device_id -> descriptor_count[0]);


      //-----Descriptor record in  Firmware Device Identification Area --------------------------//

      for(i=1 ; i <= p_device_id -> descriptor_count[0];i++){
        int a =0;
        rec *p_rec;
        p_rec = (rec *) &data[index];
        printf("    Type #%02d: ", i);
        descriptor_id(p_rec-> init_descriptor_type[0], p_rec-> init_descriptor_type[1]);
        uint8_t length = p_rec-> init_descriptor_len[1] | p_rec-> init_descriptor_len[0];
        printf("    Descriptor ID length: %d bytes\n", length);
        index += sizeof(rec);
          printf("    Descriptor ID:");
          for(a = 0; a < length; a++){
            uint8_t *p_init_des_data;
            p_init_des_data = &data[index];
            index += 1;
            printf(" %02x" , p_init_des_data[0]);
          }
        printf("\n\n");
      }

      if(fw_pkg_data_len == 0x0000){
        printf("Firmware Device Package Data Length: no data\n");
      }else{
        printf("Firmware Device Package Data: 0x");
        uint8_t *p_fw_pkg_data;
        p_fw_pkg_data = &data[index];
        index += fw_pkg_data_len;
        for(i = 0; i < fw_pkg_data_len; i++){
          printf("%02x ", p_fw_pkg_data[i]);
        }
      }
      printf("\n");

}

  printf("-----------------------------------\n");
  printf("Component Image Information List:\n");
  printf("-----------------------------------\n");
  memcpy(compo_image_count, &data[index], sizeof(compo_image_count));
  index += sizeof(compo_image_count);
  uint16_t compo_img_num =compo_image_count[0] | compo_image_count[1] << 8;
  printf("There are %02d Component Image in the PLDM payload\n\n", compo_img_num);

  for(int j = 0 ; j < compo_img_num; j++)
  {
      compo_id *p_compo_id;
      p_compo_id = (compo_id*) &data[index];
      index += sizeof(compo_id);
      printf("\33[4mComponent Image Information #%d\33[0m\n", j);
      printf("Component Classification: ");
      compo_class(p_compo_id -> compo_class[0], p_compo_id -> compo_class[1]);
      printf("Component ID: %04x\n", p_compo_id -> compo_id[0] << 8 | p_compo_id -> compo_id[1]);
      printf("Comparison Stamp: 0x");
      for(i = 3; i >= 0; i--){
        printf("%02x", p_compo_id -> compo_cmp_stamp[i]);
      }
      printf("\n");
      uint16_t options = buf_ex_bitfield16(p_compo_id ->compo_opt[0], p_compo_id -> compo_opt[1]);
      printf("Update Options: ");
      if (options == 0x0001){
          printf("Force Update\n");
      }
      else if(options == 0x0002){
          printf("Using Component Comparison stamp\n");
      }
      printf("Activation Method: 0x%04x\n", p_compo_id -> req_compo_act_meth[0] | p_compo_id -> req_compo_act_meth[1]<<8);
      printf("Image Location Offset: 0x");
      for(i=3; i >= 0; i--)
        printf("%02x", p_compo_id -> compo_offset[i]);
      printf("\n");
      uint32_t size = buf_ex_bitfield32(p_compo_id -> compo_size[0], p_compo_id -> compo_size[1],p_compo_id ->compo_size[2],p_compo_id -> compo_size[3]);
      printf("Image Size: %02d Bytes\n", size);
      printf("Component Version String Type: ");
      str_type(p_compo_id -> compo_ver_str_type[0]);
      printf("Component Version String Length: %02d Bytes\n", p_compo_id -> compo_ver_str_len[0]);
      uint8_t *p_compo_ver_str;
      p_compo_ver_str = &data[index];
      index += p_compo_id -> compo_ver_str_len[0];
      printf("Component Version String: ");
      for(i = 0; i < p_compo_id -> compo_ver_str_len[0]; i++){
        printf("%c" , p_compo_ver_str[i]);
      }
      printf("\n\n");
  }

/*------------------------------------------------------------------------------------------------------------------------
                                       checksum
--------------------------------------------------------------------------------------------------------------------------*/
  printf("-----------------------------------\n");
  printf("Package Header Checksum:\n");
  printf("-----------------------------------\n");

  checksum *p_checksum;
  p_checksum = (checksum *) &data[index];
  index += sizeof(checksum);
  printf("Checksum: 0x");
  for(i = 3; i >= 0; i--){
    printf("%02x", p_checksum -> checksum[i]);
  }
  printf("\n");

  return 0;
}
