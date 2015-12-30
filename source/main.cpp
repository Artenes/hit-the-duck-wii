//Estas são as bibliotecas que funfamos no jogo
#include <stdio.h>
#include <gccore.h>
#include <wiiuse/wpad.h>
#include <fat.h>
#include <wiisprite.h>
#include <MLlib.h>
#include <string>
#include "ftimage.h"
#include "font_TTF.h"

//Eis a frescura do c++, não sei porque, mas precisa dessa porra qui
using namespace wsp;
GameWindow gwd;

//Estão são as imagens que pessoas não cegas verão na tela
Image *Water = new Image();
Image *Aim = new Image();
Image *Duck = new Image();
Sprite *water = new Sprite();
Sprite *aim = new Sprite(); 
Sprite *duck[20];

int main () {
	
	//Eis algumas inicializações, pois tudo que acaba, primeiro começa
	fatInitDefault();
	gwd.InitVideo();
	ML_InitConsoleMode();
	ML_InitRand();
	
	//Estamos carregando as imagens do Cartão SD
	Water->LoadImage("water.png");
	Aim->LoadImage("aim.png");
	Duck->LoadImage("duck.png");
	
	//Agora vamos botar as imagens em sprites
	water->SetImage(Water);
	aim->SetImage(Aim);
	for (int i = 0;i <= 19;i++) {
	duck[i] = new Sprite();
	duck[i] -> SetImage(Duck);
	duck[i] -> SetVisible(false);
	}
	
	//Definido a posição do fundo
	water->SetPosition(0,0);
	
	//Umas várias linhas para mexermos com fonte true type
	ftImage print(640, 480);
	Sprite Text;
	print.setFont(font_TTF, font_TTF_size);
	print.setSize(32);
	print.setColor(Color(255,255,255));
	Text.SetPosition(25,10);
	Text.SetImage(&print);
	
	//Criando e ocupando espaço em camanda, só pra tudo fica bunitinho
	LayerManager manager (23);
	manager.Append(aim);
	manager.Append(&Text);
	for (int i = 0; i <= 19;i++) {
		manager.Append(duck[i]);
	}
	manager.Append(water);
	
	//Aqui inicializamos o controle e sua incrivel habilidade de infra-vermelho
	WPAD_Init();
	WPAD_SetDataFormat(WPAD_CHAN_0, WPAD_FMT_BTNS_ACC_IR);
	
	//daqui pra baixo é uma negada de variaveis só para tentar controlar o fluxo do jogo
	//Variaveis gerais
	int score = 0;
	int x,y;
	int visibleduck[20];
	bool firststage = false;
	bool secondstage = false;
	bool thirdstage = false;
	
	//Variaveis usadas na primeira fase
	int nextime = ticks_to_secs(gettime());
	int time = ticks_to_secs(gettime());
	int nextimetohide = ticks_to_secs(gettime());
	int timetohide = ticks_to_secs(gettime());
	int nextimetoshow = ticks_to_secs(gettime());
	int timetoshow = ticks_to_secs(gettime());
	bool begin = true;
	bool defposition = true;
	bool timetoface = false;
	bool timetoflee = false;
	
	//Variaveis usadas na segunda fase
	bool timetomove = false;
	
	//Variaveis usadas na terceira fase
	int nextruntime = ticks_to_secs(gettime());
	int runtime;
	int r = 0;
	int k = 0;
	bool Run = true;
	
		while(1){
		
			//Isso aqui é pra mover o ponteiro com o wiimote
			ir_t ir; 
			WPAD_IR(WPAD_CHAN_0, &ir); 
			aim->SetPosition(ir.sx-WSP_POINTER_CORRECTION_X, ir.sy-WSP_POINTER_CORRECTION_Y);
			aim->Move(-((f32)aim->GetWidth()/2), -((f32)aim->GetHeight()/2));
			aim->SetRotation(ir.angle/2);
			WPAD_ScanPads();
			
			//Isso aqui é pra sair do jogo
			int press = WPAD_ButtonsDown(0);
			if (press & WPAD_BUTTON_HOME) {
				exit(0);
			}
			
			//isso aqui é pra controlar o tempo (demais!) 
			//Variavel "global" - sera usada por todas as fases
			time = ticks_to_secs(gettime());
			
			//Variaveis que serão usada na primeira e segunda fase
			timetohide = ticks_to_secs(gettime());
			timetoshow = ticks_to_secs(gettime());
			
			//Variaveis que serão usada na terceira fase
			runtime = ticks_to_secs(gettime());
			
			//isso começa nosso jogo depois de um tempinho
			if ((time > nextime)&(begin)) {
				nextruntime = runtime + 1;
				firststage = true;
				begin = false;
				nextime = time + 60;
			}
			
//**********isso é o modelo da primeira fase****************************************************************************
			if(firststage){
				//isso aqui ajuda a definir as posições aleatórias dos patos, wow!
				if(defposition){
					
					x = ML_Rand(0,82);
					y = ML_Rand(0,132);
					duck[0] -> SetPosition(x,y);
					duck[0] -> SetVisible(true);
					visibleduck[0] = 1;
					
					x = ML_Rand(147,258);
					y = ML_Rand(0,132);
					duck[1] -> SetPosition(x,y);
					duck[1] -> SetVisible(true);
					visibleduck[1] = 1;
					
					x = ML_Rand(322,240);
					y = ML_Rand(0,132);
					duck[2] -> SetPosition(x,y);
					duck[2] -> SetVisible(true);
					visibleduck[2] = 1;
					
					x = ML_Rand(0,82);
					y = ML_Rand(195,209);
					duck[3] -> SetPosition(x,y);
					duck[3] -> SetVisible(true);
					visibleduck[3] = 1;
					
					x = ML_Rand(147,258);
					y = ML_Rand(196,209);
					duck[4] -> SetPosition(x,y);
					duck[4] -> SetVisible(true);
					visibleduck[4] = 1;
					
					x = ML_Rand(321,396	);
					y = ML_Rand(196,253);
					duck[5] -> SetPosition(x,y);
					duck[5] -> SetVisible(true);
					visibleduck[5] = 1;
					
					defposition = false;
					timetoflee = true;
					nextimetohide = timetohide + 2;
				}
				
				//isso aqui detecta se a mira pegou nos patinhos
				for (int i = 0; i <= 5; i++){
					if ((aim->CollidesWith(duck[i])) && (press & WPAD_BUTTON_B)&&(visibleduck[i])) {
						duck[i] -> SetVisible(false);
						visibleduck[i] = 0;
						score++;
					}
				}
				
				//isso aqui define quando é o tempo para os patos se escafederem
				if ((timetohide>nextimetohide)&&(timetoflee)) {
					for (int i = 0; i <= 5; i++) {
						duck[i] -> SetVisible(false);
						visibleduck[i] = 0;
					}
					nextimetoshow = timetoshow + 2;
					timetoface = true;
					timetoflee = false;
				}
				
				//isso aqui define o momento certo para os patos se mostrarem ao mundo
				if ((timetoshow>nextimetoshow)&&(timetoface)) {
					defposition = true;
					timetoface = false;
				}
				
				//isso aqui é para encerrar a fase, bro!
				if(time>nextime){
					for (int i = 0; i <= 5; i++) {
						duck[i] -> SetVisible(false);
						visibleduck[i] = 0;
					}	
					nextime = time + 60;
					secondstage = true;
					defposition = true;
					timetoface = false;
					timetoflee = false;
					firststage = false;
				}
				
			}
//FIM DA PRIMEIRA FASE *************************************************************************************************
			
//**********Isso é a aquilo chamado de segunda fase*********************************************************************
			if (secondstage){
				
				//Essa porra define as posições randomicas da cambada
				if(defposition){
					for (int i = 0; i <= 19; i++) {
					x = ML_Rand(0,450);
					y = ML_Rand(0,610);
					duck[i] -> SetPosition(x,y);
					duck[i] -> SetVisible(true);
					visibleduck[i] = 1;
					}
					defposition = false;
					timetoflee = true;
					nextimetohide = timetohide + 2;
					timetomove = true;
				}
				
				//Esta aqui vai fazer a cambada correr feito louca, ai!
				if (timetomove) {
					duck[0] -> Move(1,0);
					duck[1] -> Move(0,2);
					duck[2] -> Move(-3,0);
					duck[3] -> Move(0,-4);
					duck[4] -> Move(1,3);
					duck[5] -> Move(-2,-5);
					duck[6] -> Move(3,-2);
					duck[7] -> Move(1,2);
					duck[8] -> Move(0,-3);
					duck[9] -> Move(3,1);
					duck[10] -> Move(2,-2);
					duck[11] -> Move(5,-1);
					duck[12] -> Move(1,1);
					duck[13] -> Move(3,2);
					duck[14] -> Move(1,-1);
					duck[15] -> Move(2,-4);
					duck[16] -> Move(4,2);
					duck[17] -> Move(3,3);
					duck[18] -> Move(1,3);
					duck[19] -> Move(1,-4);
				}
				
				//isso aqui detecta se a mira pegou nos patinhos
				for (int i = 0; i <= 19; i++){
					if ((aim->CollidesWith(duck[i])) && (press & WPAD_BUTTON_B)&&(visibleduck[i])) {
						duck[i] -> SetVisible(false);
						visibleduck[i] = 0;
						score++;
					}
				}
				
				//isso aqui define quando é o tempo para os patos se escafederem
				if ((timetohide>nextimetohide)&&(timetoflee)) {
					for (int i = 0; i <= 19; i++) {
						duck[i] -> SetVisible(false);
						visibleduck[i] = 0;
					}
					nextimetoshow = timetoshow + 2;
					timetoface = true;
					timetoflee = false;
					timetomove = false;
				}
				
				//isso aqui define o momento certo para os patos se mostrarem ao mundo
				if ((timetoshow>nextimetoshow)&&(timetoface)) {
					defposition = true;
					timetoface = false;
				}
				
				//Isso daqui encerra esta fase, LoL!
				if(time>nextime){
					
					for (int i = 0; i <= 19; i++) {
						duck[i] -> SetVisible(false);
						visibleduck[i] = 0;
					}	
					nextime = time + 60;
					thirdstage = true;
					secondstage = false;
				}
				
				
			}
//FIM DA SEGUNDA FASE***************************************************************************************************
			
//**********Isso chamamos de terceira fase, filho!**********************************************************************
			if(thirdstage){
				//Essa aqui delimita as posições e tal	
				if (Run) {
					x = ML_Rand(0,400);
					y = ML_Rand(0,400);
					duck[r] -> SetPosition(x,y);
					duck[r] -> SetVisible(true);
					visibleduck[r] = 1;
					nextruntime = runtime + 1;
					Run = false;
					k = ML_Rand(1,8);
				}
				
				//isso move loucamente o pessoal
				if (!Run) {
					if (k == 1) {
						duck[r] -> Move(0,-10);
					} else if (k == 2) {
						duck[r] -> Move(10,-10);
					} else if (k == 3) {
						duck[r] -> Move(10,0);
					} else if (k == 4) {
						duck[r] -> Move(10,10);
					} else if (k == 5) {
						duck[r] -> Move(0,10);
					} else if (k == 6) {
						duck[r] -> Move(10,10);
					} else if (k == 7) {
						duck[r] -> Move(10,0);
					} else if (k == 8) {
						duck[r] -> Move(10,10);
					}
				}
				//isso detecta se o pato corredor já escafedeu da tela, daí o patinho aparecedor fode legal!1
				if ((duck[r] -> GetX() > 650)||(duck[r] -> GetX() < -68) || (duck[r] -> GetY() < -68)||(duck[r] -> GetY() > 708)) {
					r++;
					duck[r] -> SetVisible(false);
					visibleduck[r] = 0;
					Run = true;
				}
				//isso aqui reinicia a contagem de patinhos
				if (r == 19) {
					r = 0;
				}
				
				//isso aqui detecta se a mira pegou nos patinhos
				for (int i = 0; i <= 19; i++){
					if ((aim->CollidesWith(duck[i])) && (press & WPAD_BUTTON_B)&&(visibleduck[i])) {
						duck[i] -> SetVisible(false);
						visibleduck[i] = 0;
						score++;
					}
				}
				
				//isso encerra a terceira fase
				if(time>nextime){
					
					for (int i = 0; i <= 19; i++) {
						duck[i] -> SetVisible(false);
						visibleduck[i] = 0;
					}	
					nextime = time + 60;
					thirdstage = true;
					secondstage = false;
				}
			}
//FIM DA TERCEIRA FASE**************************************************************************************************
			
			print.printf("Score: %i\n", score);
			print.flush();
			print.clear();
			print.reset();
			manager.Draw(0, 0); 
			gwd.Flush();
		}
		
	return 0;
}