SRV_PATH = /usr/local/xService
LNK_PATH = /usr/local/lib
VAR_PATH = /var/run
DAE_PATH = /lib/systemd/system
EXE_PATH = ~/Easily-app-5/linux/sbin
API_PATH = ~/Easily-app-5/linux/sbin/api
INS_PATH = ~/Easily-sdk-5/linux/setup
CUR_PATH = $(abspath ./)

API_OBJS = $(API_PATH)/libxds.so.1.0 \
	$(API_PATH)/libxdk.so.1.0 \
	$(API_PATH)/libxdl.so.1.0 \
	$(API_PATH)/libtdb.so.1.0 \
	$(API_PATH)/librnet.so.1.0 \
	$(API_PATH)/libmqtt_api.so.1.0 \
	$(API_PATH)/libcoap_api.so.1.0 \
	$(API_PATH)/libtftp_api.so.1.0 \
	$(API_PATH)/libloc_api.so.1.0 \
	$(API_PATH)/libwww_api.so.1.0 \
	$(API_PATH)/libxdb_api.so.1.0 \
	$(API_PATH)/libxdb_mysql.so.1.0 \
	$(API_PATH)/libxdb_stub.so.1.0 \
	$(API_PATH)/liboau_api.so.1.0 \
	$(API_PATH)/libplot_api.so.1.0 \
	$(API_PATH)/libevent_api.so.1.0

EXE_OBJS = $(EXE_PATH)/xportm \
	$(EXE_PATH)/xportd \
	$(EXE_PATH)/xhttps \
	$(EXE_PATH)/xtcps \
	$(EXE_PATH)/xudps \
	$(EXE_PATH)/xtimers \
	$(EXE_PATH)/xtimerd

all:
	if ! test -d $(SRV_PATH); then \
	sudo mkdir $(SRV_PATH); \
	fi
	if ! test -d $(SRV_PATH)/api; then \
	sudo mkdir $(SRV_PATH)/api; \
	fi

	cd $(CUR_PATH)/xds && make -f xds.so.mk
	cd $(CUR_PATH)/xdk && make -f xdk.so.mk
	cd $(CUR_PATH)/xdl && make -f xdl.so.mk

	cd $(CUR_PATH)/tdb && make -f tdb.so.mk
	cd $(CUR_PATH)/rnet && make -f rnet.so.mk
	
	cd $(CUR_PATH)/loc_api && make -f loc_api.so.mk
	cd $(CUR_PATH)/www_api && make -f www_api.so.mk
	cd $(CUR_PATH)/xdb_mysql && make -f xdb_mysql.so.mk
	cd $(CUR_PATH)/xdb_stub && make -f xdb_stub.so.mk
	cd $(CUR_PATH)/xdb_api && make -f xdb_api.so.mk
	cd $(CUR_PATH)/oau_api && make -f oau_api.so.mk
	cd $(CUR_PATH)/mqtt_api && make -f mqtt_api.so.mk
	cd $(CUR_PATH)/coap_api && make -f coap_api.so.mk
	cd $(CUR_PATH)/tftp_api && make -f tftp_api.so.mk
	cd $(CUR_PATH)/plot_api && make -f plot_api.so.mk
	cd $(CUR_PATH)/event_api && make -f event_api.so.mk

	cd $(CUR_PATH)/xportm && make -f xportm.mk
	cd $(CUR_PATH)/xhttps && make -f xhttps.mk
	cd $(CUR_PATH)/xtcps && make -f xtcps.mk
	cd $(CUR_PATH)/xudps && make -f xudps.mk
	cd $(CUR_PATH)/xportd && make -f xportd.mk

	cd $(CUR_PATH)/xtimers && make -f xtimers.mk
	cd $(CUR_PATH)/xtimerd && make -f xtimerd.mk

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
	if ! test -d $(SRV_PATH)/sch; then \
	sudo mkdir $(SRV_PATH)/sch; \
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
	if ! test -d $(SRV_PATH)/oau; then \
	sudo mkdir $(SRV_PATH)/oau; \
	fi
	if ! test -d $(SRV_PATH)/coap; then \
	sudo mkdir $(SRV_PATH)/coap; \
	fi
	if ! test -d $(SRV_PATH)/tftp; then \
	sudo mkdir $(SRV_PATH)/tftp; \
	fi
	if ! test -d $(SRV_PATH)/mqtt; then \
	sudo mkdir $(SRV_PATH)/mqtt; \
	fi
	if ! test -d $(SRV_PATH)/plot; then \
	sudo mkdir $(SRV_PATH)/plot; \
	fi
	if ! test -d $(SRV_PATH)/event; then \
	sudo mkdir $(SRV_PATH)/event; \
	fi

	sudo cp -rf $(INS_PATH)/cfg $(SRV_PATH);
	sudo cp -rf $(INS_PATH)/lic $(SRV_PATH);
	sudo cp -rf $(INS_PATH)/ssl $(SRV_PATH);
	sudo cp -rf $(INS_PATH)/sch $(SRV_PATH);

	sudo cp -f $(INS_PATH)/readme.txt $(SRV_PATH);
	sudo cp -f $(INS_PATH)/xportd.config $(SRV_PATH);
	sudo cp -f $(INS_PATH)/xtimerd.config $(SRV_PATH);

	sudo cp -f $(INS_PATH)/xService.sh $(SRV_PATH);
	sudo chmod +x $(SRV_PATH)/xService.sh; 

	sudo cp -f $(INS_PATH)/xSchedule.sh $(SRV_PATH);
	sudo chmod +x $(SRV_PATH)/xSchedule.sh; 

	if ! test -d $(VAR_PATH); then \
	sudo mkdir $(VAR_PATH); \
	fi
	sudo cp -f $(INS_PATH)/xportd.pid $(VAR_PATH);
	sudo chmod 777 $(VAR_PATH)/xportd.pid;

	sudo cp -f $(INS_PATH)/xtimerd.pid $(VAR_PATH);
	sudo chmod 777 $(VAR_PATH)/xtimerd.pid;

	sudo cp -f $(INS_PATH)/xportd.service $(DAE_PATH);

	sudo cp -f $(INS_PATH)/xtimerd.service $(DAE_PATH);

	cd $(CUR_PATH)/xds && make install -f xds.so.mk
	cd $(CUR_PATH)/xdk && make install -f xdk.so.mk
	cd $(CUR_PATH)/xdl && make install -f xdl.so.mk

	cd $(CUR_PATH)/tdb && make install -f tdb.so.mk
	cd $(CUR_PATH)/rnet && make install -f rnet.so.mk

	cd $(CUR_PATH)/loc_api && make install -f loc_api.so.mk
	cd $(CUR_PATH)/www_api && make install -f www_api.so.mk
	cd $(CUR_PATH)/xdb_mysql && make install -f xdb_mysql.so.mk
	cd $(CUR_PATH)/xdb_stub && make install -f xdb_stub.so.mk
	cd $(CUR_PATH)/xdb_api && make install -f xdb_api.so.mk
	cd $(CUR_PATH)/oau_api && make install -f oau_api.so.mk
	cd $(CUR_PATH)/tftp_api && make install -f tftp_api.so.mk
	cd $(CUR_PATH)/coap_api && make install -f coap_api.so.mk
	cd $(CUR_PATH)/mqtt_api && make install -f mqtt_api.so.mk
	cd $(CUR_PATH)/plot_api && make install -f plot_api.so.mk
	cd $(CUR_PATH)/event_api && make install -f event_api.so.mk

	cd $(CUR_PATH)/xportm && make install -f xportm.mk
	cd $(CUR_PATH)/xhttps && make install -f xhttps.mk
	cd $(CUR_PATH)/xtcps && make install -f xtcps.mk
	cd $(CUR_PATH)/xudps && make install -f xudps.mk
	cd $(CUR_PATH)/xportd && make install -f xportd.mk
	cd $(CUR_PATH)/xtimers && make install -f xtimers.mk
	cd $(CUR_PATH)/xtimerd && make install -f xtimerd.mk
	cd $(CUR_PATH)

uninstall:
	cd $(CUR_PATH)/xds && make uninstall -f xds.so.mk
	cd $(CUR_PATH)/xdk && make uninstall -f xdk.so.mk
	cd $(CUR_PATH)/xdl && make uninstall -f xdl.so.mk

	cd $(CUR_PATH)/tdb && make uninstall -f tdb.so.mk
	cd $(CUR_PATH)/rnet && make uninstall -f rnet.so.mk

	cd $(CUR_PATH)/loc_api && make uninstall -f loc_api.so.mk
	cd $(CUR_PATH)/www_api && make uninstall -f www_api.so.mk
	cd $(CUR_PATH)/xdb_mysql && make uninstall -f xdb_mysql.so.mk
	cd $(CUR_PATH)/xdb_stub && make uninstall -f xdb_stub.so.mk
	cd $(CUR_PATH)/xdb_api && make uninstall -f xdb_api.so.mk
	cd $(CUR_PATH)/oau_api && make uninstall -f oau_api.so.mk
	cd $(CUR_PATH)/tftp_api && make uninstall -f tftp_api.so.mk
	cd $(CUR_PATH)/coap_api && make uninstall -f coap_api.so.mk
	cd $(CUR_PATH)/mqtt_api && make uninstall -f mqtt_api.so.mk
	cd $(CUR_PATH)/plot_api && make uninstall -f plot_api.so.mk
	cd $(CUR_PATH)/event_api && make uninstall -f event_api.so.mk

	cd $(CUR_PATH)/xportm && make uninstall -f xportm.mk
	cd $(CUR_PATH)/xhttps && make uninstall -f xhttps.mk
	cd $(CUR_PATH)/xtcps && make uninstall -f xtcps.mk
	cd $(CUR_PATH)/xudps && make uninstall -f xudps.mk
	cd $(CUR_PATH)/xportd && make uninstall -f xportd.mk
	cd $(CUR_PATH)/xtimers && make uninstall -f xtimers.mk
	cd $(CUR_PATH)/xtimerd && make uninstall -f xtimerd.mk
	cd $(CUR_PATH)

.PHONY : test
clean:
	-rm -f $(API_OBJS)
	-rm -f $(EXE_OBJS)