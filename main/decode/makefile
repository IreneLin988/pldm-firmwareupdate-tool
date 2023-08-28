objects = parse_fxn.o
pldm_decode.exe: libparse.a
	gcc parse.c libparse.a -lz -ljson-c -o pldm_decode.exe
libparse.a: $(objects)
	ar -rc libparse.a $(objects)
parse_fxn.o: parse_fxn.c
	gcc -c -o parse_fxn.o parse_fxn.c
clean: 
	rm libparse.a $(objects) pldm_decode.exe pldm_update_header.bin