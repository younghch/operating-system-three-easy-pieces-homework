#ifndef __common_socket_h__
#define __common_socket_h__

#include <assert.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define LOCAL_HOST  "127.0.0.1"
#define EOFS        "#!EOF*&"
#define PORT        8080
#define BUFFER_SIZE 1024

#define Setsockopt(socket, level, optname, optval, optlen)  assert(setsockopt(socket, level, optname, optval, optlen) != -1);
#define Bind(socket, address, address_len)                  assert(bind(socket, (struct sockaddr*)address, address_len) == 0);
#define Listen(socket, backlog)                             assert(listen(socket, backlog) == 0);
#define Connect(socket, address, address_len)               assert(connect(socket, (struct sockaddr*)address, address_len) != -1);
#define Send(socket, buffer, length, flags)                 assert(send(socket, buffer, length, flags) != -1);
#define Inet_pton(af, src, dst)                             assert(inet_pton(af, src, dst) > 0);
#endif