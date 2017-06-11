all:
	$(if $(USE_SSL), c++ -std=c++11 -o test test.cpp tcp.cpp irc.cpp -D_USE_SSL_ -lcrypt -lssl, c++ -std=c++11 -o test test.cpp tcp.cpp irc.cpp)

clean:
	rm -f test
	rm -f nohup*

run:
	nohup ./test &

kill:
	killall test
