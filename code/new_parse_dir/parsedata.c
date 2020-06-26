/*░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
  ▩ File Name: test1.c
  ▩ Author: top-down
  ▩ Description:数据抓取和处理文件
  ▩ Created Time: 2020年04月15日 星期三 14时56分10秒
  ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░*/
#include "parsedata.h"



MYSQL mysql; //用户查询哈希表
//策略规则
struct TimeRules times;
struct AddrRules addrs;
void init_policy_rules() {
	times.begin_date = (char *)calloc(128,1);
	times.end_date = (char *)calloc(128,1);
	times.begin_time = (char *)calloc(128,1);
	times.end_time = (char *)calloc(128,1);
	times.week = (char *)calloc(128,1);
	addrs.begin_addr = (char *)calloc(128,1);
	addrs.end_addr = (char *)calloc(128,1);
	rules_init(&times, &addrs);
}

void get_cut_time(char *op_time) {
	time_t timep;
	time(&timep);
	strftime(op_time, 32, "%Y-%m-%d %H:%M:%S",localtime(&timep));
}

void data_parse(unsigned char content[] ,char *source_addr) {
	
	char message_flag = content[3];
	MYSQL_RES *res;
	MYSQL_ROW row;
	char op_buff[128] = {0};
	strcat(op_buff,"select op_obj from user_hash_table where op_addr = \'");
	strcat(op_buff,source_addr);
	strcat(op_buff,"\'");
	if(!operat_mysql(&mysql,op_buff)) {
		return;
	}
	res = mysql_store_result(&mysql);
	row = mysql_fetch_row(res);
	if(message_flag == 1) {//登录报文
		int index = 36;//第36字节开始用户名
		int start = 0;
		char user[128] = {0};
		while(content[index]) {
			user[start] = content[index];
			++index;
			++start;
		}
		if(row == NULL) {
			printf("%s is new user\n",source_addr);
			char val[128] = {0};
			strcat(val,"insert into user_hash_table values(\'");
			strcat(val,source_addr);
			strcat(val,"\',\'");
			strcat(val,user);
			strcat(val,"\')");
			if(!operat_mysql(&mysql,val)) {
				return;
			}
		}
	} else if (message_flag == 0) {//命令报文
		struct command_message commandmessage;
		
		char op_time[32] = {0};
		get_cut_time(op_time);

		int packet_size = (int)(content[0] + content[1]*100 + content[2]*10000);
		packet_size -= 1;//命令类型占一个字节
		commandmessage.command_type = COMMAND_TYPE[content[4]];
		commandmessage.command_des = COMMAND_DES[content[4]];
		commandmessage.command_content = (char*)calloc(128,1);
		memcpy(commandmessage.command_content, content+5, packet_size);
		commandmessage.sourceaddr = source_addr;
		commandmessage.user = row[0];
		char val[255] = {0};
		strcat(val,"insert into web_data_table values(\'");
		strcat(val,op_time);
		strcat(val,"\',\'");
		strcat(val,commandmessage.sourceaddr);
		strcat(val,"\',\'");
		strcat(val,commandmessage.user);
		strcat(val,"\',\'");
		strcat(val,commandmessage.command_type);
		strcat(val,"\',\'");
		strcat(val,commandmessage.command_des);
		strcat(val,"\',\'");
		strcat(val,commandmessage.command_content);
		strcat(val,"\')");
		if(!operat_mysql(&mysql,val)) {
			return;
		}
	}
}


void tcp_callback_func(struct tcp_stream *stream_message, void **arg) {

	char policy_time[32] = {0};
	char policy_date[32] = {0};
	char policy_week[32] = {0};
	time_t timep;
	time(&timep);
	strftime(policy_time, 32,"%H:%M:%S",localtime(&timep));
	strftime(policy_week, 32,"%A",localtime(&timep));
	strftime(policy_date, 32,"%m-%d",localtime(&timep));
	
	if(strcmp(policy_time,times.begin_time) < 0 || strcmp(policy_time,times.end_time) > 0) {
		printf("不符合时间规则\n");
	} else if(strcmp(policy_date,times.begin_date) < 0 || strcmp(policy_date,times.end_date) > 0) {
		printf("不符合日期规则\n");
	} else if(strcmp(policy_week, times.week) == 0) {
		printf("不符合星期规则\n");
	} else {

		int index = 0;
		char source_addr[32] = {0};//源地址信息
		char all_addr[1024] = {0};//总地址信息
		unsigned char content[65535] = {0};

		struct tuple4 ip_and_port = stream_message->addr;//获取TCP连接的地址信息
		strcpy(all_addr, inet_ntoa(*((struct in_addr*) &(ip_and_port.saddr))));
		if(strcmp(all_addr, addrs.begin_addr) < 0 || strcmp(all_addr, addrs.end_addr) > 0) {
			printf("不符合地址规则\n");
			return;
		}
		sprintf(all_addr + strlen(all_addr), ":%i", ip_and_port.source);
		memcpy(source_addr,all_addr,strlen(all_addr));
		strcat(all_addr, " <---> ");
		strcat(all_addr, inet_ntoa(*((struct in_addr*) &(ip_and_port.daddr))));
		sprintf(all_addr + strlen(all_addr), ":%i", ip_and_port.dest);
		strcat(all_addr, "\n");
		switch (stream_message->nids_state) {// 判断LIBNIDS的状态 
			case NIDS_JUST_EST:// 表示TCP客户端和TCP服务器端建立连接状态 
				stream_message->client.collect++;
				stream_message->server.collect++;
				stream_message->server.collect_urg++;
				stream_message->client.collect_urg++;
				printf("%s完成TCP连接建立\n", all_addr);
				return ;
			case NIDS_CLOSE:
				printf("--------------------------------\n");
				printf("%sTCP连接正常关闭\n", all_addr);
				return ;
			case NIDS_DATA:
				{
					struct half_stream *hlf;
					if(stream_message->server.count_new)
					{
						hlf = &stream_message->server;
						memcpy(content, hlf->data, hlf->count_new);
						content[hlf->count_new] = '\0';
						data_parse(content ,source_addr);
					}
				}
			default:
				break;
		}
		return ;
	}
}
int main()
{
	MY_MYSQL sqlmessage = {"192.168.47.138","root","123456","ASDB",3306,NULL,0};
	if(!connect_mysql(&mysql, sqlmessage)) {
		return 1;
	}
	init_policy_rules();


	nids_params.device="ens33";
	nids_params.pcap_filter="port 3306";
	struct nids_chksum_ctl temp;
	temp.netaddr = 0;
	temp.mask = 0;
	temp.action = 1;
	nids_register_chksum_ctl(&temp,1);  
	/*这段是相关与计算校验和的，比较新的网卡驱动会自动计算校验和，我们要做的就是把它关掉*/
	if (!nids_init())
		/* Libnids初始化 */
	{
		printf("出现错误：%s\n", nids_errbuf);
		return 1;
	}
	nids_register_tcp(tcp_callback_func);
	/* 注册回调函数 */
	nids_run();
	/* Libnids进入循环捕获数据包状态 */
	return 0;
}
