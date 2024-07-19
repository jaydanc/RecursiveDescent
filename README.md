# Recursive Descent
A recursive descent (LL) parser for simple arithmetic expressions with modified operator precedence.<br>
Used to evaluate expressions where the multiplication and division precedence is inline with addition and subtraction.
<hr>

### The context-free grammar used is defined as:<br><br>

* Expression -> Binary<br>
* Binary -> Unary | (("\*" | "/" | "+" | "-") Unary)\*<br>
* Unary -> ("-") Unary | Primary<br>
* Primary -> Literal | "(" Expression ")";<br>
<hr>

### Built using:

* Visual Studio 2022
* Clang Compiler Tools for Windows
