#!/bin/bash

echo $?
icegridadmin --Ice.Config=./config/forum_admin.conf -ua -pa -e " server stop ServerId "
echo "server stop ServerId"
#icegridadmin --Ice.Config=./config/forum_admin.conf -ua -pa -e " node shutdown ForumNodeName "
#echo "node shutdown ForumNodeName"
icegridadmin --Ice.Config=./config/forum_admin.conf -ua -pa -e " application remove AppName "
echo "application remove AppName"
#icegridadmin --Ice.Config=./config/forum_admin.conf -ua -pa -e " registry shutdown Master "
#echo "registry shutdown Master"
