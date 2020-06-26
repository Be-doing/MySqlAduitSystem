/*░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
▩ File Name: policyrules.h
▩ Author: top-down
▩ Description: 
▩ Created Time: 2020年05月28日 星期四 20时01分17秒
░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░*/

#include<stdio.h>
#include<string.h>
struct TimeRules {
	char *begin_date;
	char *end_date;
	char *begin_time;
	char *end_time;
	char *week;

};

struct AddrRules {
	char *begin_addr;
	char *end_addr;
};
void rules_init(struct TimeRules *time_rules, struct AddrRules *addr_rules);
