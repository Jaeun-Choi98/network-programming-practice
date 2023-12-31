#include "..\Common.h"

#define MULTICASTIP "235.7.8.9"
#define REMOTEPORT 9000
#define BUFSIZE 512

int main(int argc, char* argv[]) {
	int retval;
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// 멀티캐스트 TTL 설정
	DWORD ttl = 2;
	retval = setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL,
		(const char*)&ttl, sizeof(ttl));
	if (retval == SOCKET_ERROR) err_quit("setsockopt()");

	sockaddr_in remoteaddr;
	memset(&remoteaddr, 0, sizeof(remoteaddr));
	remoteaddr.sin_family = AF_INET;
	remoteaddr.sin_port = htons(REMOTEPORT);
	inet_pton(AF_INET, MULTICASTIP, &remoteaddr.sin_addr);

	char buf[BUFSIZE + 1];
	int len;

	while (1) {
		printf("\n[보낼 데이터] ");
		if (fgets(buf, BUFSIZE + 1, stdin) == NULL)
			break;

		len = (int)strlen(buf);
		if (buf[len - 1] == '\n')
			buf[len - 1] = '\0';
		if (strlen(buf) == 0)
			break;

		retval = sendto(sock, buf, strlen(buf), 0,
			(sockaddr*)&remoteaddr, sizeof(remoteaddr));
		if (sock == INVALID_SOCKET) {
			err_display("sendto()");
			break;
		}
		printf("[UDP] %d바이트 보냈습니다.\n", retval);
	}
	closesocket(sock);
	WSACleanup();
	return 0;
}