# Multithreaded HTTP proxy
Simple dumb proxy with cache in RAM

- Uses `pthread` for each connection
- OOP-designed
- Only GET-requests are supported

## Build
	mkdir build/
	cd build/
	cmake ..
	make
	
## Run
	./ProxyOnThreads [port-to-listen]