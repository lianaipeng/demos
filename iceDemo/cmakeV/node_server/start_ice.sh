#!/bin/bash
echo "I Will start registry and node!"

rm -rf ice/registry/*
echo "rm -rf ice/registry/*"
rm -rf ice/node/*
echo "rm -rf ice/node/*"

nohup icegridregistry --Ice.Config=ice/config/registry.conf &
echo "nohup icegridregistry --Ice.Config=ice/config/registry.conf &"
sleep 1
nohup icegridnode --Ice.Config=ice/config/node.conf --nochdir &
echo "nohup icegridnode --Ice.Config=ice/config/node.conf --nochdir &"
sleep 1
