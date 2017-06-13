IRC_CONFIG=f
USE_SSL=f
CONF=

ifeq ($(IRC_CONFIG), t)
	CONF=-D_IRC_CONFIG_H_
endif

all:
ifeq ($(USE_SSL), t)
	c++ -std=c++11 -o test test.cpp lfm.cpp utils.cpp tcp.cpp irc.cpp $(CONF) -D_USE_SSL_ -lcrypt -lssl -lpugixml
else
	c++ -std=c++11 -o test test.cpp lfm.cpp utils.cpp tcp.cpp irc.cpp $(CONF) -lcrypt -lssl -lpugixml
endif

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
