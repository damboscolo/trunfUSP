#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>
#include <time.h>

//STRUCTS
typedef struct {
    int cod, idade, ms, nivel,legal;
    char nome[20],familia[10],foto[100];
}Carta;
typedef struct{
    char nome[20];
    int codCarta[30];//CARTAS DE CADA JOGADOR
}Jogador;

//VARIAVEIS
int numCartas = 0;
Carta *carta;
Jogador jogador[2];//2 JOGADORES
int cartaEmpatada[20];//cartas empatadas sao guardadas,
                     //jogador vencedor da proxima rodada leva!
int mostrou=0;//mostrou 0 vezes
Carta guardaMostrada[2];// = (Carta*) malloc(2*sizeof(Carta));//guarda jogador 1 e 2

//FUNCOES PARTE GRAFICA
void menu(void);
void menuClick(void);
void sortearCarta(void);
void iniciarJogo(int ganhou);
void ganhadorRodada(int criterio);
void continuarJogo(void);
void creditos(void);
void sair(void);
void gameOver(int perdedor);
void mostracriterioscartas(int criterio);

//FUNCOES
void lercartas(void);
void sorteiacartas();
int ganhador(int criterio);
int reorganizacartas(int ganhador);
int fimjogo(void);
void salvarJogo(int cabouJogo);
void zeraJogo(void);
//--------------------------------------------------------------------------------------------------------------------------
//                                                       MAIN
//--------------------------------------------------------------------------------------------------------------------------
int main(){
    initwindow(959,633,"TrunfUSP");//cria janela
    setusercharsize(200,200,100,100);
    strcpy(jogador[0].nome, "JOGADOR");
    strcpy(jogador[1].nome, "COMPUTADOR");

    menu();//chama menu
    menuClick();//define os botoes do menu
    getch();
}

//--------------------------------------------------------------------------------------------------------------------------
//                                                   PARTE GRAFICA
//--------------------------------------------------------------------------------------------------------------------------
void menu(void){
    readimagefile("menu.jpg",0,0,959,633);
    clearmouseclick(WM_LBUTTONUP);//limpa click mouse
    settextstyle(BOLD_FONT, HORIZ_DIR, 56);
    mostrou=0;
}

void menuClick(void){
    clearmouseclick(WM_LBUTTONUP);//limpa click mouse

    while(1){
        if (mousex()>=33 && mousex()<=334 && mousey()>= 517 && mousey()<=610 && ismouseclick(WM_LBUTTONUP)){//INICIAR JOGO
            zeraJogo();
            sortearCarta();
        }else if (mousex()>=354 && mousex()<=609 && mousey()>= 517 && mousey()<=610 && ismouseclick(WM_LBUTTONUP)){//CONTINUAR JOGO
            lercartas();//Le cartas
            continuarJogo();
            iniciarJogo(0);
        }else if (mousex()>=674 && mousex()<=927 && mousey()>= 517 && mousey()<=610 && ismouseclick(WM_LBUTTONUP)){//CREDITOS
            creditos();
        }else if (mousex()>=880 && mousex()<=943 && mousey()>= 12 && mousey()<=70 && ismouseclick(WM_LBUTTONUP)){//SAIR
            sair();
        }else if (mousex()>=894 && mousex()<=959 && mousey()>= 576 && mousey()<=633 && ismouseclick(WM_LBUTTONUP)){//Voltar
            menu();
            menuClick();
        }else{
            clearmouseclick(WM_LBUTTONUP);
        }
    }
}

void sortearCarta(void){
    cleardevice();//limpa tudo
    readimagefile("sortear.jpg",0,0,959,633);
    clearmouseclick(WM_LBUTTONUP);

    settextstyle(BOLD_FONT, HORIZ_DIR, 56);
    setcolor(YELLOW);
    outtextxy(300, 58, "CLIQUE EM SORTEAR CARTA PARA COMECAR O JOGO");

    while(1){//Configura botoes da tela
        if (mousex()>=401 && mousex()<=552 && mousey()>= 434 && mousey()<=555 && ismouseclick(WM_LBUTTONUP)){//Sortear carta
            lercartas();//Le cartas
            sorteiacartas();//sorteia
            iniciarJogo(0);
        }else if (mousex()>=894 && mousex()<=959 && mousey()>= 576 && mousey()<=633 && ismouseclick(WM_LBUTTONUP)){//Voltar
            menu();
            menuClick();
        }else{
            clearmouseclick(WM_LBUTTONUP);
        }
    }
}

void iniciarJogo(int ganhou){
    int criterio = 0, fim = 0, j, k;

    cleardevice();//limpa tudo
    readimagefile("carta.jpg",0,0,959,633);
    clearmouseclick(WM_LBUTTONUP);
    mostracriterioscartas(criterio);//0
    fim = fimjogo();//verifica se acabou jogo
    gameOver(fim);
    setcolor(YELLOW);

    while(1){//Configura botoes da tela
        if (mousex()>=894 && mousex()<=959 && mousey()>= 576 && mousey()<=633 && ismouseclick(WM_LBUTTONUP)){//Voltar
            salvarJogo(0);
            menu();
            menuClick();
        }else{
            clearmouseclick(WM_LBUTTONUP);
        }

        if(ganhou == 0 || ganhou == 2){//Jogador ganhou ou empatou
            outtextxy(350, 58, "SUA VEZ DE JOGAR,  CLIQUE  EM  UM  ATRIBUTO");
            if (mousex()>=119 && mousex()<=347 && mousey()>= 395 && mousey()<=430 && ismouseclick(WM_LBUTTONUP)){//Idade
                criterio = 1;
                ganhadorRodada(criterio);
            }else if (mousex()>=119 && mousex()<=347 && mousey()>= 437 && mousey()<=472 && ismouseclick(WM_LBUTTONUP)){//idioma
                criterio = 2;
                ganhadorRodada(criterio);
            }else if (mousex()>=119 && mousex()<=347 && mousey()>= 481 && mousey()<=519 && ismouseclick(WM_LBUTTONUP)){//pesquisa
                criterio = 3;
                ganhadorRodada(criterio);
            }else if (mousex()>=119 && mousex()<=347 && mousey()>= 524 && mousey()<=560 && ismouseclick(WM_LBUTTONUP)){//legal
                criterio = 4;
                ganhadorRodada(criterio);
            }else if (mousex()>=894 && mousex()<=959 && mousey()>= 576 && mousey()<=633 && ismouseclick(WM_LBUTTONUP)){//Voltar
                salvarJogo(0);
                menu();
                menuClick();
            }else{
                clearmouseclick(WM_LBUTTONUP);
            }
        }else{//computador ganhou, ele escolhe
            srand (time (NULL));
            criterio = rand()%3+1;//SORTEIA ATE 4 SEM ZERO
            outtextxy(300, 58, "AGUARDE, COMPUTADOR ESTA ESCOLHENDO ATRIBUTO");
            readimagefile("bolinha.jpg",410,350,383,330);//BOLINHAS
            delay(800);
            readimagefile("bolinha.jpg",450,350,423,330);
            delay(800);
            readimagefile("bolinha.jpg",490,350,463,330);
            delay(800);
            ganhadorRodada(criterio);
        }
    }
}

void ganhadorRodada(int criterio){
    int ganhou,j, cartasGanhas=0;
    char *a;
    a = (char*) malloc(sizeof(char)*10);//alloca a

    ganhou = ganhador(criterio);
    cartasGanhas = reorganizacartas(ganhou);
    a = itoa(cartasGanhas, a, 10);

    setcolor(YELLOW);
    printf("\nGANHOU %d\n",ganhou);
    if(ganhou==0){//jogador ganhou
        cleardevice();//limpa tudo
        readimagefile("cartajogadorvenceu.jpg",0,0,959,633);
         outtextxy(350, 58, "VOCÊ VENCEU RODADA!   GANHOU");
        outtextxy(638, 58, a);
        outtextxy(652, 58, "CARTAS");
        clearmouseclick(WM_LBUTTONUP);
    }else if(ganhou==1){//computador ganhou
        cleardevice();//limpa tudo
        readimagefile("cartacompvenceu.jpg",0,0,959,633);
        outtextxy(295, 58, "COMPUTADOR VENCEU RODADA!   GANHOU");
        outtextxy(650, 58, a);
        outtextxy(663, 58, "CARTAS");
        clearmouseclick(WM_LBUTTONUP);
    }else if(ganhou==2){//empatou
        cleardevice();//limpa tudo
        readimagefile("cartaempatou.jpg",0,0,959,633);
        outtextxy(460,  58,  "E M P A T O U");//escreve  letreiro
    }

    mostracriterioscartas(criterio);//1/Mostra de novo, pois mostra carta de computador tbm

    //---
    while(1){//Configura botoes da tela
        if (mousex()>=896 && mousex()<=959 && mousey()>= 578 && mousey()<=633 && ismouseclick(WM_LBUTTONUP)){//Voltar
            salvarJogo(0);
            menu();
            menuClick();
        }else if (mousex()>=402 && mousex()<=552 && mousey()>= 317 && mousey()<=437 && ismouseclick(WM_LBUTTONUP)){//PROXIMA RODADA
            cleardevice();//limpa tudo
            iniciarJogo(ganhou);
        }else{
            clearmouseclick(WM_LBUTTONUP);
        }
    }
}

void mostracriterioscartas(int criterio){
    int i,j,k;
    char *a;
    a = (char*) malloc(sizeof(char)*10);//alloca a

    setbkcolor(BLACK);
    setcolor(YELLOW);
    if(mostrou == 0){//nao passou ainda
        strcpy(guardaMostrada[0].nome, carta[jogador[0].codCarta[0]].nome); //copia para guardaMostrada, o nome da carta
        guardaMostrada[0].idade = carta[jogador[0].codCarta[0]].idade;
        guardaMostrada[0].ms = carta[jogador[0].codCarta[0]].ms;
        guardaMostrada[0].nivel = carta[jogador[0].codCarta[0]].nivel;
        guardaMostrada[0].legal = carta[jogador[0].codCarta[0]].legal;
        strcpy(guardaMostrada[0].foto, carta[jogador[0].codCarta[0]].foto);

        strcpy(guardaMostrada[1].nome, carta[jogador[1].codCarta[0]].nome);
        guardaMostrada[1].idade = carta[jogador[1].codCarta[0]].idade;
        guardaMostrada[1].ms = carta[jogador[1].codCarta[0]].ms;
        guardaMostrada[1].nivel = carta[jogador[1].codCarta[0]].nivel;
        guardaMostrada[1].legal = carta[jogador[1].codCarta[0]].legal;
        strcpy(guardaMostrada[1].foto, carta[jogador[1].codCarta[0]].foto);

        //mostra foto do professor
        readimagefile(guardaMostrada[0].foto,143,202, 327,343);

        //setbkcolor(3);
        setcolor(YELLOW);
        outtextxy(200, 168, guardaMostrada[0].nome);
        a = itoa(guardaMostrada[0].idade, a, 10);//transforma int em string
        outtextxy(310, 404, a);
        a = itoa(guardaMostrada[0].ms, a, 10);//transforma int em string
        outtextxy(313, 404+40, a);
        a = itoa(guardaMostrada[0].nivel, a, 10);//transforma int em string
        outtextxy(313, 449+40, a);
        a = itoa(guardaMostrada[0].legal, a, 10);//transforma int em string
        outtextxy(313, 494+40, a);
        mostrou++;//+1
    }else{
        if(mostrou == 1){//passando pela segunda vez
            //mostra foto do professor
            readimagefile(guardaMostrada[0].foto,143,202, 327,343);
            //setbkcolor(3);
            outtextxy(200, 168, guardaMostrada[0].nome);
            a = itoa(guardaMostrada[0].idade, a, 10);//transforma int em string
            outtextxy(310, 404, a);
            a = itoa(guardaMostrada[0].ms, a, 10);//transforma int em string
            outtextxy(313, 404+40, a);
            a = itoa(guardaMostrada[0].nivel, a, 10);//transforma int em string
            outtextxy(313, 449+40, a);
            a = itoa(guardaMostrada[0].legal, a, 10);//transforma int em string
            outtextxy(313, 494+40, a);

            readimagefile(guardaMostrada[1].foto,630,202, 449,343);
            outtextxy(694, 168, guardaMostrada[1].nome);
            a = itoa(guardaMostrada[1].idade, a, 10);//transforma int em string
            outtextxy(794, 404, a);
            a = itoa(guardaMostrada[1].ms, a, 10);//transforma int em string
            outtextxy(797, 404+40, a);
            a = itoa(guardaMostrada[1].nivel, a, 10);//transforma int em string
            outtextxy(797, 449+40, a);
            a = itoa(guardaMostrada[1].legal, a, 10);//transforma int em string
            outtextxy(797, 494+40, a);

            //PINTA DE AMARELO O ATRIBUTO SELECIONADO
            setbkcolor(YELLOW);
            setcolor(BLACK);
            switch(criterio){
                case 1:
                    readimagefile("idade.jpg",118,393,348,355);
                    readimagefile("idade.jpg",607,393,838,355);
                    a = itoa(guardaMostrada[0].idade, a, 10);//MUDA COR LETRA
                    outtextxy(310, 404, a);
                    a = itoa(guardaMostrada[1].idade, a, 10);
                    outtextxy(794, 404, a);
                    break;
                case 2:
                    readimagefile("idioma.jpg",118,436,348,398);
                    readimagefile("idioma.jpg",607,436,838,398);
                    a = itoa(guardaMostrada[0].ms, a, 10);
                    outtextxy(313, 404+40, a);
                    a = itoa(guardaMostrada[1].ms, a, 10);
                    outtextxy(797, 404+40, a);
                    break;
                case 3:
                    readimagefile("pesquisa.jpg",118,436+43,348,441);
                    readimagefile("pesquisa.jpg",607,436+43,838,441);
                    a = itoa(guardaMostrada[0].nivel, a, 10);
                    outtextxy(313, 449+40, a);
                    a = itoa(guardaMostrada[1].nivel, a, 10);
                    outtextxy(797, 449+40, a);
                    break;
                case 4:
                    readimagefile("legal.jpg",118,479+43,348,484);
                    readimagefile("legal.jpg",607,479+43,838,484);
                     a = itoa(guardaMostrada[0].legal, a, 10);
                    outtextxy(313, 494+40, a);
                    a = itoa(guardaMostrada[1].legal, a, 10);
                    outtextxy(797, 494+40, a);
                    break;
            }
            for(i=0;i<2;i++){
                strcpy(guardaMostrada[i].foto, carta[jogador[i].codCarta[0]].foto);
                strcpy(guardaMostrada[i].nome, carta[jogador[i].codCarta[0]].nome);
                guardaMostrada[i].idade = carta[jogador[i].codCarta[0]].idade;
                guardaMostrada[i].ms = carta[jogador[i].codCarta[0]].ms;
                guardaMostrada[i].nivel = carta[jogador[i].codCarta[0]].nivel;
                guardaMostrada[i].legal = carta[jogador[i].codCarta[0]].legal;
            }
            mostrou=0;//zera
        }
    }
    //SE TIVER CARTAS EMPATADAS, MOSTRA --------
    j=0;//conta numero cartas empatadas
    while(cartaEmpatada[j]!=0){
        j++;
    }
    if(j!=0){//SE TIVER EMPATADO, MOSTRA
        a = itoa(j, a, 10);//transforma int em string
        setcolor(BLACK);
        setbkcolor(WHITE);
        outtextxy(390, 280, a);
        outtextxy(403, 280, "CARTAS EMPATADAS");
    }

    //MOSTRA NA TELA NUMERO DE CARTA JOGADORES --------
    j=0;//conta numero cartas jogador
    while(jogador[0].codCarta[j]!=0){
        j++;
    }
    k=0;//conta numero cartas computador
    while(jogador[1].codCarta[k]!=0){
        k++;
    }
    setcolor(WHITE);
    setbkcolor(BLACK);
    outtextxy(112, 605, jogador[0].nome);
    a = itoa(j, a, 10);//transforma int em string
    outtextxy(270, 605, a);
    outtextxy(290, 605, "CARTAS");

    outtextxy(602, 605, jogador[1].nome);
    a = itoa(k, a, 10);//transforma int em string
    outtextxy(770, 605, a);
    outtextxy(790, 605, "CARTAS");

    free(a);//libera ponteiro
}

void gameOver(int perdedor){
    if(perdedor==0 || perdedor==1){
        cleardevice();//limpa tudo
        if(perdedor==0){//JOGADOR PERDEU
            readimagefile("gameover.jpg",0,0,959,633);
        }else{//JOGADOR VENCEU
            readimagefile("venceu.jpg",0,0,959,633);
        }

        while(1){//Configura botoes da tela
            if (mousex()>=896 && mousex()<=959 && mousey()>= 578 && mousey()<=633 && ismouseclick(WM_LBUTTONUP)){//Voltar
                salvarJogo(1);
                menu();
                menuClick();
            }else{
            clearmouseclick(WM_LBUTTONUP);
            }
        }
    }
}

void creditos(void){
    cleardevice();//limpa tudo
    readimagefile("creditos.jpg",0,0,959,633);
    clearmouseclick(WM_LBUTTONUP);

    while(1){//Configura botoes da tela
        if (mousex()>=894 && mousex()<=959 && mousey()>= 576 && mousey()<=633 && ismouseclick(WM_LBUTTONUP)){//Voltar
            menu();
            menuClick();
        }else{
            clearmouseclick(WM_LBUTTONUP);
        }
    }
}

void sair(void){
    free(carta);//desaloca vetor
    closegraph();
    exit(1);
}

//--------------------------------------------------------------------------------------------------------------------------
//                                                       FUNCOES
//--------------------------------------------------------------------------------------------------------------------------
void lercartas(){
    FILE *f = fopen("cartas.txt", "r");//só lê
    int i=1,tam=1;

    if(f==NULL){//Se arquivo nao existe, finaliza
        printf("Arquivo nao existe");
        return;
    }
    carta = (Carta*) malloc(2*sizeof(Carta));//aloca carta

    //EOF = end of file/ [^/] = lê String até a barra
    while(fscanf(f,"%d/%[^/]/%[^/]/%d/%d/%d/%d/%[^/]/\n", &carta[i].cod, carta[i].nome,&carta[i].familia,&carta[i].idade,&carta[i].ms,&carta[i].nivel,&carta[i].legal, carta[i].foto)!=EOF){
        i++; //quant cartas lidas
        carta = (Carta*) realloc(carta, sizeof(Carta)*(i+1));//aumenta alocacao conforme numero de cartas
    }
    numCartas = i-1;//-1 pois i comeca em 1, carta[1].cod
}

void sorteiacartas(){
    int i,j=0,k,m,l;
    int sorteadas[numCartas];
    srand (time (NULL));//SEMENTE RANDOM

    for(i=0; i<numCartas;i++){
        sorteadas[i] = rand()%numCartas+1;//SORTEIA, SEM O ZERO

        for(k=0; k<i; k++){//Se tiver repetidas, sorteia de novo
            if(sorteadas[i] == sorteadas[k]){
                sorteadas[i] = rand()%numCartas+1;//sorteia novamente
                k=-1;
            }
        }
        printf("\nCartas: %d", sorteadas[i]);
    }

    settextstyle(BOLD_FONT, HORIZ_DIR, 56);
    //DIVIDE CARTAS ENTRE JOGADOR E COMPUTADOR
    printf("\n\n%s",jogador[0].nome);//Cartas jogador
    while(j<(numCartas/2)){
        jogador[0].codCarta[j] = sorteadas[j];//0 até metade Cartas
        printf("\nCartas: %d",jogador[0].codCarta[j]);
        j++;
        //outtextxy(398+j+j, 178+j+j+j, *char(jogador[1].codCarta[i]));
    }
    i=0;
    printf("\n\n%s",jogador[1].nome);//Cartas computador
    while(j<numCartas){
        jogador[1].codCarta[i] = sorteadas[j];//Metade das cartas ate o FIM
        printf("\nCartas2: %d",jogador[1].codCarta[i]);
        j++;
        i++;
    }
}

int ganhador(int criterio){
    switch(criterio){
        case 1: //idade
            if(carta[jogador[0].codCarta[0]].idade == carta[jogador[1].codCarta[0]].idade){
                return 2;//empate
            }else{
                if(carta[jogador[0].codCarta[0]].idade > carta[jogador[1].codCarta[0]].idade){
                    return 0;//jogador venceu
                }else{
                    return 1;//computador venceu
                }
            }
        case 2: //doutor
            if(carta[jogador[0].codCarta[0]].ms == carta[jogador[1].codCarta[0]].ms){
                return 2;//empate
            }else{
                if(carta[jogador[0].codCarta[0]].ms > carta[jogador[1].codCarta[0]].ms){
                    return 0;//jogador venceu
                }else{
                    return 1;//computador venceu
                }
            }
        case 3: //nivel
            if(carta[jogador[0].codCarta[0]].nivel == carta[jogador[1].codCarta[0]].nivel){
               return 2;//empate
            }else{
                if(carta[jogador[0].codCarta[0]].nivel > carta[jogador[1].codCarta[0]].nivel){
                    return 0;//jogador venceu
                }else{
                    return 1;//computador venceu
                }
            }
        case 4: //legal
            if(carta[jogador[0].codCarta[0]].legal == carta[jogador[1].codCarta[0]].legal){
                return 2;//empate
            }else{
                if(carta[jogador[0].codCarta[0]].legal > carta[jogador[1].codCarta[0]].legal){
                    return 0;//jogador venceu
                }else{
                    return 1;//computador venceu
                }
            }
    }
}

int reorganizacartas(int ganhador){
    int i=0,iEmpate=0, cartasGanhas=0;
    if(ganhador!=2){
        while(jogador[ganhador].codCarta[i] != 0){
            i++;//acha ultima posicao das cartas do ganhador
        }

        //GANHADOR: GANHA AS DUAS CARTAS EM JOGO, E JOGA PRO FIM DO VETOR
        jogador[ganhador].codCarta[i] = jogador[0].codCarta[0];//ultima posicao
        jogador[ganhador].codCarta[i+1] = jogador[1].codCarta[0];//ultima +1//VIROU A ULTIMA

        cartasGanhas=2;
        //Se tiver cartas empatadas de rodadas anteriores, jogador recebe todas!
        iEmpate=0;
        while(cartaEmpatada[iEmpate]!=0){
            jogador[ganhador].codCarta[i+2+iEmpate] = cartaEmpatada[iEmpate];//Recebendo cartas na ultima posicao
            cartaEmpatada[iEmpate] = 0;//Zera cartas empatadas
            iEmpate++;//quant cartas empatadas
        }
        //iCartaEmpatada=iEmpate;
        cartasGanhas = cartasGanhas + iEmpate;//quant cartas ganhas
    }else{//EMPATE
        i=0;//Acha fim de cartaEmpatada
        while(cartaEmpatada[i]!=0){
            i++;
        }
        cartaEmpatada[i] = jogador[0].codCarta[0];
        cartaEmpatada[i+1] = jogador[1].codCarta[0];
    }

    //DELETA PRIMEIRAS POSICOES JOGADORES, POIS CARTAS ESTAO GUARDADAS EM cartaEmpatada[]
    jogador[0].codCarta[0] = 0;
    jogador[1].codCarta[0] = 0;
    //REORGANIZA O VETOR, PASSA PRA POSICAO ZERO
   for(i=1; i<=numCartas; i++){
        jogador[0].codCarta[i-1] = jogador[0].codCarta[i];
        jogador[1].codCarta[i-1] = jogador[1].codCarta[i];
    }
    return cartasGanhas;
}

int fimjogo(){
    int i,j,cartaZero=0;

    for(i=0;i<2;i++){//2 jogadores
        for(j=0; j<numCartas; j++){
            if(jogador[i].codCarta[j] == 0){
                cartaZero++;
            }
        }
        if(cartaZero == numCartas){ //Se todas Cartas forem ZERO, jogador i PERDEU!
            return i;//FIM DE JOGO PRA JOGAR i
        }
        cartaZero=0;//zera contador
    }
    return 2;//jogo nao acabou ainda
}

void salvarJogo(int cabouJogo){
    FILE *f = fopen("jogosalvo.txt","w");//WRITE = escreve e cria novo arquivo
    int i=0,j=0, cartaJog=0, cartaComp=0, cartaEmpat=0;
    //guarda cartas jogadores
    if(cabouJogo != 1){//SE NAO ACABOU JOGO, SALVA
        while(jogador[0].codCarta[cartaJog]!=0){
            cartaJog++;
        }
        while(jogador[1].codCarta[cartaComp]!=0){
            cartaComp++;
        }
        while(cartaEmpatada[cartaEmpat]!=0){
            cartaEmpat++;
        }

        fprintf(f,"%d/%d/%d\n", cartaJog,cartaComp,cartaEmpat);//escreve quant cartas jogadores

        for(i=0;i<cartaJog;i++){
            fprintf(f,"%d/", jogador[0].codCarta[i]);//escreve cartas jogador
        }
        fprintf(f, "\n");
        for(i=0;i<cartaComp;i++){
            fprintf(f,"%d/", jogador[1].codCarta[i]);//escreve cartas computador
        }
        fprintf(f,"\n");
        for(i=0;i<cartaEmpat;i++){
            fprintf(f,"%d/", cartaEmpatada[i]);//escreve cartas empatadas
        }
        fclose(f);//fecha arquivo
    }
}

void continuarJogo(void){
    FILE *f = fopen("jogosalvo.txt", "r");//só lê
    int i=0,tam=1, cartaJog=0, cartaComp=0, cartaEmpat=0;;

    if(f==NULL){//Se arquivo nao existe, finaliza
        printf("Arquivo nao existe");
        return;
    }

    zeraJogo();//ZERA CARTAS DO JOGO, DEPOIS COLOCA AS NOVAS

    fscanf(f,"%d/%d/%d\n", &cartaJog, &cartaComp, &cartaEmpat);//LE QUANT CARTAS
    for(i=0;i<cartaJog;i++){
        fscanf(f,"%d/", &jogador[0].codCarta[i]);
    }
    for(i=0;i<cartaComp;i++){
        fscanf(f,"%d/", &jogador[1].codCarta[i]);
    }
    for(i=0;i<cartaEmpat;i++){
        fscanf(f,"%d/", &cartaEmpatada[i]);
    }
}

void zeraJogo(void){
    int i=0;

    //ZERA VETORES
    for(i=0;i<30;i++){
        jogador[0].codCarta[i]=0;
    }
    for(i=0;i<30;i++){
        jogador[1].codCarta[i]=0;
    }
    for(i=0;i<20;i++){
        cartaEmpatada[i]=0;
    }
}


