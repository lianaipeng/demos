
################################
## 在同一个node下启动多个server
xml文件中，node name 必须相同，app name 必须不同



################################
## node与server 分开启动
需要创建
/data/web/ice/registry
/data/web/ice/node


ice 为registry和node相关的配置，日志，临时文件所在
forum 程序在  ../forum/ 下
gridNode 程序在  ../../makeV/gridNode/


forum 和 gridNode 目录下必须添加log目录
(node.conf 配置文件中有IceGrid.Node.Output=./log)


#node 与 server 分离
node的启动目录和server的启动目录不同时，配置文件有所不同

/data/web/ice/log      # node日志
/data/web/ice/config/  # node配置
/data/web/forum/       # server路径

node的配置文件
    IceGrid.Node.Output=./log        
    # node下
    # 各个server的标准输出(需在各个server的启动目录下创建该目录,需要在xml文件中，添加pwd=""参数) 
    Ice.StdErr=./ice/log/nodestd.err
    Ice.StdOut=./ice/log/nodestd.out
    # node的标准错误和标准输出

node启动(当前执行目录为 /data/web)
    nohup icegridnode --Ice.Config=ice/config/node.conf --nochdir &

server启动(当前路径为 /data/web/forum)
    必须以建立 log目录   因为node.conf配置 IceGrid.Node.Output=./log 在起作用

