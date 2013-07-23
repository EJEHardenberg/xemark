extern char lastRead ;
extern int paragraphCount; 

void zeroBuffer(){
	extern char BUFFER[];
	int i;
	for(i=0; i < MAXBUFFER; ++i){
		BUFFER[i] = '\0';
	}	
}

void printChar(int c){
	switch(c){
		case '<':
			printf("&lt;");
			break;
		case '>':
			printf("&gt");
			break;
		case '&':
			printf("&amp;");
			break;
		default:
			putc(c,stdout);
			break;
	}
	
}

int readLine(){
	extern char BUFFER[];
	int c;
	int i;
	for(i=0, c= getchar(); c != EOF && c != '\n'; c =getchar(), ++i){
		BUFFER[i] = c;
	}
	BUFFER[i] = '\0';
	//if we had just a newline, then we only ever put a null in.
	if(BUFFER[0] == '\0'){
		//Do a look ahead:
		int lookahead = getchar();
		if(lookahead == '\n')
			printf("</p><p>\n");
		//put it back
		ungetc(lookahead,stdin);
	}

	lastRead = c;
	return i;
}

void printLink(int begin, int cut, int length){
	int i;
	//Print non link text:
	for(i = 0; i < begin; ++i)
		printChar(BUFFER[i]);
	//Handle printing the link
	printf("<a href=\"");
	for(i = cut +2; i < length; ++i)
		printChar(BUFFER[i]);
	printf("\">");
	for(i = begin; i < cut; ++i)
		printChar(BUFFER[i]);
	printf("</a>");
}

int printBuffer(int length){
	extern char BUFFER[];
	int c;
	int i;
	//Check the buffer for links 
	int cut = 0;
	int begin = 0;
	for(i = 0; i < length && i < MAXBUFFER -1; ++i){
		if(BUFFER[i] == '-')
			if(BUFFER[i+1] == '>'){
				//We've detected a link!
				//first text to the left will be link text, all to the right will be the url (minus any space)
				cut = i;;
				//-2 becuase we skip a single space that might be by the ->
				for(begin = i; begin > 2 && BUFFER[begin-2] != ' '; --begin)
					;
				//Make up for the -2 cutting us one char short of a full word
				begin--;

			}
	}
	if(cut == 0){
		for(i=0; BUFFER[i] != EOF && i < MAXBUFFER && i < length; ++i ){
			printChar(BUFFER[i]);
		}
	}else{
		printLink(begin,cut,length);
	}
	
}

int start(){
	extern char BUFFER[];
	//Read in looking for XEMARK_<numbers>
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

void closeTitle(){
	printf("%s\n", HEAD_HTML_TITLE_STOP);
}

void openLinkHTML(){
	printf("%s", HEAD_HTML_LINK_START);
}

void closeLinkHTML(){
	printf("%s", HEAD_HTML_LINK_STOP);
}

void closeHTMLHeader(){
	printf("%s<p>\n", HEAD_HTML_BOTTOM,stdout);
}

void closeHTMLBody(){
	printf("</p>%s\n", CLOSE_HTML_BODY);
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
	extern char BUFFER[];
	int i,hLevel,identified;
	//Count the number of consectuive # and render the proper beginning of a header tag
	for(i=1; i < length && BUFFER[i] == '#'; ++i)
		;
	hLevel = i > 6 ? 6 : i;
	printf("<h%d", hLevel);
	//Check for an identifier:
	if(BUFFER[i] == ':'){
		identified = TRUE;
		printf(" id=\"");
	}else
		identified = FALSE;
	if(identified == TRUE){
		for(i++; BUFFER[i] != ':' && BUFFER[i] != '\0'; ++i){
			printChar(BUFFER[i]);
		}
		printf("\">");
	}else{
		putc('>',stdout);
		printChar(BUFFER[i]);
	}
	//Print out whatever is left in the buffer
	//First shift all chars down
	int j;
	for(i++,j=0; i < length; ++i,++j)
		BUFFER[j] = BUFFER[i];
	BUFFER[j] = '\0';
	//Zero out the rest
	printBuffer(j);
	printf("</h%d>\n", hLevel);

	return;
}

void handleList(int open){
	extern char BUFFER[];
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
			printChar(BUFFER[i]);
		printf("\"");
	}
	printf(">\n");
}

void handleListItem(int length){
	extern char BUFFER[];
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
		printChar(BUFFER[i]);
	}
	printf("\n</li>\n");

}

int renderFormat(){
	extern char BUFFER[];
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
			//End it 
			BUFFER[j] = '\0';
		}
		//read the line in the buffer for a format
		if(isSpecial( BUFFER[0]) ){
			//Handle whichever special character it i
			if(BUFFER[0] == '!'){
				//Escape character, means we don't render using format, we just print
				printf("%s\n", BUFFER);
			}else{
				//Formatting time:
				STATE = FORMATTING_ON;
				
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
								printChar(BUFFER[i]);
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
			printBuffer(length);
	
		}
	}
	zeroBuffer();
}



int checkForStyle(){
	int c;
	c = getchar();
	if(c == '~'){
		ungetc(c,stdin);
		return TRUE;
	}else{
		//If we don't have style, then we push the char back on for use after the html header is rendered
		ungetc(c,stdin);
		return FALSE;
	}
}

void renderStyle(){
	int length = readLine();
	openLinkHTML();
	int j;
	for(j=0; j < length; ++j)
		BUFFER[j] = BUFFER[j+1];
	printf("%s", BUFFER);
	closeLinkHTML();
	//Allow for more than one.
	while(checkForStyle() == TRUE)
		renderStyle();
}