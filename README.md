Program which finds a way out of a maze.

Syntax

./maze --help

or

./maze --test soubor.txt

or

./maze --rpath R C soubor.txt

or

./maze --lpath R C soubor.txt

--help causes the program to print help for using the program and exit.
--test only checks that the file given by the second argument of the program contains a proper definition of the maze map. If the image format matches the definition (see below), it prints Valid. In the opposite case (eg illegal characters, missing data or wrong values), the program prints Invalid.
--rpath searches for a passage through the maze at the entrance on row R and column C. The passage is searched using the right-hand rule (right hand always on the wall).
--lpath looks for a passage through the maze at the input on row R and column C, but using the left-hand rule.
