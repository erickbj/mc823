all: new-cliente new-servidor

new: all

new-cliente: funcoes.c

new-servidor: funcoes.c

clean:
	rm -f new-cliente new-servidor *.o *~