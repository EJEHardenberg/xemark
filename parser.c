#include <stdio.h>

#include "constants.h"

int readLine();
void closeHTMLBody();
int start(); 
int compareStem(char *,char *);
int compare(char *, char *);
void openHTMLHeader();
void closeHTMLHeader();
void closeHTMLBody();
void handleHeader();

char lastRead = 0;

int main()
{
	extern char BUFFER[];
	int i;
	for(i=0; i < MAXBUFFER; ++i){
		BUFFER[i] = '\0';
	}

	if(start() == FALSE){
		return 1;
	}

	openHTMLHeader();

	if(loadTitle() == FALSE){
		return 1;
	}

	printf("%s\n", BUFFER,stdout);
	closeHTMLHeader();

	//Now actually parse the document terms
	while(lastRead != EOF)
		renderFormat();

	closeHTMLBody();
	return 0;
}

int readLine(){
	extern char BUFFER[];
	int c;
	int i;
	for(i=0, c= getchar(); c != EOF && c != '\n'; c =getchar(), ++i){
		BUFFER[i] = c;
	}
	BUFFER[i] = '\0';
	lastRead = c;
	return i;
}

int printBuffer(){
	extern char BUFFER[];
	int c;
	int i;
	for(i=0; BUFFER[i] != EOF && i < MAXBUFFER; ++i ){
		putc(BUFFER[i],stdout);
	}
}

int start(){
	//Read in looking for XEMARK_<numbersr
	readLine();
	if(compareStem(START, BUFFER) == FALSE){
		printf("Invalid File. File must begin with %s and version number\n", START,stderr);
		return FALSE;
	}

	return TRUE;
}

int loadTitle(){
	//find and load the title into the buffer
	int c;
	for(c = getchar(); c != EOF && c != '@'; c=getchar())
		;
	//c is now either EOF or @
	if(c != '@'){
		printf("%s\n", "Document has no title, please specify one with the @ symbol",stderr);
		return FALSE;
	}
	
	//Ignore the @
	c = getchar();

	extern char BUFFER[];
	int i;
	for(i=0; i < MAXBUFFER && c != EOF && c != '\n'; ++i, c=getchar()){
		BUFFER[i] = c;
	}
	lastRead = c;
	BUFFER[i] = '\0';
	return TRUE;
}

/* compareStem compares to make sure that the two words share a common beginning such as XEMARK_0 and XEMARK_ */
int compareStem(char * string1, char * string2){
	int i,j;
	for(i=0,j=0; string1[i] != '\0' && string2[j] != '\0'; ++i, ++j){
		if(string1[i] != string2[j])
			return FALSE;
	}
	return TRUE;
}

int compare(char * string1, char * string2){
	printf("%s\n", string1);
	printf("%s\n", string2);
	int i,j;
	for(i=0,j=0; string1[i] != '\0' && string2[j] != '\0'; ++i, ++j){
		if(string1[i] != string2[j])
			return FALSE;
	}
	//Made it through but was it because one string was longer than the other?
	if( string1[i] != string2[j] )
		return FALSE;
	return TRUE;
}

void openHTMLHeader(){
	printf("%s\n", HEAD_HTML_TOP,stdout);
}

void closeHTMLHeader(){
	printf("%s\n", HEAD_HTML_BOTTOM,stdout);
}

void closeHTMLBody(){
	printf("%s\n", CLOSE_HTML_BODY);
}

int isSpecial(int c){
	int p;
	extern char * SPECIAL_CHARS;
	for(p=0; SPECIAL_CHARS[p] != '\0'; ++p){
		if(SPECIAL_CHARS[p] == c)
			return TRUE;
	}
	return FALSE;
}

void handleHeader(int length){
	int i,hLevel,identified;
	//Count the number of consectuive # and render the proper beginning of a header tag
	for(i=1; i < length && BUFFER[i] == '#'; ++i)
		;
	hLevel = i;
	printf("<h%d", hLevel);
	//Check for an identifier:
	if(BUFFER[i] == ':'){
		identified = TRUE;
		printf(" id=\"");
	}else
		identified = FALSE;
	if(identified == TRUE){
		for(i++; BUFFER[i] != ':' && BUFFER[i] != '\0'; ++i){
			putc(BUFFER[i],stdout);
		}
		printf("\">");
	}else{
		putc('>',stdout);
		putc(BUFFER[i],stdout);
	}
	//Print out whatever is left in the buffer
	for(i++; i < length; ++i)
		putc(BUFFER[i],stdout);
	printf("</h%d>\n", hLevel);

	return;
}

void handleList(int open){
	int i,identified;
	//Print out the beginning of the list
	if(open == TRUE)
		printf("<ul");
	else
		printf("</ul");
	i=1;
	identified = BUFFER[i] == ':' ? TRUE : FALSE;
	if(identified){
		printf(" id=\"");
		for(i++; BUFFER[i] != ':' && BUFFER[i] != '\0'; ++i)
			putc(BUFFER[i],stdout);
		printf("\"");
	}
	printf(">\n");
}

void handleListItem(int length){
	int i, identified;
	printf("<li");
	i=1;
	identified = BUFFER[i] == ':' ? TRUE : FALSE;
	if(identified){
		printf(" id=\"");
		for(i++; BUFFER[i] != ':' && BUFFER[i] != '\0'; ++i)
			putc(BUFFER[i],stdout);
		printf("\"");
		i++;
	}
	printf(">\n");
	//Print out the buffer
	for(; i < length && BUFFER[i] != '\0'; ++i){
		putc(BUFFER[i],stdout);
	}
	printf("\n</li>\n");

}

int renderFormat(){
	//If the last read character was a newline, and the current character is a special one.
	//Then we must render the formatter
	if(lastRead == '\n' || lastRead == '\t'){
		int length = 0;
		int i,identified;
		length = readLine();

		if(BUFFER[0] == '\t'){
			//seek to the first non tab
			for(i=1; BUFFER[i] == '\t'; ++i)
				;
			//shift everything over
			int j;
			for(j=0; j < length; ++j)
				BUFFER[j] = BUFFER[j+i];
		}
		//read the line in the buffer for a format
		if(isSpecial( BUFFER[0]) ){
			//Handle whichever special character it i
			if(BUFFER[0] == '!'){
				//Escape character, means we don't render using format, we just print
				printf("%s\n", BUFFER);
			}else{
				//Formatting time:
				extern int STATE; STATE = FORMATTING_ON;
				
				switch(BUFFER[0]){
					case '#':
						handleHeader(length);
						break;
					case '<':
						handleList(TRUE);
						break;
					case '>':
						handleList(FALSE);
						break;
					case '-':
						handleListItem(length);
						break;
					case '=':
						printf("<hr>\n");
						for(i=1; i < length; ++i)
							if(BUFFER[i] != '=')
								putc(BUFFER[i],stdout);
						printf("\n");
						break;
					case ':':
						//An identifier should not appear by itself.
						break;
				}


			}
		}else{
			//Not a special charactor. If we are in a special state that uses normal character such as :identifiers: then
			//that will be handled seperately by the ident handler, so we just print the lines... but perhaps we should
			//detect double new lines somehow and shit out <p> tags?
			printf("%s\n", BUFFER);
		}
	}
}