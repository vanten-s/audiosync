struct Client {
    int fd;
    int client_id;
};

void* handle_client(void* client);
