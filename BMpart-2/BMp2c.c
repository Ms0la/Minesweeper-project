/**
 * Implementació en C de la pràctica, per a què tingueu una
 * versió funcional en alt nivell de totes les funcions que heu 
 * d'implementar en assemblador.
 * Des d'aquest codi es fan les crides a les subrutines de assemblador. 
 * AQUEST CODI NO ES POT MODIFICAR I NO S'HA DE LLIURAR.
 **/
 
#include <stdio.h>
#include <termios.h>    //termios, TCSANOW, ECHO, ICANON
#include <unistd.h>     //STDIN_FILENO

/**
 * Constants
 */
#define DIMMATRIX  9     //dimensió de la matriu
#define SIZEMATRIX DIMMATRIX*DIMATRIX //=81


/**
 * Definició de variables globals.
 */
extern int developer;	//Variable declarada en assemblador que indica el nom del programador


/**
 * Definició de les funcions de C.
 */
void  clearscreen_C();
void  gotoxyP2_C(int, int);
void  printchP2_C(char);
char  getchP2_C();

char  printMenuP2_C();
void  printBoardP2_C();

long  countMinesP2_C(char[DIMMATRIX][DIMMATRIX]);
void  showNumMinesP2_C(long);
void  posCursorP2_C(int, int);
void  showMarkP2_C(char[DIMMATRIX][DIMMATRIX], int row, int col);
int   moveCursorP2_C(char, int, int);
long  markMineP2_C(char[DIMMATRIX][DIMMATRIX], int, int, long);
short searchMinesP2_C(char[DIMMATRIX][DIMMATRIX], int, int, char[DIMMATRIX][DIMMATRIX], short);
short checkEndP2_C(char[DIMMATRIX][DIMMATRIX], long, short);

void  printMessageP2_C(short);
void  playP2_C(char[DIMMATRIX][DIMMATRIX], char[DIMMATRIX][DIMMATRIX]);


/**
 * Definició de les subrutines d'assemblador que es criden des de C.
 */
long  countMinesP2(char[DIMMATRIX][DIMMATRIX]);
void  showNumMinesP2(long);
void  posCursorP2(int, int);
void  showMarkP2(char[DIMMATRIX][DIMMATRIX], int row, int col);
int   moveCursorP2(char, int, int);
long  markMineP2(char[DIMMATRIX][DIMMATRIX], int, int, long);
char  searchMinesP2(char[DIMMATRIX][DIMMATRIX], int, int, char[DIMMATRIX][DIMMATRIX], char);
short checkEndP2(char[DIMMATRIX][DIMMATRIX], long, short);
void  playP2(char[DIMMATRIX][DIMMATRIX], char[DIMMATRIX][DIMMATRIX]);


/**
 * Esborrar la pantalla.
 * 
 * Variables globals utilitzades:	
 * Cap
 * 
 * Paràmetres d'entrada : 
 * Cap
 *   
 * Paràmetres de sortida: 
 * Cap
 * 
 * Aquesta funció no es crida des d'assemblador
 * i no hi ha definida una subrutina d'assemblador equivalent.
 */
void clearScreen_C(){
	
    printf("\x1B[2J");
    
}


/**
 * Situar el cursor a una posició de la pantalla.
 * 
 * Variables globals utilitzades:	
 * Cap
 * 
 * Paràmetres d'entrada : 
 * (row_num) : rdi(edi) : Fila de la pantalla on es situa el cursor.
 * (col_num) : rsi(esi) : Columna de la pantalla on es situa el cursor.
 * 
 * Paràmetres de sortida: 
 * Cap
 * 
 * S'ha definit un subrutina en assemblador equivalent 'gotoxyP2' 
 * per a poder cridar aquesta funció guardant l'estat dels registres 
 * del processador. Això es fa perquè les funcions de C no mantenen 
 * l'estat dels registres. Els paràmetres són equivalents.
 */
void gotoxyP2_C(int row_num, int col_num){
	
   printf("\x1B[%d;%dH",row_num,col_num);
   
}


/**
 * Mostrar un caràcter a la pantalla a la posició del cursor.
 * 
 * Variables globals utilitzades:	
 * Cap
 * 
 * Paràmetres d'entrada : 
 * (c) : rdi(dil) : Caràcter a mostrar.
 * 
 * Paràmetres de sortida: 
 * Cap
 * 
 * S'ha definit un subrutina en assemblador equivalent 'printchP2' 
 * per a cridar aquesta funció guardant l'estat dels registres del 
 * processador. Això es fa perquè les funcions de C no mantenen 
 * l'estat dels registres. Els paràmetres són equivalents.
 */
void printchP2_C(char c){
	
   printf("%c",c);
   
}


/**
 * Llegir un caràcter des del teclat sense mostrar-lo a la pantalla
 * i retornar-lo.
 * 
 * Variables globals utilitzades:	
 * Cap
 * 
 * Paràmetres d'entrada : 
 * Cap
 * 
 * Paràmetres de sortida: 
 * (c) : rax(al) : Caràcter llegit des del teclat.
 * 
 * S'ha definit un subrutina en assemblador equivalent 'getchP2' per a
 * cridar aquesta funció guardant l'estat dels registres del processador.
 * Això es fa perquè les funcions de C no mantenen l'estat dels 
 * registres. Els paràmetres són equivalents.
 */
char getchP2_C(){

   char c;   

   static struct termios oldt, newt;

   /*tcgetattr obtenir els paràmetres del terminal
   STDIN_FILENO indica que s'escriguin els paràmetres de l'entrada estàndard (STDIN) sobre oldt*/
   tcgetattr( STDIN_FILENO, &oldt);
   /*es copien els paràmetres*/
   newt = oldt;

   /* ~ICANON per a tractar l'entrada de teclat caràcter a caràcter no com a línia sencera acabada amb /n
      ~ECHO per a què no mostri el caràcter llegit*/
   newt.c_lflag &= ~(ICANON | ECHO);          

   /*Fixar els nous paràmetres del terminal per a l'entrada estàndard (STDIN)
   TCSANOW indica a tcsetattr que canvii els paràmetres immediatament. */
   tcsetattr( STDIN_FILENO, TCSANOW, &newt);

   /*Llegir un caràcter*/
   c=(char)getchar();                 
    
   /*restaurar els paràmetres originals*/
   tcsetattr( STDIN_FILENO, TCSANOW, &oldt);

   /*Retornar el caràcter llegit*/
   return c;
   
}


/**
 * Mostrar a la pantalla el menú del joc i demana una opció.
 * Només accepta una de les opcions correctes del menú ('0'-'9') o ESC.
 * 
 * Variables globals utilitzades:	
 * (developer) :((char *)&developer): variable definida en el codi assemblador.
 * 
 * Paràmetres d'entrada : 
 * Cap
 * 
 * Paràmetres de sortida: 
 * (c) : rax(al) : Caràcter llegit des del teclat
 * 
 * Aquesta funció no es crida des d'assemblador
 * i no hi ha definida una subrutina d'assemblador equivalent.
 */
char printMenuP2_C(){
	char c = ' ';
	
	clearScreen_C();
    gotoxyP2_C(1,1);
    printf("                                     \n");
    printf("            Developed by:            \n");
	printf("         ( %s )   \n",(char *)&developer);
    printf(" ___________________________________ \n");
    printf("|                                   |\n");
    printf("|        MENU MINESWEEPER 2.0       |\n");
    printf("|___________________________________|\n");
    printf("|                                   |\n");
    printf("|          0. countMines            |\n");
    printf("|          1. showNumMines          |\n");
    printf("|          2. posCursor             |\n");
    printf("|          3. showMark              |\n");
    printf("|          4. moveCursor            |\n");
    printf("|          5. markMine              |\n");
    printf("|          6. searchMines           |\n");
    printf("|          7. checkEnd              |\n");
    printf("|          8. Play Game             |\n");
    printf("|          9. Play Game C           |\n");
    printf("|        ESC. Exit                  |\n");
    printf("|___________________________________|\n");
    printf("|                                   |\n");
    printf("|             OPTION:               |\n");
    printf("|___________________________________|\n"); 
    
    while ((c < '0' || c > '9') && c!=27) {
      gotoxyP2_C(22,23);
	  c = getchP2_C();
	}
	return c;
}


/**
 * Mostrar el tauler de joc a la pantalla. Les línies del tauler.
 * 
 * Variables globals utilitzades:	
 * Cap
 * 
 * Paràmetres d'entrada : 
 * Cap
 * 
 * Paràmetres de sortida: 
 * Cap
 * 
 * Aquesta funció es crida des de C i des d'assemblador,
 * i no hi ha definida una subrutina d'assemblador equivalent.
 */
void printBoardP2_C(){

   gotoxyP2_C(1,1);                                         //rowScreen
   printf(" _________________________________________ \n");	//01
   printf("|                     |                   |\n");	//02
   printf("|    MINESWEEPER 2.0  | Mines to Mark: __ |\n");	//03
   printf("|_____________________|___________________|\n");	//04
   printf("|                                         |\n");	//05
   printf("|     0   1   2   3   4   5   6   7   8   |\n");	//06
   printf("|   +---+---+---+---+---+---+---+---+---+ |\n");	//07
   printf("| 0 |   |   |   |   |   |   |   |   |   | |\n");	//08
   printf("|   +---+---+---+---+---+---+---+---+---+ |\n");	//09
   printf("| 1 |   |   |   |   |   |   |   |   |   | |\n");	//10
   printf("|   +---+---+---+---+---+---+---+---+---+ |\n");	//11
   printf("| 2 |   |   |   |   |   |   |   |   |   | |\n");	//12
   printf("|   +---+---+---+---+---+---+---+---+---+ |\n");	//13
   printf("| 3 |   |   |   |   |   |   |   |   |   | |\n");	//14
   printf("|   +---+---+---+---+---+---+---+---+---+ |\n");	//15
   printf("| 4 |   |   |   |   |   |   |   |   |   | |\n");	//16
   printf("|   +---+---+---+---+---+---+---+---+---+ |\n");	//17
   printf("| 5 |   |   |   |   |   |   |   |   |   | |\n");	//18
   printf("|   +---+---+---+---+---+---+---+---+---+ |\n");	//19
   printf("| 6 |   |   |   |   |   |   |   |   |   | |\n");	//20
   printf("|   +---+---+---+---+---+---+---+---+---+ |\n");	//21
   printf("| 7 |   |   |   |   |   |   |   |   |   | |\n");	//22
   printf("|   +---+---+---+---+---+---+---+---+---+ |\n");	//23
   printf("| 8 |   |   |   |   |   |   |   |   |   | |\n");	//24
   printf("|   +---+---+---+---+---+---+---+---+---+ |\n");	//25
   printf("|_________________________________________|\n");	//26
   printf("|                                         |\n");	//27
   printf("|  (m)Mark Mine   (Space)Open  (ESC)Exit  |\n"); //28
   printf("|  (i)Up   (j)Left   (k)Down   (l)Right   |\n"); //29
   printf("|_________________________________________|\n");	//30
   
}


/**
 * Recórrer la matriu (mines), rebuda como a paràmetre, per comptar 
 * el nombres de mines (numMines) y retornar el valor.
 * 
 * Variables globals utilitzades:	
 * Cap.
 * 
 * Paràmetres d'entrada : 
 * (mines)   :rdi(rdi): Adreça de la matriu on hi han les mines.
 * 
 * Paràmetres de sortida: 
 * (numMines):rax(rax): Mines que queden per marcar.
 * 
 * Aquesta funció no es crida des d'assemblador.
 * A la subrutina d'assemblador equivalent 'countMinesP2',  
 * el pas de paràmetres és equivalent.
 */
long countMinesP2_C(char mines[][DIMMATRIX]){
   int i,j;
   long numMines = 0;
   
   for (i=0;i<DIMMATRIX;i++){
	  for (j=0;j<DIMMATRIX;j++){
         if(mines[i][j]=='*') numMines++;
      }
   }
   return numMines;
}


/**
 * Converteix el valor (numMines), mines que queden per marcar,
 * (valor entre 0 i 99) en dos caràcters ASCII. 
 * Si (numMines) és més gran de 99 canviar el valor a 99.
 * S'ha de dividir el valor (numMines) entre 10, el quocient representarà 
 * les desenes (tens) i el residu les unitats (units), i després 
 * s'han de convertir a ASCII sumant 48, caràcter '0'.
 * Si les desenes (tens)==0  mostra un espai ' '.
 * Mostra els dígits (caràcter ASCII) de les desenes a la fila 3, 
 * columna 40 de la pantalla i les unitats a la fila 3, columna 41.
 * Per a posicionar el cursor s'ha de cridar a la funció gotoxyP2_C i 
 * per a mostrar els caràcters a la funció printchP2_C.
 * 
 * Variables globals utilitzades:	
 * Cap
 * 
 * Paràmetres d'entrada : 
 * (numMines) :rdi(rdi): Mines que queden per marcar.
 * 
 * Paràmetres de sortida: 
 * Cap
 * 
 * Aquesta funció no es crida des d'assemblador.
 * A la subrutina d'assemblador equivalent 'showMinesP2',  
 * el pas de paràmetres és equivalent.
 */
void showNumMinesP2_C(long numMines) {
	
   long tens, units;
   char charac;
   
   if(numMines>99) numMines=99;
   
   tens  = numMines/10;//Desenes
   units = numMines%10;//Unitats
   
   if (tens>0) charac = (char)tens + '0';   
   else charac = ' ';
   gotoxyP2_C(3, 40);   
   printchP2_C(charac);
   
   charac = (char)units + '0';
   gotoxyP2_C(3, 41);   
   printchP2_C(charac);

}


/**
 * Posicionar el cursor a la pantalla dins del tauler, en funció de
 * la fila (row) i la columna(col), rebuts com a paràmetre, 
 * posició del cursor dins del tauler.
 * Per a calcular la posició del cursor a pantalla utilitzar 
 * aquestes fórmules:
 * rowScreen=(row*2)+8
 * colScreen=(col*4)+7
 * Per a posicionar el cursor es cridar a la funció gotoxyP2_C.
 * 
 * Variables globals utilitzades:	
 * Cap
 * 
 * Paràmetres d'entrada : 
 * (row) :rdi(edi): Fila on està el cursor dins les matrius mines i marks.
 * (col) :rsi(esi): Columna on està el cursor dins les matrius mines i marks.
 * 
 * Paràmetres de sortida: 
 * Cap
 * 
 * Aquesta funció no es crida des d'assemblador.
 * A la subrutina d'assemblador equivalent 'posCurScreenP1',  
 * el pas de paràmetres és equivalent.
 */
void posCursorP2_C(int row, int col) {

   int rowScreen=(row*2)+8;
   int colScreen=(col*4)+7;
   gotoxyP2_C(rowScreen, colScreen);
   
}


/**
 * Mostrar el caràcter de la matriu (marks) a la posicionar del cursor de
 * la fila (row) i la columna (col), rebuts com a paràmetre, a la pantalla.
 * Per a posicionar el cursor s'ha de cridar a la funció posCursorP2_C.
 * i per a mostrar el caràcter a la funció printchP2_C.
 * 
 * Variables globals utilitzades:	
 * Cap
 * 
 * Paràmetres d'entrada : 
 * (marks):rdi(rdi): Adreça de la matriu amb les mines marcades i les mines de les caselles obertes.
 * (row)  :rsi(esi): Fila on està el cursor dins les matrius mines i marks.
 * (col)  :rdx(edx): Columna on està el cursor dins les matrius mines i marks.
 * 
 * Paràmetres de sortida: 
 * Cap
 * 
 * Aquesta funció no es crida des d'assemblador.
 * A la subrutina d'assemblador equivalent 'showMarkP2',  
 * el pas de paràmetres és equivalent. 
 */
 void showMarkP2_C(char marks[DIMMATRIX][DIMMATRIX], int row, int col) {
   
   posCursorP2_C(row, col);
   
   char c = marks[row][col];
   printchP2_C(c);
   
}


/**
 * Actualitzar la posició del cursor al tauler, que està a la fila (row)
 * i la columna (col), en funció de la tecla premuda (c),
 * rebuts com a parametre.
 * Si es surt fora del tauler no actualitzar la posició del cursor.
 * ( i:amunt, j:esquerra, k:avall, l:dreta)
 * Amunt i avall: ( row--/++ ) 
 * Esquerra i Dreta: ( col--/++ )
 * Retornar l'index que indica la posició del cursor dins la matriu
 * amb la següent fórmula: index = row*DIMMATRIX+col
 * No s'ha de posicionar el cursor a la pantalla.
 *  
 * Variables globals utilitzades:	
 * Cap   
 * 
 * Paràmetres d'entrada : 
 * (c)     :rdi(dil): Caràcter llegit de teclat.
 * (row)   :rsi(esi): Fila on està el cursor dins les matrius mines i marks.
 * (col)   :rdx(edx): Columna on està el cursor dins les matrius mines i marks.
 * 
 * Paràmetres de sortida: 
 * (index) :rax(eax): Índex que indica la posició del cursor dins la matriu.
 * 
 * Aquesta funció no es crida des d'assemblador.
 * A la subrutina d'assemblador equivalent 'moveCursorP2', 
 * el pas de paràmetres és equivalent.
 */
int moveCursorP2_C(char c, int row, int col){
   
   switch(c){
     case 'i': //amunt
         if (row>0) row--;
      break;
      case 'j': //esquerra
         if (col>0) col--;
      break;
      case 'k': //avall
         if (row<DIMMATRIX-1) row++;
      break;
      case 'l': //dreta
		 if (col<DIMMATRIX-1) col++;
      break;  
	}
	int index = row*DIMMATRIX+col;
	return index;
}


/**
 * Marcar/desmarcar una mina a la matriu (marks) a la posició actual
 * del cursor indicada per les variables (row) fila i (col) columna,
 * rebuts com a paràmetre
 * Si en aquella posició de la matriu (marks) hi ha un espai en blanc i 
 * no hem marcat totes les mines, marquem una mina posant una 'M' a la 
 * matriu (marks) i decrementem el nombre de mines que queden per 
 * marcar (numMines), si en aquella posició de la matriu (marks) hi ha 
 * una 'M', posarem un espai (' ') a la matriu (marks) i incrementem 
 * el nombre de mines que queden per marcar (numMines).
 * Si hi ha un altre valor no canviarem res.
 * Retornar el nombre de mines (numMines) que queden per marcar actualitzat .
 * Mostrar el canvi fet a la matriu (marks) cridant la funció showMarkP2_C.
 * 
 * Variables globals utilitzades:	
 * Cap
 * 
 * Paràmetres d'entrada : 
 * (marks)   :rdi(rdi): Adreça de la matriu amb les mines marcades i les mines de les caselles obertes.
 * (row)     :rsi(esi): Fila on està el cursor dins les matrius mines i marks.
 * (col)     :rdx(edx): Columna on està el cursor dins les matrius mines i marks.
 * (numMines):rcx(rcx): Mines que queden per marcar.
 * 
 * Paràmetres de sortida: 
 * (numMines) : rax(ax) : Mines que queden per marcar.
 * 
 * Aquesta funció no es crida des d'assemblador.
 * A la subrutina d'assemblador equivalent 'mineMarkerP2', 
 * el pas de paràmetres és equivalent.
 */
long markMineP2_C(char marks[DIMMATRIX][DIMMATRIX], int row, int col, long numMines) {
	
	if (marks[row][col] == ' ' && numMines > 0) {
		marks[row][col] = 'M';         //Marcar
		numMines--;
	} else {
		if (marks[row][col] == 'M') {
			marks[row][col] = ' ';     //Desmarcar
			numMines++;
		}
	}
	showMarkP2_C(marks, row, col);	
	return numMines;
} 
 

/**
 * Obrir casella. Mirar quantes mines hi ha al voltant de la 
 * posició actual del cursor a la fila (row) i la columna (col)
 * de la matriu (mines), rebuts com a paràmetre.
 * Si a la posició actual de la matriu (marks) hi ha un espai (' ') 
 *   Mirar si a la matriu (mines) hi ha una mina ('*').
 *   Si hi ha una mina canviar l'estat (state) a 3 "Explosió".
 *	 Sinó, comptar quantes mines hi ha al voltant de la posició 
 *     actual i actualitzar la posició de la matriu (marks) amb 
 *     el nombre de mines (caràcter ASCII del valor), per fer-ho, cal 
 *     sumar 48 ('0') al valor.
 * Mostrar el canvi fet a la matriu (marks) cridant la funció showMarkP2_C.
 * Si no hi ha un espai, vol dir que hi ha una mina marcada ('M') o la 
 * casella ja s'ha obert (hi ha el nombre de mines que ja s'ha calculat 
 * anteriorment), no fer res.
 * Retornar l'estat del joc actualitzat (state).
 *  
 * Variables globals utilitzades:	
 * Cap
 * 
 * Paràmetres d'entrada :
 * (marks):rdi(rdi) : Adreça de la matriu amb les mines marcades i les mines de les caselles obertes.
 * (row)  :rsi(esi): Fila on està el cursor dins les matrius mines i marks.
 * (col)  :rdx(edx): Columna on està el cursor dins les matrius mines i marks.
 * (mines):rcx(rcx): Adreça de la matriu amb les mines.
 * (state):r8 (r8w): Estat del joc. 
 * 
 * Paràmetres de sortida: 
 * (state):rax(ax) : Estat del joc. 
 *  
 * Aquesta funció no es crida des d'assemblador.
 * Hi ha una subrutina d'assemblador equivalent 'searchMinesP2', 
 * el pas de paràmetres és equivalent.
 */
short searchMinesP2_C(char marks[DIMMATRIX][DIMMATRIX], int row, int col, char mines[DIMMATRIX][DIMMATRIX], short state) {
	
	int neighbors = 0;
	
	if (marks[row][col]==' ') {
		if (mines[row][col]!=' ') {
			state = 3;
		} else {
			if (row > 0) { 
				if ((col > 0) && (mines[row-1][col-1]=='*')) neighbors++;          //UpLeft
				if (mines[row-1][col]=='*') neighbors++;                           //UpCenter
				if ((col < DIMMATRIX-1) && (mines[row-1][col+1]=='*')) neighbors++;//UpRight
			}
			if ((col > 0) && (mines[row][col-1]=='*')) neighbors++;                //CenterLeft
			if ((col < DIMMATRIX-1) && (mines[row][col+1]=='*')) neighbors++;      //CenterRight
			if (row < DIMMATRIX-1) { 
				if ((col > 0) && (mines[row+1][col-1]=='*')) neighbors++;          //DownLeft
				if (mines[row+1][col]=='*') neighbors++;                           //DownCenter
				if ((col < DIMMATRIX-1) && (mines[row+1][col+1]=='*')) neighbors++;//DownRight
			}
			marks[row][col] = (char)neighbors+'0';
			showMarkP2_C(marks, row, col);
		}
	}
		
	return state;
	
} 


/**
 * Verificar si hem marcat totes les mines (numMines=0) i hem obert o
 * marcat amb una mina a totes les altres caselles i no hi ha cap espai 
 * en blanc (' ') a la matriu (marks), si és així, canviar l'estat 
 * (state) a 2 "Guanya la partida".
 * Retornar l'estat del joc actualitzat (state).
 * 
 * Variables globals utilitzades:	
 * Cap
 * 
 * Paràmetres d'entrada : 
 * (marks)   :rdi(rdi): Adreça de la matriu amb les mines marcades i les mines de les caselles obertes.
 * (numMines) :rsi(rsi): Mines que queden per marcar.
 * (state)   :rdx(dx) : Estat del joc. 
 * 
 * Paràmetres de sortida: 
 * (state)    : rax(alx) : Estat del joc.  
 *  
 * Aquesta funció no es crida des d'assemblador.
 * Hi ha una subrutina d'assemblador equivalent 'checkEndP2', 
 * el pas de paràmetres és equivalent.
 */
short checkEndP2_C(char marks[DIMMATRIX][DIMMATRIX], long numMines, short state) {
	
	short notOpenMarks = 0;
	int i,j;
	
	if (numMines == 0) {
		for (i=0;i<DIMMATRIX;i++){
			for (j=0;j<DIMMATRIX;j++){
				if (marks[i][j] == ' ') notOpenMarks++;
			}
		}
		if (notOpenMarks == 0) {
			state = 2;
		}
	}
	
	return state;
} 


/**
 * Mostrar un missatge a sota del tauler segons el valor de la variable 
 * (state) que es rep com a paràmetre.
 * state: 0: Hem premut la tecla 'ESC' per a sortir del joc.
 * 		  1: Continuem jugant.
 * 		  2: Guanya, s'han marcat totes les mines i s'han obert la resta de posicions.
 * 		  3: Explosió, s'ha obert una mina.
 * S'espera que es premi una tecla per a continuar.
 *  
 * Variables globals utilitzades:	
 * Cap
 * 
 * Paràmetres d'entrada : 
 * (state) : rdi(dil) : Estat del joc. 
 * 
 * Paràmetres de sortida: 
 * Cap
 *   
 * Aquesta funció es crida des de C i des d'assemblador,
 * i no hi ha definida una subrutina d'assemblador equivalent.
 */
void printMessageP2_C(short state) {

   gotoxyP2_C(32,14);
   
   switch(state){
      case 0:
         printf("<<<< EXIT: ESC >>>>");
        break;
      case 2:
         printf("++++ YOU WIN ! ++++");
      break;
      case 3:
         printf("---- BOOOM !!! ----");
      break;
   }
   
   getchP2_C();
   
}
 

/**
 * Joc del Buscamines
 * Funció principal del joc
 * Permet jugar al joc del buscamines cridant totes les funcionalitats.
 *
 * Pseudocodi:
 * Mostrar el tauler de joc (cridar la funció PrintBoardP2_C).
 * Comptar el nombre de mines (numMines) que hi ha a la matriu (mines) 
 * cridant la funció countMinesP2_C.
 * Marcar una mina a la fila (row)=8, columna (col)=8 
 * cridant a la funció markMineP2_C.
 * Mostrar el nombre de mines (numMines) cridant la funció showNumMinesP2_C.
 * 
 * Inicialitzar l'estat del joc, (state=1) per començar a jugar.
 * Fixar la posició inicial del cursor a la fila (row)=5, columna (col)=4.
 * 
 * Mentre (state=1) fer:
 *   Posicionar el cursor a la pantalla dins del tauler, en funció de
 *   la fila (row) i la columna(col) cridant la funci posCursorP2_C.
 *   Llegir una tecla i guardar-la a la variable local (charac) cridant
 *     a la funció getchP2_C.  
 *   Segons la tecla llegida cridarem a la funció corresponent.
 *     - ['i','j','k' o 'l']  cridar a la funció moveCursorP2_C i 
 *                            actualitzar la fila (row) i la columa (col)
 *                            a partir del valor retornar (indexMat).
 *     - 'm'                  cridar a la funció markMineP2_C i 
 *                            la funció showNumMinesP1_C per actualitzar
 *                            el valor de (numMines) al tauler.
 *     - '<espace>'(codi ASCII 32) cridar a la funció searchMinesP2_C.
 *     - '<ESC>'   (codi ASCII 27) posar (state = 0) per a sortir.   
 *   Verificar si hem marcat totes les mines i si hem obert totes  
 *   les caselles cridant a la funció checkEndP2_C.
 * Fi mentre.
 * Sortir:
 * Si s'ha obert una mina (state==3) mostrar totes les mines de 
 * la matriu (mines) cridant la funció showMarkP2_C.
 * Mostrar el missatge de sortida que correspongui cridant a la funció
 * printMessageP2_C.
 * S'acabat el joc.
 * 
 * Variables globals utilitzades:
 * Cap
 * 
 * Paràmetres d'entrada : 
 * (marks):rdi(rdi): Adreça de la matriu amb les mines marcades i les mines de les caselles obertes.
 * (mines) :rsi(rsi): Adreça de la matriu amb les mines.
 * 
 * Paràmetres de sortida: 
 * Cap
 * 
 * Aquesta funció no es crida des d'assemblador.
 * Hi ha una subrutina d'assemblador equivalent 'playP2', 
 * el pas de paràmetres és equivalent.
 */
void playP2_C(char marks[][DIMMATRIX],char mines[][DIMMATRIX]){
		
   long  numMines;// Mines que queden per marcar.
   short state;   // 0: Sortir, hem premut la tecla 'ESC' per a sortir.
                  // 1: Continuem jugant.
                  // 2: Guanyat, s'han marcat totes les mines i hem obert la resta de posicions. 
                  // 3: Explosió, hem obert una mina.
   int  row;
   int  col;	     
   
   printBoardP2_C();
   numMines = countMinesP2_C(mines);
   row=8;
   col=8;
   numMines = markMineP2_C(marks, row, col, numMines); 
   showNumMinesP2_C(numMines);  

   state = 1;
   row=5;
   col=4;

   while (state == 1) {    
	 posCursorP2_C(row, col);
	 
	 char charac = getchP2_C();	  
	
     if (charac>='i' && charac<='l') {  
       int indexMat = moveCursorP2_C(charac, row, col);
       row = indexMat/DIMMATRIX;
       col = indexMat%DIMMATRIX; 
     }
     if (charac=='m') {                 
       numMines = markMineP2_C(marks, row, col, numMines);
       showNumMinesP2_C(numMines); 
     }
     if (charac==' ') {                 
       state = searchMinesP2_C(marks, row, col, mines, state);
     }
     if (charac==27) {                  
       state = 0;
     }
     state = checkEndP2_C(marks, numMines, state);
   }
   if (state==3) {
     for (row=0;row<DIMMATRIX;row++){
       for (col=0;col<DIMMATRIX;col++){
         showMarkP2_C(mines, row, col);  
       }
     }
   }
   printMessageP2_C(state);	 

}


/**
 * Programa Principal
 * 
 * ATENCIÓ: A cada opció es crida a una subrutina d'assemblador.
 * A sota hi ha comentada la funció en C equivalent que us donem feta 
 * per si voleu veure com funciona.
 */
int main(void){   

   // Matriu 9x9 amb les mines (Hi han 20 mines marcades).
   char mines[DIMMATRIX][DIMMATRIX] = { {' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                        {' ','*',' ',' ',' ',' ',' ',' ',' '},
                                        {' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                        {' ',' ',' ','*',' ','*',' ',' ',' '},
                                        {' ','*','*','*','*','*','*','*',' '},
                                        {' ',' ',' ','*',' ','*',' ',' ',' '},
                                        {' ','*','*','*','*','*','*',' ',' '},
                                        {' ',' ',' ','*',' ',' ',' ',' ',' '},
                                        {' ',' ',' ',' ',' ',' ',' ',' ','*'} };

   // Matriu 9x9 on es marcaran les mines.
   char marks[DIMMATRIX][DIMMATRIX] = { {' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                        {' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                        {' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                        {' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                        {' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                        {' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                        {' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                        {' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                        {' ',' ',' ',' ',' ',' ',' ',' ',' '} };

   long  numMines = 0;// Mines que queden per marcar.
   short state    = 1;//Estat del joc.
                      // '0': Sortir, hem premut la tecla 'ESC' per a sortir.
                      // '1': Continuem jugant.
                      // '2': Guanyat, s'han marcat totes les mines i hem obert la resta de posicions. 
                      // '3': Explosió, hem obert una mina.
   int row;
   int col; 
   int op=-1;
   char charac;  
   while (op!=27) {
	  clearScreen_C();
      op = printMenuP2_C();
      switch(op){
		case 27:
        break;
        case '0':  //Comptar les mines de la matriu mines.
          clearScreen_C();
          printBoardP2_C();
          //=======================================================
          numMines = countMinesP2(mines);
          ///numMines = countMinesP2_C(mines);
          //=======================================================
          showNumMinesP2_C(numMines);
          gotoxyP2_C(32,3);
          printf("              Press any key               ");
          getchP2_C();
        break;
        case '1':  //Mostrar en número de mines per marcar.
          clearScreen_C();  
          printBoardP2_C();   
          numMines = 100;
          //=======================================================
          showNumMinesP2(numMines);
          ///showNumMinesP2_C(numMines);
          //=======================================================
          gotoxyP2_C(32,3);
          printf("              Press any key               ");
          getchP2_C();
        break;
        case '2': //Posiciona el cursor dins el tauler.
          clearScreen_C();    
          printBoardP2_C();
          gotoxyP2_C(32,3);
          row=8;
          col=8;
          printf("              Press any key               ");
          //=======================================================
          posCursorP2(row,col);
          ///posCursorP2_C(row,col);  
          //=======================================================
          getchP2_C();
        break;
        case '3': //Mostrar una posició de la matriu marks.
          printf(" %c",op);
          clearScreen_C();    
          printBoardP2_C();
          gotoxyP2_C(32,3);
          printf("              Press any key               ");
          row=8;
          col=8;
          charac=marks[row][col];  
          marks[row][col] = 'M';  
          //=======================================================
          showMarkP2(marks, row, col);
          ///showMarkP2_C(marks, row, col);
          //=======================================================
          marks[row][col]=charac; 
          getchP2_C();
        break;
        case '4':  //Moure el cursor
          clearScreen_C(); 
          printBoardP2_C();
          gotoxyP2_C(32,3);
          printf("Move cursor: i:Up, j:Left, k:Down, l:Right");
          int row = 8;
          int col = 8;    
          posCursorP2_C(row,col);
          charac = getchP2_C();	
	      if (charac >= 'i' && charac <= 'l') { 
		    //===================================================
		  	int indexMat = moveCursorP2(charac, row, col);
            ///int indexMat = moveCursorP2_C(charac, row, col);
		    //===================================================
		    row = indexMat/DIMMATRIX;
            col = indexMat%DIMMATRIX; 
            gotoxyP2_C(32,3);
            printf("              Press any key               ");
            printf("%d", indexMat);
		    posCursorP2_C(row,col);
          } else {
			gotoxyP2_C(32,3);
            printf("             Incorrect option             ");
		  }
          getchP2_C();
        break;
        case '5': //Marcar Mina
          clearScreen_C();  
          printBoardP2_C();
          numMines=20;
          showNumMinesP2_C(numMines);
          gotoxyP2_C(32,3);
          printf("        Mark a Mine: m:mark/unmark        ");
          row=8;
          col=8;
          showMarkP2_C(marks, row, col);     
          posCursorP2_C(row,col);
          charac = getchP2_C();
          if (charac=='m') {
		    //===================================================
           	numMines = markMineP2(marks, row, col, numMines);
            ///numMines = markMineP2_C(marks, row, col, numMines);
			//===================================================
		    showNumMinesP2_C(numMines);
		    gotoxyP2_C(32,3);
            printf("              Press any key               ");
            printf("%ld", numMines);
            getchP2_C();
          } else {
            gotoxyP2_C(32,3);
            printf("             Incorrect option             ");
            getchP2_C();
            gotoxyP2_C(32,3);
            printf("                                          ");
          }
          break;
          case '6': //Comptar quantes mines hi ha al voltant d'una posició
            clearScreen_C();  
            printBoardP2_C();
            gotoxyP2_C(32,3);
            printf("               Press SPACE                ");
            row=5; 
            col=4;//5 BOOM!!!
            posCursorP2_C(row,col);
            charac = getchP2_C();
            if (charac==' ') {
			  //===================================================
			  state = searchMinesP2(marks, row, col, mines, state);
              ///state = searchMinesP2_C(marks, row, col, mines, state);
			  //===================================================
			  if (state==1) {
				gotoxyP2_C(32,3);
                printf("              Press any key               ");
				showMarkP2_C(marks, row, col);
				getchP2_C();
			  } else {
				showMarkP2_C(mines, row, col);
			    printMessageP2_C(state);
			    state=1;
			  }
			} else {
              gotoxyP2_C(29,3);
              printf("             Incorrect option             ");
              getchP2_C();
              gotoxyP2_C(32,3);
              printf("                                          ");
			}
          break;
          case '7': //Verificar si hem marcat totes les mines i obert totes les posicions.
            clearScreen_C(); 
            printBoardP2_C();
            numMines=0;
            showNumMinesP2_C(numMines);
 			//===================================================
            state = checkEndP2(marks, numMines, state);
            ///state = checkEndP2_C(marks, numMines, state);
			//===================================================
			if (state==1) {
			  gotoxyP2_C(32,3);
			  printf("              Not Finished!!              ");
			  getchP2_C();
			} else {
			  printMessageP2_C(state);
			}
         break;
         case '8': //Joc complet en assemblador
           clearScreen_C();
           for (row=0;row<DIMMATRIX;row++){
             for (col=0;col<DIMMATRIX;col++){
               marks[row][col]= ' ';  
             }
           }
           //=======================================================
           playP2(marks, mines);
           //=======================================================
         break;
         case '9': //Joc complet en C
           clearScreen_C();
           for (row=0;row<DIMMATRIX;row++){
             for (col=0;col<DIMMATRIX;col++){
               marks[row][col]= ' ';  
             }
           }
           //=======================================================
           playP2_C(marks, mines);
           //=======================================================
         break;
      }
   }
   printf("\n\n");
   
   return 0;
}
