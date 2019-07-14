CFLAGS  = -W -Wall -Icore/mongoose -pthread -g -pipe -lpthread -ldl -Icore -I. $(COPT)

.PHONY: clear

main: clear
	$(CC) $(CFLAGS) -DUSE_WEBSOCKET core/main.c core/mongoose/mongoose.c -o service

clear:
	-rm service