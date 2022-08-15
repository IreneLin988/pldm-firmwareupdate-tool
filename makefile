SUBDIRS = main/PLDM_PKG_HEADER
SUBDIRS_decode = main/decode
all:
	(cd $(SUBDIRS) && make);
	(cd main && make);
decode:
	(cd $(SUBDIRS_decode) && make);
	(cd main/decode && ./pldm_decode.exe);
encode:
	(cd main && ./pldm_encode.exe);
clean:
	(cd $(SUBDIRS) && make clean);
	(cd main && make clean); 
	(cd main/decode && make clean); 