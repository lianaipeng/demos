
make clean
make

mkdir -p /data/registry
icegridregistry --Ice.Config=config/registry.cfg
./server --Ice.Config=config/server.cfg
./client --Ice.Config=config/client.cfg

