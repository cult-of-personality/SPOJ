﻿Team Number: 36
Group 2
Members:                        Shreyansh Gandhi                        2012A7PS070P
                                Himanshu Sharma                         2012C6PS545P


About our submission:
The Lexer and Parser are working correctly, and the parsing tree is also being constructed.
The AST and Symbol table are partially completed.


Changes in grammar:
We have removed the <P> non-terminal and constant terminal.
	<P>	-> 	constant | e
Also we made a slight change in <functioncall>
earlier:	<statement>	->	<functioncall>                                //partial
		<functioncall>	->	call id(<toSend>);
now:            <statement>	->      <functioncall>;     	                      //partial
                <functioncall>  ->      call id(<toSend>)


Details:
First Lexer should be executed followed by te Parser, as Parser takes the output of Lexer as input.
The Lexer takes a .txt file as input in the command line argument, parses it and returns any 
lexical errors found along with the number of errors. The output is given in a text file named lexer_output.txt.
The Parser takes a lexer_output.txt file as input in the command line (as generated in the output of the lexer) it 
parses the output of the lexer according to the language and returns successful for complete parsing.
It also constructs the parsing tree with indentations in increasing depth.

A part of the symbol table is being generated by the parser itself. While building the symbol table from
the parser, we would build only on declarations, arguments, new functions and new structures.
However since it is incomplete the code for the same has been commented out in the submission.
For the AST, some of the pruning functions are working and some of the token terminals are being deleted.
The AST pruning algorithms are removing the parentheses, those nonterminals which derive only one terminal or nonterminal and nonterminals deriving epsilon.
The final AST has not yet been generated as we have not yet processed expressions by pushing up the operators between the operands.
