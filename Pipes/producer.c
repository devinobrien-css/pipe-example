#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/types.h>

#define STDIN 0
#define STDOUT 1
#define STDERR 2

int main(int argc, char * argv[])
{
	int fd, input_size, ppid, cpid; //ppid - parent id, cpid - child id

	//ERROR CHECK : FILE OPEN?
	if((fd = open("input.txt", O_RDONLY)) == -1)
	{
		write(STDOUT,"ERROR OPENING FILE",20);
	}
	else
	{
		char buffer[700], inputBuffer[700]; 

	//gather file info
		struct stat status;
		int fileInfo = stat("input.txt", &status);
		//ERROR CHECK : INFO RECIEVED?
  		if((fileInfo) != 0)
  		{
  			write(STDERR,"ERROR IN STAT",15);
  		}
  		else
  		{
  		//output from stat - full file size (in bytes)
  			input_size = status.st_size;

  		//read from file using file size
  			read(fd,buffer,input_size);
			  close(fd);

			/*Purpose of fork:
		   * Parent - writes info to output pipe(fdo), waits for child to execute, 
		   *          then reads info from input pipe(fdi)
		   * Child - reads info from STDIN(output pipe, fdo), interprets, and sends back to STDOUT(input pipe)
		   */
			 pid_t pid;
		
			/*Input/Output Pipes:
			 * fdi - input - producer reading in from consumer
			 * fdo - output - producer sending to consumer
			 */
			  int fdo[2];
  			int fdi[2];

 	   		pipe(fdo);
  			pipe(fdi);

  			pid = fork();

  			if(pid < 0)
  			{
  				write(STDOUT,"ERROR IN FORK",15);
  			}
  			else if(pid > 0)
  			{
  			//PARENT
          ppid = getpid();
  			//configure location settings
  				//note: stdout stays unchanged for printing to console after read from input pipe
  				close(fdo[0]); 
  				dup2(fdi[0],STDIN);//standard in is in from pipe (from consumer)
  				close(fdi[1]);

  			//write data from file to output pipe (NOT STDOUT, fdo[1])
  				write(fdo[1],buffer,input_size);

  			//wait for data to be interpreted in consumer
  				wait(0);

  			//read back data from input pipe
  				read(STDIN,inputBuffer,input_size);

  			//write recieved data from input pipe (output of consumer)
  				write(STDOUT,inputBuffer,strlen(inputBuffer));
          char pcInfo[200];
          sprintf(pcInfo,"\n\nProcessInfo:\n–––––––––––––\nParentPID:%d\nFileDescriptorOut:%d\nChildPID:%d\nFileDescriptorIn:%d\n",ppid,fdo[1],cpid,fdo[0]);

          write(STDOUT,pcInfo,strlen(pcInfo));
  			}
  			else
  			{
  			//CHILD
          cpid = getpid();
  			//configure location settings
  				//note: stdin of pgm is output pipe(from producer -> consumer), stdout is input pipe(from consumer -> producer)
  				dup2(fdo[0],STDIN);
  				close(fdo[1]);
  				dup2(fdi[1],STDOUT);
  				close(fdi[0]);

  			//exec()
  				char * args[] = {"./consumer",NULL};
  				execv(args[0],args);

  			//ERROR CHECK : EXEC FAILURE? 
  				write(STDERR,"FAILURE TO EXEC",14);		
  			}
  		}
	}
}
