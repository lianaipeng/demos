#/bin/bash

# 判断参数是否正确 
if [ $# != 2 ]
then
    echo "arg count is" $# "must be two"
    echo "please run with update file and environment"
    exit
fi

# 判断文件是否存在
if [ ! -e $1 ]
then 
    echo $1 "is not exit"
    exit
fi

# 判断环境是否存在
envir=''
if [ $2 == 'dev' ] ;then
    if [ $1 == 'chat_server' ] ;then 
        envir='/data/kefu_dev/chat/'
    elif [ $1 == 'monitor-server' ] ;then 
        envir='/data/kefu_dev/chat/monitor/'
    elif [ $1 == 'web-front' ] ;then 
        #envir='/data/kefu_dev/web-front/'
        envir='/data/kefu_dev/web-front/ice-server/'
    elif [ $1 == 'app_server' ] ;then 
        envir='/data/kefu_dev/app_server/'
    elif [ $1 == 'web-poll' ] ;then 
        envir='/data/kefu_dev/web-poll/'
    fi
elif [ $2 == 'v1' ] ;then
    if [ $1 == 'chat_server' ] ;then 
        envir='/data/kefu_v1/chat1/'
    elif [ $1 == 'monitor-server' ] ;then 
        envir='/data/kefu_v1/chat1/monitor/'
    elif [ $1 == 'web-front' ] ;then 
        envir='/data/kefu_v1/web-front/'
    elif [ $1 == 'app_server' ] ;then 
        envir='/data/kefu_v1/app_server1/'
    elif [ $1 == 'web-poll' ] ;then 
        envir='/data/kefu_v1/web-poll/'
    fi
elif [ $2 == "release" ] ;then
    echo $1 "will be update to release" 
elif [ $2 == "master" ] ;then
    echo $1 "will be update to master" 
else
    echo $1 "is not correct arg"   
    exit
fi
echo "$1 will be update to $envir" 


dateStr=`date "+_%Y_%m_%d"`
#echo $envir$1$dateStr
if [ -e $envir$1 ];then 
    if [ ! -e $envir$1$dateStr ];then
        mv $envir$1 $envir$1$dateStr
        echo "copy $1 as $1$dateStr"
    else 
        echo "replease $envir$1"
    fi
fi
cp $1 $envir
sudo chown kefu $envir$1
sudo chgrp kefu $envir$1

