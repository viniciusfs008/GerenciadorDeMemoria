//BIBLIOTECAS ---------------------------------------------------------------------------------------------

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

typedef struct allocator{
    int inicio, fim, tamanho;
    char* nome;
    struct allocator *prox;
} processo;


//VARIAVEIS GLOBAIS ---------------------------------------------------------------------------------------


//PROTOTIPAÇÃO ---------------------------------------------------------------------------------------------

//usada para pegar o comando e separar os tokens que o compoe
void leAnalisaComando(char*** cmd);
//executa os comandos
void executa(char*** cmd, processo** memoria);
//cria um novo processo
processo *criaProcesso(char* nome, int tamanho, int inicio, int fim);
//finaliza o shell
void finaliza(char* cmd, int *flag);

//MAIN -----------------------------------------------------------------------------------------------------

int main(int argc, char *argv[]){
    int tamanho=0, flag = 1;
    char** cmd = NULL;
    processo *memoria = NULL;

    //tamanho = atoi(argv[1]);
    tamanho = 1000;

    memoria = criaProcesso(NULL, tamanho, 0, tamanho-1);

    while(flag == 1){

        //le e analisa o comando
        leAnalisaComando(&cmd);
        //printf("%s\n%s\n%s", cmd[0], cmd[1], cmd[2]);

        //fecha o shell 
        finaliza(cmd[0], &flag);

        //executa o comando
        executa(&cmd, &memoria);
    }
    return 0;
}

//IMPLEMENTAÇÕES ------------------------------------------------------------------------------------------

//usada para pegar o comando e separar os tokens que o compoe
void leAnalisaComando(char*** cmd){
    size_t size = 256;
    char *comando = (char*) malloc(size * sizeof(char));

    //printa o diretorio atual
    printf("\n\033[0;32;40m@allocator\033[m>> ");

    //pega a linha de comando
    getline(&comando, &size, stdin);
    comando[strlen(comando) -1 ] = '\0';

    char* token = strtok(comando," ");

    int count = 0;
    while(token != NULL){
        count++;
        *cmd = (char**) realloc(*cmd, count * sizeof(char*));
        (*cmd)[count-1] = token;
        token = strtok(NULL," ");
    }
}

//executa os comandos
void executa(char*** cmd, processo** memoria){

    processo *aux = (*memoria);

    //CASO COMANDO SEJA REQUISIÇÃO
    if ((**cmd)[0] == 'R' && (**cmd)[1] == 'Q'){

        processo *aux2 = NULL;

        //estrategia de alocação first-fit
        if ((*cmd)[3][0] == 'F'){
            while (aux != NULL && (aux->nome != NULL || aux->tamanho < atoi((*cmd)[2]))){
                aux = aux->prox;
            }
        
            if(aux != NULL){
                if (aux->tamanho == atoi((*cmd)[2]))
                    aux->nome = (*cmd)[1];
                else{
                    aux2 = aux->prox;
                    aux->prox = criaProcesso((*cmd)[1], atoi((*cmd)[2]), aux->fim - atoi((*cmd)[2]) + 1, aux->fim);  
                    aux->fim = aux->fim - atoi((*cmd)[2]);
                    aux->tamanho = aux->tamanho - atoi((*cmd)[2]);
                    aux->prox->prox = aux2;
                }
               
                printf("Processo alocado com sucesso\n");
            }

            else
                printf("Nao a nenhuma lacuna com tamanho suficiente\n");
        }

        //estrategia de alocação best-fit
        else if ((*cmd)[3][0] == 'B'){
            processo *aux3 = NULL;
            processo *aux4 = NULL;
            processo *anterior = NULL;
            int menor = 1000000000;

            while (aux != NULL){
                if (aux->nome == NULL && aux->tamanho >= atoi((*cmd)[2]) && aux->tamanho < menor){
                    aux4 = anterior;
                    aux3 = aux;
                    menor = aux->tamanho;
                }
                anterior = aux;
                aux = aux->prox;
            }
        
            if(aux3 != NULL){
                if (aux3->tamanho == atoi((*cmd)[2]))
                    aux3->nome = (*cmd)[1];
                else{
                    aux2 = aux3->prox;
                    aux3->prox = criaProcesso((*cmd)[1], atoi((*cmd)[2]), aux3->fim - atoi((*cmd)[2]) + 1, aux3->fim);
                    aux3->fim = aux3->fim - atoi((*cmd)[2]);
                    aux3->tamanho = aux3->tamanho - atoi((*cmd)[2]);
                    aux3->prox->prox = aux2;
                }
                
                printf("Processo alocado com sucesso\n");
            }

            else
                printf("Nao a nenhuma lacuna com tamanho suficiente\n");
        }

        //estrategia de alocação Worst-fit
        else if ((*cmd)[3][0] == 'W'){
            processo *aux3 = NULL;
            processo *aux4 = NULL;
            processo *anterior = NULL;
            int maior = 0;

            while (aux != NULL){
               if (aux->nome == NULL && aux->tamanho > atoi((*cmd)[2]) && aux->tamanho > maior){
                    aux3 = aux;
                    aux4 = anterior;
                    maior = aux->tamanho;
                }
                anterior = aux;
                aux = aux->prox;
            }
        
            if(aux3 != NULL){
                if (aux3->tamanho == atoi((*cmd)[2]))
                    aux3->nome = (*cmd)[1];
                else{
                    aux2 = aux3->prox;
                    aux3->prox = criaProcesso((*cmd)[1], atoi((*cmd)[2]), aux3->fim - atoi((*cmd)[2]) + 1, aux3->fim);  
                    aux3->fim = aux3->fim - atoi((*cmd)[2]);
                    aux3->tamanho = aux3->tamanho - atoi((*cmd)[2]);
                    aux3->prox->prox = aux2;
                }
                printf("Processo alocado com sucesso\n");
            }

            else
                printf("Nao a nenhuma lacuna com tamanho suficiente\n");
    
        }

        //falha
        else
            printf("Comando de alocacao invalido\n");
        
    }

    //CASO COMANDO SEJA LIBERAÇÃO
    else if ((**cmd)[0] == 'R' && (**cmd)[1] == 'L'){
        processo *anterior = NULL;
        char *nome = (*cmd)[1];

        if (aux->nome == NULL){//caso seja uma lacuna ja pula para o proximo
            anterior = aux;
            aux = aux->prox;
        }
    
        while (aux != NULL && strcmp(aux->nome, nome) != 0){
            anterior = aux;
            aux = aux->prox;
            if (aux->nome == NULL){ //caso seja uma lacuna ja pula para o proximo
                anterior = aux;
                aux = aux->prox;
            }
        }

        if (aux != NULL){ //encontrou o processo
            
            //caso 1 esta no começo ou no fim
            if((aux->prox == NULL) || (anterior == NULL)){
                if (aux->prox == NULL){
                    if (anterior->nome != NULL)
                        aux->nome = NULL;
                    else{
                        anterior->fim = aux->fim;
                        anterior->tamanho = anterior->tamanho + aux->tamanho;
                        anterior->prox = aux->prox;
                        free(aux);
                    }
                }

                else{
                    if (aux->prox->nome != NULL)
                        aux->nome = NULL;
                    else{
                        processo *aux2;
                        aux->fim = aux->prox->fim;
                        aux->tamanho = aux->tamanho + aux->prox->tamanho;
                        aux2 = aux->prox;
                        aux->prox = aux->prox->prox;
                        aux->nome = NULL;
                        free(aux2);
                    }
                }
                
                aux->nome = NULL;
                printf("Processo removido com sucesso\n");
            }

            //caso 2 esta entre 2 processos
            else if (anterior->nome != NULL && aux->prox->nome != NULL){
                aux->nome = NULL;
                printf("Processo removido com sucesso\n");
            }

            //caso 3 há lacunas ajacentes
            else if (anterior->nome == NULL && aux->prox->nome == NULL){
                processo *dep = aux->prox;
                anterior->fim = dep->fim;
                anterior->prox = dep->prox;
                anterior->tamanho = anterior->tamanho + aux->tamanho + dep->tamanho;
                free(aux);
                free(dep);
                printf("Processo removido com sucesso\n");
            }

            //caso 4 há apenas uma lacuna adjacente
            else{
                if (anterior->nome == NULL){ //ESQUERDA
                    anterior->fim = aux->fim;
                    anterior->tamanho = anterior->tamanho + aux->tamanho;
                    anterior->prox = aux->prox;
                    free(aux);
                }

                else{ //DIREITA
                    processo *aux2;
                    aux->fim = aux->prox->fim;
                    aux->tamanho = aux->tamanho + aux->prox->tamanho;
                    aux2 = aux->prox;
                    aux->prox = aux->prox->prox;
                    aux->nome = NULL;
                    free(aux2);
                }

                printf("Processo removido com sucesso\n");
                
            }
        }
        
        else{
            printf("Processo não encontrado\n");
        }
         
    }

    //CASO COMANDO SEJA COMPACTAR
    else if ((**cmd)[0] == 'C'){
        processo *anterior = NULL;
        processo *aux2 = aux;
        int tamanho = 0;

        if(aux2->nome == NULL){
            tamanho += aux2->tamanho;
            aux = aux2->prox;
            free(aux2);
        }

        (*memoria) = aux;
        aux2 = aux;

        while (aux2 != NULL){
            if(aux2->nome == NULL){
                tamanho += aux2->tamanho;
                anterior->prox = aux2->prox; //PULA O BURACO
                free(aux2);
                aux2 = anterior->prox;
            }
            else{
                anterior = aux2;
                aux2 = aux2->prox;
            }
        }

        int base = 0;

        while (aux != NULL){
            aux->inicio = base;
            aux->fim = base + aux->tamanho - 1;
            base += aux->tamanho;
            anterior = aux;
            aux = aux->prox;
        }
        
        anterior->prox = criaProcesso(NULL, tamanho, base, base + tamanho - 1);

    }

    //CASO COMANDO SEJA STATUS
    else if ((**cmd)[0] == 'S' && (**cmd)[1] == 'T' && (**cmd)[2] == 'A' && (**cmd)[1] == 'T'){
       while (aux != NULL){
            if (aux->nome != NULL)
                printf("Enderecos %d[%d:%d] Processo %s\n", aux->tamanho, aux->inicio, aux->fim, aux->nome);
            else
                printf("Enderecos %d[%d:%d] Nao utilizados\n", aux->tamanho, aux->inicio, aux->fim);
            aux = aux->prox;
       }  
    }

    //CASO COMANDO SEJA INVALIDO
    else{
        printf("Comando Invalido!\n");
    }
    
}

//cria um novo processo na memoria
processo* criaProcesso(char* nome, int tamanho, int inicio, int fim){
    processo *novo = (processo *) malloc(sizeof(processo));
    novo->inicio = inicio;
    novo->fim = fim;
    novo->tamanho = tamanho;
    novo->nome = nome;
    novo->prox = NULL;
    return novo;
}

//finaliza o shell
void finaliza(char *cmd, int *flag){
    if((cmd[0] == 'Q') && (cmd[1] == 'U') && (cmd[2] == 'I') && (cmd[3] == 'T')){
        printf("\n------------------------------- Simulador Do Vini Finalizado ---------------------------------\n\n\n\n");
        flag = 0;
        exit(1);
    }
}