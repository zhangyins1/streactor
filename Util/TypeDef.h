#ifndef __TYPE_DEF_H_H__
#define __TYPE_DEF_H_H__

#define INVALID_FD -1
#define INVALID_PORT 0
#define INVALID_INDEX -1
#define MAX_ADDR_STR_SIZE 16  // byte
#define LISTEN_BACKLOG 511
#define GAME_SERVER_IP "172.16.1.129"
#define GAME_SERVER_PORT 8600
#define TCP_CLIENT_CONNECT_RETRY_CNT 3
#ifdef _WIN32
//#define ST_WIN32
#endif

#define CODE_LINE __LINE__

typedef signed char        int8_t;
typedef short              int16_t;
typedef int                int32_t;
typedef long int           int64_t;
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long int  uint64_t;
typedef unsigned char	   byte_t;
typedef int				   msgid_t;
typedef int				   st_socket_t;
typedef unsigned short	   st_port_t;
typedef unsigned int	   st_sessionid_t;

enum class FDTYPES
{
	FD_TYPE_ERROR = 0,
	FD_TYPE_ACCEPT,
	FD_TYPE_SERVER,
	FD_TYPE_CLINET,
};

enum
{
	MAX_FILE_HANDLE_COUNT = 1024,
	MAX_EPOLL_SIZE = 9999,
	EPOLL_WAIT_TIMEOUT_MS = 50,
	CLIENT_TIMEOUT = 20,
	OTHERSERVER_TIMEOUT = 5,
};

enum T_WS_HEAD_TYPE
{
	WS_BEFORE_HEAD_LEN = 2,				// �ʼ��2���ֽ����жϰ�ͷ�ĳ���
};

enum T_WS_OPCODE_TYPE
{
	WS_OPCODE_NONE = -100,                         //�հ�
	WS_OPCODE_DATA_END = -2,                           //websocket֡����֡��text��bin��������
	WS_OPCODE_GET = -1,                           //websocket��һ��get��
	WS_OPCODE_DATA_NEXT = 0,                            //websocket��һ֡�ĺ����֡��Ҳ���Ƿ�Ƭ�ĵ�1�����ϵ�֡
	WS_OPCODE_DATA_FIRST = 1,                            //websocket�İ��ĵ�һ֡
	WS_OPCODE_CLOSE = 0x8,                          //websocket�ر�����֡��ֻ��1֡
};

enum EStReturnValue
{
	ST_ERR = -1,
	ST_OK = 0,
};

#endif // !__TYPE_DEF_H_H__
