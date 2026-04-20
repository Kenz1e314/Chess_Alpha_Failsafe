# Makefile
# Author(s):
# History:
#   * 4/19/2026 Created first draft of Makefile


all:  bin/chess

LH_TEST:  bin/LH_TEST

bin/chess:  main.o chess.o gamestate.o moves.o computer.o user.o
	gcc -std=c11 -Wall main.o chess.o gamestate.o moves.o computer.o user.o -o bin/chess

main.o:  ./src/main.c ./src/chess.h ./src/gamestate.h ./src/moves.h ./src/computer.h ./src/user.h ./src/stubfunctions.h
	gcc -c -std=c11 -Wall ./src/main.c -o main.o

chess.o:  ./src/chess.c ./src/chess.h
	gcc -c -std=c11 -Wall ./src/chess.c -o chess.o

gamestate.o:  ./src/gamestate.c ./src/gamestate.h ./src/chess.h ./src/gamestate.h ./src/moves.h ./src/stubfunctions.h ./src/computer.h ./src/user.h
	gcc -c -std=c11 -Wall ./src/gamestate.c -o gamestate.o

moves.o:  ./src/moves.c ./src/moves.h
	gcc -c -std=c11 -Wall ./src/moves.c -o moves.o

computer.o:  ./src/computer.c ./src/computer.h ./src/chess.h ./src/gamestate.h ./src/moves.h ./src/stubfunctions.h ./src/user.h
	gcc -c -std=c11 -Wall ./src/computer.c -o computer.o

user.o:  ./src/user.c ./src/user.h ./src/chess.h ./src/gamestate.h ./src/moves.h ./src/stubfunctions.h ./src/computer.h
	gcc -c -std=c11 -Wall ./src/user.c -o user.o


bin/LH_TEST:  mainLHPrototype.o chess.o gamestate.o moves.o computer.o user.o
	gcc -std=c11 -Wall mainLHPrototype.o chess.o gamestate.o moves.o computer.o user.o -o bin/chess

mainLHPrototype.o:  ./src/mainLHPrototype.c ./src/chess.h ./src/gamestate.h ./src/moves.h ./src/computer.h ./src/user.h ./src/stubfunctions.h
	gcc -c -std=c11 -Wall ./src/mainLHPrototype.c -o mainLHPrototype.o




clean:
	rm -rf *.o
	rm -rf bin/chess

test:
	@echo 'Assuming executable has been created'
	./bin/chess
	

tar:  
	tar -zcvf Chess_V1.0.tar.gz ./README ./COPYRIGHT ./INSTALL ./bin ./doc/Chess_UserManual.pdf
	tar -zcvf Chess_V1.0_src.tar.gz ./README ./COPYRIGHT ./INSTALL ./doc ./bin ./src ./Makefile
