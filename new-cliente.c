#include "funcoes.h"

#define MAXLINE 20*4096

int main(int argc, char **argv) {
    int sockfd;
    char error[MAXLINE + 1];
    struct sockaddr_in servaddr;

    // Verifica se foi passado o endereco IP ao qual o cliente ira se conectar e a porta
    if (argc != 3) {
        strcpy(error, "uso: ");
        strcat(error, argv[0]);
        strcat(error, " <IPaddress>");
        strcat(error, " <Port>");
        perror(error);
        exit(1);
    }

    // Tenta criar o socket para conexao com o servidor.
    if (!(sockfd = CreateSocket(AF_INET, SOCK_STREAM, 0))) {
        exit(1);
    }

    // Copia os argumentos de entrada (IP e porta)
    char *ipaddr = argv[1];
    int port = atoi(argv[2]);

    servaddr = CreateAddress(AF_INET, port);

    // Converte o endereco IP passado como argumento em numero binario,
    // escrevendo o resultado no endereco criado
    if (InetPtoN(AF_INET, ipaddr, &servaddr.sin_addr) != 0) {
        exit(1);
    }

    // Inicia uma conexao com o socket criado
    if (Connect(sockfd, (struct sockaddr *) &servaddr) != 0) {
        exit(1);
    }

    // Endereco IP e porta onde ira se conectar
    printf("Endereco IP a se conectar: %s\n", ipaddr);
    printf("Porta a se conectar:  %d\n", port);

    if (PrintLocalAddress(sockfd, servaddr) != 0) {
        exit(1);
    }

    while (1) {
        // buffers estaticos para a leitura do comando e do retorno
        // do servidor
        char cmd[200];
        char received[200];

        printf("Digite um comando: ");
        scanf("%[^\n]s", cmd);
        printf("Comando é %s\n", cmd);
        setbuf(stdin, NULL);

	// Envia o comando para o servidor
        WriteSocket(sockfd, cmd, strlen(cmd) + 1);

	if (!strcmp(cmd, "quit") || !strcmp(cmd, "exit"))
		break;
        
        // Faz a leitura da saída do comando
        if (ReadSocket(sockfd, received, MAXLINE) != 0) {
            return (1);
        }
        printf("Saída do comando '%s':\n%s\n", cmd, received);
    }

    printf("Cliente desconectado.\n");

    exit(0);
}
