# GerenciadorDeMemoria

Um Simulador de gerenciamento de memória feito por mim como trabalho para a matéria de Sistemas Operacionais 1

Abra o terminal na pasta do arquivo e use:

gcc -o allocator.exe allocator.c

para gerar o executável. Depois, para executa-lo, use:

./allocator.exe

Possui os seguintes comandos: RQ (requisição), RL (liberação), C (compactação), STAT (status) e QUIT (saída). 
Por exemplo, uma Requisição de 400 bytes será feita da seguinte forma: 

alocador> RQ P0 400 W

O primeiro parâmetro para o comando RQ é o novo processo (no caso P0) que requer a memória, 
seguido pela quantidade de memória que está sendo solicitada em bytes e, finalmente, a estratégia de alocação: 
(W) worst-fit; (F) first-fit; (B) Best-fit.

Similarmente, o comando para liberar a memória alocada para o processo P0 aparecerá como:

alocador> RL P0

O comando para realizar a compactação da memória deve ser feito da seguinte forma:

alocador> C

Por fim, o comando STAT deve ser usado para obter um relatório sobre o status de ocupação da memória:

alocador> STAT


