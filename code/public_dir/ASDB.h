/*░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
▩ File Name: ASDB.h
▩ Author: top-down
▩ Description: 
▩ Created Time: 2020年06月17日 星期三 16时37分26秒
░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░*/

#include<stdio.h>
#include<mysql/mysql.h>
typedef struct MY_MYSQL {
	const char *host;
	const char *user;
	const char *passwd;
	const char *dbname;
	unsigned int port;
	const char *unix_socket;
	unsigned long clientflag;
}MY_MYSQL;
int connect_mysql(MYSQL *mysql, MY_MYSQL sqlmessqge);
int operat_mysql(MYSQL *mysql, const char *operat);
void close_mysql(MYSQL *mysql);
