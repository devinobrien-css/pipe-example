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

int chars;
int space;
int words;
int lines;

void getBufferInfo(char * buffer);

int main(int argc, char * argv[])
{
//OPEN FILE
	int fd; 
	if((fd = open("output.txt",O_CREAT|O_WRONLY)) == -1)
	{
		write(STDERR,"ERROR OPENING FILE",20);
	}
	else
	{
		char info[100]; char buffer[700];

	//read in from input pipe
		read(STDIN,buffer,700);

	//analyze buffer info - get chars, words, lines
		getBufferInfo(buffer);
		sprintf(info,"Characters(w/o Spaces): %d\nCharacters(w/ Spaces): %d\nWords: %d\nLines: %d\n",chars,chars+space,words,lines);
		
	//write to file
		write(fd,info,strlen(info));

	//write to output pipe
		write(STDOUT,buffer,strlen(buffer));
		close(fd);
		
	}
	
}

void getBufferInfo(char * buffer)
{
	lines = space = words = chars = 0; char cur;
	for(int i = 0; i <= strlen(buffer); i++)
	{
		cur = buffer[i];
		
		if (cur == ' ' || cur == '\t' || cur == '\n' || cur == '\0')
		{
		    words++;
		    if(cur == ' ')
		    {
		    	space++;
		    }
		    else if(cur == '\n' || cur == '\0')
			{
				lines++;
			}
		}
		else if(cur <= 126 && cur >= 33)
		{
			chars++;
		}
	}
}

