#!/bin/bash

rm -rf data/registry/*
rm -rf data/node/*

nohup icegridregistry --Ice.Config=./config/forum_registry.conf &
sleep 3
nohup icegridnode --Ice.Config=./config/forum_node.conf &
sleep 3
icegridadmin --Ice.Config=./config/forum_admin.conf -ua -pa -e " application add ./config/forum_app.xml "
icegridadmin --Ice.Config=./config/forum_admin.conf -ua -pa -e " server start ServerId "

