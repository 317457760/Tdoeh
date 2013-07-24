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
#include <string>
#include "tdoeh_base.h"

namespace Tdoeh{

class CSocket : public CBase{
    int m_iFd;
	bool m_bOwner;
    int m_iDomain;

public:
    CSocket();
    ~CSocket();

private:
    inline int bind(const char *szAddress);
	inline int connect(const char *szAddress);
    int getPeerName(std::string &strPeerAddress);
    int getSockName(std::string &strSockAddress);

public:
    int fd() const;

    int initialize(int fd, bool bOwner, int iDomain = AF_INET);
	int create(int iType = SOCK_STREAM, int iDomain = AF_INET, bool bBlock = true);
    void close();

	int bind(const char *szAddress, int iPort);
	int listen(int iBackLog);
	int accept(CSocket &objSock, bool bBlock = true);
	int accept(int *pFd, bool bBlock = true);
	int connect(const char *szAddress, uint16_t nPort);

    int recv(void *pvBuf, size_t iLen, int iFlags = 0);
	int send(const void *pvBuf, size_t iLen, int iFlags = 0);
    int recvfrom(void *pvBuf, size_t iLen, std::string &strFromAddr, uint16_t *pnFromPort, int iFlags = 0);
	int sendto(const void *pvBuf, size_t iLen, const char *szToAddr, uint16_t iToPort, int iFlags = 0);

    int getPeerName(std::string &strPeerAddress, uint16_t &iPeerPort);
    int getSockName(std::string &strSockAddress, uint16_t &iSockPort);

    int setSockOpt(int iOpt, const void *pvOptVal, socklen_t nOptLen, int iLevel = SOL_SOCKET);
    int getSockOpt(int iOpt, void *pvOptVal, socklen_t *pnOptLen, int iLevel = SOL_SOCKET);
    int setReuse(bool bReuse);
    int setBlock(bool bBlock);
    //void setNoCloseWait();
    //void setKeepAlive();
    //void setTcpNoDelay();
    //int getRecvBufferSize();
    //int setRecvBufferSize(int sz);
    //int getSendBufferSize();
    //int setSendBufferSize(int sz);
};

}
