/*************************************************************************************
 * 文件名: tdoeh_socket.h
 * 作  者: 思凡
 * 日  期: 2013年07月01日
 * 描  述: 对于socket的封装
 * 修  改: 
 *************************************************************************************/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#include "tdoeh_base.h"

namespace Tdoeh{

class CSocket{
    int m_iFd;
	bool m_bOwner;
    int m_iDomain;

public:
    CSocket();
    ~CSocket();

public:
    int fd() const;

    int initialize(int fd, bool bOwner, int iDomain = AF_INET);
	int create(int iSocketType = SOCK_STREAM, int iDomain = AF_INET);
    int close();

    void getPeerName(string &sPeerAddress, uint16_t &iPeerPort);
    void getSockName(string &sSockAddress, uint16_t &iSockPort);

	void bind(const string &sServerAddr, int port);
	int listen(int connBackLog);
	int accept(CSocket &sock, bool bNoBlock = false);
	int connect(const string &sServerAddr, uint16_t port);

    int recv(void *pvBuf, size_t iLen, int iFlag = 0);
	int send(const void *pvBuf, size_t iLen, int iFlag = 0);
    int recvfrom(void *pvBuf, size_t iLen, string &sFromAddr, uint16_t &iFromPort, int iFlags = 0);
	int sendto(const void *pvBuf, size_t iLen, const string &sToAddr, uint16_t iToPort, int iFlags = 0);


	int setSockOpt(int opt, const void *pvOptVal, socklen_t optLen, int level = SOL_SOCKET);
	int getSockOpt(int opt, void *pvOptVal, socklen_t &optLen, int level = SOL_SOCKET);
    int setblock(bool bBlock = false);
    //void setNoCloseWait();
    //void setKeepAlive();
    //void setTcpNoDelay();
    int getRecvBufferSize();
    int setRecvBufferSize(int sz);
    int getSendBufferSize();
    int setSendBufferSize(int sz);
};

}