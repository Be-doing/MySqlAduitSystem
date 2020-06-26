/*░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
  ▩ File Name: test.c
  ▩ Author: top-down
  ▩ Description: 
  ▩ Created Time: 2020年04月15日 星期三 14时28分43秒
  ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░*/

#include <stdio.h>
#include <pcap.h>
int main(int argc,char *argv[]){
	char *dev, errbuf[PCAP_ERRBUF_SIZE];
	dev=pcap_lookupdev(errbuf);
	if(dev==NULL){
		fprintf(stderr,"couldn't find default device: %s\n",errbuf);
		return(2);
	}
	printf("Device: %s\n",dev);
	return(0);
}
