#include <stdio.h>

#include "constants.h"
#include "functions.h"


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

