/*░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
▩ File Name: ASDB.c
▩ Author: top-down
▩ Description: 
▩ Created Time: 2020年06月17日 星期三 17时08分42秒
░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░*/

#include"ASDB.h"

int connect_mysql(MYSQL *mysql, MY_MYSQL sqlmessqge) {
	if(mysql_init(mysql) == NULL) {
		printf("mysql init faild\n");
		return 0;
	}
	if(mysql_real_connect(mysql,sqlmessqge.host,sqlmessqge.user,sqlmessqge.passwd,sqlmessqge.dbname,
	sqlmessqge.port,sqlmessqge.unix_socket,sqlmessqge.clientflag) == NULL) {
		printf("mysql connect faild\n");
		return 0;
	}
	mysql_query(mysql,"SET NAMES UTF8");
	return 1;
}

int operat_mysql(MYSQL *mysql, const char *operat) {
	if(mysql_query(mysql,operat) == 1) {
		printf("%s operat faild\n",operat);
		return 0;
	}
	return 1;
}

void close_mysql(MYSQL *mysql) {
	mysql_close(mysql);
}
