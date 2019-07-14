#ifndef SOCKET_H
#define SOCKET_H

typedef int (*jag_function) (struct mg_connection *conn);

struct route_struct
{
	struct route_struct *next;
	char *route;
	char *request_method;
	jag_function callback_function;
	int id;
};

int push_route(struct route_struct *route);
int pop_route(struct route_struct *route);
void Route(struct route_struct *root, char *route, char* request_method ,jag_function callback);

int response(const char* dir, const char* name, const char* value, char *data);

#include <socket.c>
#endif