uint16_t buf_ex_bitfield16(uint8_t buf_zero, uint8_t buf_one);
uint32_t buf_ex_bitfield32(uint8_t buf_zero, uint8_t buf_one, uint8_t buf_two, uint8_t buf_three);
void reverse(uint8_t arr[]);
uint8_t str_type(uint8_t type);
uint16_t descriptor_id(uint8_t buf_zero, uint8_t buf_one);
uint16_t compo_class(uint8_t buf_zero, uint8_t buf_one);
int GetPowerNum(int num);

#define HEADER_ID_LENTH 16

typedef struct pkg_header_info {
  uint8_t pkg_header_id[HEADER_ID_LENTH];
  uint8_t pkg_header_revision[1];
  uint8_t pkg_header_size[2];
  uint8_t UTC_offset[2];
  uint8_t msec[3];
  uint8_t sec[1];
  uint8_t min[1];
  uint8_t hr[1];
  uint8_t day[1];
  uint8_t mon[1];
  uint8_t yr[2];
  uint8_t UTC[1];
  uint8_t compo_bitmap_len[2];
  uint8_t pkg_ver_str_type[1];
  uint8_t pkg_ver_str_len[1];
}__attribute__((packed)) header_info;

uint8_t dev_id_rec_count[1]; //query device id cmd

typedef struct fw_device_id_area
{   uint8_t rec_len[2]; // the length of this Record
    uint8_t descriptor_count[1];
    uint8_t dev_update_opt_flag[4];
    uint8_t compo_image_setver_str_type[1];
    uint8_t compo_image_setver_str_len[1]; //compo_image_setver_str
    uint8_t fw_pkg_data_len[2]; //fw_pkg_data_len
}__attribute__((packed)) device_id;

typedef struct rec_des //Record Descriptors
{
    uint8_t init_descriptor_type[2];
    uint8_t init_descriptor_len[2];
}__attribute__((packed)) rec;

uint8_t compo_image_count[2];

typedef struct compo_info_id_area
{
    uint8_t compo_class[2];
    uint8_t compo_id[2];
    uint8_t compo_cmp_stamp[4];
    uint8_t compo_opt[2];
    uint8_t req_compo_act_meth[2];
    uint8_t compo_offset[4];
    uint8_t compo_size[4];
    uint8_t compo_ver_str_type[1];
    uint8_t compo_ver_str_len[1];
}__attribute__((packed)) compo_id;


typedef struct checksum
{
    uint8_t checksum[4];
}__attribute__((packed)) checksum;
