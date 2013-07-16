
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


##In BNF:
======================

<p>
 &lt;char&gt; ::= &lt;ascii character&gt;</p>
<p>
 &lt;text&gt; ::= &lt;char&gt;, {&lt;char&gt;}</p>
<p>
 &lt;number&gt; ::= '0' | '1' | '2' | '3' | '4' | '5'| '6' | '7' | '8' | '9' </p>
<p>
 &lt;newline&gt; ::= '\n'</p>
<p>
 &lt;tab&gt; ::= '\t'</p>
<p>
 &lt;format_on&gt; ::= &lt;newline&gt; | &lt;tab&gt;</p>
<p>
 &lt;escape&gt; ::= '!' , &lt;format_on&gt;</p>
<p>
 &lt;header&gt; ::= '#', {'#'}</p>
<p>
 &lt;item&gt; ::= '-', {&lt;char&gt;}</p>
<p>
 &lt;list&gt; ::= '&lt;' &lt;format&gt; '&gt;'</p>
<p>
 &lt;title&gt; ::= '@', {&lt;char&gt;}, &lt;newline&gt;</p>
<p>
 &lt;identifier&gt; ::= ":" &lt;char&gt; ,{ &lt;char&gt; } ":"</p>
<p>
 &lt;divide&gt; ::= "=", {"="}</p>
<p>
 &lt;formatter&gt; ::= &lt;header&gt; | &lt;list&gt; |  &lt;divide&gt; | &lt;item&gt;</p>
<p>
 &lt;format&gt; ::= &lt;format_on&gt; &lt;formatter&gt; &lt;newline&gt;</p>
<p>
 &lt;term&gt; ::= &lt;formatter&gt;[&lt;identifier&gt;] | &lt;escape&gt; | &lt;text&gt;</p>
<p>
 &lt;page&gt; ::= &lt;title&gt; </p>
<p>
 &lt;version&gt; ::= &lt;number&gt; , {&lt;number&gt;}</p>
<p>
 &lt;start&gt; ::= '#XEMARK_' &lt;version&gt; &lt;newline&gt;</p>
<p>
 &lt;document&gt; ::= &lt;start&gt; &lt;title&gt; , [{&lt;term&gt;}]</p>
<p>
</p>



documents must start with a <start> term otherwise they are invalid

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

## You only to use one = to render an hr
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

```

