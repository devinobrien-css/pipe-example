# pipe-example
These files give a brief example of interprocess communication using pipes. The producer file reads in from a .txt file, then creates 2 pipes for communication to and from consumer. 
The producer will write the data from the .txt file into pipe  A, then producer executes consumer. Consumer then reads in from pipe A, interprets the data, prints results to a file and returns the data read to pipe B. Producer then reads from pipe B, and prints the data to the console along with information about the processes. 
