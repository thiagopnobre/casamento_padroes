#include <stdlib.h>
#include <stdio.h>
#include <string.h>
/*
define o tamanho máximo do alfabeto como 255, pois este é maior código ASCII
*/
#define MAXCHAR 255


/*
casamento de padrão
*/
void BoyerMooreHorspool(char* texto, long tamanho_texto, char* padrao, long tamanho_padrao){
    long i,j,k,distancia[MAXCHAR + 1];
    /*
    inicializa todos os elementos do vetor de distâncias com o tamanho do padrão
    */
    for(j = 0 ; j <= MAXCHAR ; j++)
        distancia[j] = tamanho_padrao;
    /*
    calula a distancia de cada um dos elementos do padrão e substitui suas posições
    equivalentes no vetor de distâncias
    */
    for(j = 1 ; j < tamanho_padrao ; j++)
        distancia[padrao[j-1]] = tamanho_padrao - j;

    /*
    percorre todo o texto buscando pelos padrões e imprimindo as posições de suas
    ocorrências
    */
    i = tamanho_padrao;
    while(i <= tamanho_texto){
        k = i;
        j = tamanho_padrao;
        /*
        realiza comparações do padrão com texto de trás para frente
        */
        while(texto[k-1] == padrao[j-1] && j>0){
            k--;
            j--;
        }

        /*
        se o padrão foi encontrado, imprime a posição de seu primeiro caractere,
        em relação à posição absoluta no arquivo
        */
        if (j == 0){
            long espacos_comeco = ((k/60)+1)*5; //são os espaços no começo de cada linha no arquivo
            long espacos_meio = k/10; //são os espaços após cada palavra no arquivo
            long caracteres_fim = (k/60)*10; //são caracteres extras após o fim da linha no arquivo
            long posicao = espacos_comeco + espacos_meio + caracteres_fim + k + 1; //posição absoluta em relação ao arquivo
            printf("%3ld\n", posicao);
        }
        /*
        percorre a distância necessária para o próximo teste de casamento de padrão
        */
        i += distancia[texto[k-1]];
    }
}


int main(int argc, char **argv){
    //printf("argc: %d\nargv[0]: %s\nargv[1]: %s\nargv[2]: %s\n", argc, argv[0], argv[1], argv[2]);

    long tamanho_arquivo;
    FILE* arquivo = fopen(argv[2],"r"); //abre o arquivo
    /* cálcula o tamanho do arquivo */
    fseek(arquivo, 0L, SEEK_END);
    tamanho_arquivo = ftell(arquivo);
    fseek(arquivo, 0L, SEEK_SET);

    /*
    realiza a leitura do arquivo dividindo-o linhas e divindo cada linha em palavras,
    concatenando-as, uma a uma, em um texto que irá conter todas as informações
    relevantes do arquivo
    */
    char *palavra = (char*)malloc(20*sizeof(char));
    char *linha = (char*)malloc(100*sizeof(char));
    char *texto = (char*)malloc(tamanho_arquivo*sizeof(char));
    strcpy(texto,""); //inicializa o texto

    while (!feof(arquivo)){
    fscanf(arquivo, "%[^\r\n]s", linha); // lê a linha
    fscanf(arquivo, "\r\n"); //quebra de linha
    palavra = strtok(linha, " 0123456789/");//lê a primeira palavra da linha
    while(palavra != NULL){
          strcat(texto, palavra); //concatena a palavra ao fim do texto
          //sprintf(texto, "%s%s", texto, palavra);
          palavra = strtok(NULL, " 0123456789/"); //lê o restante das palavras da linha, uma a uma
        }
    }
    fclose(arquivo);

    //printf("%s\n\n", texto);

    BoyerMooreHorspool(texto, strlen(texto), argv[1], strlen(argv[1]));

    return 0;
}
