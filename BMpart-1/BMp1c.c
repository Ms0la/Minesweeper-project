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
#define DIMMATRIX  9    //dimensió de la matriu
#define SIZEMATRIX DIMMATRIX*DIMMATRIX  //=81

/**
 * Definició de variables globals.
 */
extern int developer;	//Variable declarada en assemblador que indica el nom del programador

// Matriu 9x9 amb les mines (Hi han 20 mines marcades).
char mines[DIMMATRIX][DIMMATRIX] = { {' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                     {' ',' ',' ',' ',' ',' ',' ',' ',' '},
                                     {' ','*',' ',' ',' ',' ',' ',' ',' '},
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
                                     
long  numMines; // Mines que queden per marcar.
short state;    // Estat del joc.
                // 0: Sortir, hem premut la tecla 'ESC' per a sortir.
                // 1: Continuem jugant.
                // 2: Guanyat, s'han marcat totes les mines.

char charac;    //Caràcter llegit de teclat i per a escriure a pantalla.
int  rowScreen; //Fila per a posicionar el cursor a la pantalla.
int  colScreen; //Columna per a posicionar el cursor a la pantalla.
int  row;       //Fila on està el cursor dins les matrius mines i marks.
int  col;       //Columna on està el cursor dins les matrius mines i marks.

/**
 * Definició de les subrutines d'assemblador que es criden des de C.
 */
void countMinesP1();
void showNumMinesP1();
void posCursorP1();
void showMarkP1();
void moveCursorP1();
void markMineP1();
void checkEndP1();
void playP1();

/**
 * Definició de les funcions de C.
 */
void clearscreen_C();
void gotoxyP1_C();
void printchP1_C();
void getchP1_C();

void printMenuP1_C();
void printBoardP1_C();

void countMinesP1_C();
void showNumMinesP1_C();
void posCursorP1_C();
void showMarkP1_C();
void moveCursorP1_C();
void markMineP1_C();
void checkEndP1_C();

void printMessageP1_C();
void playP1_C();


/**
 * Esborrar la pantalla
 * 
 * Variables globals utilitzades:	
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
 * (rowScreen) : Fila de la pantalla on es situa el cursor.
 * (colScreen) : Columna de la pantalla on es situa el cursor.
 * 
 * S'ha definit un subrutina en assemblador equivalent 'gotoxyP1' per a 
 * poder cridar aquesta funció guardant l'estat dels registres del 
 * processador.
 * Això es fa perquè les funcions de C no mantenen l'estat dels registres.
 */
void gotoxyP1_C(){
	
   printf("\x1B[%d;%dH",rowScreen,colScreen);
   
}


/**
 * Mostrar un caràcter a la pantalla a la posició del cursor.
 * 
 * Variables globals utilitzades:	
 * (charac) : Caràcter a mostrar.
 * 
 * S'ha definit un subrutina en assemblador equivalent 'printchP1' per a
 * cridar aquesta funció guardant l'estat dels registres del processador.
 * Això es fa perquè les funcions de C no mantenen l'estat dels registres.
 */
void printchP1_C(){

   printf("%c",charac);
   
}


/**
 * Llegir un caràcter des del teclat sense mostrar-lo a la pantalla 
 * i emmagatzemar-lo a la variable (charac).
 * 
 * Variables globals utilitzades:	
 * (charac) : Caràcter llegit des del teclat.
 * 
 * S'ha definit un subrutina en assemblador equivalent 'getchP1' per a
 * cridar aquesta funció guardant l'estat dels registres del processador.
 * Això es fa perquè les funcions de C no mantenen l'estat dels 
 * registres.
 */
void getchP1_C(){

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
   charac = (char) getchar();                 
    
   /*restaurar els paràmetres originals*/
   tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
   
}


/**
 * Mostrar a la pantalla el menú del joc i demana una opció.
 * Només accepta una de les opcions correctes del menú ('0'-'9') o ESC
 * 
 * Variables globals utilitzades:	
 * (developer) : ((char *)&developer): Variable definida en el codi assemblador.
 * (rowScreen) : Fila de la pantalla on es situa el cursor.
 * (colScreen) : Columna de la pantalla on es situa el cursor.
 * (charac)    : Caràcter llegit des del teclat.
 * 
 * Aquesta funció no es crida des d'assemblador
 * i no hi ha definida una subrutina d'assemblador equivalent.
 */
void printMenuP1_C(){
	clearScreen_C();
    rowScreen = 1;
    colScreen = 1;
    gotoxyP1_C();
    printf("                                     \n");
    printf("            Developed by:            \n");
	printf("         ( %s )   \n",(char *)&developer);
    printf(" ___________________________________ \n");
    printf("|                                   |\n");
    printf("|        MENU MINESWEEPER 1.0       |\n");
    printf("|___________________________________|\n");
    printf("|                                   |\n");
    printf("|          0. countMines            |\n");
    printf("|          1. showNumMines          |\n");
    printf("|          2. posCursor             |\n");
    printf("|          3. showMark              |\n");
    printf("|          4. moveCursor            |\n");
    printf("|          5. markMine              |\n");
    printf("|          6. -(new in P2)-         |\n");
    printf("|          7. checkEnd              |\n");
    printf("|          8. Play Game             |\n");
    printf("|          9. Play Game C           |\n");
    printf("|        ESC. Exit                  |\n");
    printf("|___________________________________|\n");
    printf("|                                   |\n");
    printf("|             OPTION:               |\n");
    printf("|___________________________________|\n"); 
    
    charac=' ';
    while ((charac < '0' || charac > '9') && charac!=27) {
      rowScreen = 22;
      colScreen = 23;
      gotoxyP1_C();
	  getchP1_C(); 
	  printchP1_C();
	}
	
}


/**
 * Mostrar el tauler de joc a la pantalla. Les línies del tauler.
 * 
 * Variables globals utilitzades:	
 * (rowScreen) : Fila de la pantalla on es situa el cursor.
 * (colScreen) : Columna de la pantalla on es situa el cursor.
 *  
 * Aquesta funció es crida des de C i des d'assemblador,
 * i no hi ha definida una subrutina d'assemblador equivalent.
 */
void printBoardP1_C(){
	
   rowScreen = 1;
   colScreen = 1;
   gotoxyP1_C();                                            //rowScreen
   printf(" _________________________________________ \n");	//01
   printf("|                     |                   |\n");	//02
   printf("|   MINESWEEPER 1.0   | Mines to Mark: __ |\n");	//03
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
   printf("|  (m)Mark Mine                (ESC)Exit  |\n"); //28
   printf("|  (i)Up   (j)Left   (k)Down   (l)Right   |\n"); //29
   printf("|_________________________________________|\n");	//30
   
}


/**
 * Recórrer la matriu (mines) per comptar el nombres de mines (numMines) que hi ha.
 * 
 * Variables globals utilitzades:	
 * (mines)    : Matriu on hi han les mines.
 * (numMines) : Mines que queden per marcar.
 *  
 * Aquesta funció no es crida des d'assemblador.
 * Hi ha una subrutina d'assemblador equivalent 'countMinesP1'.
 */
void countMinesP1_C(){
   int i,j;
   
   numMines = 0;
   for (i=0;i<DIMMATRIX;i++){
	  for (j=0;j<DIMMATRIX;j++){
         if(mines[i][j]=='*') numMines++;
      }
   }
   
}


/**
 * Converteix el valor del nombre de mines que queden per marcar (numMines)
 * (entre 0 i 99) en dos caràcters ASCII.
 * Si (numMines) és més gran de 99 canviar el valor a 99.
 * S'ha de dividir el valor (numMines) entre 10, el quocient representarà 
 * les desenes (tens) i el residu les unitats (units), 
 * després convertir a ASCII sumant 48, caràcter '0'.
 * Si les desenes (tens)==0  mostra un espai ' '.
 * Mostra els dígits (caràcter ASCII) de les desenes a la fila 3, 
 * columna 40 de la pantalla i les unitats a la fila 3, columna 41.
 * Per a posicionar el cursor es cridar a la funció gotoxyP1_C i per a 
 * mostrar els caràcters a la funció printchP1_C.
 * 
 * Variables globals utilitzades:	
 * (rowScreen) : Fila de la pantalla on es situa el cursor.
 * (colScreen) : Columna de la pantalla on es situa el cursor.
 * (numMines)  : Mines que queden per marcar.
 * (charac)    : Caràcter a escriure a pantalla.
 * 
 * Aquesta funció no es crida des d'assemblador.
 * Hi ha una subrutina en assemblador equivalent 'showMinesP1'. 
 */
 void showNumMinesP1_C() {
	
	if(numMines>99) numMines=99;
	
	long tens  = numMines/10;//Desenes
	long units = numMines%10;//Unitats
	
	
	if (tens>0) charac = (char)tens + '0';   
	else charac = ' ';
	rowScreen = 3;
	colScreen = 40;
	gotoxyP1_C();
	printchP1_C();
	
	charac = (char)units + '0';
	rowScreen = 3;
	colScreen = 41;
	gotoxyP1_C();   
	printchP1_C();
	
}


/**
 * Posicionar el cursor a la pantalla dins del tauler, en funció de
 * la fila (row) i la columna(col), posició del cursor dins del tauler.
 * Per a calcular la posició del cursor a pantalla utilitzar 
 * aquestes fórmules:
 * rowScreen=(row*2)+8
 * colScreen=(col*4)+7
 * Per a posicionar el cursor es cridar a la funció gotoxyP1_C.
 * 
 * Variables globals utilitzades:	
 * (row)      : Fila per a accedir a les matrius mines i marks.
 * (col)      : Columna del cursor dins del tauler.
 * (rowScreen): Fila de la pantalla on es situa el cursor.
 * (colScreen): Columna de la pantalla on es situa el cursor.
 * 
 * Aquesta funció no es crida des d'assemblador.
 * Hi ha una subrutina en assemblador equivalent 'posCurScreenP1',  
 */
void posCursorP1_C() {

   rowScreen=(row*2)+8;
   colScreen=(col*4)+7;
   gotoxyP1_C();
   
}


/**
 * Mostrar el caràcter de la posició indicada per la fila (row) i la
 * columna (col) de la matriu (marks) a la pantalla.
 * Per a posicionar el cursor es cridar a la funció posCursorP1_C 
 * i per a mostrar el caràcter a la funció printchP1_C.
 * 
 * Variables globals utilitzades:	
 * (marks) : Matriu amb les mines marcades.
 * (row)   : Fila del cursor dins del tauler.
 * (col)   : Columna del cursor dins del tauler.
 * (charac): Caràcter a escriure a pantalla.
 * 
 * Aquesta funció no es crida des d'assemblador.
 * Hi ha una subrutina en assemblador equivalent 'showMarkP1',  
 */
void showMarkP1_C() {
   
   posCursorP1_C();
   
   charac = marks[row][col];
   printchP1_C();
   
}


/**
 * Actualitzar la posició del cursor al tauler indicada per les variables
 * (row) fila i (col) columna, en funció a la tecla premuda (charac).
 * Si es surt fora del tauler no actualitzar la posició del cursor.
 * ( i:amunt, j:esquerra, k:avall, l:dreta)
 * Amunt i avall: ( row--/++ ) 
 * Esquerra i Dreta: ( col--/++ ) 
 * No s'ha de posicionar el cursor a la pantalla.
 *  
 * Variables globals utilitzades:	
 * (charac): Caràcter llegit de teclat.
 * (row)   : Fila del cursor dins del tauler.
 * (col)   : Columna del cursor dins del tauler.
 * 
 * Aquesta funció no es crida des d'assemblador.
 * Hi ha una subrutina en assemblador equivalent 'moveCursorP1'. 
 */
void moveCursorP1_C(){
   
   switch(charac){
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
	
}


/**
 * Marcar/desmarcar una mina a la matriu (marks) a la posició actual del
 * cursor indicada per les variables (row) fila i (col) columna.
 * Si en aquella posició de la matriu (marks) hi ha un espai en blanc i 
 * no hem marcat totes les mines, marquem una mina posant una 'M' a la 
 * matriu (marks) i decrementarem el nombre de mines que queden per 
 * marcar (numMines), si en aquella posició de la matriu (marks) hi ha 
 * una 'M', posarem un espai (' ') a la matriu (marks) i incrementarem 
 * el nombre de mines que queden per marcar (numMines).
 * Si hi ha un altre valor no canviarem res.
 * Mostrar el canvi fet a la matriu (marks) cridant la funció showMarkP1.
 * 
 * Variables globals utilitzades:	
 * (marks)   : Matriu amb les mines marcades.
 * (row)     : Fila del cursor dins del tauler.
 * (col)     : Columna del cursor dins del tauler.
 * (numMines): Mines que queden per marcar.
 * 
 * Aquesta funció no es crida des d'assemblador.
 * Hi ha una subrutina en assemblador equivalent 'markMineP1'.
 */
void markMineP1_C() {
    
	if (marks[row][col] == ' ' && numMines > 0) {
		marks[row][col] = 'M';      //Marcar
		numMines--;
	} else {
		if (marks[row][col] == 'M') {
			marks[row][col] = ' ';  //Desmarcar
			numMines++;
		}
	}
	showMarkP1_C();	
} 

  
/**
 * Verificar si hem marcat totes les mines. 
 * Si (numMines==0) canvia l'estat (state=2) "Guanya la partida".
 * 
 * Variables globals utilitzades:	
 * (numMines) : Mines que queden per marcar.
 * (state)    : Estat del joc.
 * 
 * Aquesta funció no es crida des d'assemblador.
 * Hi ha una subrutina en assemblador equivalent 'checkEndP1'.
 */
void checkEndP1_C() {
	
	if (numMines == 0) {
		state = 2;
	}
	
} 


/**
 * Mostra un missatge a sota del tauler segons el valor de la variable 
 * (state).
 * state: 0: Hem premut la tecla 'ESC' per a sortir del joc.
 * 		  1: Continuem jugant.
 * 		  2: Guanya, s'han marcat totes les mines.
 * S'espera que es premi una tecla per a continuar.
 * 
 * Variables globals utilitzades:	
 * (rowScreen) : Fila de la pantalla on es situa el cursor.
 * (colScreen) : Columna de la pantalla on es situa el cursor.
 * (state)     : Estat del joc.
 * 
 * Aquesta funció es crida des de C i des d'assemblador.
 * No hi ha una subrutina en assemblador equivalent.
 */
void printMessageP1_C() {

   rowScreen = 32;
   colScreen = 14;
   gotoxyP1_C();  
   
   switch(state){
      case 0:
         printf("<<<< EXIT: ESC >>>>");
        break;
      case 2:
         printf("++++ YOU WIN ! ++++");
      break;
   }
   
   getchP1_C();
   
}
 

/**
 * Joc del Buscamines.
 * Funció principal del joc.
 * Permet jugar al joc del buscamines cridant totes les funcionalitats.
 *
 * Pseudocodi:
 * Mostrar el tauler de joc (cridar la funció PrintBoardP1_C).
 * Comptar el nombre de mines (numMines) que hi ha a la matriu (mines) 
 * cridant la funció countMinesP1_C.
 * Marcar una mina a la fila (row)=8, columna (col)=8 
 * cridant a la funció markMineP1_C.
 * Mostrar el nombre de mines (numMines) cridant la funció showNumMinesP1_C.
 * 
 * Inicialitzar l'estat del joc, (state=1) per començar a jugar.
 * Fixar la posició inicial del cursor a la fila (row)=5, columna (col)=4.
 * 
 * Mentre (state=1) fer:
 *   Posicionar el cursor a la pantalla dins del tauler, en funció de
 *   la fila (row) i la columna(col) cridant la funció posCursorP1_C.
 *   Llegir una tecla i guardar-la a la variable (charac) cridant
 *     a la funció getchP1_C. 
 *   Segons la tecla llegida cridarem a la funció corresponent.
 *     - ['i','j','k' o 'l']    cridar a la funció moveCursorP1_C.
 *     - 'm'                    cridar a la funció markMineP1_C i 
 *                              la funció showNumMinesP1_C per actualitzar
 *                              el valor de (numMines) al tauler.
 *     - '<ESC>'   (codi ASCII 27) posar (state = 0) per a sortir.   
 *   Verificar si hem marcat totes les mines (numMines==0) cridant a 
 *   la funció checkEndP1_C.
 * Fi mentre.
 * Sortir:
 * Si s'ha obert una mina (state==3) mostrar totes les mines de 
 * la matriu (mines) cridant la funció showMarkP1_C.
 * Mostrar el missatge de sortida que correspongui cridant a la funció
 * printMessageP1_C.
 * S'acaba el joc.

 * Variables globals utilitzades:	
 * (state) : Estat del joc.
 * (row)   : Fila del cursor dins del tauler.
 * (col)   : Columna del cursor dins del tauler.
 * (charac): Caràcter llegit des del teclat i a mostrar.
 * 
 * Aquesta funció no es crida des d'assemblador.
 * Hi ha una subrutina en assemblador equivalent 'playP1'.
 */
void playP1_C(){
  
   printBoardP1_C();
   
   countMinesP1_C(); 	      
   row=8;
   col=8;
   markMineP1_C();             
   showNumMinesP1_C();            
   
   state = 1;
   row=5;
   col=4;	     
      
   while (state == 1) {
	 posCursorP1_C();
     getchP1_C();             
	 if (charac>='i' && charac<='l') { 
       moveCursorP1_C();
     }
     if (charac=='m') {       
       markMineP1_C();
       showNumMinesP1_C();  
     }
     if (charac==27) {        
       state = 0;
     }
     checkEndP1_C();
   }
   
   printMessageP1_C();	 
   
}


/**
 * Programa Principal
 * 
 * ATENCIÓ: A cada opció es crida a una subrutina d'assemblador.
 * A sota hi ha comentada la funció en C equivalent que us donem feta 
 * per si voleu veure com funciona.
 *  */
int main(void){   

   int op=-1;
   while (op!=27) {
	  clearScreen_C();
      printMenuP1_C();
      op = charac;
      switch(op){
        case 27:
        break;
        case '0':  //Comptar les mines de la matriu mines.
          clearScreen_C();  
          printBoardP1_C();
          //=======================================================
          countMinesP1();
          //countMinesP1_C();
          //=======================================================
          showNumMinesP1_C();
          //showNumMinesP1();
          rowScreen = 32;
          colScreen = 3;
          gotoxyP1_C();
          printf("              Press any key               ");
          getchP1_C();
        break;
        case '1':  //Mostrar el número de mines per marcar.
          clearScreen_C();  
          printBoardP1_C();   
          numMines=100;
          //=======================================================
          showNumMinesP1();
          ///showNumMinesP1_C(); 
          //=======================================================
          rowScreen = 32;
          colScreen = 3;
          gotoxyP1_C();
          printf("              Press any key               ");
          getchP1_C();
        break;
        case '2': //Posiciona el cursor dins el tauler.
          clearScreen_C();    
          printBoardP1_C();
          rowScreen = 32;
          colScreen = 3;
          gotoxyP1_C();
          printf("              Press any key               ");
          row=8;
          col=8;
          //=======================================================
          posCursorP1();
          ///posCursorP1_C();  
          //=======================================================
          getchP1_C();
        break;
        case '3': //Mostrar una posició de la matriu marks.
          clearScreen_C();    
          printBoardP1_C();
          rowScreen = 32;
          colScreen = 3;
          gotoxyP1_C();
          printf("              Press any key               ");
          row=8;
          col=8;
          charac=marks[row][col];  
          marks[row][col] = 'M';  
          //=======================================================
          showMarkP1();
          ///showMarkP1_C();  
          //=======================================================
          marks[row][col]=charac; 
          getchP1_C();
        break;
        case '4':  //Moure el cursor.
          clearScreen_C(); 
          printBoardP1_C();
          rowScreen = 32;
          colScreen = 3;
          gotoxyP1_C();
          printf("Move cursor: i:Up, j:Left, k:Down, l:Right");
          row=8;
          col=8;
          posCursorP1_C(); 
          getchP1_C();	
	      if (charac >= 'i' && charac <= 'l') { 
		    //===================================================
		    moveCursorP1();
		    ///moveCursorP1_C();
		   //===================================================
		    rowScreen = 32;
            colScreen = 3;
            gotoxyP1_C();
		    printf("              Press any key               ");
		    posCursorP1_C();
          } else {
			rowScreen = 32;
            colScreen = 3;
            gotoxyP1_C();
			printf("             Incorrect option             ");
		  }
          getchP1_C();
        break;
        case '5': //Marcar Mina.
          clearScreen_C();  
          printBoardP1_C();
          numMines=20;
          showNumMinesP1_C();
          rowScreen = 32;
          colScreen = 3;
          gotoxyP1_C();
          printf("        Mark a Mine: m:mark/unmark        ");
          row=8;
          col=8;
          showMarkP1_C();  
          posCursorP1_C();    
          getchP1_C();
   		  if (charac=='m') {
			//===================================================
            markMineP1();
			///markMineP1_C();
			//===================================================
			showNumMinesP1_C();
			rowScreen = 32;
            colScreen = 3;
            gotoxyP1_C();
		    printf("              Press any key               ");
		    getchP1_C();
		  } else {
			rowScreen = 32;
            colScreen = 3;
            gotoxyP1_C();
			printf("             Incorrect option             ");
			getchP1_C();
			rowScreen = 32;
            colScreen = 3;
            gotoxyP1_C();
			printf("                                          ");
          }
        break;
        case '6': 	     
          rowScreen = 25;
	      colScreen = 0;
          gotoxyP1_C();
          printf(" This option will be developed in P2 ");
          getchP1_C();
        break;
        case '7': //Verificar si hem marcat totes les mines.
          clearScreen_C(); 
          printBoardP1_C();
          numMines=0;
          showNumMinesP1_C();
 		  //===================================================
          checkEndP1();
		  ///checkEndP1_C();
		  //===================================================
		  if (state==1) {
			rowScreen = 32;
            colScreen = 3;
            gotoxyP1_C();
			printf("              Not Finished!!              ");
			getchP1_C();
		  } else {
			printMessageP1_C(state);
		  }
        break;
        
        case '8': //Joc complet en assemblador.
          clearScreen_C();
          for (row=0;row<DIMMATRIX;row++){
            for (col=0;col<DIMMATRIX;col++){
              marks[row][col]= ' ';  
            }
          }
          //=======================================================
          playP1();
          //=======================================================
        break;
        case '9': //Joc complet en C.
          clearScreen_C();
          for (row=0;row<DIMMATRIX;row++){
            for (col=0;col<DIMMATRIX;col++){
              marks[row][col]= ' ';  
            }
          }
          //=======================================================
          playP1_C();
          //=======================================================
        break;
        	 
      }
   }
   printf("\n\n");
   
   return 0;
}
