/*░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
▩ File Name: policyrules.c
▩ Author: top-down
▩ Description: 
▩ Created Time: 2020年05月28日 星期四 20时22分34秒
░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░*/

#include"policyrules.h"
#define POLICYRULES_PATH "/home/top-down/workspace/MySqlAduitSystem/code/new_parse_dir/policyrules.config"
#define FLAG "start\n"

const char flag = '~';

void splitstring(char* buff, char *left, char *right) {
	if(buff == NULL) {
		return;
	}
	while((*buff) != flag) {
		*left = *buff;
		++buff;
		++left;
	}
	++buff;
	while((*buff) != '\n') {
		*right = *buff;
		++right;
		++buff;
	}
}

void rules_init(struct TimeRules *time_rules, struct AddrRules *addr_rules) {
	FILE *fd = fopen(POLICYRULES_PATH,"r");
	if(fd == NULL) {
		printf("open policyrules.config faild\n");
		fclose(fd);
		return;
	}
	char buff[128] = {0};
	while(fgets(buff,128,fd)) {
		if(strcmp(buff,FLAG) == 0) {
			break;
		}
	}
	memset(buff,128,'0');
	int line = 0;
	while(fgets(buff,128,fd)) {
		if(strcmp(buff,"NULL") != 0) {
			if(line == 0) {
				splitstring(buff,time_rules->begin_date,time_rules->end_date);
			} else if (line == 1) {
				splitstring(buff,time_rules->begin_time, time_rules->end_time);
			} else if(line == 2) {
				strcpy(time_rules->week, buff);
			} else {
				splitstring(buff, addr_rules->begin_addr, addr_rules->end_addr);
			}
		}
		++line;
		memset(buff,128,'0');
	}
	printf("rules_init successful\n");
	fclose(fd);
}

