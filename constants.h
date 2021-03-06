//Allow users to override buffer size through define
#ifndef MAXBUFFER
	#define MAXBUFFER 1000
#endif

#define TRUE 1
#define FALSE 0

char BUFFER[MAXBUFFER];

char * START  = "#XEMARK_";
char * SPECIAL_CHARS = "#<>-!=:\t";
char lastRead = 0;
int paragraphCount = 0;

#define HEAD_HTML_TOP "<html><head><title>"
#define HEAD_HTML_LINK_START "<link rel=\"stylesheet\" type=\"text/css\" href=\""
#define HEAD_HTML_LINK_STOP ".css\">"
#define HEAD_HTML_TITLE_STOP "</title>"
#define HEAD_HTML_BOTTOM "</head><body>"
#define CLOSE_HTML_BODY "</body></html>"

//States
#define FORMATTING_ON 1
#define FORMATTING_OFF 0
int STATE = FORMATTING_OFF;
