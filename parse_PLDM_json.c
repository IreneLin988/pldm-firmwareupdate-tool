#include <stdio.h>
#include <json-c/json.h>
#include <sys/stat.h>
#include "pldmlib.h"
extern json_pldm all;
extern json_layer1 l1;
extern json_layer2 l2;
extern json_layer3 l3;

int parsed_all(){
  FILE *fp;
  struct stat f_size;
  fp = fopen("PLDM.json","r");
  if(!fp){
      perror("PLDM json file open fialed");
      return 1;
  }
  stat("PLDM.json", &f_size);
  int init_file_size = f_size.st_size;
  uint8_t *buf;
  buf = malloc(init_file_size *sizeof(uint8_t));
  if (!buf)
  {
      perror("Failed");
      fclose(fp);
      return 1;
  }
  fread(buf, init_file_size, 1, fp);
	fclose(fp);
  all.parsed_json = json_tokener_parse(buf);
  if(!all.parsed_json){
    free(buf);
    return 1;
  }
  free(buf);
  json_object_object_get_ex(all.parsed_json, "PLDM", &all.pldm);

  return 0;
}

int parsed_layer1(){
  all.layer1 = json_object_array_get_idx(all.pldm,0);
  if(!all.layer1)
    return 1;
	json_object_object_get_ex(all.layer1, "Package Header Revision", &l1.pkg_header_revision);
  json_object_object_get_ex(all.layer1, "Package Version String Type", &l1.pkg_ver_str_type);
  json_object_object_get_ex(all.layer1, "Package Version String", &l1.pkg_ver_str);

  return 0;
}

int parsed_layer2(){
  all.layer2 = json_object_array_get_idx(all.pldm,1);
  if(!all.layer2)
    return 1;
  json_object_object_get_ex(all.layer2, "Firmware Device List", &l2.fd_list);

  return 0;
}

int parsed_layer3(){
  all.layer3 = json_object_array_get_idx(all.pldm,2);
  if(!all.layer3)
    return 1;
  json_object_object_get_ex(all.layer3, "Component Image Information List", &l3.compo_img_list);

  return 0;
}
