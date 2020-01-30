
-linux
cd /usr/local/xService
vi ./.bashrc
export XSERVICE_ROOT="/usr/local/xService"
export XSERVICE_DATA="/usr/local/xService/nfs"
source ./.bashrc

--linux
vi /etc/ld.so.conf
/usr/local/lib
/usr/local/xService/api

ldconfig

sudo systemctl enable xportd.service
systemctl daemon-reload
systemctl list-unit-files
systemctl start xportd.service
systemctl stop xportd.service
systemctl reload xportd.service
systemctl status xportd.service