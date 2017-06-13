#!/bin/sh

PIPE=/tmp/irc_fifo
OUTPUT=bot.out

while true; do
    if test -p $PIPE; then
        if read line < $PIPE; then
            echo "$line" >> $OUTPUT
        fi
    else
        exit
    fi
done
