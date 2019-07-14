int _index(struct mg_connection *conn){
    char data[32768], *headers = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
    const struct mg_request_info *request_info = mg_get_request_info(conn);

    memset(data,NULL,32768);
    response("www/index.html","@HOLA","<h1>HOLA!!</h1>\n", data);
    //response("","@_TEST2","<h1>TESTE2</h1>!!\n", data);

    //mg_websocket_write(conn,WEBSOCKET_OPCODE_TEXT,data, strlen(data));
    mg_write(conn, headers, strlen(headers));
    mg_write(conn, data, strlen(data));
    return 1;
}

