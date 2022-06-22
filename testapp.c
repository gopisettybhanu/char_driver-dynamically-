#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

int8_t write_buf[1024];
int8_t read_buf[1024];

int main()
{
	int fd;
	char option;
	
	printf("Welcome to the demo of character devie driver \n");

	fd = open("/dev/my_device",O_RDWR);
	printf("%d",fd);
	if(fd < 0) {
		printf("Cannot open device file \n");
		return 0;
	}

	while(1) {
		scanf("%c", &option);

		switch(option) {
			case '1':
				scanf(" %[^\t\n]s", write_buf);
				printf("Data writing\n");
				write(fd,write_buf,strlen(write_buf)+1);
				break;
			case '2':
				printf("data is reading\n");
				read(fd,read_buf,1024);
				break;
			case '3':
				close(fd);
				exit(1);
				break;
			default:
				printf("enter valid info");
				break;
	}
}
close(fd);
}
