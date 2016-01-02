compile: build
	g++ -std=c++14 main.cc -o BondService

build:
	protoc -I=. --cpp_out=. *.proto

clean:
	rm -rf BondService *.pb.h *.pb.cc
