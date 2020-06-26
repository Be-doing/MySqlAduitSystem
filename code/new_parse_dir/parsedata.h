/*░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
  ▩ File Name: test1.c
  ▩ Author: top-down
  ▩ Description:数据抓取和处理文件
  ▩ Created Time: 2020年04月15日 星期三 14时56分10秒
  ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░*/

/*
	header
*/
#include <stdio.h>

/* Libnids的头文件，必须包含 */
#include "nids.h"
#include "pcap.h"
#include "libnet.h"
#include <time.h>
#include "policyrules.h"
#include"../public_dir/ASDB.h"
/*
   ====================================================================================
   准备模块：建立与MySQL报文对应的数据结构,存储MySQL数据报分析结果
   ====================================================================================
*/
struct command_message {//命令信息
	const char *command_type;
	const char *command_des;
	char *command_content;
	char *sourceaddr;
	char *user;
};



const unsigned int MAX_USER_MESSAGE_SIZE = 1048;//一条映射信息的最大长度

const char *COMMAND_TYPE[] = {"0","COM_QUIT","COM_INIT_DB","COM_QUERY","COM_FIELD_LIST","COM_CREATE_DB",
"COM_DROP_DB","COM_REFRESH","COM_SHUTDOWN","COM_STATISTICS","COM_PROCESS_INFO"};

const char *COMMAND_DES[] = {"无操作","关闭连接","切换数据库","SQL查询请求","获取数据表字段信息","创建数据库",
"删除数据库","清除缓存","停止服务器","获取服务器统计信息","获取当前连接的列表"};
/*
   ====================================================================================
   工具
   ====================================================================================
*/
//获取当前时间
void get_cur_time(char *op_time);
/*
   ====================================================================================
   数据抓取与分析模块：利用libnids进行数据抓取,并对数据进行分析
   ====================================================================================
 */
//对抓取到的正文进行分析
void data_parse(unsigned char content[], char *source_addr);
//libnids回调函数，抓取数据
void tcp_callback_func(struct tcp_stream *stream_message, void **arg);
