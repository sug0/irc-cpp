all:
	c++ -std=c++11 -o test test.cpp tcp.cpp irc.cpp

clean:
	rm -f test
