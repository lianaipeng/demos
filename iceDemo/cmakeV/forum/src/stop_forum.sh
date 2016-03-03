#!/bin/bash

echo $?
icegridadmin --Ice.Config=./config/forum_admin.conf -ua -pa -e " server stop ForumServerId "
echo "server stop ForumServerId"
icegridadmin --Ice.Config=./config/forum_admin.conf -ua -pa -e " node shutdown ForumNodeName "
echo "node shutdown ForumNodeName"
icegridadmin --Ice.Config=./config/forum_admin.conf -ua -pa -e " registry shutdown Master "
echo "registry shutdown Master"
