#include "funcoes.h"

/**
 * Cria um socket de dominio IPv4, do tipo stream (com conexoes de duas vias),
 * mostrando mensagens de erro, se for o caso.
 * 
 * @param family Familia do socket (p. ex. AF_INET).
 * @param type Tipo do protocolo (p. ex. SOCK_STREAM).
 * @param flags Flags para o protocolo, tipicamente 0.
 * @return Descritor do socket criado em caso de sucesso e 1 em caso de erro.
 */
int CreateSocket(int family, int type, int flags) {
	int sockfd;

	if ((sockfd = socket(family, type, flags)) < 0) {
		perror("socket error");
		return (1);
	}

	return sockfd;
}

/**
 * Cria uma estrutura de endereco.
 * 
 * @param family Familia do protocolo (p. ex. AF_INET).
 * @param port Numero da porta.
 * @return Estrutura com o endereco do servidor.
 */
struct sockaddr_in CreateAddress(int family, int port) {
	struct sockaddr_in servaddr;

	// Zera os bytes da variavel servaddr
	bzero(&servaddr, sizeof (servaddr));

	// Define a familia do endereco como IPv4, a mesma do socket criado
	// Converte a porta passada como parametro, a mesma utilizada no servidor, em bytes para a rede
	servaddr.sin_family = family;
	servaddr.sin_port = htons(port);

	return servaddr;
}

/**
 * Faz o bind de um dado socket, mostrando mensagens de erro se for
 * o caso.
 * 
 * @param listenfd Descritor do socket de escuta.
 * @param addr Estrutura de endereco para a qual fazer o bind.
 * @param addrlen Tamanho da estrutura de endereco.
 * @return 0 para sucesso, 1 para falha.
 */
int Bind(int listenfd, const struct sockaddr *addr, socklen_t addrlen) {
	if (bind(listenfd, addr, addrlen) == -1) {
		perror("bind");
		return (1);
	}
	return (0);
}

/**
 * Conecta um socket a um servidor especificado, mostrando mensagens de erro
 * se for o caso.
 * 
 * @param sockfd Descritor do socket.
 * @param addr Ponteiro para a estrutura de endereco do
 * computador remoto ao qual se conectar.
 * @return 0 para sucesso, 1 para falha.
 */
int Connect(int sockfd, const struct sockaddr *addr) {
	socklen_t addrlen = (socklen_t) sizeof (*addr);
	if (connect(sockfd, addr, addrlen) < 0) {
		perror("connect error");
		return (1);
	}
	return (0);
}

/**
 * Poe um socket determinado no modo de escuta, mostrando mensagens de erro
 * se for o caso.
 * 
 * @param listenfd Descritor do socket a ser colocado no modo de escuta.
 * @param backlog Tamanho maximo da fila de escuta.
 * @return 0 para sucesso, 1 para falha.
 */
int Listen(int listenfd, int backlog) {
	if (listen(listenfd, backlog) == -1) {
		perror("listen");
		return (1);
	}
	return (0);
}

/**
 * Aceita uma conexao entrante, mostrando mensagens de erro se for o caso.
 * 
 * @param listenfd Descritor do socket de escuta que aceitara a conexao.
 * @return -1 em caso de falha ou connfd (descritor do socket da conexao
 * aceita) em caso de sucesso.
 */
int Accept(int listenfd, struct sockaddr *client) {
	int connfd;
	socklen_t len = sizeof(*client);
	
	if ((connfd = accept(listenfd, client, &len)) == -1) {
		perror("accept");
		printf("Do not accept connection");
		return (-1);
	}

	return connfd;
}

/**
 * Cria uma estrutura de endereco para o servidor.
 * 
 * @param port Numero da porta.
 * @param family Familia do protocolo (p. ex. AF_INET).
 * @return Estrutura com o endereco do servidor.
 */
struct sockaddr_in CreateServerAddress(int family, int port) {
	struct sockaddr_in servaddr;
	/*
	 * Zera os bytes da variavel serveraddr
	 */
	bzero(&servaddr, sizeof (servaddr));

	/*
	 * Define a familia do endereco igual ao do listen socket
	 *
	 * htons e htonl -> Converte valores entre o host e a rede em bytes
	 * definindo qual endereco e a porta que o servidor ira se conectar
	 */
	servaddr.sin_family = family;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(port);

	return servaddr;
}

/**
 * Converte um endereco IP na forma "por extenso" para um formato binario,
 * mostrando mensagens de erro se for o caso.
 * 
 * @param af Familia do protocolo (p. ex AF_INET).
 * @param src Representacao "por extenso" do endereco.
 * @param dst Endereco de memoria para onde copiar o valor ja convertido.
 * @return 0 para sucesso, 1 para falha.
 */
int InetPtoN(int af, const char *src, void *dst) {
	if (inet_pton(af, src, dst) <= 0) {
		perror("inet_pton error");
		return (1);
	}
	return (0);
}

/**
 * Le um valor a partir do socket especificado, mostrando mensagens de erro
 * se for o caso.
 * 
 * @param sockfd Descritor do socket de onde ler.
 * @param buf Buffer de leitura.
 * @param count Numero de bytes a serem lidos do socket para o buffer.
 * @return 0 para sucesso, 1 para falha.
 */
int ReadSocket(int sockfd, void *buf, size_t count) {
	if (read(sockfd, buf, count) < 0) {
		perror("Failed to read socket!");
		return (1);
	}
	return (0);
}

/**
 * Escreve um valor para o socket especificado, mostrando mensagens de erro
 * se for o caso.
 * 
 * @param sockfd Descritor do socket de onde ler.
 * @param buf Buffer de escrita.
 * @param count Numero de bytes a serem lidos do socket para o buffer.
 */
void WriteSocket(int sockfd, void *buf, size_t count) {
	write(sockfd, buf, count);
}

/**
 * Imprime o endereco IP e a porta locais, mostrando mensagens de erro se
 * for o caso.
 * 
 * @param sockfd Descritor do socket.
 * @param servaddr Estrutura de endereco para a qual serao escritos os valores
 * de IP e porta locais.
 * @return 0 para sucesso, 1 para falha.
 * @see getsockname.
 */
int PrintLocalAddress(int sockfd, struct sockaddr_in servaddr) {
	socklen_t tamanho = sizeof (servaddr);
	if (getsockname(sockfd, (struct sockaddr *) &servaddr, &tamanho) != 0) {
		perror("Erron on get socket name");
		return (1);
	} else {
		printf("Endereco IP local: %s\n", (char *) inet_ntoa(servaddr.sin_addr));
		printf("Porta local:  %d\n", (int) ntohs(servaddr.sin_port));
		return (0);
	}
}

/**
 * Executa um comando do sistema, mostrando mensagens de erro se for o caso.
 * 
 * @param command A sequencia de caracteres representando um comando do
 * sistema.
 * @return the output of command or NULL on error.
 */
char *ExecuteSystemCmd(const char *command) {
	FILE *proc = popen(command, "r");
	if (proc == NULL) {
		printf("Invoking command %s failed.", command);
		return NULL;
	}

	char output[5000];
	char buf[256];

	output[0] = '\0';

	while (!feof(proc)) {
		if (fgets(buf, 128, proc) != NULL)
			strcat(output, buf);
	}

	buf[0] = '\0';
	pclose(proc);

	return strdup(output);
}
