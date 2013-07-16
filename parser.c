#include <stdio.h>

#include "constants.h"
#include "functions.h"

int readLine();
void closeHTMLBody();
int start(); 
int compareStem(char *,char *);
int compare(char *, char *);
void openHTMLHeader();
void closeHTMLHeader();
void closeHTMLBody();
void handleHeader();
void zeroBuffer();

int main()
{
	extern char BUFFER[];
	zeroBuffer();

	if(start() == FALSE){
		return 1;
	}

	openHTMLHeader();

	if(loadTitle() == FALSE){
		return 1;
	}

	printf("%s\n", BUFFER,stdout);
	closeHTMLHeader();

	zeroBuffer();

	//Now actually parse the document terms
	while(lastRead != EOF)
		renderFormat();

	closeHTMLBody();
	return 0;
}

