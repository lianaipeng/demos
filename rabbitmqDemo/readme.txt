
第一种：
reciver:
queuename 
bindingkey

reciver1:
queuename1 
bindingkey

sender:
routingkey = bindingkey  
reciver And reciver1 两个程序都能收到


第二种：
reciver:
queuename 
bindingkey

reciver1:
queuename1 
bindingkey1

sender:
routingkey = bindingkey  
reciver 能收到
reciver1 收不到

