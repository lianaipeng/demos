

make clean
make

mkdir registry
icegridregistry --Ice.Config=registry.cfg
./server --Ice.Config=server.cfg
./client --Ice.Config=client.cfg
