#include <stdio.h>

#include "constants.h"
#include "functions.h"

int main()
{
	extern char BUFFER[];
	extern char lastRead;
	zeroBuffer();

	if(start() == FALSE){
		return 1;
	}

	openHTMLHeader();

	if(loadTitle() == FALSE){
		return 1;
	}

	printf("%s\n", BUFFER,stdout);
	closeTitle();

	//Find links to css and render each
	//Does the line after the title contain a ~?
	if(checkForStyle() == TRUE)
		renderStyle();

	closeHTMLHeader();

	zeroBuffer();

	//Now actually parse the document terms
	while(lastRead != EOF)
		renderFormat();

	closeHTMLBody();
	return 0;
}