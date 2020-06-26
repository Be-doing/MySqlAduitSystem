/*░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
▩ File Name: test.cpp
▩ Author: top-down
▩ Description: 
▩ Created Time: 2020年06月17日 星期三 17时17分56秒
░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░*/

#include<iostream>
#include"ASDB.h"
using namespace std;
int main() {
	MYSQL mysql;
	MY_MYSQL sqlmessage = {
	"192.168.47.138","root","123456","ASDB",3306,NULL,0};
	if(connect_mysql(&mysql, sqlmessage)) {
		//const char *op = "insert into web_data_table values(\'2020-05-27 16:15:16\',\'192.168.47.137:46824\',\'gongbenwuzang\',\'COM_QUERY\',\'SQL查询请求\',\'select @@version_comment limit 1\')";
		//operat_mysql(&mysql,op);
		const char *op = "select op_obj from user_hash_table where op_addr = \'192.168.47.136:42422\'";
		operat_mysql(&mysql,op);
		MYSQL_RES *res;
		MYSQL_ROW row;
		res = mysql_store_result(&mysql);
		while((row = mysql_fetch_row(res))) {
			int i = mysql_num_fields(res);
			for(int j = 0; j < i; ++j) {
				printf("%s\t",row[j]);
			}
			printf("\n");
		}
		cout << "OK" << endl;
		close_mysql(&mysql);
	} else {
		cout << "NOT" <<endl;
	}
	return 0;
}
