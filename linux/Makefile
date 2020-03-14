SRV_PATH = /usr/local/xService
LNK_PATH = /usr/local/lib
VAR_PATH = /var/run
DAE_PATH = /lib/systemd/system
EXE_PATH = ~/Easily-app-5/linux/sbin
API_PATH = ~/Easily-app-5/linux/sbin/api
INS_PATH = ~/Easily-sdk-5/linux/setup
CUR_PATH = $(abspath ./)

API_OBJS = $(API_PATH)/libxdk.so.1.0 \
	$(API_PATH)/libxdl.so.1.0 \
	$(API_PATH)/librad_api.so.1.0 \
	$(API_PATH)/libradnet.so.1.0 \
	$(API_PATH)/libpub_api.so.1.0 \
	$(API_PATH)/libsub_api.so.1.0 \
	$(API_PATH)/libmqtt.so.1.0 \
	$(API_PATH)/libloc_api.so.1.0 \
	$(API_PATH)/libwww_api.so.1.0 \
	$(API_PATH)/libxdb_api.so.1.0 \
	$(API_PATH)/libxdb_mysql.so.1.0 \
	$(API_PATH)/libsms_api.so.1.0 \
	$(API_PATH)/libsms_aliyun.so.1.0

EXE_OBJS = $(EXE_PATH)/xportm \
	$(EXE_PATH)/xportd \
	$(EXE_PATH)/xhttps \
	$(EXE_PATH)/xslots

all:
	if ! test -d $(SRV_PATH); then \
	sudo mkdir $(SRV_PATH); \
	fi
	if ! test -d $(SRV_PATH)/api; then \
	sudo mkdir $(SRV_PATH)/api; \
	fi

	cd $(CUR_PATH)/xds && make -f xds.ar.mk
	cd $(CUR_PATH)/xdk && make -f xdk.so.mk
	cd $(CUR_PATH)/xdl && make -f xdl.so.mk

	cd $(CUR_PATH)/xdb_mysql && make -f xdb_mysql.so.mk
	cd $(CUR_PATH)/sms_aliyun && make -f sms_aliyun.so.mk
	cd $(CUR_PATH)/radnet && make -f radnet.so.mk
	cd $(CUR_PATH)/mqtt && make -f mqtt.so.mk

	cd $(CUR_PATH)/loc_api && make -f loc_api.so.mk
	cd $(CUR_PATH)/www_api && make -f www_api.so.mk
	cd $(CUR_PATH)/xdb_api && make -f xdb_api.so.mk
	cd $(CUR_PATH)/sms_api && make -f sms_api.so.mk
	cd $(CUR_PATH)/rad_api && make -f rad_api.so.mk
	cd $(CUR_PATH)/sub_api && make -f sub_api.so.mk
	cd $(CUR_PATH)/pub_api && make -f pub_api.so.mk

	cd $(CUR_PATH)/xportm && make -f xportm.mk
	cd $(CUR_PATH)/xhttps && make -f xhttps.mk
	cd $(CUR_PATH)/xslots && make -f xslots.mk
	cd $(CUR_PATH)/xportd && make -f xportd.mk

test:
	@echo $(CUR_PATH)

install:
	if ! test -d $(SRV_PATH); then \
	sudo mkdir $(SRV_PATH); \
	fi
	if ! test -d $(SRV_PATH)/api; then \
	sudo mkdir $(SRV_PATH)/api; \
	fi
	if ! test -d $(SRV_PATH)/cfg; then \
	sudo mkdir $(SRV_PATH)/cfg; \
	fi
	if ! test -d $(SRV_PATH)/lic; then \
	sudo mkdir $(SRV_PATH)/lic; \
	fi
	if ! test -d $(SRV_PATH)/ssl; then \
	sudo mkdir $(SRV_PATH)/ssl; \
	fi
	if ! test -d $(SRV_PATH)/nfs; then \
	sudo mkdir $(SRV_PATH)/nfs; \
	fi
	if ! test -d $(SRV_PATH)/loc; then \
	sudo mkdir $(SRV_PATH)/loc; \
	fi
	if ! test -d $(SRV_PATH)/www; then \
	sudo mkdir $(SRV_PATH)/www; \
	fi
	if ! test -d $(SRV_PATH)/xdb; then \
	sudo mkdir $(SRV_PATH)/xdb; \
	fi
	if ! test -d $(SRV_PATH)/sms; then \
	sudo mkdir $(SRV_PATH)/sms; \
	fi
	if ! test -d $(SRV_PATH)/rad; then \
	sudo mkdir $(SRV_PATH)/rad; \
	fi
	if ! test -d $(SRV_PATH)/sub; then \
	sudo mkdir $(SRV_PATH)/sub; \
	fi
	if ! test -d $(SRV_PATH)/pub; then \
	sudo mkdir $(SRV_PATH)/pub; \
	fi

	sudo cp -rf $(INS_PATH)/cfg $(SRV_PATH);
	sudo cp -rf $(INS_PATH)/lic $(SRV_PATH);
	sudo cp -rf $(INS_PATH)/ssl $(SRV_PATH);

	sudo cp -f $(INS_PATH)/readme.txt $(SRV_PATH);
	sudo cp -f $(INS_PATH)/xportd.config $(SRV_PATH);

	sudo cp -f $(INS_PATH)/xService.sh $(SRV_PATH);
	sudo chmod +x $(SRV_PATH)/xService.sh; 

	if ! test -d $(VAR_PATH); then \
	sudo mkdir $(VAR_PATH); \
	fi
	sudo cp -f $(INS_PATH)/xportd.pid $(VAR_PATH);
	sudo chmod 777 $(VAR_PATH)/xportd.pid;

	sudo cp -f $(INS_PATH)/xportd.service $(DAE_PATH);

	cd $(CUR_PATH)/xdk && make install -f xdk.so.mk
	cd $(CUR_PATH)/xdl && make install -f xdl.so.mk

	cd $(CUR_PATH)/xdb_mysql && make install -f xdb_mysql.so.mk
	cd $(CUR_PATH)/sms_aliyun && make install -f sms_aliyun.so.mk
	cd $(CUR_PATH)/radnet && make install -f radnet.so.mk
	cd $(CUR_PATH)/mqtt && make install -f mqtt.so.mk

	cd $(CUR_PATH)/loc_api && make install -f loc_api.so.mk
	cd $(CUR_PATH)/www_api && make install -f www_api.so.mk
	cd $(CUR_PATH)/xdb_api && make install -f xdb_api.so.mk
	cd $(CUR_PATH)/sms_api && make install -f sms_api.so.mk
	cd $(CUR_PATH)/rad_api && make install -f rad_api.so.mk
	cd $(CUR_PATH)/sub_api && make install -f sub_api.so.mk
	cd $(CUR_PATH)/pub_api && make install -f pub_api.so.mk

	cd $(CUR_PATH)/xportm && make install -f xportm.mk
	cd $(CUR_PATH)/xhttps && make install -f xhttps.mk
	cd $(CUR_PATH)/xslots && make install -f xslots.mk
	cd $(CUR_PATH)/xportd && make install -f xportd.mk
	cd $(CUR_PATH)

.PHONY : test
clean:
	-rm -f $(API_OBJS)
	-rm -f $(EXE_OBJS)