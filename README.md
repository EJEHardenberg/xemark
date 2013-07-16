
xemark is a simple markdown grammar that allows for blog html to be generated from a set of simple BNF rules


# Rules
=====================

- All documents begin with #XEMARK_ followed by a version number.
	<
	- Currently the version doesn't matter so long as there is a number.
	>
- All formatting characters are the first character of a line, or after tabs
- If you'd like no indent whatsover, escape the formatter with a !
- You can use the < character to begin a list, and > to end one.
- While creating a list, use the - to mark an item.
- You can use the = to create a dividing line.
- If you want to target html with an id, then after the format character use a :your id here: and it will be placed into the html
- The number of # will determine the level of the header. (1-6)


##In BNF:
======================

 <char> ::= <ascii character>
 <text> ::= <char>, {<char>}
 <number> ::= '0' | '1' | '2' | '3' | '4' | '5'| '6' | '7' | '8' | '9' 
 <newline> ::= '\n'
 <tab> ::= '\t'
 <format_on> ::= <newline> | <tab>
 <escape> ::= '!' , <format_on>
 <header> ::= '#', {'#'}
 <item> ::= '-', {<char>}
 <list> ::= '<' <format> '>'
 <title> ::= '@', {<char>}, <newline>
 <identifier> ::= ":" <char> ,{ <char> } ":"
 <divide> ::= "=", {"="}
 <formatter> ::= <header> | <list> |  <divide> | <item>
 <format> ::= <format_on> <formatter> <newline>
 <term> ::= <formatter>[<identifier>] | <escape> | <text>
 <page> ::= <title> 
 <version> ::= <number> , {<number>}
 <start> ::= '#XEMARK_' <version> <newline>
 <document> ::= <start> <title> , [{<term>}]



documents must start with a <start> term otherwise they are invalid

