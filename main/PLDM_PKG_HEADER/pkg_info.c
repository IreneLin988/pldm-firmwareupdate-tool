#include <stdio.h>
#include <json-c/json.h>
#include <string.h>
#include <stdlib.h>
#include "data_trans_fxn.h"
#include "pldmlib.h"
#include <sys/stat.h>
extern json_pldm all;
extern json_layer1 l1;

uint8_t *pkg_info(int *pkg_info_size, uint16_t *compo_bitmap_len){

  if ( !pkg_info_size  || !compo_bitmap_len){
    return NULL;
  }
  *pkg_info_size = 0;
  uint8_t PLDM_id[16] = { 0xF0, 0x18, 0x87, 0x8C, 0xCB, 0x7D, 0x49, 0x43, 0x98, 0x00, 0xA0, 0x2F,
                          0x05, 0x9A, 0xCA, 0x02 }; //Refer to different version of PLDM FW Update Spec
  uint8_t pkg_revision;
  uint8_t pkg_header_size [2] = {0};
  uint8_t *cur_time;
  uint8_t pkg_release_time[13];
  uint16_t b_compo_bitmap_len;
  uint8_t b_pkg_str_type;
  uint8_t b_pkg_str_len;
  cur_time = timestamp104();
  memcpy(pkg_release_time, cur_time, sizeof(pkg_release_time));
  pkg_revision = json_object_get_int(l1.pkg_header_revision);
  b_compo_bitmap_len = *compo_bitmap_len * 8; //Transforming bitfield into Byte
  b_pkg_str_type = str_type(json_object_get_string(l1.pkg_ver_str_type));
  b_pkg_str_len = strlen(json_object_get_string(l1.pkg_ver_str));
  uint8_t str[b_pkg_str_len];
  memcpy(str, json_object_get_string(l1.pkg_ver_str), b_pkg_str_len);
  uint8_t field_offset[PKG_HEADER_INFO_FIELD] = {
                                                 sizeof(PLDM_id),
                                                 sizeof(pkg_revision),
                                                 sizeof(pkg_header_size),
                                                 sizeof(pkg_release_time),
                                                 sizeof(b_compo_bitmap_len),
                                                 sizeof(b_pkg_str_type),
                                                 sizeof(b_pkg_str_len),
                                                 sizeof(str)
                                                };
  for (int i = 0; i < PKG_HEADER_INFO_FIELD -1 ; i++){
    field_offset[i+1] = field_offset[i] + field_offset[i+1];
  }
  *pkg_info_size = field_offset[PKG_HEADER_INFO_FIELD -1];
  uint8_t *pkg_header_info = malloc(*pkg_info_size * sizeof(uint8_t));
  if (!pkg_header_info){
      printf("pkg_header_info allocated failed");
      return NULL;
  }
  memcpy(pkg_header_info, PLDM_id, sizeof(PLDM_id));
  memcpy(pkg_header_info + field_offset[0], &pkg_revision,  sizeof(pkg_revision));
  memcpy(pkg_header_info + field_offset[1], &pkg_header_size,  sizeof(pkg_header_size));
  memcpy(pkg_header_info + field_offset[2], &pkg_release_time,  sizeof(pkg_release_time));
  memcpy(pkg_header_info + field_offset[3], &b_compo_bitmap_len,  sizeof(b_compo_bitmap_len));
  memcpy(pkg_header_info + field_offset[4], &b_pkg_str_type,  sizeof(b_pkg_str_type));
  memcpy(pkg_header_info + field_offset[5], &b_pkg_str_len,  sizeof(b_pkg_str_len));
  memcpy(pkg_header_info + field_offset[6], &str,  sizeof(str));

  return pkg_header_info;
}

// int main(){
//   uint8_t pkg_info_size;
//   uint8_t *pkg_info_buf;
//   uint16_t compo_bitmap_len =1;
//   pkg_info(&pkg_info_buf, &pkg_info_size, &compo_bitmap_len);
//
//   FILE *b_file;
// 	b_file = fopen("pkg_info.bin","wb");
//   printf("pkg_info_size %d\n", pkg_info_size);
//   fwrite(pkg_info_buf, pkg_info_size, 1, b_file);
//   free(pkg_info_buf);
//   fclose(b_file);
//   return 0;
// }
