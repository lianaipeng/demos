
# app.xml
#    adater name and id 必须相同，还不知道原因 

mkdir registry node

icegridnode --Ice.Config=config 
icegridnode --Ice.Config=node2.cfg 

icegridadmin --Ice.Config=config -ua -pa
>>> application add app.xml
# server可以自动启动

./client --Ice.Config=client.cfg
