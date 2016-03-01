### 常用命令：
*   icegridadmin --Ice.Config=scripts/local/admin.config.client -uadmin -padmin -e  ' help'
*   icegridadmin --Ice.Config=scripts/local/admin.config.client -uadmin -padmin -e  'application update conf/beta/application.xml '


### 部署：
    1. 启动register:
		cd scripts/local
		启动master: sh run-master.sh
		启动slave: sh run-slave.sh [如本机已经启动了master，一般不需要启动slave了]
		注：
			在一台机器上只需要启动一个register即可，如需启动多个，需要自行修改配置文件

	2. 启动node:
		cd scripts/local
		sh run-node.sh
		注：
			a. 目前node以本机ip命名，如需启动多个，可自行修改
			b. 本地需要有hostname: grid.local

### 本地调试:
    0. cd follow_collection
    1. ice_stage=beta ./dummy_server
    2. ./follow_client --Ice.Config=config.client
    
