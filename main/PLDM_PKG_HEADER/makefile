objects = pkg_info.o FD_ID.o compo.o data_trans_fxn.o parse_PLDM_json.o
 
libpldm.a: $(objects)
	ar -rc libpldm.a $(objects)

pkg_info.o: pkg_info.c pldmlib.h
	gcc -c -o pkg_info.o pkg_info.c

FD_ID.o: FD_ID.c pldmlib.h
	gcc -c -o FD_ID.o FD_ID.c

compo.o: compo.c pldmlib.h
	gcc -c -o compo.o compo.c

data_trans_fxn.o: data_trans_fxn.c data_trans_fxn.h
	gcc -c -o data_trans_fxn.o data_trans_fxn.c

parse_PLDM_json.o: parse_PLDM_json.c pldmlib.h
	gcc -c -o parse_PLDM_json.o parse_PLDM_json.c

clean: 
	rm libpldm.a $(objects)