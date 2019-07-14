#include <stdlib.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <mongoose.h>
#include <socket.h>

struct route_struct *router;

#include <function_handler.c>
#include <Routes.c>

static void websocket_ready_handler(struct mg_connection *conn){
    static char *message = "Websocket ready";
    mg_websocket_write(conn, WEBSOCKET_OPCODE_TEXT, message, strlen(message));
}

static int websocket_data_handler(struct mg_connection *conn, int flags, char *data, size_t data_len){
    (void) flags; // Unused
    mg_websocket_write(conn, WEBSOCKET_OPCODE_TEXT, data, data_len);
    return memcmp(data, "exit", 4);
}

static int begin_handler(struct mg_connection *conn){
    const struct mg_request_info *request_info = mg_get_request_info(conn);
    struct route_struct *pointer;
    int flag = 0;
    char url[120] = "webservice";

    strcat(url,request_info->uri);
    pointer = router;

    while(1){
        if ((!strcmp(pointer->route, request_info->uri)) && (!strcmp(pointer->request_method, request_info->request_method))){
            pointer->callback_function(conn);
            flag = 1;
            break;
        }
        if (!pointer->next){
            break;
        }
        pointer = pointer->next;
    }

    if(!flag){
        mg_send_file(conn, url);  
    } 

    return 1;
}



int main(int argc, char const *argv[]){
    struct mg_context *ctx, *websocket;
    struct mg_callbacks callbacks, callbacks2;

    const char *options[] = {
        "listening_ports", "8080",
        "document_root", "www",
        NULL
    };

    //mg_get_header retorna los valores del header
    //handle_file_request retorna archivo
    //opendir
    //closedir

    router = (struct route_struct *)malloc(sizeof(struct route_struct)); 
    router->id = 0;
    router->route = "";
    router->request_method = "";

    Routes();

    memset(&callbacks, 0, sizeof(callbacks));
    callbacks.begin_request = begin_handler;
    callbacks2.websocket_ready = websocket_ready_handler;
    callbacks2.websocket_data = websocket_data_handler;
    
    websocket = mg_start(&callbacks2, NULL, NULL);

    callbacks.websocket_connect = websocket;

    ctx = mg_start(&callbacks, NULL, options);
    getchar();  // Wait until user hits "enter"
    mg_stop(ctx);

    while(router->next){
        pop_route(router);
    }
    free(router);

    return 0;
}