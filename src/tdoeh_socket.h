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

private:
    inline int bind(const char *szAddress);
	inline int connect(const char *szAddress);
    int getPeerName(string &sPeerAddress);
    int getSockName(string &sSockAddress);

public:
    int fd() const;

    int initialize(int fd, bool bOwner, int iDomain = AF_INET);
	int create(int iType = SOCK_STREAM, int iDomain = AF_INET, bool bBlock = true);
    void close();

	void bind(const char *szAddress, int iPort);
	int listen(int iBackLog);
	int accept(CSocket &objSock, bool bBlock = true);
	int connect(const char *szAddress, uint16_t nPort);

    int recv(void *pvBuf, size_t iLen, int iFlags = 0);
	int send(const void *pvBuf, size_t iLen, int iFlags = 0);
    int recvfrom(void *pvBuf, size_t iLen, string &strFromAddr, uint16_t *pnFromPort, int iFlags = 0);
	int sendto(const void *pvBuf, size_t iLen, const char *szToAddr, uint16_t iToPort, int iFlags = 0);

    int getPeerName(string &sPeerAddress, uint16_t &iPeerPort);
    int getSockName(string &sSockAddress, uint16_t &iSockPort);

	int setSockOpt(int opt, const void *pvOptVal, socklen_t optLen, int level = SOL_SOCKET);
	int getSockOpt(int opt, void *pvOptVal, socklen_t &optLen, int level = SOL_SOCKET);
    int setReuse(bool bReuse = true);
    int setBlock(bool bBlock = true);
    //void setNoCloseWait();
    //void setKeepAlive();
    //void setTcpNoDelay();
    //int getRecvBufferSize();
    //int setRecvBufferSize(int sz);
    //int getSendBufferSize();
    //int setSendBufferSize(int sz);
};

}
