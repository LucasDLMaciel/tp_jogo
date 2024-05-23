CFLAGS=-Llib -Iinclude -no-pie 
LIBS=-l:libraylib.a -pthread -ldl -lm -lxcb -lX11

Binario: main.o funcoes.o
	gcc main.o funcoes.o -o jogo -lraylib

# Quando quiser adicionar um módulo novo, coloque
# sempre $(CFLAGS) e $(LIBS) no final!

main.o: main.c
	gcc -c main.c -lraylib


funcoes.o: funcoes.c funcoes.h
	gcc -c funcoes.c -lraylib

# O comando `make run` vai compilar e rodar seu código!
# Não se esqueca de colocar seu módulo depois de `jogo`
run: jogo
	./jogo

# Se quiser recompilar seu jogo do 0, o comando `make clean` vai
# limpar a pasta de artefatos de compilação, etc.
# Não se esqueça de adicionar seu módulo depois de `jogo`
clean:
	rm jogo main.o funcoes.o

.PHONY: run clean