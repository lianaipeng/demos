#!/bin/bash

icegridadmin --Ice.Config=./config/forum_admin.conf -ua -pa -e " application add ./config/app.xml "
icegridadmin --Ice.Config=./config/forum_admin.conf -ua -pa -e " server start ServerId "

