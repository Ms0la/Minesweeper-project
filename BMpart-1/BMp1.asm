section .note.GNU-stack noalloc noexec nowrite progbits
section .data               
;Canviar Nom i Cognom per les vostres dades.
developer db "_Martí_ _Solà_",0

;Constants que també estan definides en C.
DIMMATRIX    equ 9
SIZEMATRIX   equ 81

section .text            
;Variables definides en Assemblador.
global developer     
                         
;Subrutines d'assemblador que es criden des de C.
global countMinesP1, showNumMinesP1, posCursorP1, showMarkP1
global moveCursorP1, markMineP1, checkEndP1, playP1	  

;Variables globals definides en C.
extern mines, marks, numMines, state, 
extern charac, rowScreen, colScreen, row, col

;Funcions de C que es criden des de assemblador
extern gotoxyP1_C, getchP1_C, printchP1_C
extern printBoardP1_C, printMessageP1_C	

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; ATENCIÓ: Recordeu que en assemblador les variables i els paràmetres 
;;   de tipus 'char' s'han d'assignar a registres de tipus  
;;   BYTE (1 byte): al, ah, bl, bh, cl, ch, dl, dh, sil, dil, ..., r15b
;;   les de tipus 'short' s'han d'assignar a registres de tipus 
;;   WORD (2 bytes): ax, bx, cx, dx, si, di, ...., r15w
;;   les de tipus 'int' s'han d'assignar a registres de tipus 
;;   DWORD (4 bytes): eax, ebx, ecx, edx, esi, edi, ...., r15d
;;   les de tipus 'long' s'han d'assignar a registres de tipus 
;;   QWORD (8 bytes): rax, rbx, rcx, rdx, rsi, rdi, ...., r15
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Les subrutines en assemblador que heu d'implementar són:
;;   countMinesP1, showNumMinesP1,  posCursorP1, showMarkP1 
;;   moveCursorP1, markMineP1, checkEndP1.
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Aquesta subrutina es dóna feta. NO LA PODEU MODIFICAR.
; Situar el cursor a la fila indicada per la variable (rowScreen) i a 
; la columna indicada per la variable (colScreen) de la pantalla,
; cridant la funció gotoxyP1_C.
; 
; Variables globals utilitzades:   
; (rowScreen): Fila de la pantalla on posicionem el cursor.
; (colScreen): Columna de la pantalla on posicionem el cursor.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
gotoxyP1:
   push rbp
   mov  rbp, rsp
   ;guardem l'estat dels registres del processador perquè
   ;les funcions de C no mantenen l'estat dels registres.
   push rax
   push rbx
   push rcx
   push rdx
   push rsi
   push rdi
   push r8
   push r9
   push r10
   push r11
   push r12
   push r13
   push r14
   push r15

   call gotoxyP1_C
 
   ;restaurar l'estat dels registres que s'han guradat a la pila.
   pop r15
   pop r14
   pop r13
   pop r12
   pop r11
   pop r10
   pop r9
   pop r8
   pop rdi
   pop rsi
   pop rdx
   pop rcx
   pop rbx
   pop rax

   mov rsp, rbp
   pop rbp
   ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Aquesta subrutina es dóna feta. NO LA PODEU MODIFICAR.
; Mostrar un caràcter guardat a la variable (charac) a la pantalla, 
; en la posició on està el cursor, cridant la funció printchP1_C.
; 
; Variables globals utilitzades:   
; (charac): Caràcter que volem mostrar.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
printchP1:
   push rbp
   mov  rbp, rsp
   ;guardem l'estat dels registres del processador perquè
   ;les funcions de C no mantenen l'estat dels registres.
   push rax
   push rbx
   push rcx
   push rdx
   push rsi
   push rdi
   push r8
   push r9
   push r10
   push r11
   push r12
   push r13
   push r14
   push r15

   call printchP1_C
 
   ;restaurar l'estat dels registres que s'han guradat a la pila.
   pop r15
   pop r14
   pop r13
   pop r12
   pop r11
   pop r10
   pop r9
   pop r8
   pop rdi
   pop rsi
   pop rdx
   pop rcx
   pop rbx
   pop rax

   mov rsp, rbp
   pop rbp
   ret
   

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Aquesta subrutina es dóna feta. NO LA PODEU MODIFICAR.
; Llegir una tecla i guarda el caràcter associat a la variable (charac)
; sense mostrar-la per pantalla, cridant la funció getchP1_C. 
; 
; Variables globals utilitzades:   
; (charac): Caràcter que llegim de teclat.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
getchP1:
   push rbp
   mov  rbp, rsp
   ;guardem l'estat dels registres del processador perquè
   ;les funcions de C no mantenen l'estat dels registres.
   push rax
   push rbx
   push rcx
   push rdx
   push rsi
   push rdi
   push r8
   push r9
   push r10
   push r11
   push r12
   push r13
   push r14
   push r15

   call getchP1_C
 
   ;restaurar l'estat dels registres que s'han guradat a la pila.
   pop r15
   pop r14
   pop r13
   pop r12
   pop r11
   pop r10
   pop r9
   pop r8
   pop rdi
   pop rsi
   pop rdx
   pop rcx
   pop rbx
   pop rax
   
   mov rsp, rbp
   pop rbp
   ret 
   

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Recórrer la matriu (mines) per comptar el nombres de mines (numMines) que hi ha.
; 
; Variables globals utilitzades:	
; (mines)    : Matriu on posem les mines.
; (numMines) : Mines que queden per marcar.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
countMinesP1:
   push rbp
   mov  rbp, rsp
   ;guardar l'estat dels registres que es modifiquen en aquesta 
   ;subrutina i que no s'utilitzen per retornar valors.
   push rax
   push rbx
   push rcx
   
   mov rax, mines
   mov rbx, 0 ;Farem servir el registre en mode 64 bits, ja que sino despres no podem fer l'adreçament relatiu
   mov ecx, 0
   
   loop_mat:
   
	cmp BYTE[rax+rbx*1], '*' ;Accedim una per una a les posicions de l'array i les comparem
	jne not_add
	inc ecx
   
	not_add:
	inc rbx
	cmp rbx, SIZEMATRIX
	jl loop_mat
   
   mov DWORD[numMines], ecx
  
   cm_end:
   pop rcx
   pop rbx
   pop rax
   ;restaurar l'estat dels registres que s'han guardat a la pila.		
   mov rsp, rbp
   pop rbp
   ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Converteix el valor del nombre de mines que queden per marcar (numMines)
; (entre 0 i 99) en dos caràcters ASCII.
; Si (numMines) és més gran de 99 canviar el valor a 99.
; S'ha de dividir el valor (numMines) entre 10, el quocient representarà 
; les desenes (tens) i el residu les unitats (units), 
; després convertir a ASCII sumant 48, caràcter '0'.
; Si les desenes (tens)==0  mostra un espai ' '.
; Mostra els dígits (caràcter ASCII) de les desenes a la fila 3, 
; columna 40 de la pantalla i les unitats a la fila 3, columna 41.
; Per a posicionar el cursor es cridar a la subrutina gotoxyP1 i per a 
; mostrar els caràcters a la subrutina printchP1_C.
; 
; Variables globals utilitzades:	
; (rowScreen) : Fila de la pantalla on es situa el cursor.
; (colScreen) : Columna de la pantalla on es situa el cursor.
; (numMines)  : Mines que queden per marcar.
; (charac)    : Caràcter a escriure a pantalla.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
showNumMinesP1:
   push rbp
   mov  rbp, rsp
   ;guardar l'estat dels registres que es modifiquen en aquesta 
   ;subrutina i que no s'utilitzen per retornar valors.
   push rax
   push rbx
   push rdx
   
   mov eax, DWORD[numMines]
   cmp eax, 99 
   jle not_bigger ;comprovem que numMines es mes petit o igual a 99
   mov eax, 99
   mov DWORD[numMines], eax ;si es major a 99, actualitzem el seu nou valor en la variable
   
   not_bigger:
   mov rdx, 0
   mov rbx, 10
   div rbx ;rax desenes, rdx unitats
   cmp rax, 0
   jg cert
   
   mov BYTE[charac], ' '; else
   jmp write_1
   
   cert:
   add al, '0' ; accedint a al, tindrem el byte final del registre rax
   mov BYTE[charac], al ;com som accedint al byte final, es pot convertir en char sense problema
   
   write_1:
   mov DWORD[rowScreen], 3
   mov DWORD[colScreen], 40
   call gotoxyP1
   call printchP1
   
   add dl, '0'
   mov [charac], dl
   mov DWORD[rowScreen], 3
   mov DWORD[colScreen], 41
   call gotoxyP1
   call printchP1
   
   snm_End:
   ;restaurar l'estat dels registres que s'han guardat a la pila.
   pop rdx
   pop rbx
   pop rax
   mov rsp, rbp
   pop rbp
   ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Posicionar el cursor a la pantalla dins del tauler, en funció de
; la fila (row) i la columna(col), rebuts com a paràmetre,
; posició del cursor dins del tauler.
; Per a calcular la posició del cursor a pantalla utilitzar 
; aquestes fórmules:
; rowScreen=(row*2)+8
; colScreen=(col*4)+7
; Per a posicionar el cursor es cridar a la subrutina gotoxyP1.
; 
; Variables globals utilitzades:	
; (row)      : Fila per a accedir a les matrius mines i marks.
; (col)      : Columna del cursor dins del tauler.
; (rowScreen): Fila de la pantalla on es situa el cursor.
; (colScreen): Columna de la pantalla on es situa el cursor.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
posCursorP1:
   push rbp
   mov  rbp, rsp
   ;guardar l'estat dels registres que es modifiquen en aquesta 
   ;subrutina i que no s'utilitzen per retornar valors.	
   
   push rax
    
   ;row 
   mov eax, DWORD[row]
   imul eax, 2
   add eax, 8
   mov DWORD[rowScreen], eax
   
   ;col
   mov eax, DWORD[col]
   imul eax, 4
   add eax, 7
   mov DWORD[colScreen], eax
   
   call gotoxyP1
   
   pc_end:
   ;restaurar l'estat dels registres que s'han guardat a la pila.
   pop rax
   mov rsp, rbp
   pop rbp
   ret
   
   
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Mostrar el caràcter de la posició indicada per la fila (row) i la
; columna (col) de la matriu (marks) a la pantalla.
; Per a posicionar el cursor es cridar a la subrutina posCursorP1 
; i per a mostrar el caràcter a la subrutina printchP1.
; 
; Variables globals utilitzades:	
; (marks) : Matriu amb les mines marcades.
; (row)   : Fila del cursor dins del tauler.
; (col)   : Columna del cursor dins del tauler.
; (charac): Caràcter a escriure a pantalla.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
showMarkP1
   push rbp
   mov  rbp, rsp
   ;guardar l'estat dels registres que es modifiquen en aquesta 
   ;subrutina i que no s'utilitzen per retornar valors.
   push rax
   push rcx
   push rdx
   
   call posCursorP1
   mov rax, marks
   mov ecx, DWORD[row]
   mov edx, DWORD[col]
   imul ecx, DIMMATRIX 
   add ecx, edx ; Fem row*NCOLS + col per a accedir una per una a les posicions de l'array bidimensional
   mov dl, BYTE[rax+rcx] ; Rax pos inicial, rcx desplaçament
   
   mov BYTE[charac], dl
   call printchP1
   
   smk_end:
   ;restaurar l'estat dels registres que s'han guardat a la pila.
   pop rdx
   pop rcx
   pop rax
   mov rsp, rbp
   pop rbp
   ret
   
   
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;		
; Actualitzar la posició del cursor al tauler indicada per les variables
; (row) fila i (col) columna, en funció a la tecla premuda (charac).
; Si es surt fora del tauler no actualitzar la posició del cursor.
; ( i:amunt, j:esquerra, k:avall, l:dreta)
; Amunt i avall: ( row--/++ ) 
; Esquerra i Dreta: ( col--/++ ) 
; No s'ha de posicionar el cursor a la pantalla.
;  
; Variables globals utilitzades:	
; (charac): Caràcter llegit de teclat.
; (row)   : Fila del cursor dins del tauler.
; (col)   : Columna del cursor dins del tauler.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; 
moveCursorP1:
   push rbp
   mov  rbp, rsp
   ;guardar l'estat dels registres que es modifiquen en aquesta 
   ;subrutina i que no s'utilitzen per retornar valors.
   push rax
   push rbx
   push rcx
   
   mov al, BYTE[charac] ;al ja que es un char
   
   up:
   cmp al, 'i'
   jne left
   mov ebx, DWORD[row] ;importem la variable que toqui un cop hem passat la comparacio, per a evitar d'haver-ne d'importar sempre dues
   cmp ebx, 0
   jle mc_end ;si no podem fer el moviment perque sortim del tauler, acabem l'execucio
   dec ebx
   mov DWORD[row], ebx
   jmp mc_end ;break
   
   left:
   cmp al, 'j'
   jne down
   mov ebx, DWORD[col]
   cmp ebx, 0
   jle mc_end
   dec ebx
   mov DWORD[col], ebx
   jmp mc_end
   
   down:
   cmp al, 'k'
   jne right
   mov ebx, DWORD[row]
   mov ecx, DIMMATRIX
   dec ecx
   cmp ebx, ecx
   jge mc_end
   inc ebx
   mov DWORD[row], ebx
   jmp mc_end
   
   right:
   cmp al, 'l'
   jne mc_end
   mov ebx, DWORD[col]
   mov ecx, DIMMATRIX
   dec ecx
   cmp ebx, ecx
   jge mc_end
   inc ebx
   mov DWORD[col], ebx
   jmp mc_end
   
   
   mc_end:
   ;restaurar l'estat dels registres que s'han guardat a la pila.

   pop rcx
   pop rbx
   pop rax
   mov rsp, rbp
   pop rbp
   ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; 
; Marcar/desmarcar una mina a la matriu (marks) a la posició actual del
; cursor indicada per les variables (row) fila i (col) columna.
; Si en aquella posició de la matriu (marks) hi ha un espai en blanc i 
; no hem marcat totes les mines, marquem una mina posant una 'M' a la 
; matriu (marks) i decrementarem el nombre de mines que queden per 
; marcar (numMines), si en aquella posició de la matriu (marks) hi ha 
; una 'M', posarem un espai (' ') a la matriu (marks) i incrementarem 
; el nombre de mines que queden per marcar (numMines).
; Si hi ha un altre valor no canviarem res.
; Mostrar el canvi fet a la matriu (marks) cridant la subrutina showMarkP1.
; 
; Variables globals utilitzades:	
; (marks)   : Matriu amb les mines marcades.
; (row)     : Fila del cursor dins del tauler.
; (col)     : Columna del cursor dins del tauler.
; (numMines): Mines que queden per marcar.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
markMineP1:
   push rbp
   mov  rbp, rsp
   ;guardar l'estat dels registres que es modifiquen en aquesta 
   ;subrutina i que no s'utilitzen per retornar valors.
   push rax
   push rbx
   push rcx
   push rdx
   
   mov rax, marks
   mov ebx, DWORD[numMines]
   mov ecx, DWORD[row]
   mov edx, DWORD[col] 
   
   imul ecx, DIMMATRIX ; Multipliquem la fila per el nombre de columnes i sumem la columna 
   add ecx, edx ;Aixi no hem d'iterar per a obtenir la posicio de l'array que volem
   mov dl, BYTE[rax+rcx] ; Rax pos inicial, rcx desplaçament
   
   cmp dl, ' '
   jne else_if
   cmp ebx, 0
   jle else_if
   mov dl, 'M' ;Marquem la posicio
   mov BYTE[rax+rcx], dl ;Modifiquem l'array
   dec ebx
   mov DWORD[numMines], ebx
   jmp mm_out ;fem aquest salt per a evitar que ara ens entri en el segon if i anuli el canvi
   
   
   else_if:
   cmp dl, 'M'
   jne mm_end
   mov dl, ' ' ;Desmarquem la posicio
   mov BYTE[rax+rcx], dl ;Modifiquem l'array
   inc ebx
   mov DWORD[numMines], ebx
   
   mm_out:
   call showMarkP1

   mm_end:
   ;restaurar l'estat dels registres que s'han guardat a la pila.
   pop rdx
   pop rcx
   pop rbx
   pop rax
    
   mov rsp, rbp
   pop rbp
   ret
	
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Verificar si hem marcat totes les mines. 
; Si (numMines==0) canvia l'estat (state=2) "Guanya la partida".
; 
; Variables globals utilitzades:	
; (numMines) : Mines que queden per marcar.
; (state)    : Estat del joc.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
checkEndP1:
   push rbp
   mov  rbp, rsp
   ;guardar l'estat dels registres que es modifiquen en aquesta 
   ;subrutina i que no s'utilitzen per retornar valors.
   push rax
   
   mov rax, [numMines]
   cmp rax, 0
   jne checkEndP1_End
   mov rax, 2
   mov [state], rax
	
   checkEndP1_End:
   ;restaurar l'estat dels registres que s'han guardat a la pila.
   pop rax
   mov rsp, rbp
   pop rbp
   ret
   
   
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Joc del Buscamines.
; Subrutina principal del joc.
; Permet jugar al joc del buscamines cridant totes les funcionalitats.
;
; Pseudo codi:
; Mostrar el tauler de joc (cridar la funció PrintBoardP1_C).
; Comptar el nombre de mines (numMines) que hi ha a la matriu (mines) 
; cridant la subrutina countMinesP1.
; Marcar una mina a la fila (row)=8, columna (col)=8 
; cridant a la subrutina markMineP1.
; Mostrar el nombre de mines (numMines) cridant la subrutina showNumMinesP1.
; 
; Inicialitzar l'estat del joc, (state=1) per començar a jugar.
; Fixar la posició inicial del cursor a la fila (row)=5, columna (col)=4.
; 
; Mentre (state=1) fer:
;   Posicionar el cursor a la pantalla dins del tauler, en funció de
;   la fila (row) i la columna(col) cridant la subrutina posCursorP1.
;   Llegir una tecla i guardar-la a la variable (charac) cridant
;     a la subrutina getchP1. 
;   Segons la tecla llegida cridarem a la subrutina corresponent.
;     - ['i','j','k' o 'l']    cridar a la subrutina moveCursorP1.
;     - 'm'                    cridar a la subrutina markMineP1 i 
;                              la subrutina showNumMinesP1 per actualitzar
;                              el valor de (numMines) al tauler.
;     - '<ESC>'   (codi ASCII 27) posar (state = 0) per a sortir.   
;   Verificar si hem marcat totes les mines (numMines==0) cridant a 
;   la subrutina checkEndP1.
; Fi mentre.
; Sortir:
; Si s'ha obert una mina (state==3) mostrar totes les mines de 
; la matriu (mines) cridant la subrutina showMarkP2.
; Mostrar el missatge de sortida que correspongui cridant a la funció
; printMessageP1_C.
; S'acaba el joc.

; Variables globals utilitzades:	
; (state) : Estat del joc.
; (row)   : Fila del cursor dins del tauler.
; (col)   : Columna del cursor dins del tauler.
; (charac): Caràcter llegit des del teclat i a mostrar.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
playP1:
   push rbp
   mov  rbp, rsp
   ;guardar l'estat dels registres que es modifiquen en aquesta 
   ;subrutina i que no s'utilitzen per retornar valors.   
   push rax

   call printBoardP1_C        ;printBoardP1_C();

   call countMinesP1          ;numMines = countMinesP1_C();
   mov  DWORD[row], 8         ;row=8; 
   mov  DWORD[col], 8         ;col=8;
   call markMineP1            ;numMines = markMineP1_C();
   call showNumMinesP1        ;showNumMinesP1_C(); 
   
   mov  WORD[state], 1        ;state = 1;
   mov  DWORD[row], 5         ;row=5;
   mov  DWORD[col], 4         ;col=4;

   p_while:              
     cmp  WORD[state], 1      ;while (state == 1)
     jne  p_printMessage

     call posCursorP1         ;showMinesP1_C();
     		
     call getchP1             ;getchP1_C(); 
     mov  al, BYTE[charac] 

     p_move:
     cmp al, 'i'              ;if (charac>='i' && charac<='l')
     jl  p_mark
     cmp al, 'l'           
     jg  p_mark
       call moveCursorP1      ;moveCursorP1_C();
     p_mark:
     cmp al, 'm'              ;if (charac=='m')
     jne  p_esc
       call markMineP1        ;markMineP1_C();
       call showNumMinesP1    ;showNumMinesP1_C(); 
     p_esc:
     cmp al, 27               ;if (charac==27)
     jne  p_check
       mov BYTE[state], 0     ;state = 0;

     p_check:
     call checkEndP1          ;checkEndP1_C();

     jmp  p_while

   p_printMessage:
   cmp r9w, 3             ;if(state==3){
   jne p_endif1
     mov DWORD[row], 0          ;row=0;
     p_for1:             
	 cmp DWORD[row], DIMMATRIX  ;for (row=0;row<DIMMATRIX;row++){
	 jge p_endfor1
	   mov DWORD[col], 0        ;col=0;
	   p_for2:
	   cmp DWORD[col], DIMMATRIX;for (col=0;col<DIMMATRIX;col++){
	   jge p_endfor2 
         call showMarkP1        ;showMarkP1_C(mines,row, col);
         inc DWORD[col]         ;col++
         jmp p_for2
       p_endfor2:                 ;}
       inc DWORD[row]             ;row++
       jmp p_for1
     p_endfor1
   p_endif1:                    ;}
   
   call printMessageP1_C      ;printMessageP1_C();
    
   p_end:
   ;restaurar l'estat dels registres que s'han guardat a la pila.
   pop rax
   		
   mov rsp, rbp
   pop rbp
   ret

