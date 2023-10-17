#include "..\Common.h"

char* SERVERIP = (char*)"127.0.0.1";
#define SERVERPORT 9000
#define BUFSIZE 50

int main(int argc, char* argv[]) {
	int retval;

	if (argc > 1) SERVERIP = argv[1];

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	

	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(SERVERPORT);
	inet_pton(AF_INET, SERVERIP, &serveraddr.sin_addr);
	
	// ������ ��ſ� ����� ���� 1. �������� ������ ����
	char buf[BUFSIZE];
	const char* testdata[] = {
		"�ȳ��ϼ���",
		"�ݰ�����",
		"�ȳ��� �輼��"
	};

	// �����͸� ���� ������ ���� ����/���� ����/���� ���Ḧ �ݺ��Ѵ�.
	for (int i = 0;i < 3;i++) {
		SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
		if (sock == INVALID_SOCKET)
			err_quit("socket()");

		retval = connect(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
		if (retval == INVALID_SOCKET)
			err_quit("connect()");
		printf("[TCP Ŭ���̾�Ʈ] ���� ����: ���� �ּ�=%s, ��Ʈ ��ȣ=%d\n", SERVERIP, SERVERPORT);
		
		// ������ ������
		memset(buf, '#', sizeof(buf));
		strncpy(buf, testdata[i], strlen(testdata[i]));
		retval = send(sock, buf, strlen(testdata[i]), 0);
		if (retval == INVALID_SOCKET) {
			err_display("send()");
			break;
		}
		printf("[TCP Ŭ���̾�Ʈ] %d����Ʈ�� ���½��ϴ�.\n", retval);
		closesocket(sock);
	}

	return 0;
}