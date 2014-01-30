#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <stdio.h>
#include <stdlib.h>

#include "zmq.h"
#if _DEBUG
#pragma comment(lib, "libzmq-v100-mt-gd-4_0_3.lib")
#else
#pragma comment(lib, "libzmq-v100-mt-4_0_3.lib")
#endif

int main(int argc, char* argv[])
{
	void *context = zmq_ctx_new();
	void *socket = zmq_socket(context, ZMQ_REP);
	int rv = zmq_bind(socket, "tcp://*:12346");
	if (rv != 0) {
		printf("zmq_bind() failed...%s\n", zmq_strerror(zmq_errno()));
		return -1;
	}

	printf("start zeromq-rep...\n");

	char *buf = NULL;
	int buf_size = 1024 * 1024 * 16; // 16MB
	buf = (char *)malloc(buf_size);
	memset(buf, 'A', buf_size-1);

	while (1) {
		// recieve
		int size = zmq_recv(socket, buf, buf_size, 0);
		
		DWORD t;
		memcpy(&t, buf, 4);

		// reply message
		size = zmq_send(socket, buf, size, 0);
		printf ("reply : t=%d, size=%d\n", t, size);
	}

	zmq_close(socket);
	zmq_ctx_destroy(context);

	return 0;
}

