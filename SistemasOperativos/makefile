# make -> para compilar tudo (all)
# make all/manager/manager.o...
all: feed manager # chama os dois (feed.o e o manager.o)

manager: manager.o
	gcc -o manager manager.o -lpthread

#manager.o: manager.c header.h
manager.o: manager.c
	gcc -c manager.c

feed: feed.o
	gcc -o feed feed.o -lpthread

feed.o: feed.c
	gcc -c feed.c

clean:
	rm -f manager feed manager.o feed.o

clean-manager:
	rm -f manager manager.o

clean-feed:
	rm -f feed feed.o