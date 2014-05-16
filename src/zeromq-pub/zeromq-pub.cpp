#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <stdio.h>
#include <stdlib.h>

#include "zmq.h"
#if _DEBUG
#pragma comment(lib, "libzmq-v100-mt-gd-4_0_4.lib")
#else
#pragma comment(lib, "libzmq-v100-mt-4_0_4.lib")
#endif

int main(int argc, char* argv[])
{
	void *context = zmq_ctx_new();
	void *socket = zmq_socket(context, ZMQ_PUB);
	int rv = zmq_bind(socket, "tcp://*:12345");
	if (rv != 0) {
		printf("zmq_bind() failed...%s\n", zmq_strerror(zmq_errno()));
		return -1;
	}

	printf("start zeromq-pub...\n");

	// create message...?
	char *buf = NULL;
	int buf_size = 1024 * 1024 * 4;
	buf = (char *)malloc(buf_size);
	memset(buf, 0, buf_size);
	memset(buf, 'A', buf_size-1);

	DWORD t = 0;
	while (1) {
		DWORD st = ::GetTickCount();

		memcpy(buf, &t, 4);

		// publish message
		int size = zmq_send(socket, buf, buf_size, 0);
		printf("send : %d, %d\n", t, buf_size);

		DWORD et = ::GetTickCount();
		DWORD diff = et - st;
		if (diff > 33) diff = 33;
		Sleep(33 - diff);

		t ++;
		if (t%100 == 0) {
			printf("diff = %d\n", diff);
		}
	}

	zmq_close(socket);
	zmq_ctx_destroy(context);

	return 0;
}

