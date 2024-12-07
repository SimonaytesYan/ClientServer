#pragma once

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

const size_t kServerPort = 8080;

sockaddr_in createServerAddr();
