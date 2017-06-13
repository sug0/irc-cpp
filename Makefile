all:
	$(if $(USE_SSL), c++ -std=c++11 -o test test.cpp utils.cpp tcp.cpp irc.cpp -D_USE_SSL_ -lcrypt -lssl, c++ -std=c++11 -o test test.cpp utils.cpp tcp.cpp irc.cpp)

clean:
	rm -f test
	rm -f bot.out

run:
	rm -f bot.out
	rm -f /tmp/irc_fifo
	./test -p &
	sleep 1
	./pipe_handler.sh &

kill:
	killall test
