/*************************************************************************************
 * 文件名: tdoeh_socket.cpp
 * 作  者: 思凡
 * 日  期: 2013年07月05日
 * 描  述: 
 * 修  改: 
 *************************************************************************************/
#include "tdoeh_socket.h"

namespace Tdoeh{

CSocket::CSocket() : m_iFd(INVALID_HANDLE), m_bOwner(true), m_iDomain(AF_INET)
{
}

CSocket::~CSocket()
{
    if(m_bOwner){
        close();
    }
}

int CSocket::fd() const
{
    return m_iFd;
}

int CSocket::initialize(int fd, bool bOwner, int iDomain)
{
    if(m_bOwner){
        close();
    }

    if(fd < 0){
        TDOEH_SET_ERROR_NUMBER_RETURN(E_ARGUMENTS);
    }
    switch(iDomain){
        case AF_INET:
        case AF_UNIX:
            break;

        default:
            TDOEH_SET_ERROR_NUMBER_RETURN(E_ARGUMENTS);
    }

    m_iFd == fd;
    m_bOwner = bOwner;
    m_iDomain = iDomain;

    return E_SUCCESS;
}

int CSocket::create(int iType, int iDomain, bool bBlock)
{
    switch(iType){
        case SOCK_STREAM:
        case SOCK_DGRAM:
            break;

        default:
            TDOEH_SET_ERROR_NUMBER_RETURN(E_ARGUMENTS);
    }

    switch(iDomain){
        case AF_INET:
            break;
        case AF_UNIX:
        case AF_LOCAL:
            iDomain = AF_UNIX;
            break;

        default:
            TDOEH_SET_ERROR_NUMBER_RETURN(E_ARGUMENTS);
    }

    if(bBlock){
        iType |= SOCK_CLOEXEC;
    }else{
        iType |= SOCK_NONBLOCK | SOCK_CLOEXEC;
    }

    m_iFd = socket(iDomain, iType, 0);
    if(m_iFd == INVALID_HANDLE){
        TDOEH_SET_ERROR_NUMBER_RETURN(errno);
    }

    m_iDomain = iDomain;

    return E_SUCCESS;
}

void CSocket::close()
{
    if(m_iFd != INVALID_HANDLE){
        ::close(m_iFd);
        m_iFd == INVALID_HANDLE;
    }
}

int CSocket::bind(const char *szAddress)
{
    unlink(szAddress);
    struct sockaddr_un stBindAddr;
    bzero(&stBindAddr, sizeof(struct sockaddr_un));
    stBindAddr.sun_family = m_iDomain;
    strncpy(stBindAddr.sun_path, szAddress, sizeof(stBindAddr.sun_path));
    if(::bind(m_iFd, (struct sockaddr *)&stBindAddr, sizeof(stBindAddr)) < 0){
        TDOEH_SET_ERROR_NUMBER_RETURN(errno);
    }

    return E_SUCCESS;
}

int CSocket::bind(const char *szAddress, int iPort)
{
    if(m_iDomain == AF_UNIX){
        return this->bind(szAddress);
    }

    struct sockaddr_in stBindAddr;
    stBindAddr.sin_family = m_iDomain;
    stBindAddr.sin_port = htons(iPort);

    if(strlen(szAddress) <= 0){
        stBindAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    }else{
        stBindAddr.sin_addr.s_addr = inet_aton(szAddress);	
        if(stBindAddr.sin_addr.s_addr == 0){
            TDOEH_SET_ERROR_NUMBER_RETURN(errno);
        }
    }

    if(::bind(m_iFd, (struct sockaddr *)&stBindAddr, sizeof(stBindAddr)) < 0){
        TDOEH_SET_ERROR_NUMBER_RETURN(errno);
    }
    
    return E_SUCCESS;
}

int CSocket::listen(int iBackLog)
{
    if(::listen(m_iFd, iBackLog) < 0){
        TDOEH_SET_ERROR_NUMBER_RETURN(errno);
    }

    return E_SUCCESS;
}


int CSocket::accept(CSocket &objSock, bool bBlock)
{
    int iFd;
    int iFlags = SOCK_CLOEXEC;
    struct sockaddr stSockAddr;
    socklen_t iSockLen = sizeof(sockaddr);

    if(!bBlock){
        iFlags |= SOCK_NONBLOCK;
    }

    while(true){
        iFd = ::accept4(m_iFd, &stSockAddr, &iSockLen, iFlags);
        if(iFd >= 0){
            break;
        }

        if(errno != EINTR){
            TDOEH_SET_ERROR_NUMBER_RETURN(errno);
        }
    }

    return objSock.initialize(iFd, true, m_iDomain);
}

int CSocket::connect(const char *szAddress, uint16_t nPort)
{
    if(m_iDomain == AF_UNIX){
        return this->connect(szAddress);
    }

    struct sockaddr_in stServerAddr;
    stServerAddr.sin_family = m_iDomain;
    stServerAddr.sin_addr.s_addr = inet_aton(szAddress);
    if(stServerAddr.sin_addr.s_addr == 0){
        TDOEH_SET_ERROR_NUMBER_RETURN(errno);
    }
    stServerAddr.sin_port = htons(nPort);

    if(::connect(m_iFd, (struct sockaddr *)&stServerAddr, sizeof(struct sockaddr)) < 0){
        TDOEH_SET_ERROR_NUMBER_RETURN(errno); 
    }

    return E_SUCCESS;
}

int CSocket::connect(const char *szAddress)
{
    struct sockaddr_un stServerAddr;
    stServerAddr.sin_family = m_iDomain;
    strncpy(stServerAddr.sun_path, szAddress, sizeof(stServerAddr.sun_path));

    if(::connect(m_iFd, (struct sockaddr *)&stServerAddr, sizeof(struct sockaddr)) < 0){
        TDOEH_SET_ERROR_NUMBER_RETURN(errno); 
    }

    return E_SUCCESS;
}

int CSocket::recv(void *pvBuf, size_t iLen, int iFlags)
{
    ssize_t nRecvLen = ::recv(m_iFd, pvBuf, iLen, iFlags);
    if(nRecvLen < 0){
        TDOEH_SET_ERROR_NUMBER_RETURN(errno);
    }

    return nRecvLen;
}

int CSocket::send(void *pvBuf, size_t iLen, int iFlags)
{
    ssize_t nSendLen = ::send(m_iFd, pvBuf, iLen, iFlags);
    if(nSendLen < 0){
        TDOEH_SET_ERROR_NUMBER_RETURN(errno);
    }

    return nSendLen;
}

int CSocket::recvfrom(void *pvBuf, size_t iLen, string &strFromAddr, uint16_t *pnFromPort, int iFlags)
{
    struct sockaddr_in stFromAddr;
    socklen_t nAddrLen = sizeof(struct sockaddr);

    ssize_t nRecvLen = ::recvfrom(m_iFd, pvBuf, iLen, 
                            (struct sockaddr *)&stFromAddr, &nAddrLen, iFlags);
    if(nRecvLen < 0){
        TDOEH_SET_ERROR_NUMBER_RETURN(errno);
    }

    char szAddress[20];
    if(inet_ntop(m_iDomain, stFromAddr.sin_addr, szAddress, sizeof(szAddress)) == NULL){
        TDOEH_SET_ERROR_NUMBER_RETURN(errno);
    }
    strFromAddr = szAddress;
    *pnFromPort = ntohs(stFromAddr.sin_port);

    return nRecvLen;
}

int CSocket::sendto(const void *pvBuf, size_t iLen, const char *szToAddr, uint16_t iToPort, int iFlags)
{
    struct sockaddr_in stToAddr;
    stToAddr.sin_family = m_iDomain;
    stToAddr.sin_addr.s_addr = inet_aton(szToAddr);
    stToAddr.sin_port = htons(iToPort);
    int iSendLength = ::sendto(m_iFd, pvBuf, iLen, &stToAddr, sizeof(stToAddr), iFlags);
    if(iSendLength < 0){
        TDOEH_SET_ERROR_NUMBER_RETURN(errno);
    }

    return iSendLength;
}

}
