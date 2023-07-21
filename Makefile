
all: filefiller sizelang

filefiller: filefiller.cc
	g++ -o filefiller filefiller.cc

sizelang: sizelang.cc
	g++ -o sizelang sizelang.cc