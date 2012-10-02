#include "funcoes.h"
#include <time.h>
#include <stdio.h>
#include <sys/wait.h>

#define LISTENQ 10
#define MAXDATASIZE 10240
#define MAXLINE 40*4096


void sig_chld(int signo)
{
	pid_t pid;
	int stat;

	while ( (pid = waitpid(-1, &stat, WNOHANG)) > 0)
		printf("Filho %d encerrado\n", pid);
	
	return;
}

int main(int argc, char **argv) {
	int listenfd, connfd;
	struct sockaddr_in servaddr;
	struct sockaddr_in client;
	time_t currentTime;
	struct tm *currtime;

	if (argc != 3) {
		perror("Numero invalido de argumentos.\nInforme a porta e o backlog.");
		exit(1);
	}


	// Cria o socket de escuta
	if (!(listenfd = CreateSocket(AF_INET, SOCK_STREAM, 0))) {
		exit(1);
	}

	// Copia o parametro de entrada porta e converte para o formato correto
	int port = atoi(argv[1]);

	// Copia o backlog da entrada convertendo para inteiro
	int backlog = atoi(argv[2]);

	// Cria a estrutura de endereco do servidor
	servaddr = CreateServerAddress(AF_INET, port);

	// Faz Bind do nome em serveraddr para o socket listenfd
	if (Bind(listenfd, (struct sockaddr *) &servaddr, sizeof (servaddr)) != 0) {
		exit(1);
	}

	// Ouve por conexoes no socket listenfd
	if (Listen(listenfd, backlog) != 0) {
		exit(1);
	}

	// Tratamento do SIGCHLD
	signal (SIGCHLD, sig_chld);

	FILE *log = fopen("connections.log", "a+");

	for (;;) {
		// Zera a variavel client
		bzero(&client, sizeof(client));

		// Aceita a conexao no socket listenfd, e retorna um descritor do socket
		// aceito podendo realizar operacoes com este socket.
		if ((connfd = Accept(listenfd, (struct sockaddr *)&client)) < 0) {
			if (errno == EINTR)
				continue;
			exit(1);
		}

		// Tenta realizar um fork, para que possa tratar mais de um cliente
		// ao mesmo tempo
		pid_t child = fork();

		// O processo filho toma conta da conexao estabelecida, e realiza
		// as leituras e execucoes dos comandos.
		if (child == 0) {

			close(listenfd);

			struct sockaddr_in own_client = client;

			time(&currentTime);
			currtime = localtime(&currentTime);

			fprintf(log, "%d/%d, %d:%d: Cliente (%s:%d) conectou\n", 
				currtime->tm_mday, currtime->tm_mon, 
				currtime->tm_hour, currtime->tm_min, 
				inet_ntoa(own_client.sin_addr), own_client.sin_port);
			fflush(log);

			while (1) {
				// Buffer de entrada do comando
				char cmd[200];

				// Faz a leitura do comando a ser executado
				if (ReadSocket(connfd, cmd, MAXLINE) != 0) {
					return (1);
				}

				if (!strcmp(cmd, "quit") || !strcmp(cmd, "exit"))
					break;
				
				// Executa o comando enviado pelo cliente
				char *output;
				output = ExecuteSystemCmd(cmd);
				if (!output)
					return (1);


				printf("Cliente ( %s:%d) executando comando '%s'\n", inet_ntoa(own_client.sin_addr), own_client.sin_port, cmd);

				WriteSocket(connfd, output, strlen(output) + 1);
				
				free(output);
			}

			time(&currentTime);
			currtime = localtime(&currentTime);

			fprintf(log, "%d/%d, %d:%d: Cliente (%s:%d) desconectou\n", 
				currtime->tm_mday, currtime->tm_mon, 
				currtime->tm_hour, currtime->tm_min, 
				inet_ntoa(own_client.sin_addr), own_client.sin_port);
			fflush(log);

			printf("%d/%d, %d:%d: Cliente (%s:%d) desconectou\n", 
				currtime->tm_mday, currtime->tm_mon, 
				currtime->tm_hour, currtime->tm_min, 
				inet_ntoa(own_client.sin_addr), own_client.sin_port);

			return 0;


		}

		// Retarda o fechamento da conexão do filho em 5 segundos
		sleep(5);

		// Fecha a conexao do filho
		close(connfd);

		// O processo pai continua a execucao e fica esperando um novo cliente
		// se conectar.
	}
	
	fclose(log);

	return (0);
}
