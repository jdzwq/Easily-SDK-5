SRV_PATH = /usr/local/xService
LNK_PATH = /usr/local/lib
VAR_PATH = /var/run
DAE_PATH = /lib/systemd/system
EXE_PATH = ~/Easily-app-5/linux/sbin
API_PATH = ~/Easily-app-5/linux/sbin/api
INS_PATH = ~/Easily-sdk-5/install
CUR_PATH = $(abspath ./)

API_OBJS = $(API_PATH)/libxdu.so.1.0 \
	$(API_PATH)/libxdk.so.1.0 \
	$(API_PATH)/libxdl.so.1.0 \
	$(API_PATH)/libxds.so.1.0 \
	$(API_PATH)/libloc_api.so.1.0 \
	$(API_PATH)/libwww_api.so.1.0

EXE_OBJS = $(EXE_PATH)/xportm \
	$(EXE_PATH)/xportd \
	$(EXE_PATH)/xhttps \

all:
	if ! test -d $(SRV_PATH); then \
	sudo mkdir $(SRV_PATH); \
	fi
	if ! test -d $(SRV_PATH)/api; then \
	sudo mkdir $(SRV_PATH)/api; \
	fi

	cd $(CUR_PATH)/xdp && make -f xdp.ar.mk
	cd $(CUR_PATH)/xdu && make -f xdu.so.mk
	cd $(CUR_PATH)/xdk && make -f xdk.so.mk
	cd $(CUR_PATH)/xdl && make -f xdl.so.mk
	cd $(CUR_PATH)/xds && make -f xds.so.mk
	cd $(CUR_PATH)/loc_api && make -f loc_api.so.mk
	cd $(CUR_PATH)/www_api && make -f www_api.so.mk
	cd $(CUR_PATH)/xportm && make -f xportm.mk
	cd $(CUR_PATH)/xhttps && make -f xhttps.mk
	cd $(CUR_PATH)/xportd && make -f xportd.mk

test:
	@echo $(CUR_PATH)

install:
	cd $(CUR_PATH)/xdu && make install -f xdu.so.mk
	cd $(CUR_PATH)/xdk && make install -f xdk.so.mk
	cd $(CUR_PATH)/xdl && make install -f xdl.so.mk
	cd $(CUR_PATH)/xds && make install -f xds.so.mk
	cd $(CUR_PATH)/loc_api && make install -f loc_api.so.mk
	cd $(CUR_PATH)/www_api && make install -f www_api.so.mk
	cd $(CUR_PATH)/xportm && make install -f xportm.mk
	cd $(CUR_PATH)/xhttps && make install -f xhttps.mk
	cd $(CUR_PATH)/xportd && make install -f xportd.mk
	cd $(CUR_PATH)

	sudo cp -rpf $(INS_PATH)/* $(SRV_PATH);
	sudo chmod +x $(SRV_PATH)/xService.sh; 

	if ! test -d $(VAR_PATH); then \
	sudo mkdir $(VAR_PATH); \
	fi

	sudo cp -f $(INS_PATH)/xportd.pid $(VAR_PATH);
	sudo chmod 777 $(VAR_PATH)/xportd.pid; 

	sudo cp -f $(INS_PATH)/xportd.service $(DAE_PATH);

.PHONY : test
clean:
	-rm -f $(API_OBJS)
	-rm -f $(EXE_OBJS)