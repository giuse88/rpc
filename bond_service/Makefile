build: build-proto
	g++ -std=c++14 server.cc -o BondService ServiceBondInput.pb.cc ServiceBondOutput.pb.cc ServiceContainer.cc -lzmq -lprotobuf
	g++ -std=c++14 client.cc -o BondClient ServiceBondInput.pb.cc ServiceBondOutput.pb.cc -lzmq -lprotobuf

build-proto:
	protoc -I=. --cpp_out=. *.proto

clean:
	rm -rf BondService *.pb.h *.pb.cc
