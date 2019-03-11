# Include the nanopb provided Makefile rules
include vendor/nanopb/extra/nanopb.mk

vendor:
	git submodule update --init --recursive --force sdk/vendor/nanopb
	git submodule update --init --recursive --force sdk/vendor/tinyproto

proto:
	rm -rf src/proto && mkdir src/proto && cd ../proto && $(PROTOC) $(PROTOC_OPTS) sdk.proto --nanopb_out=../sdk/src/proto/
