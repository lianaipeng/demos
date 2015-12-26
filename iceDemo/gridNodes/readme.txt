
# app.xml
#    adater name and id 必须相同，还不知道原因 

mkdir -p data/registry data/node

# 第一种
#################################################
icegridnode --Ice.Config=config/config 
icegridnode --Ice.Config=./config/node2.cfg

icegridadmin --Ice.Config=config/config -ua -pa
>>> application add config/app.xml
# server可以自动启动

./client --Ice.Config=./config/client.cfg 



# 也可以用gridNode的方式 分开配置文件
# 第二种
#################################################
icegridregistry --Ice.Config=config/registry.cfg 
icegridnode --Ice.Config=./config/node1.cfg
icegridnode --Ice.Config=./config/node2.cfg

# icegridadmin --Ice.Config=./config/node.cfg -ua -pa  #也可以
# admin的配置文件为 Ice.Default.Locator=IceGrid/Locator:tcp -p 5061 
# 与node的类似
icegridadmin --Ice.Config=./config/admin.cfg -ua -pa
>>> application add ./config/app.xml

./client --Ice.Config=./config/client.cfg 

