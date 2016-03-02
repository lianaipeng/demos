#!/bin/bash

echo $?
icegridadmin --Ice.Config=./config/forum_admin.conf -ua -pa -e " server stop ServerId "
echo "server stop ServerId"
icegridadmin --Ice.Config=./config/forum_admin.conf -ua -pa -e " node shutdown NodeName "
echo "node shutdown NodeName"
icegridadmin --Ice.Config=./config/forum_admin.conf -ua -pa -e " registry shutdown Master "
echo "registry shutdown Master"
