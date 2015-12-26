
# app.xml
#    adater name and id 必须相同，还不知道原因 

mkdir -p data/registry data/replica1 data/node1 data/node2 


icegridregistry --Ice.Config=./config/master.cfg
icegridregistry --Ice.Config=config/replica1.cfg

icegridnode --Ice.Config=config/node1.cfg
icegridnode --Ice.Config=config/node2.cfg

icegridadmin --Ice.Config=config/admin.cfg -ua -pa
>>> application add config/app.xml
# server可以自动启动

./client --Ice.Config=config/client.cfg

# 可以杀死master replica1还可以用
# 杀死node 程序也就死了
# 替换程序需用 admin
