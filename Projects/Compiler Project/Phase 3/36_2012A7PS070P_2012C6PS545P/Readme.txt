Team Number: 36
Group 2
Members:                        Shreyansh Gandhi                        2012A7PS070P
                                Himanshu Sharma                         2012C6PS545P
********************************************************************************************************************************************

About our submission:
The Lexer, Parser, and Abstract Syntax Tree are working correctly, and the parsing tree is also being constructed.
The Symbol table has been completed, but remains untested.
Some of the Semantic checking has been included in the Symbol Table construction.
The Type Checking has been partially completed.

********************************************************************************************************************************************
Changes in grammar:
We have removed the <P> non-terminal and constant terminal.
	<P>	-> 	constant | e
Also we made a slight change in <functioncall>
earlier:	<statement>	->	<functioncall>                                //partial
		<functioncall>	->	call id(<toSend>);
now:            <statement>	->      <functioncall>;     	                      //partial
                <functioncall>  ->      call id(<toSend>)

********************************************************************************************************************************************
Details:-
First Lexer should be executed followed by te Parser, as Parser takes the output of Lexer as input.
The Lexer takes a .txt file as input in the command line argument, parses it and returns any 
lexical errors found along with the number of errors. The output is given in a text file named lexer_output.txt.

The Parser takes a lexer_output.txt file as input in the command line (as generated in the output of the lexer) it parses the output of the
lexer according to the language and returns successful for complete parsing. It also constructs the parsing tree with indentations in increasing depth.

The Abstract Syntax Tree is obtained by pruning the semicolons, parantheses, non-terminals that derive epsilon as well all non-terminals
that have a single child. The AST is found to contain about 30 to 40 per cent of the nodes in the raw parse tree.

The Symbol Table is built as a nested hash structure. The outside is a hash table in which function names are passed and a corresponding
node is returned which contains information such as return type, whether arguments are present, and the hash table which contains all 
variables corresponding to this function. The information about variables stored in the inner hash structure is name, data type, a flag
representing whether the variable is an array. The symbol table is partially built.

Semantic Rules are incorporated within the construction of the symbol table. For example, re-declaration of variables and functions can be 
caught by checking if an entity by the same name exists in the has structure before inserting a new entity.

The Type Checking phase has been implemented partially, but the general idea there was to incorporate rules such as compatibility of actual
and formal parameters, compatibilty of return types and return statements, validity of operations applied on variables belonging to certain 
data types.

The Code Generation Phase could not be implemented.
