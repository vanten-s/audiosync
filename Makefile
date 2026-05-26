.PHONY: common client server

common:
	cd ./common && make

client:
	cd ./client && make

server:
	cd ./server && make

clean:
	cd ./common && make clean
	cd ./client && make clean
	cd ./server && make clean
