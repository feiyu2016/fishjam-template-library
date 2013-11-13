#ifndef FTL_SOCKET_HPP
#define FTL_SOCKET_HPP
#pragma once

#ifdef USE_EXPORT
#  include "ftlSocket.h"
#endif
//#include <winsock.h>
#include <mswsock.h>        //包含 SO_CONNECT_TIME 等宏定义，可能会造成其他错误?

namespace FTL
{
	CFWinsockEnvInit::CFWinsockEnvInit(WORD wVersionRequested /* = MAKEWORD(2, 2) */)
	{
		ZeroMemory(&m_wsaData, sizeof(m_wsaData));
		m_initResult = WSAStartup(wVersionRequested, &m_wsaData);

		//Success
		if (0 != m_initResult)
		{
			FTLTRACEEX(tlError, TEXT("WSAStartup Error: %d(%s)"), 
				m_initResult, CFNetErrorInfo(m_initResult).GetConvertedInfo());
		}
		FTLASSERT(m_initResult == 0);
		FTLASSERT(wVersionRequested == m_wsaData.wVersion);
	}
	
	CFWinsockEnvInit::~CFWinsockEnvInit()
	{
		if (0 == m_initResult)
		{
			WSACleanup();
		}
	}
	
	///////////////////////////////////////////////////////////////////////////////
#if 0
	CFSockAddrIn& CFSockAddrIn::Copy(const CFSockAddrIn& sin)
	{
		memcpy(this, &sin, Size());
		return *this;
	}

	// IsEqual
	bool CFSockAddrIn::IsEqual(const CFSockAddrIn& sin) const
	{
		// Is it Equal? - ignore 'sin_zero'
		return (memcmp(this, &sin, Size()-sizeof(sin_zero)) == 0);
	}

	///////////////////////////////////////////////////////////////////////////////
	// IsGreater
	bool CFSockAddrIn::IsGreater(const CFSockAddrIn& sin) const
	{
		// Is it Greater? - ignore 'sin_zero'
		return (memcmp(this, &sin, Size()-sizeof(sin_zero)) > 0);
	}

	///////////////////////////////////////////////////////////////////////////////
	// IsLower
	bool CFSockAddrIn::IsLower(const CFSockAddrIn& sin) const
	{
		// Is it Lower? - ignore 'sin_zero'
		return (memcmp(this, &sin, Size()-sizeof(sin_zero)) < 0);
	}

	///////////////////////////////////////////////////////////////////////////////
	// CreateFrom
	bool CFSockAddrIn::CreateFrom(LPCTSTR sAddr, LPCTSTR sService, int nFamily /*=AF_INET*/)
	{
		Clear();
		FTLASSERT(FALSE);
		CFConversion conv;
		sin_addr.s_addr = htonl(inet_addr(conv.TCHAR_TO_MBCS(sAddr)));

		sin_family = nFamily;
#if 0
		sin_addr.s_addr = htonl( CSocketComm::GetIPAddress(sAddr) );
		sin_port = htons( CSocketComm::GetPortNumber( sService ) );
		sin_family = nFamily;
#endif 
		return !IsNull();
	}
#endif

	CFSocket::CFSocket()
	{
		m_socket = INVALID_SOCKET;
		m_socketType = stTCP;
		m_bASync = TRUE;
		m_nSession = (UINT)(-1);
	}

	CFSocket::~CFSocket()
	{
		FTLASSERT(!IsOpen() && TEXT("Forget Close The Socket"));
		Close();
	}

	BOOL CFSocket::IsOpen()
	{
		BOOL isOpen = (INVALID_SOCKET != m_socket);
		return isOpen;
	}

	int CFSocket::Open(FSocketType st, BOOL bAsync)
	{
		FTLASSERT(INVALID_SOCKET == m_socket && TEXT("Can not Create Socket Twice"));

		int rc = NO_ERROR;
		m_socketType = st;
		m_bASync = bAsync;
		DWORD dwFlags = m_bASync ? WSA_FLAG_OVERLAPPED : 0;
		switch(m_socketType)
		{
		case stTCP:
			m_socket = WSASocket(AF_INET, SOCK_STREAM, 0/*IPPROTO_TCP*/, NULL, 0, dwFlags);
			break;
		case stUDP:
			m_socket = WSASocket(AF_INET, SOCK_DGRAM, 0/*IPPROTO_UDP*/, NULL, 0, dwFlags);
			break;
		default:
			FTLASSERT(FALSE);
			break;
		}
		if (INVALID_SOCKET == m_socket)
		{
			NET_VERIFY(SOCKET_ERROR);
			rc = SOCKET_ERROR;
		}
		return rc;
	}

	int CFSocket::Close()
	{
		int rc = NO_ERROR;
		SAFE_CLOSE_SOCKET(m_socket);
		//if (INVALID_SOCKET != m_socket)
		//{ 
		//    NET_VERIFY(closesocket(m_socket));
		//    m_socket = INVALID_SOCKET;
		//}
		m_nSession = (UINT)(-1);
		return rc;
	}

	int CFSocket::Shutdown(INT how)
	{
		FTLASSERT(INVALID_SOCKET != m_socket);
		int rc = NO_ERROR;
		rc = shutdown(m_socket, how);
		return rc;
	}

	int CFSocket::Connect(const CFSocketAddress& addrConnect)// (LPCTSTR pszAddr, INT nSocketPort)
	{
		//FTLASSERT(pszAddr);
		//FTLASSERT(nSocketPort);

		int rc = NO_ERROR;
		int nLength = sizeof(addrConnect);
		//sockaddr_in addrConnect = {0};

		//addrConnect.sin_family = AF_INET;
		//addrConnect.sin_port = htons(nSocketPort);
		//addrConnect.sin_addr.S_un.S_addr = inet_addr(CT2A(pszAddr));

		NET_VERIFY(WSAConnect(m_socket, addrConnect.lpSockaddr, nLength, NULL,NULL,NULL,NULL));

		return rc;
	}


	int CFSocket::Send(const BYTE* pBuf, INT len, DWORD flags)
	{
		int rc = NO_ERROR;
		WSABUF sendBuf = {0};
		sendBuf.len = (ULONG)len;
		sendBuf.buf = (char*)pBuf;
		DWORD dwNumberOfSend = 0;
		NET_VERIFY(WSASend(m_socket,&sendBuf, 1,&dwNumberOfSend,flags, NULL, NULL));
		return rc;
	}

	int CFSocket::Recv(BYTE* pBuf, INT len, DWORD flags)
	{
		int rc = NO_ERROR;

		if (m_socketType == stUDP)
		{
			//WSAEMSGSIZE
		}

		WSABUF recvBuf = {0};
		recvBuf.len = len;
		recvBuf.buf = (char*)pBuf;
		DWORD dwNumberOfRecv = 0;

		NET_VERIFY(WSARecv(m_socket,&recvBuf, 1, &dwNumberOfRecv, &flags ,NULL,NULL));
		return rc;
	}

	CFSocket*  CFSocket::Accept()
	{
		SOCKET soAccept = INVALID_SOCKET;
		//CFSockAddrIn addrClient;
		struct sockaddr_in addrClient = {0};
		int nLength = sizeof(struct sockaddr_in);
		CFSocket *pRetClient = NULL;

		// Check if there is something in the listen queue.
		soAccept = WSAAccept(m_socket, (struct sockaddr *) &addrClient, 
			&nLength, NULL, NULL);

		// Check for errors.
		if (soAccept != INVALID_SOCKET) 
		{
			// Get a pointer to the free ClientSocket 
			// instance from the pool.
			pRetClient = new CFSocket();
			if (pRetClient == NULL) 
			{
				// There are no free instances in the pool, maximum 
				// number of accepted client connections is reached.
				::shutdown(soAccept,2);
				::closesocket(soAccept);
			}
			else 
			{
				// Everything is fine, so associate the instance 
				// with the client socket.
				pRetClient->Associate(soAccept,&addrClient);
			}
		}

		return pRetClient;
	}

	int CFSocket::Associate(SOCKET socket, PSOCKADDR_IN pForeignAddr)
	{
		FTLASSERT(m_socket == INVALID_SOCKET);
		int rc = NO_ERROR;
		m_socket = socket;
		memcpy(&m_foreignAddr, pForeignAddr, sizeof(SOCKADDR_IN));
		return rc;
	}

	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	CFServerSocket::CFServerSocket()
	{
	}

	CFServerSocket::~CFServerSocket()
	{
	}

	void CFServerSocket::OnClose()
	{
	}

	int CFServerSocket::Bind(USHORT listenPort, LPCTSTR pszBindAddr  = NULL )
	{
		FTLASSERT(m_socket != INVALID_SOCKET);
		int rc = NO_ERROR;
		SOCKADDR_IN bindAddr_in = {0};
		bindAddr_in.sin_family = AF_INET;
		bindAddr_in.sin_port = htons(listenPort);

		if (pszBindAddr)
		{
			bindAddr_in.sin_addr.S_un.S_addr = inet_addr(CT2A(pszBindAddr));
		}
		else
		{
			bindAddr_in.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
		}

		NET_VERIFY(bind(m_socket,(SOCKADDR*)&bindAddr_in, sizeof(bindAddr_in)));
		return rc;
	}

	int CFServerSocket::StartListen(INT backlog, INT nMaxClients)
	{
		int rc = NO_ERROR;
		//m_pClientSocketPool = new CFMemCacheT<CFClientSocketT <T> >(0,nMaxClients);

		FTLASSERT( m_socket != INVALID_SOCKET );
		NET_VERIFY(listen(m_socket, backlog));
		return rc;
	}

#if 0
	template<typename T>
	void CFServerSocketT<T>::ReleaseClient(CFClientSocketT<T>* pClient)
	{
		FTLASSERT(NULL != pClient);
		FTLASSERT(NULL != m_pClientSocketPool);
		if (pClient)
		{
			pClient->Close();
			if (m_pClientSocketPool)
			{
				m_pClientSocketPool->Release(pClient);
			}
		}
	}
#endif 
	//////////////////////////////////////////////////////////////////////////
	///                     CFNetClientT                                   ///
	//////////////////////////////////////////////////////////////////////////

	template< typename T >
	CFNetClientT<T>::CFNetClientT(FSocketType st)
	{
		m_pClientSocket = NULL;
		m_socketType = st;
	}

	template< typename T >
	CFNetClientT<T>::~CFNetClientT()
	{

	}

	template< typename T >
	int CFNetClientT<T>::Create()
	{
		int rc = NO_ERROR;
		m_pClientSocket = new CFSocket(m_socketType);
		m_pClientSocket->Open();
	}

	template< typename T >
	int CFNetClientT<T>::Destroy()
	{

	}

	template< typename T >
	int CFNetClientT<T>::Connect(LPCTSTR pszAddr)
	{
		int rc = NO_ERROR;
		SOCKADDR_IN conAddr = {0};
		conAddr.sin_family = AF_INET;
		conAddr.sin_port = 0;
		conAddr.sin_addr.S_un.S_addr = 0; 

		//WSAECONNREFUSED -- 想连接的计算机没有监听指定端口
		//WSAETIMEDOUT -- 
		NET_VERIFY(WSAConnect(m_pClientSocket->m_socket,SOCKADDR*(&conAddr), sizeof(conAddr),NULL,NULL,NULL,NULL));
		return rc;
	}


	//////////////////////////////////////////////////////////////////////////
	///                     CFNetServerT                                   ///
	//////////////////////////////////////////////////////////////////////////

	template< typename T >
	DWORD CFNetServerT<T>::getNumberOfConcurrentThreads()
	{
		//用于 CreateIoCompletionPort 的参数和创建线程。
		//0表示系统内安装了多少个处理器，便允许同时运行多少个线程操作IO完成端口,但通常返回 CPU个数 * 2 + 2;
		SYSTEM_INFO sysInfo = {0};
		GetSystemInfo(&sysInfo);

#pragma TODO(超线程或多核时获取的线程个数)
		DWORD numberOfConcurrentThreads = sysInfo.dwNumberOfProcessors * 2 + 2;
		return numberOfConcurrentThreads;
	}

	template< typename T >
	unsigned int CFNetServerT<T>::serverThreadProc( LPVOID lpThreadParameter )
	{
		FUNCTION_BLOCK_TRACE(DEFAULT_BLOCK_TRACE_THRESHOLD);
		unsigned int ret = 0;
		CFNetServerT<T>* pThis = static_cast< CFNetServerT<T>* >(lpThreadParameter);
		ret = pThis->doServerLoop();
		return ret;
	}

	template< typename T >
	unsigned int CFNetServerT<T>::doServerLoop()
	{
		FUNCTION_BLOCK_TRACE(DEFAULT_BLOCK_TRACE_THRESHOLD);
		//int rc = 0;

		//将Accept的客户端Socket绑定到IO完成端口，返回的将是之前创建的IO端口，而不创建新的
		//SOCKET acceptClient = 0;

		for (;;)
		{
			CFSocket* pClient = m_pServerSocket->Accept();
			if (pClient)
			{
				//HANDLE hbindIoPort = CreateIoCompletionPort((HANDLE)(pClient->m_socket),m_hIoCompletionPort,
				//	(ULONG_PTR)(pClient), 0);
				//FTLASSERT( hbindIoPort == m_hIoCompletionPort);
				//::PostQueuedCompletionStatus(m_hIoCompletionPort,1,pClient,)
			}
			//FOVERLAPPED*     pKey = new FOVERLAPPED();
			//pKey->socket = acceptClient;
			//pKey->overLapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

			//(m_pServerSocket->Accept())
			//break;
		}
		return 0;
	}

	template< typename T >
	unsigned int CFNetServerT<T>::workerThreadProc( LPVOID lpThreadParameter )
	{
		unsigned int ret = 0;
		CFNetServerT<T>* pThis = static_cast< CFNetServerT<T>* >(lpThreadParameter);
		ret = pThis->doWorkerLoop();
		return ret;
	}

	template< typename T >
	unsigned int CFNetServerT<T>::doWorkerLoop()
	{
		DWORD dwNumberOfBytesTransferred = 0;
		CFSocket* pClientSocket = NULL;//用于获得第二次CreateIoCompletionPort时传入的参数
		LPFOVERLAPPED    pOverLapped = NULL;
		BOOL bRet = FALSE;
		for (;;)
		{
			//分三种情况(注意对 PostQueuedCompletionStatus 的调用也会唤醒，需要判断并处理)：
			//1.成功取出一个Completion Packet，返回TRUE，并填写各个参数;
			//2.返回FALSE，但取出Completion Packet，pOverLapped指向失败的操作，具体GetLastError
			//3.返回FALSE，并且没有取出 Completion Packet, pOverLapped 为NULL
			API_VERIFY(GetQueuedCompletionStatus(m_hIoCompletionPort,&dwNumberOfBytesTransferred,
				(PULONG_PTR)&pClientSocket, (LPOVERLAPPED*)(&pOverLapped), INFINITE));

			if (FALSE == bRet)
			{
				if (NULL == pOverLapped)
				{
					break;
				}
				else
				{
					//Happend instead of end-of-file
				}
			}
			else if(0 == dwNumberOfBytesTransferred)
			{
				//socket close
			}
			else
			{
				//FTLTRACEEX(FTL::tlInfo, TEXT("ClientSocket[%d,Session=%d],OverLapped[mode=%d,len=%d,dataBuf=%s]\n"),
				//    pClientSocket->m_socket,pClientSocket->m_nSession, 
				//    pOverLapped->socketMode,pOverLapped->dataBuf.len, (TCHAR*)(pOverLapped->dataBuf.buf));
				//Got a valid data block
			}
		}

		return 0;
	}

	template< typename T >
	CFNetServerT<T>::CFNetServerT(FSocketType st)
	{
		m_socketType = st;
		m_pServerSocket = NULL;
		m_hIoCompletionPort = NULL;
		//m_pIoServerThreadPool = NULL;
		//m_pServerThread = NULL;
	}

	template< typename T >
	CFNetServerT<T>::~CFNetServerT()
	{
		Destroy();
	}

	template< typename T >
	int CFNetServerT<T>::Create(USHORT listenPort, LPCTSTR pszBindAddr /* = NULL */)
	{
		FTLASSERT(NULL == m_pServerSocket);
		//if (NULL != m_pServerSocket)
		//{
		//    Destroy(); 
		//}
		int rc = NO_ERROR;
		//BOOL bRet = FALSE;
		m_pServerSocket = new CFServerSocket();
		NET_VERIFY(m_pServerSocket->Open(m_socketType, TRUE));

		do 
		{
			//int nZero = 0;
			//Winsock便会在重叠I/O调用中直接使用应用程序的缓冲，避免拷贝
			//setsockopt(m_pServerSocket->m_socket,SOL_SOCKET, SO_SNDBUF,(char*)&nZero, sizeof(nZero));

			if (stTCP == m_socketType)
			{
				NET_VERIFY(m_pServerSocket->Bind(listenPort,pszBindAddr));
				if (NO_ERROR != rc)
				{
					break;
				}
			}

			//2.使用线程池创建为IO完成端口服务的工作者线程
			//m_pIoServerThreadPool = new CFThreadPool<DWORD>(0,0);

			//3.将Accept后的套接字句柄同完成端口关联到一起
			//CreateIoCompletionPort((HANLDE)acceptSocket,m_hIoCompletionPort,(ULONG_PTR)this,0);


			//4.使用Overlapped方式进行IO处理，并依赖于完成端口，进行异步
			//工作者线程中在完成端口上等待 GetQueuedCompletionStatus(m_hIoCompletionPort,)

			//m_pServerThread = new CFThread<DefaultThreadTraits>();
			//FTLASSERT(m_pServerThread);

			//if (m_pServerThread)
			//{
			//    m_pServerThread->Start((LPTHREAD_START_ROUTINE)(&CFNetServerT<T>::ServerThreadProc), this, TRUE);
			//}
		} while (FALSE);

		if (NO_ERROR != rc)
		{
			Destroy();
		}
		return rc;
	}

	template< typename T >
	int CFNetServerT<T>::Destroy()
	{
		int rc = NO_ERROR;
		BOOL bRet = FALSE;
		//在完成端口上， 向每个工作者线程都发送一个特殊的完成数据包,终止所有工作者线程的运行
		//PostQueuedCompletionStatus(m_hIoCompletionPort)
		SAFE_CLOSE_HANDLE(m_hIoCompletionPort, NULL);

		//if (m_pServerThread)
		//{
		//    m_pServerThread->StopAndWait(INFINITE,TRUE);
		//    SAFE_DELETE(m_pServerThread);
		//}
		if (m_pServerSocket)
		{
			NET_VERIFY(m_pServerSocket->Close());
			SAFE_DELETE(m_pServerSocket);
		}
		return rc;
	}


	template< typename T >
	int CFNetServerT<T>::Start(INT backlog, INT nMaxClients)
	{
		int rc = NO_ERROR;
		BOOL bRet = FALSE;



		DWORD dwNumberOfConcurrentThreads = getNumberOfConcurrentThreads();
		FTLASSERT(dwNumberOfConcurrentThreads > 0);

		//1.创建IO完成端口
		m_hIoCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 
			NULL, 
			0, 
			dwNumberOfConcurrentThreads
			);
		API_VERIFY(NULL != m_hIoCompletionPort);

		m_pWorkerThreads = new HANDLE[dwNumberOfConcurrentThreads];
		ZeroMemory(m_pWorkerThreads, sizeof(HANDLE) * dwNumberOfConcurrentThreads );
		unsigned int threadId = 0;

		for (DWORD threadIndex = 0; threadIndex < dwNumberOfConcurrentThreads; threadIndex++)
		{
			m_pWorkerThreads[threadIndex] = (HANDLE)_beginthreadex(NULL, 0, workerThreadProc, this, 0, &threadId);
		}

		m_hServerThread = (HANDLE) _beginthreadex(NULL,0,serverThreadProc, this, 0, &threadId);

		NET_VERIFY(m_pServerSocket->StartListen(backlog,nMaxClients));
		return rc;
	}

	template< typename T >
	int CFNetServerT<T>::Stop()
	{
		int rc = NO_ERROR;
		BOOL bRet = FALSE;
		if (m_pWorkerThreads)
		{
			DWORD dwNumberOfConcurrentThreads = getNumberOfConcurrentThreads();
			for (DWORD threadIndex = 0; threadIndex < dwNumberOfConcurrentThreads; threadIndex++)
			{
				SAFE_CLOSE_HANDLE(m_pWorkerThreads[threadIndex], NULL);
			}
			SAFE_DELETE_ARRAY(m_pWorkerThreads);
		}
		SAFE_CLOSE_HANDLE(m_hServerThread, NULL);
		return rc;
	}

#if 0
	size_t CFSocketUtils::readn(int fd, void* vptr, size_t n)
	{
		size_t nleft = n;
		size_t nread = 0;
		char * ptr = (char*)vptr;
		while(nleft > 0)
		{
			if((nread = read(fd,ptr,nleft)) < 0)
			{
				if(EINTR == errno)
				{
					nread = 0; //call read again
				}
				else
				{
					return (size_t)(-1);
				}
			}
			else if(0 == nread)
			{
				break;   //EOF
			}
			nleft -= nread;
			ptr += nread;
		}
		return (n-nleft); 
	}

	size_t CFSocketUtils::writen(int fd, const void* vptr, size_t n)
	{
		size_t nleft = n;
		size_t nwritten = 0;
		const char* ptr = (const char*)vptr;
		while (nleft > 0)
		{
			if((nwritten = write(fd, ptr, nleft)) <= 0)
			{
				if(EINTR == errno)
				{
					nwritten = 0; //call write again-- reset了 ?
				}
				else
				{
					return (size_t)(-1);
				}
			}
			nleft -= nwritten;
			ptr += nwritten;
		} 
		return n;
	}
#endif 

    enum SockOptValueType
    {
        sovtInt,
        sovtBOOL,
        sovtDWORD,
        sovtSocketType,
        sovtLinger,
        sovtGroup,
        sovtWsaProtocolInfo,
    };
    struct SockOptInfo
    {
        int         optName;
        TCHAR       pszOptName[40];
        int         nOptLen;
        SockOptValueType    valType;
    };

    int CFSocketUtils::DumpSocketOption(SOCKET s)
    {
        int rc = NO_ERROR;
        FTL::CFMemAllocator<CHAR> optValueBuffer(1024);
        //CHAR optValueBuffer[1024] = {0};

        static SockOptInfo allOptions[] = 
        {
            { SO_TYPE,          _T("SO_TYPE"),          sizeof(int), sovtSocketType }, //SOCK_STREAM 等 

            { SO_DEBUG,         _T("SO_DEBUG"),         sizeof(BOOL), sovtBOOL },   //FALSE
            { SO_ACCEPTCONN,    _T("SO_ACCEPTCONN"),    sizeof(BOOL), sovtBOOL },   //FALSE
            { SO_REUSEADDR,     _T("SO_REUSEADDR"),     sizeof(BOOL), sovtBOOL },   //FALSE
            { SO_KEEPALIVE,     _T("SO_KEEPALIVE"),     sizeof(BOOL), sovtBOOL },   //FALSE
            { SO_DONTROUTE,     _T("SO_DONTROUTE"),     sizeof(BOOL), sovtBOOL },   //FALSE

            { SO_BROADCAST,     _T("SO_BROADCAST"),     sizeof(BOOL), sovtBOOL },   //Fail -- WSAENOPROTOOPT
            { SO_USELOOPBACK,   _T("SO_USELOOPBACK"),   sizeof(BOOL), sovtBOOL },   //Fail -- WSAENOPROTOOPT

            { SO_DONTLINGER,    _T("SO_DONTLINGER"),    sizeof(BOOL), sovtBOOL },
            { SO_LINGER,        _T("SO_LINGER"),        sizeof(LINGER), sovtLinger}, //onoff=0, linger=0
            
            { SO_OOBINLINE,     _T("SO_OOBINLINE"),     sizeof(BOOL), sovtBOOL },   //FALSE

            { SO_SNDBUF,        _T("SO_SNDBUF"),        sizeof(int), sovtInt },     //8192
            { SO_RCVBUF ,       _T("SO_RCVBUF "),       sizeof(int), sovtInt },     //8192
            { SO_SNDLOWAT,      _T("SO_SNDLOWAT"),      sizeof(int), sovtInt },     //Fail -- WSAENOPROTOOPT
            { SO_RCVLOWAT,      _T("SO_RCVLOWAT"),      sizeof(int), sovtInt },     //Fail -- WSAENOPROTOOPT
            { SO_SNDTIMEO,      _T("SO_SNDTIMEO"),      sizeof(int), sovtInt },     //0
            { SO_RCVTIMEO,      _T("SO_RCVTIMEO"),      sizeof(int), sovtInt },     //0
            { SO_ERROR,         _T("SO_ERROR"),         sizeof(int), sovtInt },     //0

            { SO_GROUP_ID,      _T("SO_GROUP_ID"), sizeof(GROUP), sovtGroup },      //0
            { SO_GROUP_PRIORITY, _T("SO_GROUP_PRIORITY"), sizeof(int), sovtInt },   //0
            { SO_MAX_MSG_SIZE,  _T("SO_MAX_MSG_SIZE"), sizeof(unsigned int), sovtInt }, //1073741823 (ERROR_UNHANDLED_ERROR 或 2^30-1)

            //szProtocol=MSAFD Tcpip [TCP/IP], iVersion=2, iAddressFamily=2
            { SO_PROTOCOL_INFO, _T("SO_PROTOCOL_INFO"), sizeof(WSAPROTOCOL_INFO), sovtWsaProtocolInfo},
            
            { SO_CONDITIONAL_ACCEPT, _T("SO_CONDITIONAL_ACCEPT"), sizeof(BOOL), sovtBOOL }, //FALSE

            { SO_MAXDG,         _T("SO_MAXDG"), sizeof(DWORD), sovtDWORD },         //1073741823(ERROR_UNHANDLED_ERROR?)
            { SO_MAXPATHDG,     _T("SO_MAXPATHDG"), sizeof(DWORD), sovtDWORD },     //1073741823(ERROR_UNHANDLED_ERROR?)
            { SO_UPDATE_ACCEPT_CONTEXT, _T("SO_UPDATE_ACCEPT_CONTEXT"), sizeof(DWORD), sovtDWORD }, //Fail -- WSAENOPROTOOPT
            { SO_CONNECT_TIME,  _T("SO_CONNECT_TIME"), sizeof(DWORD), sovtDWORD },  // -1
#if(_WIN32_WINNT >= 0x0501)
            { SO_UPDATE_CONNECT_CONTEXT, _T("SO_UPDATE_CONNECT_CONTEXT"), sizeof(DWORD), sovtDWORD }, //Fail -- WSAENOPROTOOPT
#endif //(_WIN32_WINNT >= 0x0501)

            //{ XXXX, _T("XXXXX"), sizeof(DWORD), sovtDWORD },

        };

        for (int i = 0; i < _countof(allOptions); i++)
        {
            FTLASSERT(allOptions[i].nOptLen <= optValueBuffer.GetCount());
            if (allOptions[i].nOptLen <= optValueBuffer.GetCount())
            {
                ZeroMemory(optValueBuffer, optValueBuffer.GetCount());

                NET_VERIFY_EXCEPT1(getsockopt(s, SOL_SOCKET, allOptions[i].optName, 
                    (CHAR*)optValueBuffer, &allOptions[i].nOptLen),
                    WSAENOPROTOOPT);
                if (NO_ERROR == rc)
                {
                    switch (allOptions[i].valType)
                    {
                    case sovtInt:
                        FTLTRACE(TEXT("%s (int) is %d\n"), allOptions[i].pszOptName, 
                            *((int*)optValueBuffer.GetMemory()));
                        break;
                    case sovtBOOL:
                        FTLTRACE(TEXT("%s (BOOL) is %s\n"), allOptions[i].pszOptName, 
                            *((BOOL*)optValueBuffer.GetMemory()) ? TEXT("TRUE") : TEXT("FALSE") );
                        break;
                    case sovtDWORD:
                        FTLTRACE(TEXT("%s (DWORD) is %d\n"), allOptions[i].pszOptName, 
                            *((DWORD*)optValueBuffer.GetMemory()));
                        break;
                    case sovtSocketType:
                        {
                            int nSocketType = *((int*)optValueBuffer.GetMemory());
                            FTLTRACE(TEXT("%s (SocketType) is %d(%s)\n"), allOptions[i].pszOptName, 
                                nSocketType, FNetInfo::GetSocketType(nSocketType));
                        }
                        break;
                    case sovtLinger:
                        {
                            LINGER* pLinger = (LINGER*)optValueBuffer.GetMemory();
                            FTLTRACE(TEXT("%s (LINGER), onoff=%d, linger=%d\n"), allOptions[i].pszOptName, 
                                pLinger->l_onoff, pLinger->l_linger);
                        }
                        break;
                    case sovtGroup:
                        {
                            GROUP nGroup = *(GROUP*)optValueBuffer.GetMemory();
                            FTLTRACE(TEXT("%s (Group) group=%d\n"), allOptions[i].pszOptName, 
                                nGroup);
                        }
                        break;
                    case sovtWsaProtocolInfo:
                        {
                            WSAPROTOCOL_INFO* pProtInfo = (WSAPROTOCOL_INFO*)optValueBuffer.GetMemory();
                            FTLTRACE(TEXT("%s (ProtocolInfo), szProtocol=%s, iVersion=%d, iAddressFamily=%d\n"), 
                                allOptions[i].pszOptName, 
                                pProtInfo->szProtocol, pProtInfo->iVersion, pProtInfo->iAddressFamily);
                        }
                        break;
                    default:
                        FTLASSERT(FALSE);
                        break;
                    }
                }
                else
                {
                    FTLTRACE(TEXT("getsockopt for %s Fail, reason is %s\n"),
                        allOptions[i].pszOptName, CFNetErrorInfo(WSAGetLastError()).GetConvertedInfo());
                }
            }
        }

        return NO_ERROR;
    }

}
#endif //FTL_SOCKET_HPP