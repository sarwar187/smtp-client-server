#include"mail_client.h"

char mail[1000];
char buf[100],str[100],buf1[200];
  char command[6][50] = {
		{"helo yahoo.com"},
		{"mail from:sarwar187@yahoo.com"},
		{"rcpt to:"},
		{"data"},
		{"quit"}
	};
	char from[2000] = {"\nFrom:sarwar187@yahoo.com\n"};
	char to[200] = {"To:"},new[50];
	char date[40] = {"Date:"};	
	char extra[2],port[10] = {"25"},sub1[100]= {"subject:"},sub2[100];
		
prepare(char *p){
	FILE *fp;
	fp = fopen(p,"r");
	if(fp == NULL){
		printf("Incorrect File\n");
		exit(0);
	}
	int i =0;
	char c;
	while(1){	
		c = getc(fp);
		if(c==EOF)
			break;
		mail[i] = c;
		i++;		
	}
	mail[i++] = '.';	
	mail[i++] = '\n';
	mail[i] = '\0';
	fclose(fp);
}


likhi(char *p){
	int i = 0;
	while(p[i]!='\n'){
		printf("%c",p[i]);
		i++;
	}
	printf("\n");
}



int main(argc, argv)
int argc;
char *argv[];
{
	int sc,index1,index2,length;
	time_t t;
	char *time;
	struct sockaddr_in server;
	struct hostent *hp;
	extra[0] = 13;
	extra[1] = 10;
	index1 = 0;
	index2 = 0;
	time = (char*)ctime(t);
	strcat(date,time);
	if(argc!=4){
		printf("Incorrect number of arguments\n");
		return 0;
	}
	while(argv[2][index1]){
		if(argv[2][index1] == 34)
			continue;
		sub2[index2] = argv[2][index1];
		index1++;
		index2++;
	}		
	sub2[index2] = '\n';
	sub2[index2+1] = '\0';
	strcat(sub1,sub2);
	sc=socket(AF_INET,SOCK_STREAM,0);
	if (sc<0){
		perror("opening virtual circuit socket");
		exit(-1);	
	};
	int temp = 0;
	while(1){
		if(argv[1][temp] == '@')
			break;
		to[temp+3] = argv[1][temp];
		new[temp] = argv[1][temp];
		temp++;
	}
	to[temp+3] = '@';
	new[temp] = '@';
	temp++;
	to[temp+3] = '\0';
	int temp1 = 0,flag = 0;
	while(1){
		if(!argv[1][temp])
			break;
		if(argv[1][temp] == ':')
		{
			flag = 1;
			break;
		}
		str[temp1] = argv[1][temp];
		new[temp] = argv[1][temp];
		temp1++;
		temp++;
	}
	new[temp] = '\0';
	index1 = 0;
	temp++;
	if(flag == 1){
		while(argv[1][temp]){
			port[index1] = argv[1][temp];
			index1++;
			temp++;
		}
	}
	else{
		port[0] = '2';
		port[1] = '5';
		port[2] = '\0';
	}
	str[temp1] = '\0';
	strcat(to,str);
	strcat(command[2],new);
	length = strlen(to);
	to[length] = '\n';
	to[length+1] = '\0';
	length = strlen(date);
	date[length] = '\n';
	date[length+1] = '\0';
	
	prepare(argv[3]);
	strcat(from,to);
	strcat(from,sub1);
	strcat(from,date);	
	strcat(from,mail);
	server.sin_family=AF_INET;
	if(flag==0){
		if((hp=gethostbyname("mail.cse.univdhaka.edu"))==NULL){
			close(sc);
			fprintf(stderr,"canâ€™t find host %s\n");
			exit(-1);
		};
	}
	else{
		if((hp=gethostbyname(str))==NULL){
			close(sc);
			fprintf(stderr,"canâ€™t find host %s\n");
			exit(-1);
		};
	}
	bcopy(hp->h_addr,&(server.sin_addr.s_addr),hp->h_length);
	server.sin_port=htons(atoi(port));
	if(connect(sc, (struct sockaddr *) &server,sizeof(server))<0){
		close(sc);
		perror("connecting stream socket");
		exit(0);
	};
	recv(sc,buf1, sizeof(buf1),0);
	if(buf1[0] != '2'){
		likhi(buf1);
		close(sc);
		exit(0);
	}
	int i = 0;
	for(i=0;i<4;i++){
		if(send(sc,command[i],sizeof(command[i]),0)<0)
			perror("sending message");
		if(send(sc,extra,sizeof(extra),0)<0)
			perror("sending message");
		recv(sc,buf1, sizeof(buf1),0);
		if((buf1[0]!='2')&&(buf1[0]!='3')){
			likhi(buf1);
			close(sc);
			exit(0);
		}
	}
		
	if(send(sc,from,strlen(from),0)<0)
		perror("sending message");
		
	recv(sc,buf1, sizeof(buf1),0);
	
	if(send(sc,command[4],strlen(command[4]),0)<0)
		perror("sending message");

	if(send(sc,extra,sizeof(extra),0)<0)
			perror("sending message");
	
	recv(sc,buf1, sizeof(buf1),0);
	if(buf1[0] != '2'){
		likhi(buf1);
		close(sc);
		exit(0);
	}
	
	close(sc);
	exit(0);
}

