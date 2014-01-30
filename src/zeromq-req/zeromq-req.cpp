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
	void *socket = zmq_socket(context, ZMQ_REQ);

	int rv = zmq_connect(socket, "tcp://127.0.0.1:12346");
	if (rv != 0) {
		printf("zmq_connect() failed...%s\n", zmq_strerror(zmq_errno()));
		return -1;
	}

	printf("start zeromq-req...\n");

	int buf_size = 1024 * 1024 * 4;
	char *buf = (char *)malloc(buf_size);
	memset(buf, 0, buf_size);
	char *recv_buf = (char *)malloc(buf_size);
	memset(buf, 0, buf_size);

	DWORD t = 0;
	while (1) {
		DWORD st = ::GetTickCount();

		memcpy(buf, &t, 4);

		// request
		int size = zmq_send(socket, buf, buf_size, 0);
		if (size == -1) {
			printf("zmq_send() failed...%s\n", zmq_strerror(zmq_errno()));
			continue;
		}

		// recieve message
		memset(buf, 0, buf_size);
		size = zmq_recv(socket, recv_buf, buf_size, 0);
		if (size == -1) {
			printf("zmq_recv() failed...%s\n", zmq_strerror(zmq_errno()));
			continue;
		}

		DWORD recv_t;
		memcpy(&recv_t, recv_buf, 4);
		printf ("recv : t=%d, size=%d\n", recv_t, size);

		// sleep
		DWORD et = ::GetTickCount();
		DWORD diff = et - st;
		if (diff > 33) diff = 33;
		Sleep(33 - diff);

		t ++;
	}

	zmq_close(socket);
	zmq_ctx_destroy(context);

	return 0;
}

