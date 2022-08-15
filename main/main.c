#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <zlib.h>
#include <json-c/json.h>
#include "pldmlib.h"

json_pldm all;
json_layer1 l1;
json_layer2 l2;
json_layer3 l3;

int main() {

  int pkg_info_size;
  int fd_id_size;
  int compo_info_size;
  uint16_t compo_bitmap_len;
  uint8_t *pkg_info_buf;
  uint8_t *fd_id_buf;
  uint8_t *compo_info_buf;

  int error = parsed_all();
  if (error == 1){
    printf("Parsing PLDM.json failed\n");
    return 1;
  }

  error = parsed_layer1();
  if (error == 1){
    printf("Parsing Package Header Information failed\n");
    return 1;
  }

  error = parsed_layer2();
  if (error == 1){
    printf("Parsing Firmware Device ID Record failed\n");
    return 1;
  }

  error = parsed_layer3();
  if (error == 1){
    printf("Parsing Component Image Information failed\n");
    return 1;
  }

  fd_id_buf = FD_ID(&fd_id_size, &compo_bitmap_len);
  if (!fd_id_buf){
    printf("Firmware Device ID Record Encoding ERROR\n");
    return 1;
  }

  pkg_info_buf = pkg_info(&pkg_info_size, &compo_bitmap_len);
  if (!pkg_info_buf){
    printf("Package Header Information Encoding ERROR\n");
    free(fd_id_buf);
    return 1;
  }
  uint16_t cur_size = pkg_info_size + fd_id_size;

  compo_info_buf = compo(&compo_info_size, &cur_size);
  if (!compo_info_buf){
    printf("Component Image Information Encoding ERROR\n");
    free(pkg_info_buf);
    free(fd_id_buf);
    return 1;
  }
  cur_size = cur_size + compo_info_size + CHECKSUM_SIZE;

  /* Writing the whole package header size(included checksum field) into the PKG Header INFO AREA */
  memcpy(pkg_info_buf + PKG_HEADER_SIZE_OFFSET, &cur_size, sizeof(cur_size));
  FILE *b_file;
	b_file = fopen("PLDM_FW_PKG_Header.bin","wb");
  if(!b_file){
    perror("Failed\n");
    free(compo_info_buf);
    free(pkg_info_buf);
    free(fd_id_buf);
    return 1;
  }
  printf("Create the PLDM_FW_PKG_Header.bin successfully\n");

  fwrite(pkg_info_buf, pkg_info_size, 1, b_file);
  free(pkg_info_buf);
  fwrite(fd_id_buf, fd_id_size, 1, b_file);
  free(fd_id_buf);
  fwrite(compo_info_buf, compo_info_size, 1, b_file);
  free(compo_info_buf);
  fclose(b_file);

  /* Calculating the Checksum of PLDM Firmware package header*/
  /* NOTICE: It did NOT include the checksum field           */
	b_file = fopen("PLDM_FW_PKG_Header.bin","rb");
  if(!b_file){
    perror("Failed\n");
    return 1;
  }

  unsigned char *pldm = malloc((cur_size - CHECKSUM_SIZE) * sizeof(unsigned char));
  if(!pldm){
    perror("Failed\n");
    fclose(b_file);
    return 1;
  }
  fread(pldm, cur_size, 1, b_file);
	fclose(b_file);

  unsigned long crc = crc32(0L, Z_NULL, 0);
  crc = crc32(crc, (const unsigned char*)pldm, cur_size - CHECKSUM_SIZE);
  printf("Checksum: %lx\n",crc);
  free(pldm);

  /* Writing the checksum(4 Bytes) into the end of PLDM FW PKG Header*/
	b_file = fopen("PLDM_FW_PKG_Header.bin","ab");
  if(!b_file){
    perror("Failed\n");
    return 1;
  }
  fwrite(&crc, CHECKSUM_SIZE, 1, b_file);
	fclose(b_file);
  system("cat image_payload.bin >> PLDM_FW_PKG_Header.bin");

  return 0;
}

  // FILE *img;
	// img = fopen("img_2.bin","wb");
  // uint8_t *img_bin = malloc(60 * sizeof(uint8_t));
  // for(int i = 0; i < 20; i++)
  //       *(img_bin+i) = 3;
  // for(int i = 20; i < 40; i++)
  //       *(img_bin+i) = 0x33;
  // for(int i = 40; i < 60; i++)
  //       *(img_bin+i) = 3;
  // // printf("%d\n", *img_bin);
  // fwrite(img_bin, 60, 1, img);
  // fclose(img);
