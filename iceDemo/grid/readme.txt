
make clean
make
icegridregistry --Ice.Config=registry.cfg
./server --Ice.Config=server.cfg
./client --Ice.Config=client.cfg
