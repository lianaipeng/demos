
# app.xml
#    adater name and id 必须相同，还不知道原因 

# admin相当于一个工具来操作registry, registry管理node

mkdir -p data/registry data/node

# 第一种
###############################################
icegridnode --Ice.Config=./config/config 

icegridadmin --Ice.Config=./config/config -ua -pa
>>> application add ./config/app.xml
# server可以自动启动

./client --Ice.Config=./config/client.cfg


# 第二种
###############################################
icegridregistry --Ice.Config=config/registry.cfg 
icegridnode --Ice.Config=./config/node.cfg

# icegridadmin --Ice.Config=./config/node.cfg -ua -pa  #也可以
# admin的配置文件为 Ice.Default.Locator=IceGrid/Locator:tcp -p 5061 
# 与node的类似
icegridadmin --Ice.Config=./config/admin.cfg -ua -pa
>>> application add ./config/app.xml

./client --Ice.Config=./config/client.cfg 

