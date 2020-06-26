/*░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
▩ File Name: audit.h
▩ Author: top-down
▩ Description: 
▩ Created Time: 2020年05月25日 星期一 13时42分59秒
░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░*/

#include<iostream>
#include<string>
#include<string.h>
#include<unordered_map>
#include<vector>
#include<fstream>
#include <ctemplate/template.h>
#include"../code/public_dir/ASDB.h"
using namespace std;
struct OpMessage {//操作信息
       char op_time[32];
	   char op_addr[128];
	   char op_user[128];
	   char op_type[32];
	   char op_desc[128];
	   char op_deta[255];
};
class MySQLAuditSystem {

public:
	void InitSystem();
	void HomePageRun(bool flag, string& body, string username);
	void SystemPageRun(string& start_time, string& end_time, string& start_addr, \
	string& end_addr, string& week, string& body);
	void NormalRun(string& body,string username);
	void VIPRun(string& body, string username);
	private:
	void SplitString(string& str, struct OpMessage* op_message);
	void GetMessage();
private:
	MYSQL mysql;
	unordered_map<string,int> _hash_message;
	vector<OpMessage> _capture_message;
};
