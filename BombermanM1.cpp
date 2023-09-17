//JOGO BOMBERMAN: Trabalho M1 de ALGORITMOS E PROGRAMACAO II
//CRIADO POR: Carlos Daniel Rebelo - Kaua Betineli Correia

//bibliotecas utilizadas
#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

#define ROW 15 //define a quantidade de linhas do mapa
#define COL 25 //define a quantidade de colunas do mapa

using namespace std;


//substitui a posicao atual do jogador por uma bomba
void colocarBomba(int linha, int coluna, int m[ROW][COL], bool &bombaMapa, bool &tempo, clock_t &inicioBomba){

    if(bombaMapa == false){ //bombaMapa == false significa que nao ha outra bomba no mapa
        if(tempo == false){ 
            m[linha][coluna] = 3; //substitui a posicao do jogador por uma bomba
            inicioBomba = clock(); //contabiliza um tempo de inicio para a detonacao da bomba
            tempo = true; //torna tempo = true pois comecou a contar o tempo
            bombaMapa = true; //define que existe uma bomba no mapa, garantindo que nao possa ser posicionada mais de uma bomba ao mesmo tempo
        }
    }
        
    
}


//substitui a posicao atual da bomba, em cima, em baixo, a direta e a esquerda na explosao da bomba
void animacaoBomba(int m[ROW][COL], int i, int j){
    m[i][j] = 7; //torna a posicao atual como "7"

    //substitui as posicoes que encostam na bomba em "7" caso nao sejam paredes fixas
    if(m[i-1][j] != 1){
        m[i-1][j] = 7;
    }
    if(m[i+1][j] != 1){
        m[i+1][j] = 7;
    }
    if(m[i][j-1] != 1){
        m[i][j-1] = 7;
    }
    if(m[i][j+1] != 1){
        m[i][j+1] = 7;
    }

}



//ativa se tempo==true
void tempoBomba(int m[ROW][COL], bool &tempo, clock_t &inicioBomba, clock_t &fimBomba, bool &bombaExplosao){
    
    fimBomba = clock(); //registra um tempo final para a explosao da bomba
    if ((fimBomba-inicioBomba)/CLOCKS_PER_SEC == 2){ //se a diferenca entre o inicio e o fim for de 2 segundos, entra na condicao
        for(int i=0;i<ROW;i++){ //varre o mapa
            for(int j=0;j<COL;j++){ //varre o mapa
                if(m[i][j] == 3){ //verifica onde esta a bomba no mapa
                    animacaoBomba(m, i, j); //executa a animacao de explosao da bomba
                    bombaExplosao = true; //torna bombaExplosao = true
                    inicioBomba = clock(); //contabiliza novamente o tempo inicial para sincronizar com a explosao da bomba
                    
                }
            }
        }
        //torna tempo = false para evitar a repeticao
        tempo = false;
        
    }
    
}



void animacaoBombaFinal(int m[ROW][COL], clock_t &inicioBomba,clock_t &fimBomba, bool &bombaExplosao, bool &bombaMapa){
    fimBomba = clock(); //contabiliza o tempo final depois que a bomba estiver 2 segundos no mapa
    if((fimBomba-inicioBomba)/CLOCKS_PER_SEC == 1){ //assim que passar 1 segundos depois do inicio da explosao da bomba, entra na codicao
        for(int i=0;i<ROW;i++){ //varre o mapa
            for(int j=0;j<COL;j++){ //varre o mapa
                if(m[i][j] == 7){ //os "7" (animacao de explosao da bomba) que estiverem no mapa sao os que encostam na bomba antes de ela explodir
                    m[i][j] = 0; //transforma os "7" em caminhos - transforma a animacao de explosao da bomba em caminho para o jogador andar
                    
                }
            }
            bombaMapa = false; //define que nao existem mais bombas no mapa, podendo o jogador colocar uma bomba novamente
            bombaExplosao = false; //determina que a explosao da bomba encerrou
        }
                

            
    }
    
}


//verifica se a posicao do jogador for igual a "7": explosao da bomba, "5": inimigo; caso alguma das condicoes for atingida, personadoAtingido = true
void verificaPersonagemColisao(int m[ROW][COL], int linha, int coluna, bool &personagemAtingido){
    if(m[linha][coluna] == 7 || m[linha][coluna] == 5){
        personagemAtingido = true;
    }
}


//realiza a acao de acordo com a tecla pressionada e verifica as condicoes para cada switch para o jogador poder andar ou colocar a bomba no mapa
void acaoPersonagem(char tecla, int &linha, int &coluna, int m[ROW][COL], bool &bombaMapa, bool &tempo, clock_t &inicioBomba){
    switch(tecla){
        case 72: case 'w': //cima
            if(m[linha-1][coluna] == 0 || m[linha-1][coluna] == 3 || m[linha-1][coluna] == 7 || m[linha-1][coluna] == 5){
                linha--;
                
            }
        break;

        case 80: case 's': //baixo
            if(m[linha+1][coluna] == 0 || m[linha+1][coluna] == 3 || m[linha+1][coluna] == 7 || m[linha+1][coluna] == 5){
                linha++;
                
            }
        break;

        case 75:case 'a': //esquerda
            if(m[linha][coluna-1] == 0 || m[linha][coluna-1] == 3 || m[linha][coluna-1] == 7 || m[linha][coluna-1] == 5){
                coluna--;
                
            }
        break;

        case 77: case 'd': //direita
            if(m[linha][coluna+1] == 0 || m[linha][coluna+1] == 3 || m[linha][coluna+1] == 7 || m[linha][coluna+1] == 5){
                coluna++;
                
            }
        break;

        case 'b': //coloca uma bomba na posicao do jogador
            colocarBomba(linha, coluna, m, bombaMapa, tempo, inicioBomba);

            
        
    }   
}

//verifica na matriz "m" onde ha inimigos "5" e faz os inimigos se moverem
void direcaoInimigo(int m[ROW][COL], int i, int j){
    int direcao, passosQueIraDar;
    direcao = rand()%4+1; //da a direcao de forma aleatoria //1- cima, 2- direita, 3- baixo, 4, esquerda
    passosQueIraDar = rand()%3+1; //randomicamente verifica a quantidade de passos que o inimigo ira dar, de 1 a 3 passos
    int incrementa = 0;
    bool caminhoBloqueado = false;
    while(incrementa < passosQueIraDar && caminhoBloqueado == false){ //inicia o loop para movimentar o inimigo ate a alguma das condicoes serem quebradas
        switch(direcao){ //switch de acordo com a direcao escolhida aleatoriamente
            case 1: //inimigo vai para cima
                
                if(m[i-1][j] == 0){
                    m[i][j] = 0;
                    m[i-1][j] = 5;
                } else if(m[i-1][j] == 7){
                    m[i][j] = 0;
                } else if(m[i-1][j] == 1 || m[i-1][j] == 2){
                    caminhoBloqueado = true;
                }

                break;


            case 3: //inimigo vai para baixo
                
                if(m[i+1][j] == 0){
                    m[i][j] = 0;
                    m[i+1][j] = 5;
                } else if(m[i+1][j] == 7){
                    m[i][j] = 0;

                } else if(m[i+1][j] == 1 || m[i+1][j] == 2){
                    caminhoBloqueado = true;
                }
                
                break;


            case 2: //inimigo vai para a direita
                
                if(m[i][j+1] == 0){
                    m[i][j] = 0;
                    m[i][j+1] = 5;
                } else if(m[i][j+1] == 7){
                    m[i][j] = 0;
                } else if(m[i][j+1] == 1 || m[i][j+1] == 2){
                    caminhoBloqueado = true;
                }
                
                break;


            case 4: //inimigo vai para a esquerda
                
                if(m[i][j-1] == 0){
                    m[i][j] = 0;
                    m[i][j-1] = 5;
                } else if(m[i][j-1] == 7){
                    m[i][j] = 0;
                } else if(m[i][j-1] == 1 || m[i][j-1] == 2){
                    caminhoBloqueado = true;
                }
                
                break;

                
        }

        incrementa++;
    }
    
    caminhoBloqueado = false;
    incrementa = 0;
}


//verifica a diferenca de 1 segundo para cada movimentacao do inimigo
void acaoInimigo(int m[ROW][COL], bool &inimigoMovimentado, clock_t inicioInimigo){
    inimigoMovimentado = true;
    clock_t fimInimigo;
    fimInimigo = clock();
    if((fimInimigo-inicioInimigo)/CLOCKS_PER_SEC == 1){ //se a diferenca entre a movimentacao de cada inimigo for 1, entra na condicao
        for(int i = 0; i<ROW; i++){
            for(int j = 0; j<COL; j++){
                if(m[i][j] == 5){
                    direcaoInimigo(m, i, j);
                    inimigoMovimentado = false;
                }
            }
        }
    }
}


int quantidadeInimigo(int m[ROW][COL]){
    int quantidade = 0;
    for(int i = 0; i<ROW; i++){ //varre a matriz do mapa
        for(int j = 0; j<COL; j++){
            if(m[i][j] == 5){ //se a posicao for "5": inimigo, aumenta em 1 a quantidade
                quantidade++;
            }            
        }
    }
    return quantidade; //retorna o valor da quantidade para a funcao
}

void imprimeMapa(int linha, int coluna, int m[ROW][COL], bool &matrizMapaIncorreta){

    ///Imprime o jogo: mapa e personagem.
    for(int i=0;i<ROW;i++){
        for(int j=0;j<COL;j++){
            if(i==linha && j==coluna){
                if(m[linha][coluna] != 3){ //verifica se o personagem esta em cima de uma bomba
                    cout<<char(2); //imprime personagem char(2) se estiver em "caminho"
                } else{
                    cout<<char(1); //imprime personagem char(1) se estiver em cima de "bomba"
                }
            } else {
                switch (m[i][j]){ //verifica na posicao da matriz do mapa cada elemento para ser substituido
                    case 0: cout<<" "; break; //caminho
                    case 1: cout<<char(219); break; //parede
                    case 2: cout<<char(178); break; //parede fragil
                    case 3: cout<<char(208); break; //bomba
                    case 5: cout<<"@"; break; //inimigo
                    case 7: cout<<char(4); break; //explosao da bomba
                    default: matrizMapaIncorreta = true; break; //caso algum dos valores na matriz m nao for um dos acima, o sistema é interrompido
                    
                } //fimSwitch
            }
        }
        cout<<"\n";
    } //fimImprimeMapa

}

//de acordo com a condicao atingida, imprime no terminal o texto respectivo a situacao
void endingScene(bool personagemAtingido, int qtdeInimigo, bool matrizMapaIncorreta){
    system("cls"); //limpa o terminal e em seguida verifica as condicoes
    if(personagemAtingido == true){
        cout << "VOCE PERDEU!!!!";
    } else if(qtdeInimigo == 0){
        cout << "VOCE GANHOU!!!!";
    } else if(matrizMapaIncorreta == true){
        cout << "ERRO ENCONTRADO: Valor no mapa nao condizente. Gentileza modificar.";
    }
    
}






int main(){ //inicio main

        HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO     cursorInfo;
        GetConsoleCursorInfo(out, &cursorInfo);
        cursorInfo.bVisible = false;
        SetConsoleCursorInfo(out, &cursorInfo);

        short int CX=0, CY=0;
        COORD coord;
        coord.X = CX;
        coord.Y = CY;
        //COMANDOS PARA REPOSICIONAR O CURSOR NO INICIO DA TELA


    //modifica o mapa através dos elementos da matriz - se for alterar o tamanho, modificar os valores ROW e COL
    int m[ROW][COL]={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                     1,0,0,0,0,0,5,0,5,0,0,2,2,0,0,5,0,0,0,2,2,0,0,0,1,
                     1,0,1,0,1,1,0,1,0,1,1,1,1,1,1,1,0,1,0,1,1,0,1,0,1,
                     1,0,0,5,1,1,0,0,2,2,0,2,1,2,2,0,0,0,0,1,1,0,0,5,1,
                     1,0,1,0,2,0,0,1,2,0,2,0,1,2,0,0,0,1,0,0,2,0,1,0,1,
                     1,5,0,2,2,2,0,2,0,0,2,0,1,0,0,0,0,0,0,1,1,0,0,0,1,
                     1,0,1,2,1,1,0,1,0,0,0,0,1,2,2,0,0,1,0,1,1,0,1,0,1,
                     1,0,0,2,1,1,2,0,0,0,5,0,0,2,0,5,0,0,0,1,1,2,0,0,1,
                     1,0,1,5,1,1,0,1,2,0,0,0,1,2,0,0,0,1,0,1,1,0,1,0,1,
                     1,0,0,0,1,1,0,5,2,2,0,0,1,2,0,0,0,0,0,1,1,2,0,2,1,
                     1,0,1,0,2,2,0,1,0,2,0,0,1,0,0,0,0,1,0,0,0,0,1,0,1,
                     1,0,0,0,1,1,0,0,2,0,0,0,1,0,0,0,0,0,0,1,1,0,0,0,1,
                     1,0,1,0,1,1,0,1,0,1,1,1,1,1,1,1,0,1,0,1,1,2,1,0,1,
                     1,0,0,0,0,0,0,0,2,0,0,2,5,0,0,0,0,0,0,0,0,0,0,0,1,
                     1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,};


    //posicao inicial do personagem onde ele ira aparecer no mapa
    int linha=1, coluna=1;
    
    //declaracao de variaveis que serao utilizadas no decorrer do codigo
    char tecla;
    bool bombaMapa = false;
    bool tempo = false;
    bool bombaExplosao = false;
    bool personagemAtingido = false;
    bool inimigoMovimentado = false;
    bool matrizMapaIncorreta = false;
    clock_t inicioBomba, fimBomba;
    clock_t inicioInimigo;
    int qtdeInimigo = quantidadeInimigo(m);
    
    //limpa o terminal antes de iniciar o loop
    system("cls");

    //loop que continua executando nas condições de o personagem não ter sido atingido ou que haja inimigos no mapa
    while(personagemAtingido == false && qtdeInimigo > 0 && matrizMapaIncorreta == false){

        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

        //imprime mapa, personagem, bomba e sua explosao e inimigos
        imprimeMapa(linha, coluna, m, matrizMapaIncorreta);
        

        ///executa os movimentos
         if ( _kbhit() ){ //verifica se uma tecla do jogador for pressionada
            tecla = getch(); //registra a tecla do jogador
            acaoPersonagem(tecla, linha, coluna, m, bombaMapa, tempo, inicioBomba); //realiza uma acao de acordo com a tecla pressionada
            
         }

        //caso o jogador coloque uma bomba, executa a funcao ate dar 2 segundos de diferenca de quando colocou para iniciar a explosao
        if(tempo==true){
            tempoBomba(m, tempo, inicioBomba, fimBomba, bombaExplosao);
            
        }
        //apos 2 segundos com a bomba no chao, torna bombaExplosao == true e executa a animacao de explosao da bomba
        if(bombaExplosao == true){
            animacaoBombaFinal(m, inicioBomba, fimBomba, bombaExplosao, bombaMapa);
        }

        //depois de um inimigo ja ter sido movimentado, torna-se false para os demais inimigos serem movimentados
        if(inimigoMovimentado == false){
            //registra o inimigo do inimigo, para quando a diferenca for de 1 segundo, outro inimigo ser movimentado
            inicioInimigo = clock();
        }
        
        //movimenta os inimigos no mapa "altera a posicao do inimigo"
        acaoInimigo(m, inimigoMovimentado, inicioInimigo);

        //verifica sempre a colisao do jogador no mapa com outras entidades
        verificaPersonagemColisao(m, linha, coluna, personagemAtingido);

        //verifica a quantidade atual de inimigos no mapa
        qtdeInimigo = quantidadeInimigo(m);



    } //fim do laço do jogo
    
    //caso uma das condicoes seja quebrada, executa endingScene
    //basta que ou o personagem seja atingido, ou nao tenha inimigos no mapa, ou algum valor da matriz do mapa esteja divergente ao codigo
    endingScene(personagemAtingido, qtdeInimigo, matrizMapaIncorreta);
    

    return 0;
} //fim main