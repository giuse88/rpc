compile: build
	g++ -std=c++14 AddressBookWriter.cc addressbook.pb.cc -o AddressBookWriter -lprotobuf
	g++ -std=c++14 AddressBookReader.cc addressbook.pb.cc -o AddressBookReader -lprotobuf

build:
	protoc -I=. --cpp_out=. *.proto
