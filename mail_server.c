#include"mail_server.h"

int main(int argc, char *argv[])
{
  FILE *fp;
	int ss,s1;
	struct sockaddr_in name;
	char buf[1024],user[50],txt[6] = {".txt"};
	int cc;
	int length,error;
	struct sockaddr from;
	char valid[6][100] = {
		{"helo"},
		{"mail from"},
		{"rcpt to"},
		{"data"},
		{"quit"}
	};
	
	char command[10][100] = {
		{"220 welcome to sarwar's SMTP server\n"},
		{"250 helo pleased to meet you\n"},
		{"250 2.1.0 Sender Ok.....\n"},
		{"250 2.1.5 Recepient Ok....\n"},
		{"354 Enter mail, end with . on a line by itself\n"},
		{"221 2.0.0 closing connection\n"},
		{"250 2.0.0 Message accepted for delivery\n"},
		{"500 5.5.1 Command unrecognized:"},
		{"500 user not available\n"} 
	};
	ss=socket(AF_INET,SOCK_STREAM,0);
	if (ss<0) {
		perror("opening virtual circuit socket");
		exit(-1);
	 }
	name.sin_family=AF_INET;
	name.sin_addr.s_addr=INADDR_ANY;
	name.sin_port=0;
	if(bind(ss,&name,sizeof(name))){
		close(ss);
		perror("binding name to stream socket");
		exit(-1);
	}
	length=sizeof(name);
	if(getsockname(ss,&name,&length)){
		perror("getting socket name");
		exit(0);
	}
	printf("port#%d\n",ntohs(name.sin_port));
	while(1){
		error = 0;
		printf("â€¦waiting for new connectionâ€¦\n");
		strcpy(command[7],"500 5.5.1 Command unrecognized:");
		listen(ss,1);
		s1=accept(ss,0,0);
		if(send(s1,command[0],sizeof(command[0]),0)<0)
			perror("sending message");
		int i = 1,flag,j;
		char save[100];
		do{
			if((cc=recv(s1,buf,sizeof(buf),0))<0)
				perror("receiving packet");
			if(cc>0){
				buf[cc]='\0';
			}
			if((buf[0]==13)&&(buf[1]==10)){
				if(i==1){
					flag = 0;
					j = 0;
					while(valid[i-1][j]){
						if(save[j] != valid[i-1][j]){
							flag = 1;
							break;
						}
						j++;
					}
					if(flag == 1){
						strcat(command[7],save);
						if(send(s1,command[7],sizeof(command[7]),0)<0)
							perror("sending message");
						error = 1;
						break;
					}
					else{
						if(send(s1,command[i],sizeof(command[i]),0)<0)
							perror("sending message");
						i++;
					}
				}
				else if(i==2){
					flag = 0;
					j = 0;
					while(valid[i-1][j]){
						if(save[j] != valid[i-1][j]){
							flag = 1;
							break;
						}
						j++;
					}
					if(flag == 1){
						strcat(command[7],save);
						if(send(s1,command[7],sizeof(command[7]),0)<0)
							perror("sending message");
						error = 1;
						break;
					}
					else{
						if(send(s1,command[i],sizeof(command[i]),0)<0)
							perror("sending message");
						i++;
					}	
				}	

				else if(i==3){
					int m = 0;
					int n = 0,flag1 = 0;				
					while(save[m]!=':')
						m++;
					m++;
					while(save[m]!='@')
					{	
						user[n] = save[m];
						n++;
						m++;
					}
					user[n] = '\0';
					strcat(user,txt);
					fp = fopen(user,"r");
					if(fp == NULL){
						flag1 = 1;
						if(send(s1,command[8],sizeof(command[8]),0)<0)
							perror("sending message");
						error = 1;
						break;					
						fclose(fp);
					}		
					fclose(fp);		
					flag = 0;			
					j = 0;	
					while(valid[i-1][j]){
						if(save[j] != valid[i-1][j]){
							flag = 1;
							break;
						}
						j++;
					}
					if(flag == 1){
						strcat(command[7],save);
						if(send(s1,command[7],sizeof(command[7]),0)<0)
							perror("sending message");
						error = 1;
						break;								
					}
					else{
						if(flag1 == 1){
							strcat(command[8],save);
							if(send(s1,command[8],sizeof(command[8]),0)<0)
								perror("sending message");
							error = 1;
							close(s1);
							break;
						}
						else{
							if(send(s1,command[i],sizeof(command[i]),0)<0)
								perror("sending message");
							i++;	
						}			
					}	
				}
				else{
					flag = 0;	
					j = 0;
					while(valid[i-1][j]){
						if(save[j] != valid[i-1][j]){
							flag = 1;
							break;
						}
						j++;
					}
					if(flag == 1){
						strcat(command[7],save);
						if(send(s1,command[7],sizeof(command[7]),0)<0)
							perror("sending message");
						error = 1;	
						break;
					}
					else{
						if(send(s1,command[i],sizeof(command[i]),0)<0)
							perror("sending message");
						i++;
					}
	
				}
			}
			else
				strcpy(save,buf);
			int length = strlen(buf);
			if(error == 1)
				break;
			if((buf[length-2]=='.')&&(buf[length-1]=='\n'))
			{
				if(send(s1,command[6],sizeof(command[6]),0)<0)
					perror("sending message");
				buf[length-3] = '\n';
				buf[length-2] = '\n';
				buf[length-1] = '\0';
				fp = fopen(user,"a+");
				int f = 1;
				while(buf[f]){
					putc(buf[f],fp);
					f++;			
				}	
				fclose(fp);
				break;			
			}
		}while(TRUE);
		if(error == 1){
			close(s1);
			continue;
		}
		if((cc=recv(s1,buf,sizeof(buf),0))<0)
			perror("receiving packet");
		if(cc>0){
			buf[cc]='\0';
	 	}
		int chk;
		chk = strcmp(valid[4],buf);
		if(chk!=0)
		{
			strcat(command[7],save);
			if(send(s1,command[7],sizeof(command[7]),0)<0)
				perror("sending message");
			close(s1);
		}
		else
		{
			if(send(s1,command[5],sizeof(command[5]),0)<0)
				perror("sending message");
			printf("transaction completed successfully current connection closed\n");
			close(s1);
		}
		close(s1);
	}	
}

