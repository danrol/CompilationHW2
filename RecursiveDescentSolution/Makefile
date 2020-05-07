objects = recursive_descent.o lex.yy.o  

recursive_descent.exe: $(objects)
	gcc -o recursive_descent.exe $(objects)


lex.yy.o : lex.yy.c recursive_descent.h
	gcc -c lex.yy.c

recursive_descent.o : recursive_descent.c recursive_descent.h 
	gcc -c recursive_descent.c
	
lex.yy.c : recursive_descent.lex
	win_flex recursive_descent.lex
	
clean :
	rm $(objects) recursive_descent.exe


	


