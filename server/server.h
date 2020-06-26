/*░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
▩ File Name: server.h
▩ Author: top-down
▩ Description: 
▩ Created Time: 2020年05月26日 星期二 20时15分59秒
░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░*/

#include<iostream>                                                                                              
#include<fstream>
#include<string>
#include"httplib.h"
#include"WebRender.h"
#include<unordered_map>
using namespace std;
namespace hb = httplib;

#define USER_MESSAGE_FILE "./user_message.list"
#define SECRET_KEY "mysql"

enum USER_TYPE {
	NO_BODY,
	ERROR_PWD,
	NORMAL_USER,
	VIP_USER
};

struct UserMessage {
	char name[32] = {0};
	char passwd[32] = {0};
	char code[32] = {0};
};

struct AllUser {
	char name[32] = {0};
	char passwd[32] = {0};
};


class UserAction {
public:
	enum USER_TYPE UserLogin(const string& user_message, const string& password);
	bool UserRegistration(const hb::Request& req);
	bool UserListInit();
private:
	bool IsVIPUser(const string& user_message);
private:
	unordered_map<string, int> _user_list;
	MYSQL _mysql;
	unordered_map<string, int> _all_user_list;
	unordered_map<string, int> _vip_user_list;
};
