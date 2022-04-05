#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifdef WIN32 // Se for windows
	#define CLEAR_SCREEN system("cls")
#else // Se for linux
	#define CLEAR_SCREEN system("clear")
#endif

/**
 * Constante para definir o nome do arquivo com os automatos
**/
char FILENAME[] = "automato.txt";

/**
 * Captura a string digitada e limpa o buffer do teclado
**/
void scanS(char *str) {
     setbuf(stdin, NULL);

     scanf("%s", str);
}

/**
 * Verifica se a letra passada consta na transação passada.
**/
bool isValidLetter(char str[255], char letter) {
    return (str[strlen(str) - 3] == letter);
}


/**
 * Verifica se o estado da transação passada é válido.
**/
bool isValidState(char str[255], char state[5]) {
    return (strstr(str, state) != NULL);
}

/**
 * Retorna uma substring da string passada.
 *
 * @param m índice inicial
 * @param n índice final
**/
char *substr(const char *src, int m, int n) {
    // Comprimento da string;
	int len = n - m;

	// Contador auxiliar
    int i;

    // Substring que será gerada somado +1
    // para adicionar o caractere delimitador no final
	char *dest = (char*)malloc(sizeof(char) * (len + 1));

    // Adiciona os caracteres na nova string gerando-a.
	for (i = m; i < n && (*(src + i) != '\0'); i++)
	{
		*dest = *(src + i);
		dest++;
	}

    // Adiciona o caractere delimitador no final
	*dest = '\0';

    // Retorna a string
	return dest - len;
}

/**
 * Retorna o index do caractere contido em uma string.
**/
int getChIndex(char *string, char ch) {
    char *e = strchr(string, ch);

    return (int)(e - string);
}

/**
 * Retorna uma substring informando como inicio e final
 * um caractere.
 *
 * @param char initCh caractere inicial
 * @param char finalCh caractere final
**/
char * getSubstr(char str[255], char initCh, char finalCh) {
    int initPos = getChIndex(str, initCh) + 1;
    int finalPos = getChIndex(str, finalCh);

    return substr(str, initPos, finalPos);
}


/**
 * Processa a palavra escrita pelo usuário
 * aplicando o algoritmo de simulação para máquina de mealy.
**/
void process(char word[255]) {
    FILE *file; // Ponteiro para arquivo
    int bufferLength = 255; // Tamanho do buff (cada linha do arquivo lido)
    char buffer[bufferLength]; // Variável que armazena o buffer
    int i, j; // Variáveis contadoras auxiliares
    int wordlen = strlen(word); // Tamanho da palavra digitada

    char * beforeEquals; // Ponteiro para a string antes do sinal de igual da linha de transições
    char * afterEquals; // Ponteiro para a string depois do sinal de igual da linha de transições
    char * state = "q0"; // Estado atual
    char * output; // Armazena a saída de cada transação
    char outputString[255] = ""; // Armazena a string inteira de todas as saídas

    bool isAccepted; // Flag auxiliar para verificar se o estado foi aceito.

    file = fopen(FILENAME, "r");

    // Itera sobre cada letra da palavra
    for(i = 0, j = 0; i < wordlen; i++) {
        isAccepted = false;

        // Exibe a string digitada inicial sem saída
        if (i == 0) {
            printf("[%s]%s\n", state, substr(word, 0, wordlen));
        }

        // Itera sobre cada linha do arquivo
        while (fgets(buffer, bufferLength, file)) {

            // Apenas faz as verificações após a linha 6
            if (j++ >= 5) {

                // Armazena as strings antes e depois do sinal de igual
                beforeEquals = strtok(buffer, "=");
                afterEquals = strtok(NULL, "=");

                // Verifica se a letra digitada consta na string antes do igual
                // e se o estado também consta na string
                if (isValidLetter(beforeEquals, word[i]) && isValidState(beforeEquals, state)) {

                    // se constar, armazena o estado e os caracteres de saída
                    state = getSubstr(afterEquals, '(', ',');
                    output = getSubstr(afterEquals, ',', ')');

                    // exibe eles
                    printf("[%s]%-25s", state, substr(word, i + 1, wordlen));
                    printf("saida: %s\n", output);

                    // concatena o resultado com os caracateres de saída.
                    if (output[0] != '@') {
                        strcat(outputString, output);
                    }

                    // marca a flag como aceita
                    isAccepted = true;

                    break;
                }
            }
        }

        // Se a flag não foi alterada, então termina a iteração pois palavra é rejeitada.
        if (! isAccepted) {
            break;
        }

        // Reseta o arquivo para fazer uma nova iteração.
        rewind(file);
    }

    printf("%s\n", isAccepted ? "ACEITADA" : "REJEITADA");
    printf("\nPALAVRA DE SAÍDA: %s \n", outputString);
}

int menu() {
    int option;

    printf("\nMAQUINA DE MEALY\n");
    printf("SELECIONE A OPCAO DESEJADA \n\n");
    printf("[1] Digitar palavra\n");
    printf("[0] Sair\n");
    printf("Opcao: ");
    scanf("%d", &option);

    CLEAR_SCREEN;

    return option;
}


int main() {
    char word[255];
    int option;

    while (option = menu()) {
        switch(option) {
            case 1:
                CLEAR_SCREEN;

                printf("Palavra a ser processada: ");
                scanS(word);

                process(word);
                break;

            default:
                printf("Opcao inexistente, digite novamente!\n");
        }
    }

    return EXIT_SUCCESS;
}
