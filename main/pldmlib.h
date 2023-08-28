
int parsed_all();
int parsed_layer1();
int parsed_layer2();
int parsed_layer3();

uint8_t *pkg_info(int *pkg_info_size, uint16_t *compo_bitmap_len);
uint8_t *FD_ID(int *fd_id_size, uint16_t *compo_bitmap_len);
uint8_t *compo(int *compo_info_size, uint16_t *cur_size);
#define PKG_HEADER_INFO_FIELD 8
#define FD_ID_REC_FIELD 10
#define COMPO_IMG_INFO_FIELD 10
#define CHECKSUM_SIZE 4
#define PKG_HEADER_SIZE_OFFSET 17
#define COMPO_LOCATION_FIELD_OFFSET 12


/*-------------layer1 : Package Header Information------------------*/
/*-------------layer2 : Firmware Device Identification Area---------*/
/*-------------layer3 : Component Image Information Area------------*/

typedef struct json_pldm{
  struct json_object *parsed_json;
  struct json_object *pldm;
  struct json_object *layer1;
  struct json_object *layer2;
  struct json_object *layer3;
}json_pldm;

typedef struct json_layer1{
  struct json_object *pkg_header_revision;
	struct json_object *pkg_ver_str_type;
	struct json_object *pkg_ver_str;
}json_layer1;

typedef struct json_layer2{
  struct json_object *fd_list;
  struct json_object *dev_rec_num;
  struct json_object *dev_opt_flag;
  struct json_object *compo_image_setver_str_type;
  struct json_object *apply_compo;
  struct json_object *compo_image_setver_str;
	struct json_object *descriptors;
	struct json_object *des_data;
  struct json_object *data;
  struct json_object *compo;
  struct json_object *compo_bitmap;
}json_layer2;

typedef struct json_layer3{
  struct json_object *compo_img_list;
  struct json_object *compo_image_num;
  struct json_object *compo_class;
  struct json_object *compo_id;
  struct json_object *compo_opt;
	struct json_object *req_compo_act_meth;
  struct json_object *act_meth;
  struct json_object *compo_size;
  struct json_object *compo_ver_str_type;
  struct json_object *compo_ver_str;
  struct json_object *compo_img;
}json_layer3;
