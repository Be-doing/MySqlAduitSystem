/*░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
▩ File Name: server.cpp
▩ Author: top-down
▩ Description: 
▩ Created Time: 2019年08月20日 星期二 22时44分19秒
░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░*/

#include<vector>
#include"server.h"
#include"../code/new_parse_dir/OPProcess.hpp"
#define SYSTEM_ADDR "192.168.47.129:10000/login"

/*策略*/
string start_time,start_addr,end_time,end_addr,week;
void SpiltStrng(const string& src, string& str1, string& str2) {
	size_t pre = src.find('~',0);
	str1=src.substr(0,pre);
	str2=src.substr(pre+1);
}
void InitRuler(){
	ifstream file("../code/new_parse_dir/policyrules.config");
	if(file.is_open()) {
		string flag;
		bool ok = false;
		int line = 0;
		while(getline(file,flag)){
			if(ok) {
				if(line == 0) {
					SpiltStrng(flag,start_time,end_time);
				} else if (line == 1) {
					string left, right;
					SpiltStrng(flag,left,right);
					start_time += " ";
					start_time += left;
					end_time += " ";
					end_time += right;
				} else if(line == 2) {
					week = flag;
				} else {
					SpiltStrng(flag,start_addr, end_addr);
				}
				++line;
				continue;
			}
			if(strcmp(flag.c_str(),"start")==0){
				ok=true;
			}
		}
	}
	cout << "InitRuler successful" << endl;
	file.close();
}

/*规则*/
UserAction userOBJ;
OPProcess parseOBJ;
MySQLAuditSystem mas;
string result;
enum USER_TYPE UserAction::UserLogin(const string& user_message,const string& password) {
	string cmd = "select * from user_table where username = \'";
	cmd += user_message + "\'";
	cout << cmd << endl;
	if(!operat_mysql(&_mysql,cmd.c_str())) {
		return NO_BODY;
	}
	MYSQL_RES *res = mysql_store_result(&_mysql);
	MYSQL_ROW row = mysql_fetch_row(res);
	if(row == NULL) {
		return NO_BODY;
	}else if(strcmp(row[1],password.c_str()) != 0) {
		return ERROR_PWD;
	} else if(strcmp(row[2],SECRET_KEY) == 0) {
		return VIP_USER;
	} else {
		return NORMAL_USER;
	}
}

bool UserAction::UserRegistration(const hb::Request& req) {
	
	string username = req.get_param_value("username");
	string cmd = "select * from user_table where username = \'";
	cmd += username + "\'";
	if(!operat_mysql(&_mysql,cmd.c_str())) {
		return false;
	}
	MYSQL_RES *res = mysql_store_result(&_mysql);
	MYSQL_ROW row = mysql_fetch_row(res);
	if(row == NULL) {
		string insert_cmd("insert into user_table values('");
		insert_cmd += username + "\',\'";
		insert_cmd += req.get_param_value("password") + "\',\'";
		insert_cmd += req.get_param_value("secretkey") + "\')";
		if(!operat_mysql(&_mysql,insert_cmd.c_str())) {
			return false;
		}
		return true;
	}
	return false;
}
bool UserAction::UserListInit() {
	MY_MYSQL sqlmessage = {"192.168.47.138","root","123456","ASDB",3306,NULL,0};
	if(!connect_mysql(&_mysql, sqlmessage)) {
		return false;
	}
	return true;
}

bool UserAction::IsVIPUser(const string& user_message) {
	size_t message_size = user_message.size();
	if(message_size > 4 && strcmp(SECRET_KEY, user_message.substr(message_size-5).c_str()) == 0) {
		return true;
	}
	return false;
}



const char* BASE_PATH = "./home/";
void GetLoginPage(const hb::Request& req, hb::Response& res) {
	string body;
	string tmp;
	ifstream file("./home/login/login.html");
	while(getline(file,tmp)) {
		body += tmp +  "\n";
	}
	res.set_content(body,"text/html");
}

void GetRegistrationPage(const hb::Request& req, hb::Response& res) {
	string body;
	string tmp;
	ifstream file("./home/register/register.html");
	while(getline(file,tmp)) {
		body += tmp +  "\n";
	}
	res.set_content(body,"text/html");
}

void OPReset(const hb::Request& req, hb::Response& res) {
	cout << "OPReset" << endl;
	InitRuler();
	parseOBJ.RESET();
	cout << "????" << endl;
	GetLoginPage(req,res);
}

void OPOn(const hb::Request& req, hb::Response& res) {
	cout << "OPon" << endl;
	InitRuler();
	parseOBJ.START();
	GetLoginPage(req,res);
}

void OPOff(const hb::Request& req, hb::Response& res) {
	cout << "OPOff" << endl;
	parseOBJ.KILL();
	GetLoginPage(req,res);
}

void LoginAction(const hb::Request& req, hb::Response& res) {
	string username = req.get_param_value("username");
	string password = req.get_param_value("password");
	string result;
	enum USER_TYPE user_type = userOBJ.UserLogin(username,password);
	if(user_type == NORMAL_USER) {
		mas.HomePageRun(false,result,username);
		res.set_content(result,"text/html");
		cout << "NORMAL_USER" << endl;
	} else if (user_type == VIP_USER) {
		mas.HomePageRun(true,result,username);
		res.set_content(result,"text/html");
		cout << "VIP_USER" << endl;
	} else if(user_type == ERROR_PWD) {
		string str="<!DOCTYPE html>\
		<html>\
			<head>\
				<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">\
			</head>\
			<body>\
				<script type=\"text/javascript\">\
				alert(\"密码错误，登录失败！\");\
				window.location.href = \"/login\"\
				</script>\
			</body>\
		</html>";
		res.set_content(str,"text/html");
	} else {
		string str="<!DOCTYPE html>\
		<html>\
			<head>\
				<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">\
			</head>\
			<body>\
				<script type=\"text/javascript\">\
				alert(\"账号不存在，登录失败！您可以尝试注册~\");\
				window.location.href = \"/login\"\
				</script>\
			</body>\
		</html>";
		res.set_content(str,"text/html");
	}
}

void RegistrationAction(const hb::Request& req, hb::Response& res) {
	if(userOBJ.UserRegistration(req)) {
		GetLoginPage(req,res);
	} else {
		string str="<!DOCTYPE html>\
		<html>\
			<head>\
				<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">\
			</head>\
			<body>\
				<script type=\"text/javascript\">\
				alert(\"用户名重复，请重新注册！\");\
				 window.location.href = \"/register/register.html\"\
				</script>\
			</body>\
		</html>";
		res.set_content(str,"text/html");
	}
}
void GetTablePage(const hb::Request& req, hb::Response& res) {
	string body;
	mas.NormalRun(body,"user");
	 res.set_content(body,"text/html");
}


void GetSystemPage(const hb::Request& req, hb::Response& res) {
	string body;
	mas.SystemPageRun(start_time,end_time,start_addr,end_addr,week,body);
	res.set_content(body,"text/html");
}

void ChangeRuler (const hb::Request& req, hb::Response& res) {
	ifstream ifile("../code/new_parse_dir/policyrules.config");
	vector<string> tmpv;
	string str;
	while(getline(ifile,str)) {
		tmpv.push_back(str);
	}
	ifile.close();
	ofstream file("../code/new_parse_dir/policyrules.config");
	if(file.is_open()) {
		string time,date,addr,week;
		time += req.get_param_value("start_time");
		time += "~";
		time += req.get_param_value("end_time");
		week = req.get_param_value("week");
		date += req.get_param_value("start_date");
		date +="~";
		date += req.get_param_value("end_date");
		addr += req.get_param_value("start_addr");
		addr += "~";
		addr += req.get_param_value("end_addr");
		tmpv[11]=date;
		tmpv[12]=time;
		tmpv[13]=week;
		tmpv[14]=addr;
		for(const auto& e : tmpv) {
			file << e << endl;
		}
		file.close();
		GetSystemPage(req,res);
	}
}

int main()
{
	hb::Server srv;
	userOBJ.UserListInit();
	srv.set_base_dir(BASE_PATH);
	mas.InitSystem();
	InitRuler();
	cout << start_time << "----" << end_time << endl;
	cout << start_addr << "----" << end_addr << endl;
	cout << week << endl;
	srv.Get("/login",GetLoginPage);
	srv.Get("/op_reset",OPReset);
	srv.Get("/op_open",OPOn);
	srv.Get("/op_close",OPOff);
	srv.Get("/system",GetSystemPage);
	srv.Get("/display",GetTablePage);
	srv.Post("/go_homepage",LoginAction);
	srv.Post("/login",RegistrationAction);
	srv.Post("/system",ChangeRuler);
	srv.listen("192.168.47.129",10000);
	return 0;
}

