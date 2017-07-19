
# 参考文档
# http://blog.csdn.net/challenge_c_plusplus/article/details/7568886 (更好)
# http://www.cnblogs.com/kingcat/archive/2012/06/14/2549050.html 
# 没有验证


wget http://prdownloads.sourceforge.net/scons/scons-2.5.1.tar.gz
export MYSCONS=/home/**/download/scons-2.5.1
python $MYSCONS/setup.py install

wget https://downloads.sourceforge.net/project/jsoncpp/jsoncpp/0.5.0/jsoncpp-src-0.5.0.tar.gz?r=https%3A%2F%2Fsourceforge.net%2Fprojects%2Fjsoncpp%2F&ts=1500453361&use_mirror=nchc
mv jsoncpp-src-0.5.0.tar.gz\?r\=https\:%2F%2Fsourceforge.net%2Fprojects%2Fjsoncpp%2F  jsoncpp-src-0.5.0.tar.gz
tar xvf  jsoncpp-src-0.5.0.tar.gz
cd jsoncpp-src-0.5.0
python $MYSCONS/script/scons platform=linux-gcc
