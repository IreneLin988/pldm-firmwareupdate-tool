#include <stdio.h>
#include <json-c/json.h>
#include <string.h>
#include <stdlib.h>
#include "data_trans_fxn.h"
#include "pldmlib.h"
#include <stdint.h>
#include <sys/stat.h>
extern json_pldm all;
extern json_layer3 l3;

uint8_t *compo(int *compo_info_size, uint16_t *cur_size){
  if(!compo_info_size || !cur_size)
    return NULL;

  uint16_t b_compo_image_num = json_object_array_length(l3.compo_img_list);
  uint8_t *compo_img_info[b_compo_image_num];
  uint8_t compo_img_info_len[b_compo_image_num];
  uint32_t b_compo_size[b_compo_image_num];
  uint32_t b_compo_offset = 0;
  *compo_info_size = 0;

  FILE *b_file;
  b_file = fopen("pldm_payload.bin","wb");
  if(!b_file){
      perror("Failed\n");
      return NULL;
  }
  b_compo_offset = b_compo_offset + *cur_size;

  for(int j= 0; j < b_compo_image_num; j++)
  {
    l3.compo_img = json_object_array_get_idx(l3.compo_img_list, j);

    json_object_object_get_ex(l3.compo_img, "Component Classification", &l3.compo_class);
    json_object_object_get_ex(l3.compo_img, "Component Identifier", &l3.compo_id);
    json_object_object_get_ex(l3.compo_img, "Component Options", &l3.compo_opt);
    json_object_object_get_ex(l3.compo_img, "Requested Component Activation Method", &l3.req_compo_act_meth);
    json_object_object_get_ex(l3.compo_img, "Component Image", &l3.compo_size);
    json_object_object_get_ex(l3.compo_img, "Component Version String Type", &l3.compo_ver_str_type);
    json_object_object_get_ex(l3.compo_img, "Component Version String", &l3.compo_ver_str);

    uint16_t b_compo_class_val = compo_class_val(json_object_get_string(l3.compo_class));
    char init_compo_id[10];
    uint8_t b_compo_id[2];
    memcpy(init_compo_id, json_object_get_string(l3.compo_id), strlen(json_object_get_string(l3.compo_id)));
    hexstr_tobin(init_compo_id, sizeof(init_compo_id), b_compo_id, sizeof(b_compo_id));
    uint32_t b_compo_cmp_stamp = 0xFFFFFFFF;
    uint16_t b_compo_opt_val = compo_opt_val(json_object_get_string(l3.compo_opt));
    uint8_t meth_len = json_object_array_length(l3.req_compo_act_meth);
    uint16_t b_meth_val = 0;
    for(int i = 0; i < meth_len; i++)
    {
      l3.act_meth = json_object_array_get_idx(l3.req_compo_act_meth,i);
      b_meth_val = act_meth_val(json_object_get_string(l3.act_meth), b_meth_val);
      printf("meth %x\n", b_meth_val );
    }
    const char *img_file = json_object_get_string(l3.compo_size);
    struct stat file_size;
    stat(img_file, &file_size);
    b_compo_size[j] = file_size.st_size;

    FILE *read_file;
    read_file = fopen(img_file,"rb");
    uint8_t *img_buf;
    img_buf = malloc(b_compo_size[j] * sizeof(uint8_t));
    if (!img_buf)
    {
      perror("Failed (img_buf)");
      fclose(b_file);
      fclose(read_file);
      return NULL;
    }
    fread(img_buf, b_compo_size[j], 1, read_file);
    fwrite(img_buf, b_compo_size[j], 1, b_file);
    fclose(read_file);
    free(img_buf);

    uint8_t ver_str_type = str_type(json_object_get_string(l3.compo_ver_str_type));
    uint8_t ver_str_len = strlen(json_object_get_string(l3.compo_ver_str));
    uint8_t str[strlen(json_object_get_string(l3.compo_ver_str))];
    memcpy(str, json_object_get_string(l3.compo_ver_str), strlen(json_object_get_string(l3.compo_ver_str)));

    uint8_t field_offset[COMPO_IMG_INFO_FIELD] = {
                                                   sizeof(b_compo_class_val),
                                                   sizeof(b_compo_id),
                                                   sizeof(b_compo_cmp_stamp),
                                                   sizeof(b_compo_opt_val),
                                                   sizeof(b_meth_val),
                                                   sizeof(b_compo_offset),
                                                   sizeof(b_compo_size[j]),
                                                   sizeof(ver_str_type),
                                                   sizeof(ver_str_len),
                                                   sizeof(str)
                                                 };
    /* Cauculating the offset of the individual field for per component image information */
    for (int i = 0; i < COMPO_IMG_INFO_FIELD-1; i++)
      field_offset[i+1] = field_offset[i] + field_offset[i+1];
    compo_img_info_len[j] = field_offset[COMPO_IMG_INFO_FIELD-1];
    *compo_info_size = *compo_info_size + field_offset[COMPO_IMG_INFO_FIELD-1];
    compo_img_info[j] = malloc(compo_img_info_len[j]);
    if (!compo_img_info[j])
    {
      printf("compo_img_info allocated failed");
      fclose(b_file);
      return NULL;
    }

    memcpy(compo_img_info[j], &b_compo_class_val,  sizeof(b_compo_class_val));
    memcpy(compo_img_info[j] + field_offset[0], &b_compo_id,  sizeof(b_compo_id));
    memcpy(compo_img_info[j] + field_offset[1], &b_compo_cmp_stamp,  sizeof(b_compo_cmp_stamp));
    memcpy(compo_img_info[j] + field_offset[2], &b_compo_opt_val,  sizeof(b_compo_opt_val));
    memcpy(compo_img_info[j] + field_offset[3], &b_meth_val,  sizeof(b_meth_val));
    memcpy(compo_img_info[j] + field_offset[4], &b_compo_offset,  sizeof(b_compo_offset));
    memcpy(compo_img_info[j] + field_offset[5], &b_compo_size[j],  sizeof(b_compo_size[j]));
    memcpy(compo_img_info[j] + field_offset[6], &ver_str_type,  sizeof(ver_str_type));
    memcpy(compo_img_info[j] + field_offset[7], &ver_str_len,  sizeof(ver_str_len));
    memcpy(compo_img_info[j] + field_offset[8], &str,  sizeof(str));
  }
  fclose(b_file);

  *compo_info_size =  *compo_info_size + sizeof(b_compo_image_num);
  b_compo_offset = b_compo_offset + *compo_info_size + CHECKSUM_SIZE;

  for(int j= 0; j < b_compo_image_num; j++)
  {
    printf("%d : %d\n",j, b_compo_offset );
    memcpy(compo_img_info[j] + COMPO_LOCATION_FIELD_OFFSET, &b_compo_offset,  sizeof(b_compo_offset));
    b_compo_offset = b_compo_offset + b_compo_size[j];
  }

  uint32_t len = 0;
  void *compo_info_buf = malloc(*compo_info_size * sizeof(uint8_t));
  if (!compo_info_buf)
  {
      printf("compo_info_buf allocated failed");
      return NULL;
  }

  memcpy(compo_info_buf, &b_compo_image_num, sizeof(b_compo_image_num));

  for(int j = 0; j < b_compo_image_num; j++)
  {
    memcpy(compo_info_buf + sizeof(b_compo_image_num) + len, compo_img_info[j], compo_img_info_len[j]);
    len = len + compo_img_info_len[j];
    free(compo_img_info[j]);
  }

  return compo_info_buf;
}


//
// int main(){
//   int compo_info_size =0;
//   uint8_t layer_size =148;
//
//   uint8_t *compo_info_buf;
//   compo(&compo_info_buf, &compo_info_size, &layer_size);
//
//   FILE *b_file;
// 	b_file = fopen("compo.bin","wb");
//   printf("compo size %d\n", compo_info_size);
//   fwrite(compo_info_buf, compo_info_size, 1, b_file);
//   free(compo_info_buf);
//   fclose(b_file);
//
// }
