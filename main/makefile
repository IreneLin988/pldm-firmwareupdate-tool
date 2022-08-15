pldm_encode.exe: PLDM_PKG_HEADER/libpldm.a
	gcc main.c PLDM_PKG_HEADER/libpldm.a -lz -ljson-c -o pldm_encode.exe
clean:
	rm -f pldm_encode.exe