#ifndef __common_socket_h__
#define __common_socket_h__

#include <assert.h>
#include <sys/socket.h>

#define PORT        8080

#define Setsockopt(socket, level, optname, optval, optlen)  assert(setsockopt(socket, level, optname, optval, optlen) == 0);
#define Bind(socket, address, address_len)                  assert(bind(socket, address, address_len) == 0);
#define Listen(socket, backlog)                             assert(listen(socket, backlog) == 0);
#define Send(socket, buffer, length, flags)                 assert(send(socket, buffer, length, flags) != -1);

#endif