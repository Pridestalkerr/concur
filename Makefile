install:
	cp -a ./include/. /usr/local/include/
	mv /usr/local/include/concur.hpp /usr/local/include/concur

uninstall:
	rm -rf /usr/local/include/concur /usr/local/include/libconcur/