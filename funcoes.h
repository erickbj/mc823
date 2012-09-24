#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * Cria um socket de dominio IPv4, do tipo stream (com conexoes de duas vias),
 * mostrando mensagens de erro, se for o caso.
 * 
 * @param family Familia do socket (p. ex. AF_INET).
 * @param type Tipo do protocolo (p. ex. SOCK_STREAM).
 * @param flags Flags para o protocolo, tipicamente 0.
 * @return Descritor do socket criado em caso de sucesso e 1 em caso de erro.
 */
int CreateSocket(int family, int type, int flags);

/**
 * Cria uma estrutura de endereco.
 * 
 * @param family Familia do protocolo (p. ex. AF_INET).
 * @param port Numero da porta.
 * @return Estrutura com o endereco do servidor.
 */
struct sockaddr_in CreateAddress(int family, int port);

/**
 * Faz o bind de um dado socket, mostrando mensagens de erro se for
 * o caso.
 * 
 * @param listenfd Descritor do socket de escuta.
 * @param addr Estrutura de endereco para a qual fazer o bind.
 * @param addrlen Tamanho da estrutura de endereco.
 * @return 0 para sucesso, 1 para falha.
 */
int Bind(int listenfd, const struct sockaddr *addr, socklen_t addrlen);

/**
 * Conecta um socket a um servidor especificado, mostrando mensagens de erro
 * se for o caso.
 * 
 * @param sockfd Descritor do socket.
 * @param addr Ponteiro para a estrutura de endereco do
 * computador remoto ao qual se conectar.
 * @return 0 para sucesso, 1 para falha.
 */
int Connect(int sockfd, const struct sockaddr *addr);

/**
 * Poe um socket determinado no modo de escuta, mostrando mensagens de erro
 * se for o caso.
 * 
 * @param listenfd Descritor do socket a ser colocado no modo de escuta.
 * @param backlog Tamanho maximo da fila de escuta.
 * @return 0 para sucesso, 1 para falha.
 */
int Listen(int listenfd, int backlog);

/**
 * Aceita uma conexao entrante, mostrando mensagens de erro se for o caso.
 * 
 * @param listenfd Descritor do socket de escuta que aceitara a conexao.
 * @return -1 em caso de falha ou connfd (descritor do socket da conexao
 * aceita) em caso de sucesso.
 */
int Accept(int listenfd, struct sockaddr *client);

/**
 * Cria uma estrutura de endereco para o servidor.
 * 
 * @param port Numero da porta.
 * @param family Familia do protocolo (p. ex. AF_INET).
 * @return Estrutura com o endereco do servidor.
 */
struct sockaddr_in CreateServerAddress(int family, int port);

/**
 * Converte um endereco IP na forma "por extenso" para um formato binario,
 * mostrando mensagens de erro se for o caso.
 * 
 * @param af Familia do protocolo (p. ex AF_INET).
 * @param src Representacao "por extenso" do endereco.
 * @param dst Endereco de memoria para onde copiar o valor ja convertido.
 * @return 0 para sucesso, 1 para falha.
 */
int InetPtoN(int af, const char *src, void *dst);

/**
 * Le um valor a partir do socket especificado, mostrando mensagens de erro
 * se for o caso.
 * 
 * @param sockfd Descritor do socket de onde ler.
 * @param buf Buffer de leitura.
 * @param count Numero de bytes a serem lidos do socket para o buffer.
 * @return 0 para sucesso, 1 para falha.
 */
int ReadSocket(int sockfd, void *buf, size_t count);

/**
 * Escreve um valor para o socket especificado, mostrando mensagens de erro
 * se for o caso.
 * 
 * @param sockfd Descritor do socket de onde ler.
 * @param buf Buffer de escrita.
 * @param count Numero de bytes a serem lidos do socket para o buffer.
 */
void WriteSocket(int sockfd, void *buf, size_t count);

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
int PrintLocalAddress(int sockfd, struct sockaddr_in servaddr);

/**
 * Executa um comando do sistema, mostrando mensagens de erro se for o caso.
 * 
 * @param command A sequencia de caracteres representando um comando do
 * sistema.
 * @return 0 em caso de sucesso, 1 para falha.
 */
char* ExecuteSystemCmd(const char *command);


