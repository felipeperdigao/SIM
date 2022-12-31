/*Breno Bonfati Meneses
Felipe Perdigão Andrade
Pedro Henrique Manta
Pietro dos Santos Luzini*/

#include <iostream>
#include <graphics.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#define ESC	27
using namespace std;

struct Item{
	int id;
	int qualidade;
	int cooldown;
};

struct Condicao{
	int val;
	int dur;
};

struct Personagem{
	int HPMaximo;
	int HPAtual;
	int tamanhoBarra;
	int barraHP;
	int corBarra;
	int ataque;
	Condicao* Cond;	//0 = Resistencia | 1 = Fraqueza | 2 = Envenamento | 3 = Fúria
};

struct Jogador{
	Personagem per;
	Item* Itens;
};

struct Inimigo{
	Personagem per;
	char inicial;
	Item comando;
	int* Drops;
};

Item CriarItem(int id){
	Item i;
	i.id = id;
	i.qualidade = 0;
	i.cooldown = 0;
	return i;
}

Condicao AplicarCondicao(int d, int v){
	Condicao c;
	c.dur = d;
	c.val = v;
	return c;
}

Personagem CriarPersonagem(int mHP, int a){
	Personagem p;
	int i;
	p.HPMaximo = mHP;
	p.HPAtual = mHP;
	p.tamanhoBarra = mHP * 4;
	p.barraHP = mHP * 4;
	p.corBarra = WHITE;
	p.ataque = a;
	p.Cond = (Condicao*) malloc(sizeof(Condicao) * 4);
	for(i = 0; i < 4; i++)
		p.Cond[i] = AplicarCondicao(0, 0);
	return p;
}

Jogador CriarJogador(){
	Jogador j;
	int i;
	j.per = CriarPersonagem(30, 5);
	j.Itens = (Item*) malloc(sizeof(Item) * 6);
	j.Itens[0] = CriarItem(0);
	j.Itens[1] = CriarItem(1);
	j.Itens[2] = CriarItem(2);
	for(i = 3; i < 6; i++) j.Itens[i] = CriarItem(-1);
	return j;
}

Inimigo CriarInimigo(char i, int n){
	Inimigo e;
	int tam;
	e.Drops = (int*) malloc(2 * sizeof(int));
	e.inicial = i;
	if(i == 'S'){
		if(n == 0 || n == 1)	e.per = CriarPersonagem(15, 5);
		else if(n == 2)		e.per = CriarPersonagem(20, 5);
		else if(n == 3)		e.per = CriarPersonagem(20, 10);
		else				e.per = CriarPersonagem(25, 10);
		e.comando = CriarItem(0);
		e.Drops[0] = 1;
		e.Drops[1] = 3;
	}else if(i == 'G'){
		if(n == 0 || n == 1) 		e.per = CriarPersonagem(20, 10);
		else if(n == 2 || n == 3)	e.per = CriarPersonagem(25, 15);
		else						e.per = CriarPersonagem(30, 15);
		e.comando = CriarItem(1);
		e.Drops[0] = 0;
		e.Drops[1] = 4;
	}else if(i == 'E'){
		if(n == 0 || n == 1)		e.per = CriarPersonagem(25, 15);
		else if(n == 2)				e.per = CriarPersonagem(25, 20);
		else if(n == 3)				e.per = CriarPersonagem(30, 20);
		else						e.per = CriarPersonagem(30, 25);
		e.comando = CriarItem(2);
		e.Drops[0] = 0;
		e.Drops[1] = 3;
	}else if(i == 'D'){
		e.per = CriarPersonagem(80, 15);
		e.comando = CriarItem(3);
		e.Drops[0] = 0;
		e.Drops[1] = 0;
	}
	return e;
}

void DesenhaVida(Personagem p, int x, int y){
	int i;
	setfillstyle(SOLID_FILL, p.corBarra);
	bar(x, y + (p.HPMaximo * 4 - p.tamanhoBarra), x + 32, y + p.HPMaximo * 4);
	
	for(i = p.HPMaximo * 4 - 1; i >= (p.HPMaximo * 4) - p.barraHP; i--){
		setcolor(COLOR(255 * i / p.HPMaximo / 4, 255 - 255 * i / p.HPMaximo / 4, 0));
		line(x, y + i, x + 32, y + i);
	}
	setlinestyle(SOLID_LINE, 0, 3);
	setcolor(COLOR(100, 100, 100));
	rectangle(x, y, x + 32, y + p.HPMaximo * 4);
}

int CalcularDano(int a, int r, int f){
	int d = a - r + f;
	if(d > 0)	return d;
	else	return 0;
}

int main()  {
	FILE* Arquivo = fopen("assets/dungeon.txt", "r");
	char* dungeon;
	char c;
	int t1, t2;
	int pos = 0;
	int dir = 0;
	int nivel = 0;
	
	void* barra;
	void* barraMascara;
	int tamBarra;
	
	void** nomes;
	void** nomesMascara;
	int tamNomes;
	int IDNome = 0;
	
	void* fundo;
	int tamFundo;
	int tela = 0;	// 0 = inicial | 1 = jogo | 2 = instruções | 3 = game over | 4 = final
	
	void* hud;
	void* hudMascara;
	int tamHud;
	
	/*void* cam;
	void* camMascara;
	int tamCam;
	
	void** camIcon;
	void** camIconMascara;
	int tamIcon;*/
	
	void* colItem;
	void* colItemMascara;
	int tamColItem;
	
	void** seta;
	void** setaMascara;
	int tamSeta;
	
	void* ini;
	void* iniMascara;
	int tamIni;
	
	void** item;
	void** itemMascara;
	int tamItem;
	
	void** num;
	void** numMascara;
	int tamNum;
	
	void** status;
	void** statusMascara;
	int tamStatus;
	
	void* bau;
	void* bauMascara;
	int tamBau;
	
	int i = 0;
	int pg = 1;
	
	int modo = 0;	//0 = exploração || 1 = combate || 2 = tesouro
	int turno = 0;
	int animando = 0;
	
	int xPos, yPos;
	char tecla;
	int comando = 0;
	int comIni = 0;
	int cura = 0;
	int envenenando = 1;
	int* Itens;
	
	Itens = (int*) malloc(2 * sizeof(int));
	Itens[0] = 0;
	Itens[1] = 0;
	
	srand(time(NULL));
	
	fscanf(Arquivo, "%d\n", &t1);
	t2 = (t1 + 1) * t1;
	dungeon = (char*) malloc(sizeof(char) * t2);
	i = 0;
	do{
		c = fgetc(Arquivo);
		if(c != EOF){
			dungeon[i] = c;
			i++;
		}
	}while(c != EOF);
	fclose(Arquivo);
	
	Jogador jogador = CriarJogador();
	Inimigo inimigo;
	int itemID;
	
	initwindow(1280, 720, "S.I.M.");
	
	if (pg == 1) pg = 2; else pg = 1;
	setactivepage(1);
	
	tamFundo = imagesize(0, 0, 1280, 720);
	fundo = malloc(tamFundo);
	readimagefile("assets/telas/inicial.jpg", 0, 0, 1280, 720);
	getimage(0, 0, 1280, 720, fundo);
	
	tamHud = imagesize(0, 0, 808, 128);
	hud = malloc(tamHud);
	hudMascara = malloc(tamHud);
	readimagefile("assets/hud/hud.jpeg", 0, 0, 808, 128);
	getimage(0, 0, 808, 128, hud);
	readimagefile("assets/hud/hud_mascara.jpeg", 0, 0, 808, 128);
	getimage(0, 0, 808, 128, hudMascara);
	
	seta = (void**) malloc(sizeof(void*) * 2);
	setaMascara = (void**) malloc(sizeof(void*) * 2);
	tamSeta = imagesize(0, 0, 64, 64);
	for(i = 0; i < 2; i++){
		seta[i] = malloc(tamSeta);
		setaMascara[i] = malloc(tamSeta);
		readimagefile("assets/hud/seta.jpg", 0, 0, 128, 64);
		getimage(0 + i * 64, 0, 64 + i * 64, 64, seta[i]);
		readimagefile("assets/hud/seta_mascara.jpg", 0, 0, 128, 64);
		getimage(0 + i * 64, 0, 64 + i * 64, 64, setaMascara[i]);
	}
	
	tamColItem = imagesize(0, 0, 264, 128);
	colItem = malloc(tamColItem);
	colItemMascara = malloc(tamColItem);
	readimagefile("assets/hud/coleta_item.jpeg", 0, 0, 264, 128);
	getimage(0, 0, 264, 128, colItem);
	readimagefile("assets/hud/coleta_item_mascara.jpeg", 0, 0, 264, 128);
	getimage(0, 0, 264, 128, colItemMascara);
	
	item = (void**) malloc(sizeof(void*) * 6);
	itemMascara = (void**) malloc(sizeof(void*) * 6);
	tamItem = imagesize(0, 0, 128, 128);
	for(i = 0; i < 6; i++){
		item[i] = malloc(tamItem);
		itemMascara[i] = malloc(tamItem);
		readimagefile("assets/hud/items.jpeg", 0, 0, 768, 128);
		getimage(0 + i * 128, 0, 128 + i * 128, 128, item[i]);
		readimagefile("assets/hud/items_mascara.jpeg", 0, 0, 768, 128);
		getimage(0 + i * 128, 0, 128 + i * 128, 128, itemMascara[i]);
	}
	
	num = (void**) malloc(sizeof(void*) * 5);
	numMascara = (void**) malloc(sizeof(void*) * 5);
	tamNum = imagesize(0, 0, 32, 32);
	for(i = 0; i < 5; i++){
		num[i] = malloc(tamNum);
		numMascara[i] = malloc(tamNum);
		readimagefile("assets/hud/numeros.jpeg", 0, 0, 160, 32);
		getimage(0 + i * 32, 0, 32 + i * 32, 32, num[i]);
		readimagefile("assets/hud/numeros_mascara.jpeg", 0, 0, 160, 32);
		getimage(0 + i * 32, 0, 32 + i * 32, 32, numMascara[i]);
	}
		
	tamBarra = imagesize(0, 0, 220, 720);
	barra = malloc(tamBarra);
	barraMascara = malloc(tamBarra);
	readimagefile("assets/hud/barra_lateral.jpg", 0, 0, 220, 720);
	getimage(0, 0, 220, 720, barra);
	readimagefile("assets/hud/barra_lateral_mascara.jpg", 0, 0, 220, 720);
	getimage(0, 0, 220, 720, barraMascara);
	
	nomes = (void**) malloc(sizeof(void*) * 5);
	nomesMascara = (void**) malloc(sizeof(void*) * 5);
	tamNomes = imagesize(0, 0, 192, 32);
	for(i = 0; i < 5; i++){
		nomes[i] = malloc(tamNomes);
		nomesMascara[i] = malloc(tamNomes);
		readimagefile("assets/hud/nomes.jpg", 0, 0, 192, 160);
		getimage(0, 0 + 32 * i, 192, 32 + 32 * i, nomes[i]);
		readimagefile("assets/hud/nomes_mascara.jpg", 0, 0, 192, 160);
		getimage(0, 0 + 32 * i, 192, 32 + 32 * i, nomesMascara[i]);
	}
	
	status = (void**) malloc(sizeof(void*) * 4);
	statusMascara = (void**) malloc(sizeof(void*) * 4);
	tamStatus = imagesize(0, 0, 128, 32);
	for(i = 0; i < 4; i++){
		status[i] = malloc(tamStatus);
		statusMascara[i] = malloc(tamStatus);
		readimagefile("assets/hud/status.jpeg", 0, 0, 128, 32);
		getimage(0 + i * 32, 0, 32 + i * 32, 32, status[i]);
		readimagefile("assets//hud/status_mascara.jpeg", 0, 0, 128, 32);
		getimage(0 + i * 32, 0, 32 + i * 32, 32, statusMascara[i]);
	}
	
	tamIni = imagesize(0, 0, 512, 448);
	ini = malloc(tamIni);
	iniMascara = malloc(tamIni);
	readimagefile("assets/inimigos/slime.jpg", 0, 0, 512, 448);
	getimage(0, 0, 512, 448, ini);
	readimagefile("assets/inimigos/slime_mascara.jpg", 0, 0, 512, 448);
	getimage(0, 0, 512, 448, iniMascara);
	
	tamBau = imagesize(0, 0, 256, 224);
	bau = malloc(tamBau);
	bauMascara = malloc(tamBau);
	readimagefile("assets/bau.jpg", 0, 0, 256, 224);
	getimage(0, 0, 256, 224, bau);
	readimagefile("assets/bau_mascara.jpg", 0, 0, 256, 224);
	getimage(0, 0, 256, 224, bauMascara);
	
	setvisualpage(2);
	
	
	mciSendString("open .\\assets\\sfx\\hud.mp3 type MPEGVideo alias fundo", NULL, 0, 0);
	mciSendString("open .\\assets\\sfx\\dish_breaking.mp3 type MPEGVideo alias ataque", NULL, 0, 0);
	mciSendString("open .\\assets\\sfx\\dragon_roar.mp3 type MPEGVideo alias chefe", NULL, 0, 0);
	mciSendString("open .\\assets\\sfx\\sword_cut.mp3 type MPEGVideo alias ataqueini", NULL, 0, 0);
	mciSendString("open .\\assets\\sfx\\dragon_battle.mp3 type MPEGVideo alias batalha_chefe", NULL, 0, 0);
	mciSendString("open .\\assets\\sfx\\drink.mp3 type MPEGVideo alias pocao", NULL, 0, 0);
	
	mciSendString("play fundo repeat", NULL, 0, 0);
	
	while(tecla != ESC){
		if (pg == 1) pg = 2; else pg = 1;
		setactivepage(pg);
		
		cleardevice();
		
		putimage(0, 0, fundo, OR_PUT);
		if(tela == 1){
			putimage(0, 0, barraMascara, AND_PUT);
			putimage(1060, 0, barraMascara, AND_PUT);
			putimage(0, 0, barra, OR_PUT);
			putimage(1060, 0, barra, OR_PUT);
			
			DesenhaVida(jogador.per, 40, 80);
			putimage(14, 36, nomesMascara[0], AND_PUT);
			putimage(14, 36, nomes[0], OR_PUT);
			if(modo == 0){
				putimage(356, 480, colItemMascara, AND_PUT);
				putimage(356, 480, colItem, OR_PUT);
				for(i = 0; i < 2; i++){
					putimage(388 + 136 * i, 496, setaMascara[i], AND_PUT);
					putimage(388 + 136 * i, 496, seta[i], OR_PUT);
				}
			}else if(modo == 1){
				for(i = 0; i < 4; i++){
					if(jogador.per.Cond[i].dur > 0){
						putimage(80, 80 + i * 36, statusMascara[i], AND_PUT);
						putimage(80, 80 + i * 36, status[i], OR_PUT);
					}
				}
				
				if(inimigo.per.HPAtual > 0  || animando){	
					DesenhaVida(inimigo.per, 1208, 80);
					putimage(1074, 36, nomesMascara[IDNome], AND_PUT);
					putimage(1074, 36, nomes[IDNome], OR_PUT);
					
					if(inimigo.inicial == 'D'){
						putimage(352, 32, iniMascara, AND_PUT);
						putimage(352, 32, ini, OR_PUT);
					}else{
						putimage(416, 160, iniMascara, AND_PUT);
						putimage(416, 160, ini, OR_PUT);
					}
					
					
					for(i = 0; i < 4; i++){
						if(inimigo.per.Cond[i].dur > 0){
							putimage(1170, 80 + i * 36, statusMascara[i], AND_PUT);
							putimage(1170, 80 + i * 36, status[i], OR_PUT);
						}
					}
				}
				
				putimage(236, 556, hudMascara, AND_PUT);
				putimage(236, 556, hud, OR_PUT);
				for(i = 0; i < 6; i++){
					if(jogador.Itens[i].id != -1){
						putimage(240 + i * 136, 558, itemMascara[i], AND_PUT);
						putimage(240 + i * 136, 558, item[i], OR_PUT);
						if(jogador.Itens[i].cooldown > 0){
							putimage(248 + i * 136, 564, numMascara[jogador.Itens[i].cooldown - 1], AND_PUT);
							putimage(248 + i * 136, 564, num[jogador.Itens[i].cooldown - 1], OR_PUT);
						}
					}
					
				}
			}else if(modo == 2){
				putimage(256, 512, colItemMascara, AND_PUT);
				putimage(256, 512, colItem, OR_PUT);
				
				putimage(480, 256, bauMascara, AND_PUT);
				putimage(480, 256, bau, OR_PUT);
				
				for(i = 0; i < 2; i++){
					putimage(256 + i * 136, 512, itemMascara[Itens[i]], AND_PUT);
					putimage(256 + i * 136, 512, item[Itens[i]], OR_PUT);
				}
			}
		}
		
		
		setvisualpage(pg);
		
		delay(5);
		fflush(stdin);
		if (kbhit()){
			tecla = getch();
			//printf("%d ", tecla);
		}
		
		if(tela == 0){
			if(tecla == 65 || tecla == 97){
				tela = 1;
				readimagefile("assets/telas/dungeon.jpg", 0, 0, 1280, 720);
				getimage(0, 0, 1280, 720, fundo);
				tecla = 0;
				modo = 0;
			}
			if(tecla == 83 || tecla == 115){
				tela = 2;
				readimagefile("assets/telas/instrucoes.jpg", 0, 0, 1280, 720);
				getimage(0, 0, 1280, 720, fundo);
				tecla = 0;
			}
		}else if(tela == 1){
			if(inimigo.per.HPAtual <= 0 && !animando && modo == 1){
				if(inimigo.inicial == 'D'){
					tela = 4;
					readimagefile("assets/telas/final.jpg", 0, 0, 1280, 720);
					getimage(0, 0, 1280, 720, fundo);
					tecla = 0;
					mciSendString("stop batalha_chefe", NULL, 0, 0);
					mciSendString("play fundo", NULL, 0, 0);
				}else{
					mciSendString("stop batalha", NULL, 0, 0);
					mciSendString("play fundo", NULL, 0, 0);
					turno = 0;
					modo = 2;
					if(nivel < 5)	nivel += 1;
					jogador.per.HPMaximo += 5;
					jogador.per.HPAtual = jogador.per.HPMaximo;
					jogador.per.tamanhoBarra = jogador.per.HPAtual * 4;
					animando = 1;
					Itens = inimigo.Drops;
					for(i = 0; i < 6; i++)
						jogador.Itens[i].cooldown = 0;
					for(i = 0; i < 4; i++){
						jogador.per.Cond[i] = AplicarCondicao(0, 0);
					}
					tecla = 0;
				}
				
			}
			
			if(modo == 0){
				if(tecla == 65 || tecla == 97 || tecla == 83 || tecla == 115){
					if(tecla == 65 || tecla == 97){
						dir = 1;
					}else if(tecla == 83 || tecla == 115){
						dir = t1 + 1;
					}
					comando = 1;
				}
				if(comando){
					pos += dir;
					if(dungeon[pos] == 'A'){
						jogador.per.HPAtual *= 0.85;
						jogador.per.barraHP = jogador.per.HPAtual * 4;
						animando = 1;
					}else if(dungeon[pos] == 'I'){
						modo = 2;
						do{
							Itens[1] = rand() % 6;
						}while(Itens[0] == Itens[1]);
					}else{
						free(inimigo.per.Cond);
						inimigo = CriarInimigo(dungeon[pos], nivel);
						modo = 1;
						mciSendString("stop fundo", NULL, 0, 0);
						mciSendString("play batalha", NULL, 0, 0);
						if(dungeon[pos] == 'S'){
							readimagefile("assets/inimigos/slime.jpg", 0, 0, 384, 336);
							getimage(0, 0, 384, 336, ini);
							readimagefile("assets/inimigos/slime_mascara.jpg", 0, 0, 384, 336);
							getimage(0, 0, 384, 336, iniMascara);
							IDNome = 1;
						}else if(dungeon[pos] == 'G'){
							readimagefile("assets/inimigos/goblin.jpg", 0, 0, 384, 336);
							getimage(0, 0, 384, 336, ini);
							readimagefile("assets/inimigos/goblin_mascara.jpg", 0, 0, 384, 336);
							getimage(0, 0, 384, 336, iniMascara);
							IDNome = 2;
						}else if(dungeon[pos] == 'E'){
							readimagefile("assets/inimigos/esqueleto.jpg", 0, 0, 384, 336);
							getimage(0, 0, 384, 336, ini);
							readimagefile("assets/inimigos/esqueleto_mascara.jpg", 0, 0, 384, 336);
							getimage(0, 0, 384, 336, iniMascara);
							IDNome = 3;
						}
					}
					comando = 0;
					turno = 0;
					tecla = 0;
				}
			}else if(modo == 1){
				if(!animando){
					//seleção de comandos do jogador
					if(turno == 0 && jogador.per.HPAtual > 0){
						if(tecla == 81 || tecla == 113 || tecla == 87 || tecla == 119 || tecla == 69 || tecla == 101 ||
							tecla == 65 || tecla == 97 || tecla == 83 || tecla == 115 || tecla == 68 || tecla == 100){
							if(tecla == 81 || tecla == 113){
								itemID = 0;
							}else if(tecla == 87 || tecla == 119){
								itemID = 1;
							}else if(tecla == 69 || tecla == 101){
								itemID = 2;
							}else if(tecla == 65 || tecla == 97){
								itemID = 3;
							}else if(tecla == 83 || tecla == 115){
								itemID = 4;
							}else if(tecla == 68 || tecla == 100){
								itemID = 5;
							}
							comando = 1;
						}
						if(comando && jogador.Itens[itemID].id != -1 && jogador.Itens[itemID].cooldown == 0){	//ataque básico
							if(jogador.Itens[itemID].id == 0){
								inimigo.per.HPAtual -= CalcularDano(jogador.per.ataque, inimigo.per.Cond[0].val, inimigo.per.Cond[1].val);
								inimigo.per.barraHP = inimigo.per.HPAtual * 4;
								mciSendString("stop ataque", NULL, 0, 0);
								mciSendString("play ataque", NULL, 0, 0);
							}else if(jogador.Itens[itemID].id == 1){	//cura
								cura = jogador.per.HPMaximo * (0.5 + 0.2 * jogador.Itens[itemID].qualidade);
								jogador.per.HPAtual = (jogador.per.HPAtual + cura > jogador.per.HPMaximo) ?
															jogador.per.HPMaximo : jogador.per.HPAtual + cura;
								jogador.per.tamanhoBarra = jogador.per.HPAtual * 4;
								jogador.Itens[itemID].cooldown = -1;
								jogador.per.corBarra = YELLOW;
								mciSendString("stop pocao", NULL, 0, 0);
								mciSendString("play pocao", NULL, 0, 0);
							}else if(jogador.Itens[itemID].id == 2){	//defesa
								jogador.per.Cond[0] = AplicarCondicao(10 * (jogador.Itens[itemID].qualidade + 1), 2);
								jogador.Itens[itemID].cooldown = 2;
								delay(100);
							}else if(jogador.Itens[itemID].id == 3){	//envenenamento
								inimigo.per.Cond[2] = AplicarCondicao(5, 3 + jogador.Itens[itemID].qualidade);
								jogador.Itens[itemID].cooldown = 5;
								mciSendString("stop ataque", NULL, 0, 0);
								mciSendString("play ataque", NULL, 0, 0);
								delay(100);
							}else if(jogador.Itens[itemID].id == 4){	//fúria
								jogador.per.Cond[3] = AplicarCondicao(5 + 10 * jogador.Itens[itemID].qualidade, 3);
								jogador.Itens[itemID].cooldown = 5;
								mciSendString("stop pocao", NULL, 0, 0);
								mciSendString("play pocao", NULL, 0, 0);
								delay(100);
							}else if(jogador.Itens[itemID].id == 5){	//panacea
								jogador.per.Cond[1] = AplicarCondicao(0, 0);
								jogador.per.Cond[2] = AplicarCondicao(0, 0);
								jogador.Itens[itemID].cooldown = 5 - jogador.Itens[itemID].qualidade;
								mciSendString("stop pocao", NULL, 0, 0);
								mciSendString("play pocao", NULL, 0, 0);
								delay(100);
							}
							animando = 1;
							tecla = 0;
							comando = 0;
							for(i = 0; i < 4; i++){
								if(inimigo.per.Cond[i].dur > 0)		inimigo.per.Cond[i].dur--;
								if(inimigo.per.Cond[i].dur == 0)	inimigo.per.Cond[i].val = 0;
							}
							if(inimigo.comando.cooldown > 0)	inimigo.comando.cooldown--;
						}
					}
					
					if(turno == 1 && inimigo.per.HPAtual > 0){
						if(inimigo.inicial == 'D' && comIni == 1){
							comIni = 1;
						}else{
							comIni = rand() % 2;
							if(comIni == 1 && inimigo.comando.cooldown > 0)	comIni = 0; //seleção de comando do inimigo
						}
						if(comIni == 0){ //ataque básico
							jogador.per.HPAtual -= CalcularDano(inimigo.per.ataque, jogador.per.Cond[0].val, jogador.per.Cond[1].val);
							jogador.per.barraHP = jogador.per.HPAtual * 4;
							mciSendString("stop ataqueini", NULL, 0, 0);
							mciSendString("play ataqueini", NULL, 0, 0);
						}else{
							if(inimigo.comando.id == 0){ //reduçaõ de defesa
								jogador.per.Cond[1] = AplicarCondicao(5, 2);
								inimigo.comando.cooldown = 2;
								//delay(100);
							}else if(inimigo.comando.id == 1){ //envenenamento
								jogador.per.Cond[2] = AplicarCondicao(5, 4);
								inimigo.comando.cooldown = 3;
								//delay(500);
							}else if(inimigo.comando.id == 2){ //defesa
								inimigo.per.Cond[0] = AplicarCondicao(10, 2);
								inimigo.comando.cooldown = 3;
								//delay(500);
							}else if(inimigo.comando.id == 3){ // carga do dragão
								inimigo.comando.id = 4;
								//delay(500);
							}else if(inimigo.comando.id == 4){
								jogador.per.HPAtual -= CalcularDano(inimigo.per.ataque * 1.5, jogador.per.Cond[0].val, jogador.per.Cond[1].val);
								jogador.per.barraHP = jogador.per.HPAtual * 4;
								inimigo.comando.id = 5;
								inimigo.per.Cond[1] = AplicarCondicao(10, 1);
							}else if(inimigo.comando.id == 5){
								inimigo.comando.id = 3;
								inimigo.comando.cooldown = 2;
								//delay(500);
							}
							
						}
						
						animando = 1;
						for(i = 0; i < 6; i++){
							if(jogador.Itens[i].cooldown > 0) 	jogador.Itens[i].cooldown--;
						}
						for(i = 0; i < 4; i++){
							if(jogador.per.Cond[i].dur > 0)		jogador.per.Cond[i].dur--;
							if(jogador.per.Cond[i].dur == 0)	jogador.per.Cond[i].val = 0;
						}
						delay(100);
					}
				}
			}else if(modo == 2){
				if(!animando){
					if(tecla == 65 || tecla == 97 || tecla == 83 || tecla == 115){
						comando = 1;
						if(tecla == 65 || tecla == 97){
							itemID = Itens[0];
						}else if(tecla == 83 || tecla == 115){
							itemID = Itens[1];
						}
					}
					if(comando){
						if(jogador.Itens[itemID].qualidade >= 2){
							do{
								itemID = rand() % 6;
							}while(jogador.Itens[itemID].qualidade < 2);
						}else{
							if(itemID == 0){
								jogador.per.ataque += 5;
							}else{
								if(jogador.Itens[itemID].id == -1)	jogador.Itens[itemID].id = itemID;
								else								jogador.Itens[itemID].qualidade++;
							}
						}
						if(nivel == 5){
							modo = 1;
							free(inimigo.Drops);
							free(inimigo.per.Cond);
							inimigo = CriarInimigo('D', 0);
							readimagefile("assets/inimigos/dragao.jpg", 0, 0, 512, 448);
							getimage(0, 0, 512, 448, ini);
							readimagefile("assets/inimigos/dragao_mascara.jpg", 0, 0, 512, 448);
							getimage(0, 0, 512, 448, iniMascara);
							mciSendString("play chefe", NULL, 0, 0);
							mciSendString("stop batalha", NULL, 0, 0);
							mciSendString("play batalha_chefe", NULL, 0, 0);
							IDNome = 4;
						}else{
							modo = 0;
						}
						tecla = 0;
						comando = 0;
					}
				}
				
				
			}
			
			if(animando){
				if(turno == 1 || modo == 0){
					if(jogador.per.HPAtual * 4 < jogador.per.tamanhoBarra && jogador.per.tamanhoBarra > 0){
						jogador.per.tamanhoBarra--;
					}else{
						tecla = 0;
						if(envenenando && jogador.per.Cond[2].dur > 0){
							jogador.per.HPAtual = jogador.per.HPAtual - jogador.per.Cond[2].val > 0?
													jogador.per.HPAtual - jogador.per.Cond[2].val : 0;
							jogador.per.barraHP = jogador.per.HPAtual * 4;
							jogador.per.corBarra = COLOR(150, 0, 150);
							envenenando = 0;
						}else{
							turno = 0;
							animando = 0;
							envenenando = 1;
							jogador.per.corBarra = WHITE;
						}
					}
				}else if(turno == 0){
					if(inimigo.per.HPAtual * 4 < inimigo.per.tamanhoBarra && inimigo.per.tamanhoBarra > 0){
						inimigo.per.tamanhoBarra--;
					}
					if(jogador.per.HPAtual * 4 > jogador.per.barraHP){
						jogador.per.barraHP++;
					}if((inimigo.per.HPAtual * 4 >= inimigo.per.tamanhoBarra || inimigo.per.tamanhoBarra <= 0) && jogador.per.HPAtual * 4 <= jogador.per.barraHP){
						if(envenenando && inimigo.per.Cond[2].dur > 0){
							inimigo.per.HPAtual = inimigo.per.HPAtual - inimigo.per.Cond[2].val > 0?
												inimigo.per.HPAtual - inimigo.per.Cond[2].val : 0;
							inimigo.per.barraHP = inimigo.per.HPAtual * 4;
							inimigo.per.corBarra = COLOR(150, 0, 150);
							envenenando = 0;
						}else{
							animando = 0;
							tecla = 0;
							if(modo == 1)	turno = 1;
							envenenando = 1;
							inimigo.per.corBarra = WHITE;
							jogador.per.corBarra = WHITE;
						}
					}
				}
			}
			
			if(jogador.per.HPAtual <= 0){
				tela = 3;
				readimagefile("assets/telas/game_over.jpg", 0, 0, 1280, 720);
				getimage(0, 0, 1280, 720, fundo);
				tecla = 0;
				mciSendString("stop batalha", NULL, 0, 0);
				mciSendString("stop batalha_chefe", NULL, 0, 0);
				mciSendString("play fundo", NULL, 0, 0);
			}
		}else if(tela == 2){
			if(tecla == 65 || tecla == 97){
				tela = 1;
				readimagefile("assets/telas/dungeon.jpg", 0, 0, 1280, 720);
				getimage(0, 0, 1280, 720, fundo);
				tecla = 0;
				modo = 0;
				free(jogador.Itens);
				free(jogador.per.Cond);
				jogador = CriarJogador();
			}else if(tecla == 83 || tecla == 115){
				tela = 0;
				readimagefile("assets/telas/inicial.jpg", 0, 0, 1280, 720);
				getimage(0, 0, 1280, 720, fundo);
				tecla = 0;
			}
		}else if(tela == 3 || tela == 4){
			if(tecla != ESC && tecla != 0){
				tela = 0;
				readimagefile("assets/telas/inicial.jpg", 0, 0, 1280, 720);
				getimage(0, 0, 1280, 720, fundo);
				tecla = 0;
				nivel = 0;
				pos = 0;
				free(jogador.Itens);
				free(jogador.per.Cond);
				jogador = CriarJogador();
			}
		}
		
		
		
		
		delay(5);
	}
	
	
	mciSendString("stop fundo", NULL, 0, 0);
	
	free(jogador.Itens);
	free(jogador.per.Cond);
	free(inimigo.per.Cond);
	free(ini);
	free(iniMascara);
	free(barra);
	free(barraMascara);
	free(hud);
	free(hudMascara);
	for(i = 0; i < 5; i++){
		free(nomes[i]);
		free(nomesMascara[i]);
	}
	free(nomes);
	free(nomesMascara);
	free(colItem);
	free(colItemMascara);
	for(i = 0; i < 2; i++){
		free(seta[i]);
		free(setaMascara[i]);
	}
	free(seta);
	free(setaMascara);
	for(i = 0; i < 6; i++){
		free(item[i]);
		free(itemMascara[i]);
	}
	free(item);
	free(itemMascara);
	for(i = 0; i < 5; i++){
		free(num[i]);
		free(numMascara[i]);
	}
	free(num);
	free(numMascara);
	for(i = 0; i < 4; i++){
		free(status[i]);
		free(statusMascara[i]);
	}
	free(status);
	free(statusMascara);
	free(fundo);
	free(bau);
	free(bauMascara);
	free(dungeon);
	closegraph();
	return 0; 
}

