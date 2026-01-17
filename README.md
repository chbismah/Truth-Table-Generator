# Truth-Table-Generator
README – Logic Expression Analyzer
Brief Description of Program Features
This program processes propositional logic expressions and performs three main tasks:
	Tokenization
 Reads the user’s infix expression, removes invalid characters, checks brackets, and converts it into a clean token list.
    Infix to Postfix Conversion  
 Converts the logical expression into postfix form using operator precedence and a stack.
	Truth Table Generation 
Detects all variables in the expression, computes all possible truth combinations, evaluates the postfix expression for each case, and prints a complete truth table.
	Expression Saving 
Allows saving the truth table as a .txt or .csv file.
	File Loading 
Allows loading and displaying previously saved tables.
Supported operators:
! (NOT), & (AND), | (OR), > (Implication), = (Equivalence)
Supported variables: A–Z (single-character uppercase)

Example Output:
ALRS Output
Expression: (P & Q) > R
Variables: P Q R 
---------------------------------
P Q R | Result
0 0 0 | 1
0 0 1 | 1
0 1 0 | 1
0 1 1 | 1
1 0 0 | 1
1 0 1 | 1
1 1 0 | 0
1 1 1 | 1

Known Limitations
•	Only single-letter variables (A–Z) are supported
•	Expression must use the allowed operators only
•	No detection of unused variables
•	Menu options for argument validation and equivalence checking are listed but not implemented
•	Large expressions may exceed internal array size limits

