/*░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
  ▩ File Name: audit.cpp
  ▩ Author: top-down
  ▩ Description: 
  ▩ Created Time: 2020年05月25日 星期一 13时44分32秒
  ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░*/

#include"WebRender.h"

void MySQLAuditSystem::InitSystem() {
	MY_MYSQL sqlmessage = {"192.168.47.138","root","123456","ASDB",3306,NULL,0};
	if(!connect_mysql(&mysql, sqlmessage)) {
		return;
	}
	const char *op = "select * from web_data_table";
	if(!operat_mysql(&mysql,op)) {
		return;
	}
	MYSQL_RES *res = mysql_store_result(&mysql);
	MYSQL_ROW row;
	while((row = mysql_fetch_row(res))) {
		struct OpMessage op_message;
		strcpy(op_message.op_time,row[0]);
		strcpy(op_message.op_addr,row[1]);
		strcpy(op_message.op_user,row[2]);
		strcpy(op_message.op_type,row[3]);
		strcpy(op_message.op_desc,row[4]);
		strcpy(op_message.op_deta,row[5]);
		string flag(row[0]);
		flag += row[1];
		_capture_message.push_back(op_message);
		if(_hash_message[flag] == 0) {
			_hash_message[flag] = 1;
		}
	}
}

void MySQLAuditSystem::GetMessage() {
	const char *op = "select * from web_data_table where date(op_time) = curdate()";
	if(!operat_mysql(&mysql,op)) {
		return;
	}
	MYSQL_RES *res = mysql_store_result(&mysql);
	MYSQL_ROW row;
	while((row = mysql_fetch_row(res))) {
		struct OpMessage op_message;
		strcpy(op_message.op_time,row[0]);
		strcpy(op_message.op_addr,row[1]);
		strcpy(op_message.op_user,row[2]);
		strcpy(op_message.op_type,row[3]);
		strcpy(op_message.op_desc,row[4]);
		strcpy(op_message.op_deta,row[5]);
		string flag(row[0]);
		flag += row[1];
		if(_hash_message[flag] == 0) {
			_hash_message[flag] = 1;
			_capture_message.push_back(op_message);
		}
	}
}

void MySQLAuditSystem::HomePageRun(bool flag, string& body, string username) {
	ctemplate::TemplateDictionary dict("./home/homepage/homepage.html");
	//dict.ShowSection("TABLE1");
	dict.SetValue("username",username.c_str());
	if(flag) {
		dict.SetValue("system_btn","<div id=\"system_btn\"><form method=\"get\" action=\"/system\"><input type=\"image\" src=\"/homepage/image/system.png\" onmousemove=\"bigImg(this)\" onmouseout=\"normalImg(this)\" \
				alt=\"进图系统管理\" title=\"进入系统管理\"/></form></div>");
	} else {
		dict.SetValue("system_btn","");
	}
	ctemplate::Template* tpl;
	tpl = ctemplate::Template::GetTemplate("./home/homepage/homepage.html", ctemplate::DO_NOT_STRIP);
	tpl->Expand(&body, &dict);
	cout << "HTML FINISHED" << endl;
}

void MySQLAuditSystem::SystemPageRun(string& start_time, string& end_time, string& start_addr,\
string& end_addr, string& week, string& body){
	ctemplate::TemplateDictionary dict("./home/system/system.html");
	dict.SetValue("start_time",start_time.c_str());
	dict.SetValue("end_time",end_time.c_str());
	dict.SetValue("start_addr",start_addr.c_str());
	dict.SetValue("end_addr",end_addr.c_str());
	dict.SetValue("week",week.c_str());
	ctemplate::Template* tpl;
	tpl = ctemplate::Template::GetTemplate("./home/system/system.html", ctemplate::DO_NOT_STRIP);
	tpl->Expand(&body, &dict);
}


void MySQLAuditSystem::NormalRun(string& body, string username) {

	GetMessage();
	ctemplate::TemplateDictionary dict("./home/display/display.html");
	dict.ShowSection("TABLE1");
	dict.SetValue("username",username.c_str());
	for(size_t i = 0; i < _capture_message.size(); ++i)
	{
		ctemplate::TemplateDictionary* table1_dict = dict.AddSectionDictionary("TABLE_BADY");
		table1_dict->SetFormattedValue("id", "%d", i);
		table1_dict->SetValue("op_time", _capture_message[i].op_time);
		table1_dict->SetValue("op_src",_capture_message[i].op_addr);
		table1_dict->SetValue("op_user",_capture_message[i].op_user);
		table1_dict->SetValue("op_type",_capture_message[i].op_type);
		table1_dict->SetValue("op_desc",_capture_message[i].op_desc);
		table1_dict->SetValue("op_deta",_capture_message[i].op_deta);
	}
	ctemplate::Template* tpl;
	tpl = ctemplate::Template::GetTemplate("./home/display/display.html", ctemplate::DO_NOT_STRIP);
	tpl->Expand(&body, &dict);
	cout << "HTML FINISHED" << endl;
}
void MySQLAuditSystem::VIPRun(string& body, string username) {

	GetMessage();
	ctemplate::TemplateDictionary dict("./root/vip_index");
	dict.ShowSection("TABLE1");
	dict.SetValue("username",username.c_str());
	for(size_t i = 0; i < _capture_message.size(); ++i)
	{
		ctemplate::TemplateDictionary* table1_dict = dict.AddSectionDictionary("TABLE_BADY");
		table1_dict->SetFormattedValue("id", "%d", i);
		table1_dict->SetValue("op_time", _capture_message[i].op_time);
		table1_dict->SetValue("op_src",_capture_message[i].op_addr);
		table1_dict->SetValue("op_user",_capture_message[i].op_user);
		table1_dict->SetValue("op_type",_capture_message[i].op_type);
		table1_dict->SetValue("op_desc",_capture_message[i].op_desc);
		table1_dict->SetValue("op_deta",_capture_message[i].op_deta);
	}
	ctemplate::Template* tpl;
	tpl = ctemplate::Template::GetTemplate("./root/vip_index.html", ctemplate::DO_NOT_STRIP);
	tpl->Expand(&body, &dict);
	cout << "HTML FINISHED" << endl;
}
