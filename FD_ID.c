#include <stdio.h>
#include <json-c/json.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <sys/stat.h>
#include "data_trans_fxn.h"
#include "pldmlib.h"
extern json_pldm all;
extern json_layer2 l2;

uint8_t *FD_ID(int *fd_id_size, uint16_t *compo_bitmap_len){

  if(!fd_id_size || !compo_bitmap_len)
    return NULL;

  *fd_id_size = 0;
  uint8_t b_dev_rec_num = json_object_array_length(l2.fd_list);
  uint8_t *fd_rec[b_dev_rec_num];
  uint8_t *b_apply_compo[b_dev_rec_num];
  uint16_t b_rec_len[b_dev_rec_num];
  uint8_t dev_serial_num;
  uint8_t b_des_num;
  uint32_t b_dev_opt_flag;
  uint8_t apply_compo_num;
  uint8_t b_compo_setver_str_type;
  uint8_t b_compo_setver_str_len;
  uint8_t b_fd_pkg_data_len[2] = {0};
  uint16_t bitmap_len;
  int max_bitfield = 0;

  /* Using component serial number of per record to calculate the length of component bitmap  */
  for(dev_serial_num = 0; dev_serial_num < b_dev_rec_num; dev_serial_num++)
  {
      l2.compo = json_object_array_get_idx(l2.fd_list, dev_serial_num);
      json_object_object_get_ex(l2.compo, "Applicable Components", &l2.apply_compo);
      apply_compo_num =  json_object_array_length(l2.apply_compo);
      for(int i = 0; i < apply_compo_num; i++)
      {
        l2.compo_bitmap = json_object_array_get_idx(l2.apply_compo, i);
        uint16_t apply_compo_serial_num = json_object_get_int(l2.compo_bitmap);
        if (apply_compo_serial_num > max_bitfield)
          max_bitfield = apply_compo_serial_num;
      }
  }
  *compo_bitmap_len = (max_bitfield / 8) + 1;


  for(dev_serial_num = 0; dev_serial_num < b_dev_rec_num; dev_serial_num++)
  {
      l2.compo = json_object_array_get_idx(l2.fd_list, dev_serial_num);
      json_object_object_get_ex(l2.compo, "Device Update Option Flags", &l2.dev_opt_flag);
      json_object_object_get_ex(l2.compo, "Record Descriptors", &l2.descriptors);
      json_object_object_get_ex(l2.compo, "Applicable Components", &l2.apply_compo);
    	json_object_object_get_ex(l2.compo, "Component Image Set Version String Type", &l2.compo_image_setver_str_type);
      json_object_object_get_ex(l2.compo, "Component Image Set Version String", &l2.compo_image_setver_str);
      b_des_num =  json_object_array_length(l2.descriptors);
      b_dev_opt_flag = update_opt_val(json_object_get_string(l2.dev_opt_flag));

      b_apply_compo[dev_serial_num] = malloc(*compo_bitmap_len * sizeof(uint8_t));
      if (!b_apply_compo[dev_serial_num])
      {
        printf("b_apply_compo allocated failed");
        return NULL;
      }
      /* Initial individual component bitmap */
      memset(b_apply_compo[dev_serial_num], 0, *compo_bitmap_len);
      /* Creating individual component bitmap for per record */
      apply_compo_num = json_object_array_length(l2.apply_compo);
      for(int i = 0; i < apply_compo_num; i++)
      {
        l2.compo_bitmap = json_object_array_get_idx(l2.apply_compo,i);
        uint32_t apply_compo_serial_num = json_object_get_int(l2.compo_bitmap);
        int locat = apply_compo_serial_num / 8;
        int num = apply_compo_serial_num % 8;
        b_apply_compo[dev_serial_num][locat] |= 1 << num;
      }

      b_compo_setver_str_type = str_type(json_object_get_string(l2.compo_image_setver_str_type));
      b_compo_setver_str_len = strlen(json_object_get_string(l2.compo_image_setver_str));
      uint8_t str[b_compo_setver_str_len];
      memcpy(str, json_object_get_string(l2.compo_image_setver_str), b_compo_setver_str_len);
      char init_data[16];
      uint8_t hex_data[2];////edit
      uint16_t ver_str_type_len[2];
      uint8_t field_offset[FD_ID_REC_FIELD] = {
                                               sizeof(b_rec_len[dev_serial_num]),
                                               sizeof(b_des_num),
                                               sizeof(b_dev_opt_flag),
                                               sizeof(b_compo_setver_str_type),
                                               sizeof(b_compo_setver_str_len),
                                               sizeof(b_fd_pkg_data_len),
                                               *compo_bitmap_len,
                                               sizeof(str),
                                               sizeof(ver_str_type_len) * b_des_num,
                                               sizeof(hex_data) * b_des_num
                                             };

      /* Cauculating the offset of the individual field for per record */
      for (int i = 0; i < FD_ID_REC_FIELD-1; i++)
        field_offset[i+1] = field_offset[i] + field_offset[i+1];

      b_rec_len[dev_serial_num] = field_offset[FD_ID_REC_FIELD-1];
      *fd_id_size = *fd_id_size + b_rec_len[dev_serial_num];
      fd_rec[dev_serial_num] = malloc(b_rec_len[dev_serial_num]);
      if(!fd_rec[dev_serial_num])
      {
          printf("fd record buffer allocated failed");
          free(b_apply_compo[dev_serial_num]);

          return NULL;
      }

      memcpy(fd_rec[dev_serial_num], &b_rec_len[dev_serial_num], sizeof(b_rec_len));
      memcpy(fd_rec[dev_serial_num] + field_offset[0], &b_des_num, sizeof(b_des_num));
      memcpy(fd_rec[dev_serial_num] + field_offset[1], &b_dev_opt_flag, sizeof(b_dev_opt_flag));
      memcpy(fd_rec[dev_serial_num] + field_offset[2], &b_compo_setver_str_type, sizeof(b_compo_setver_str_type));
      memcpy(fd_rec[dev_serial_num] + field_offset[3], &b_compo_setver_str_len, sizeof(b_compo_setver_str_len));
      memcpy(fd_rec[dev_serial_num] + field_offset[4], &b_fd_pkg_data_len, sizeof(b_fd_pkg_data_len));
      memcpy(fd_rec[dev_serial_num] + field_offset[5], b_apply_compo[dev_serial_num], *compo_bitmap_len);
      memcpy(fd_rec[dev_serial_num] + field_offset[6], &str, sizeof(str));
      uint8_t des_offset = field_offset[7];
      free(b_apply_compo[dev_serial_num]);

      for (int i = 0; i < b_des_num ; i++)
      {
        l2.des_data = json_object_array_get_idx(l2.descriptors,i);
        json_object_object_get_ex(l2.des_data, "Initial Descriptor Type", &l2.data);
        init_des_type(json_object_get_string(l2.data),ver_str_type_len);
        json_object_object_get_ex(l2.des_data, "Initial Descriptor Data", &l2.data);
        memcpy(init_data, json_object_get_string(l2.data), strlen(json_object_get_string(l2.data)));
        hexstr_tobin(init_data, sizeof(init_data), hex_data, sizeof(hex_data));
        memcpy(fd_rec[dev_serial_num] + des_offset, &ver_str_type_len, sizeof(ver_str_type_len));
        des_offset = des_offset + sizeof(ver_str_type_len) ;
        memcpy(fd_rec[dev_serial_num] + des_offset, &hex_data, sizeof(hex_data));
        des_offset = des_offset + sizeof(hex_data);
      }
  }
  uint8_t *fd_id_buf = malloc(*fd_id_size * sizeof(uint8_t));
  if (!fd_id_buf)
  {
    printf("fd id buffer allocated failed");
    for(dev_serial_num = 0; dev_serial_num < b_dev_rec_num; dev_serial_num++)
      free(fd_rec[dev_serial_num]);
    return NULL;
  }

  *fd_id_size = *fd_id_size + sizeof(b_dev_rec_num);
  memcpy(fd_id_buf, &b_dev_rec_num, sizeof(b_dev_rec_num));
  uint32_t len = 0;
  for(dev_serial_num = 0; dev_serial_num < b_dev_rec_num; dev_serial_num++)
  {
    memcpy(fd_id_buf + sizeof(b_dev_rec_num) + len, fd_rec[dev_serial_num], b_rec_len[dev_serial_num]);
    len = len + b_rec_len[dev_serial_num];
    free(fd_rec[dev_serial_num]);
  }

  return fd_id_buf;
}

// json_pldm all;
// json_layer1 l1;
// json_layer2 l2;
// json_layer3 l3;

// int main()
// {
//   parsed_all();
//   parsed_layer2();
//   uint8_t fd_id_size;
//   uint8_t *fd_id_buf;
//   uint16_t compo_bitmap_len;
//
//   FD_ID(&fd_id_buf, &fd_id_size, &compo_bitmap_len);
//   printf("fd size: %d\n",fd_id_size );
//   printf("compo_bitmap_len %d\n", compo_bitmap_len);
//
//   FILE *b_file;
// 	 b_file = fopen("FD_ID.bin","wb");
//   fwrite(fd_id_buf, fd_id_size, 1, b_file);
//   free(fd_id_buf);
//   fclose(b_file);
//   return 0;
// }
