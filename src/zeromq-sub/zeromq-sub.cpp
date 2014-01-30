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
	void *socket = zmq_socket(context, ZMQ_SUB);
	zmq_setsockopt(socket, ZMQ_SUBSCRIBE, NULL, 0);

	int rv = zmq_connect(socket, "tcp://127.0.0.1:12345");
	if (rv != 0) {
		printf("zmq_connect() failed...%s\n", zmq_strerror(zmq_errno()));
		return -1;
	}

	printf("start zeromq-sub...\n");

	// create message...?
	char *buf = NULL;
	int buf_size = 1024 * 1024 * 4;
	buf = (char *)malloc(buf_size);
	memset(buf, 0, buf_size);

	DWORD old_t = 0;
	while (1) {
		// recieve message
		int size = zmq_recv(socket, buf, buf_size, 0);
		DWORD t;
		memcpy(&t, buf, 4);
		printf ("recv ; t=%d, size=%d\n", t, size);
	
		DWORD diff = t - old_t;
		if (diff > 1) {
			printf("!!!! DROP MESSAGE !!!! diff=%d\n", diff);
		}
		old_t = t;
	}

	zmq_close(socket);
	zmq_ctx_destroy(context);

	return 0;
}

