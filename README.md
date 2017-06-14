# irc-cpp

Simple C++ IRC bot implementation

# dependencies

For now, this project depends on [openssl](https://www.openssl.org/) and optionally
[pugixml](https://github.com/zeux/pugixml) as well as [sqlite3](https://www.sqlite.org/),
if you want to build `test.cpp`.

# compile

    # without ssl
    $ make
    
    # with ssl
    $ make USE_SSL=t

    # to include your own config in 'config.h'
    $ make IRC_CONFIG=t

# run/kill

    # run (will send the bot's output to 'bot.out')
    $ make run
    
    # kill (or type '.quit' on the irc channel)
    $ make kill

# usage

The test bot that comes with the source, `test.cpp`, has the
following commands:

    .quit       --  kill the bot :-(
    .noticeme   --  the bot will notice you
    .esmaga     --  the bot will curse at you in portuguese :-)
    .np <user>  --  the bot will tell you what you're listening to

# protips

`tcp.cpp` is a standalone tcp client you can use for pretty much
anything you want; it even comes with nifty ssl support!

# license

This shit is public domain, do whatever the fuck you want with it.
Also, don't expect quality software from a novice. :O)
