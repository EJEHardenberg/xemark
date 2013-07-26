
xemark is a simple markdown grammar that allows for blog html to be generated from a set of simple BNF rules


# Rules
=====================

- All documents begin with #XEMARK_ followed by a version number.
	- Currently the version doesn't matter so long as there is a number.
- All formatting characters are the first character of a line, or after tabs
- If you'd like no indent whatsover, escape the formatter with a !
- You can use the < character to begin a list, and > to end one.
- While creating a list, use the - to mark an item.
- You can use the = to create a dividing line.
- If you want to target html with an id, then after the format character use a :your id here: and it will be placed into the html
- The number of # will determine the level of the header. (1-6)


See the Grammaer.xe file for the BNF grammar of the xemark language


documents must start with a &lt;start&gt; term otherwise they are invalid

To compile:
`cc parser.c -o xemark`

To use:
`./xemark < inputfile > outputfile.html`

An example file:
```
#XEMARK_0
@Example Document
======================
#Hello World! 
<:target:
- This is a list
- Of things that are interesting
-:cool: Cool right?
>

## You only need to use one = to render an hr
=
But visually, it's nice to see a lot of them like this:
!============================

Notice that the ! escapes a formatter.

Format characters will render their line in the format way if they're the first character on a line, or after a sequence of tabs. This lets you do listed nests:
<
- Top level item
	<
		-It's nice to be able to write this out like this with the tabs for visual appeal of the source file
		-And plus it makes sense for listed nests
	>
- Right?
	-Although it;s
		-Not like
			-indenting it starts a new list though
>
: A colon first won't do much for you.

to compile this file down, you might do:
./xemark < example.xemark > example.html

Also you can link to things using a -&gt; between the link text and the link url. So a link like this -&gt; points_to_here. this -> points_to_here

```


If you want to run the file in a sort of 'interpretted mode', you can simply run ./a.out, type #XEMARK_0 \n @title
and type to your hearts content, seeing what the parser will generate as you type. When you're done, send an EOF to the stream to quit (typically Ctrl+D)


There is also the mark-server which is an extremely simple python server that will serve up xe files with it's current directory as root.