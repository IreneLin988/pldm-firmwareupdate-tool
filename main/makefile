pldm_encode.exe: PLDM_PKG_HEADER/libpldm.a
	gcc main.c PLDM_PKG_HEADER/libpldm.a -lz -ljson-c -o pldm_encode.exe
mv_file: pldm_update_header.bin
	cp pldm_update_header.bin decode
clean:
	rm -f pldm_encode.exe pldm_update_header.bin pldm_update_pkg.bin