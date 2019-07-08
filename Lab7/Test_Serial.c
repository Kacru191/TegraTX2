//Test Serial Connection to the Microcontroller
//For Lab7 Part 1S

#include <stdio.h> 		// Standard input/output definitions
#include <fcntl.h>		/*file cotrol definitions*/
#include <termios.h>		/*POSIX Terminal Control Definitions */
#include <unistd.h>		/*UNIX Standard Definitions */
#include <errno.h>		/* Error Number Definitions*/
#include <string.h>

void main()
{
	char write_buffer[15];
	char read_buffer[15];
	int MAXCHAR = 15;
	int bytes_written;
	int byte_read;
	struct termios options; 	// Terminal Options
	int fd;				//File descriptor for the port
	

	fd = open("/dev/ttyACM0", O_RDWR | O_NOCTTY);	//Open tty device for RD and WR ( make sure tty USB1 matches the opened USB port
	//usleep(1000000);
	if(fd == 1) {
		printf("\n Error! in Opening ttyACM0\n");
	}
	else
		printf("\n ttyACM0 Opened Successfully\n");
	
	tcgetattr(fd, &options);	// Get the current options for the port
	cfsetspeed(&options, B115200);  // Set the baud rates to 115200
	cfsetspeed(&options, B115200);
	options.c_cflag |= (CLOCAL |CREAD) ;   // enable the reciever  and set local mode
	options.c_cflag &= ~PARENB;		// NO PARITY
	options.c_cflag &= ~CSTOPB;		//1 stop bit
	options.c_cflag &= ~CSIZE;		// Mask data size
	options.c_cflag |= CS8;		// 8 bits
	options.c_cflag &= ~CRTSCTS;		// Disable hardware flow control


//Enable data to be processed as raw input
	options.c_lflag &= ~(ICANON | ECHO | ISIG);
	
	tcsetattr(fd,  TCSANOW , &options);	//Apply options immediately
	fcntl(fd, F_SETFL, FNDELAY);
	usleep(2000000);

	for(int i=0; i < 2; i++) {        // run  this loop to clear the buffer

		while(read(fd, read_buffer, 1) != 1) {	}

		printf("Clear=%s\n", read_buffer);
		
	} 


	FILE *fp;
	char str[MAXCHAR];
	char*filename= "/home/nvidia/workspace/Lab7/build/Test_Serial.txt";
	
	fp = fopen(filename,"r");
	if(fp == NULL){
		
		printf("Error opening file: %s", filename);
		//return 1;
	}
	while (fgets(str,MAXCHAR,fp) != NULL){
		printf("Data from .txt file: %s\n",str);
		//for (int n = 0; n < MAXCHAR; n++)
		//	write_buffer[0] = str[MAXCHAR];
	}
	//fclose(fp);//return 0;

	memcpy(write_buffer,str, sizeof write_buffer);
	printf("Written into buffer: \n");
	puts(write_buffer);

         //write_buffer[0] = 'L';
	 bytes_written = 0;
		
//********************************************************************
	for(int i=0; i < 6; i++) {
		bytes_written = write(fd, write_buffer, 1);

		if(bytes_written  == -1)
			printf("\nW Error = %d\n", errno);
		else
			printf("\nWrote=%s\n", write_buffer);
./
		for(int j=0; j<6; j++){
		while(read(fd, write_buffer, 1) != 1) {	
				printf("Read %c\n", write_buffer[j]);
			}		
		}
	}

//**************************************************************************
	//write_buffer[0] = 'R';
	bytes_written = 0;

	for(int i=0; i < 6; i++) {
		bytes_written = write(fd, &write_buffer, 1);

		if(bytes_written  == -1)
			printf("W Error = %d\n", errno);
		else
			printf("Wrote=%s\n", write_buffer);

		while(read(fd, read_buffer, 1) != 1) {	}

		printf("Read R =%s\n", read_buffer);
		
	}

//*******************************************************************************
	close(fd);

//scanf("%c\n", read_buffer);
//fflush(stdin);

}
