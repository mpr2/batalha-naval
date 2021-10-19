/*
    Batalha Naval
    -------------
    Neste jogo, uma frota está escondida no oceano. O objetivo é afundar todas as naves
    com a quantidade de torpedos disponível.
    A cada rodada, são apresentados na tela o oceano e a quantidade de naves e torpedos
    remanescentes. Em seguida o jogo pede para o usuário digitar um setor para o
    lançamento do torpedo. Na rodada seguinte o jogo mostra o resultado do lançamento do
    torpedo, podendo ter acertado uma nave ou se perdido na água.
    O jogo termina quando o usuário perde ou ganha.
    O usuário perde quando a quantidade de torpedos acaba, sem que todas as naves tenham
    sido afundadas.
    O usuário ganha quando consegue afundar todas as naves.
*/

#include <ctype.h>
#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>

enum game_mode {PLAY, CORR};  //enum para modos do jogo
enum sector_code  //enum para preenchimento dos setores
{
    FREE,       //setor livre
    OCCUPIED,   //setor ocupado
    S_CODE,     //submarino
    W_CODE,     //torpedo no oceano
    SINK_CODE   //nave afundada
};

const int MD = CORR;  //modo do jogo
const int ORDER = 5;  //ordem da matriz
const int D = 0;      //quantidade de destruidores
const int S = 3;      //quantidade de submarinos
const int T = 6;      //quantidade de torpedos
const int MESSAGE_LENGTH = 200;  //tamanho máximo da mensagem mostrada ao usuário

void check_and_fill(int grid[ORDER][ORDER], int i, int j);
void place_ship(int grid[ORDER][ORDER], int i, int j, int code);

int
main()
{
    int grid[ORDER][ORDER]; //matriz que representa os setores do jogo
    int i = 0;      //índices para acesso às posições da matriz
    int j = 0;
    int cont = 0;   //contador para controle de loop
    int d = D;      //contador de destruidores
    int s = S;      //contador de submarinos
    int t = T;      //contador de torpedos
    char c1 = 0;    //caracteres para entrada do código do setor
    char c2 = 0;
    char message[MESSAGE_LENGTH];   //mensagem a ser apresentada ao usuário
    message[0] = '\0';  //inicializa como string vazia

    //inicializa matriz com posições livres
    for (i = 0; i < ORDER; i++)
    {
        for (j = 0; j < ORDER; j++)
        {
            grid[i][j] = FREE;
        }
    }

    //posicionamento de submarinos na matriz
    cont = 0;
    while (cont < S)
    {
        i = rand() % ORDER;
        j = rand() % ORDER;
        if (grid[i][j] == FREE)
        {
            place_ship(grid, i, j, S_CODE);
            cont++;
        }
    }

    printf("BATALHA NAVAL\n");
    printf("-------------\n");

    //loop principal do jogo
    while (1)
    {
        //escreve a matriz na tela
        printf("\n   ");
        for (i = 0; i < ORDER; i++)
        {
            printf("%3d ", i + 1);
        }
        printf("\n   +");
        for (i = 0; i < ORDER; i++)
        {
            printf("---+");
        }
        printf("\n");
        for (i = 0; i < ORDER; i++)
        {
            printf(" %c |", 'A' + i);
            for (j = 0; j < ORDER; j++)
            {
                if (grid[i][j] == S_CODE && MD == CORR)
                    printf(" S |");
                else if (grid[i][j] == W_CODE)
                    printf(" W |");
                else if (grid[i][j] == SINK_CODE)
                    printf(" * |");
                else
                    printf("   |");
            }
            printf("\n   +");
            for (j = 0; j < ORDER; j++)
            {
                printf("---+");
            }
            printf("\n");
        }

        printf("\n");
        //apresenta na tela mensagem referente à última ação
        printf("%s\n", message);
        message[0] = '\0';
        printf("\n");
        //apresenta a quantidade de naves e torpedos
        printf("Destruidores: %d\n", d);
        printf("Submarinos: %d\n", s);
        printf("Torpedos: %d\n", t);
        printf("\n");
        //apresenta mensagens e sai do loop caso o jogador tenha ganhado ou perdido
        if (s == 0 && t >= 0)
        {
            printf("Parabéns! Você venceu!\n");
            break;
        }
        else if (t == 0 && s > 0)
        {
            printf("Você perdeu...\n");
            break;
        }
        printf("Digite o setor: ");
        //entrada dos caracteres referentes ao setor
        __fpurge(stdin);
        c1 = getchar();
        c2 = getchar();
        //verifica se o setor digitado é válido
        if (((c1 >= 'A' && c1 < 'A' + ORDER) || (c1 >= 'a' && c1 < 'a' + ORDER))
            && (c2 >= '1' && c2 < '1' + ORDER))
        {
            i = tolower(c1) - 'a';
            j = c2 - '1';
            t--;
            if (grid[i][j] == S_CODE)
            {
                grid[i][j] = SINK_CODE;
                s--;
                strcpy(message, "Você afundou um submarino");
            }
            else if (grid[i][j] == SINK_CODE || grid[i][j] == W_CODE)
            {
                strcpy(message, "Você desperdiçou um torpedo");
            }
            else
            {
                grid[i][j] = W_CODE;
                strcpy(message, "Você acertou o oceano");
            }
        }
        else
        {
            strcpy(message, "Setor inválido");
        }
    }

    return 0;
}

/*
    check_and_fill
    --------------
    Checa se o índice (i,j) da matriz grid é válido
    e marca o setor que esteja livre como ocupado
*/
void
check_and_fill(int grid[ORDER][ORDER], int i, int j)
{
    if (i >= 0 && i < ORDER && j >= 0 && j < ORDER && grid[i][j] == FREE)
        grid[i][j] = OCCUPIED;
}

/*
    place_ship
    ----------
    Posiciona a nave identificada por code no índice (i,j) da matriz grid
    e marca os setores adjacentes que estejam livres como ocupados
*/
void 
place_ship(int grid[ORDER][ORDER], int i, int j, int code)
{
    grid[i][j] = code;
    check_and_fill(grid, i - 1, j - 1);
    check_and_fill(grid, i - 1, j);
    check_and_fill(grid, i - 1, j + 1);
    check_and_fill(grid, i, j - 1);
    check_and_fill(grid, i, j + 1);
    check_and_fill(grid, i + 1, j - 1);
    check_and_fill(grid, i + 1, j);
    check_and_fill(grid, i + 1, j + 1);
}