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
global countMinesP2, showNumMinesP2  , posCursorP2, showMarkP2, 
global moveCursorP2, markMineP2  , searchMinesP2, checkEndP2, playP2

;Funcions de C que es criden des de assemblador
extern clearScreen_C, gotoxyP2_C, getchP2_C, printchP2_C
extern printBoardP2_C, printMessageP2_C

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
;; Les subrutines en assemblador que s'han de modificar per a
;; implementar el pas de paràmetres són:
;;   countMinesP2, showNumMinesP2  , posCursor, showMarkP2, 
;;   moveCursorP2, mineMarkerP2
;; La subrutina que s'ha de canviar la funcionalitat:
;;   checkEndP2
;; La subrutina que s'ha d'implementar:
;;   searchMinesP2
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Aquesta subrutina es dóna feta. NO LA PODEU MODIFICAR.
; Situar el cursor en una fila i una columna de la pantalla
; en funció de la fila (edi) i de la columna (esi) rebuts com 
; a paràmetre cridant a la funció gotoxyP2_C.
; 
; Variables globals utilitzades:	
; Cap
; 
; Paràmetres d'entrada : 
; (rowScreen): rdi(edi) : Fila de la pantalla on es situa el cursor.
; (colScreen): rsi(esi) : Columna de la pantalla on es situa el cursor.
;
; Paràmetres de sortida: 
; Cap
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
gotoxyP2:
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

   ; Quan cridem la funció gotoxyP2_C(int row_num, int row_num) des d'assemblador 
   ; el primer paràmetre (row_num) s'ha de passar pel registre rdi(edi), i
   ; el segon  paràmetre (col_num) s'ha de passar pel registre rsi(esi).	
   call gotoxyP2_C
 
   ;restaurar l'estat dels registres que s'han guardat a la pila.
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
; Aquesta subrutina es dóna feta. NO LA PODEU MODIFICAR.
; Mostrar un caràcter (dil) a la pantalla, rebut com a paràmetre, 
; en la posició on està el cursor cridant la funció printchP2_C.
; 
; Variables globals utilitzades:	
; Cap
; 
; Paràmetres d'entrada : 
; (c) : rdi(dil) : Caràcter a mostrar.
; 
; Paràmetres de sortida: 
; Cap
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
printchP2:
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

   ; Quan cridem la funció printchP2_C(char c) des d'assemblador, 
   ; el paràmetre (c) s'ha de passar pel registre rdi(dil).
   call printchP2_C
 
   ;restaurar l'estat dels registres que s'han guardat a la pila.
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
; Aquesta subrutina es dóna feta. NO LA PODEU MODIFICAR.
; Llegir una tecla i retornar el caràcter associat (al) sense 
; mostrar-lo per pantalla, cridant la funció getchP2_C.
; 
; Variables globals utilitzades:	
; Cap
; 
; Paràmetres d'entrada : 
; Cap
; 
; Paràmetres de sortida: 
; (c) : rax(al) : Caràcter llegit des del teclat.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
getchP2:
   push rbp
   mov  rbp, rsp
   ;guardem l'estat dels registres del processador perquè
   ;les funcions de C no mantenen l'estat dels registres.
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
   
   mov rax, 0
   ; Quan cridem la funció getchP2_C des d'assemblador, 
   ; retorna sobre el registre rax(al) el caràcter llegit
   call getchP2_C
 
   ;restaurar l'estat dels registres que s'han guardat a la pila.
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
   
   mov rsp, rbp
   pop rbp
   ret 



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Recórrer la matriu (mines), rebuda como a paràmetre, per comptar 
; el nombres de mines (numMines) y retornar el valor.
; 
; Variables globals utilitzades:	
; Cap.
; 
; Paràmetres d'entrada : 
; (mines)   :rdi(rdi): Adreça de la matriu on hi han les mines.
; 
; Paràmetres de sortida: 
; (numMines):rax(rax): Mines que queden per marcar.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
countMinesP2
   push rbp
   mov  rbp, rsp
   ;guardem l'estat dels registres del processador perquè
   ;les funcions de C no mantenen l'estat dels registres.  
   push rbx
   
   mov rbx, 0 ;Farem servir el registre rbx com a index relatiu
   mov rax, 0
   
   loop_mat:
	cmp BYTE[rdi+rbx*1], '*' ;Accedim una per una a les posicions de l'array i les comparem
	jne continue
	inc rax
   
	continue:
	inc rbx
	cmp rbx, SIZEMATRIX
	jl loop_mat
 
   cm_end:
   pop rbx
   ;restaurar l'estat dels registres que s'han guardat a la pila.
   			
   mov rsp, rbp
   pop rbp
   ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Converteix el valor (numMines), mines que queden per marcar,
; (valor entre 0 i 99) en dos caràcters ASCII. 
; Si (numMines) és més gran de 99 canviar el valor a 99.
; S'ha de dividir el valor (numMines) entre 10, el quocient representarà 
; les desenes (tens) i el residu les unitats (units), i després 
; s'han de convertir a ASCII sumant 48, caràcter '0'.
; Si les desenes (tens)==0  mostra un espai ' '.
; Mostra els dígits (caràcter ASCII) de les desenes a la fila 3, 
; columna 40 de la pantalla i les unitats a la fila 3, columna 41.
; Per a posicionar el cursor s'ha de cridar a la subrutina gotoxyP2 i 
; per a mostrar els caràcters a la subrutina printchP2.
; 
; Variables globals utilitzades:	
; Cap
; 
; Paràmetres d'entrada : 
; (numMines) :rdi(rdi): Mines que queden per marcar.
; 
; Paràmetres de sortida: 
; Cap
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
showNumMinesP2:
   push rbp
   mov  rbp, rsp
   push rsi
   push rax
   push rdx
   ;guardem l'estat dels registres del processador perquè
   ;les funcions de C no mantenen l'estat dels registres.	
   
   
   ;Preparem el cursor en la posició per a escriure
   mov rax, rdi ;Guardem el parametre d'entrada a rax
   mov edi, 3
   mov esi, 40
   call gotoxyP2
   
   ;Comprovem que numMines es mes petit o igual a 99
   cmp rax, 99 
   jle isolate_tens_units 
   mov rax, 99 ;Si es major a 99, actualitzem el seu nou valor
   
   ;Aillem les desenes i les unitats mitjançant una divisio
   isolate_tens_units:
   mov rdx, 0
   mov rsi, 10
   div rsi ;rax desenes, rdx unitats
   cmp rax, 0
   jg cert
   
   ;Si tenim un zero a les desenes, el substituim per un espai
   mov al, ' '
   jmp write_tens
   
   cert:
   add al, '0' ; accedint a al, tindrem el byte final del registre rax
    
   write_tens:
   mov dil, al 
   call printchP2
   
   write_units:
   mov edi, 3
   mov esi, 41
   call gotoxyP2
   add dl, '0'
   mov dil, dl
   call printchP2
   
   snm_end:
   ;restaurar l'estat dels registres que s'han guardat a la pila.
   pop rdx
   pop rax
   pop rsi
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
; Per a posicionar el cursor es cridar a la subrutina gotoxyP2.
; 
; Variables globals utilitzades:	
; Cap
; 
; Paràmetres d'entrada : 
; (row) :rdi(edi): Fila on està el cursor dins les matrius mines i marks.
; (col) :rsi(esi): Columna on està el cursor dins les matrius mines i marks.
; 
; Paràmetres de sortida: 
; Cap 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
posCursorP2:
   push rbp
   mov  rbp, rsp
   push rax
   ;guardem l'estat dels registres del processador perquè
   ;les funcions de C no mantenen l'estat dels registres.	
    
   ;Calculem la fila 
   mov eax, edi
   imul eax, 2
   add eax, 8
   mov edi, eax
   
   ;Calculem la columna
   mov eax, esi
   imul eax, 4
   add eax, 7
   mov esi, eax
   
   call gotoxyP2
   
   pc_end:
   ;restaurar l'estat dels registres que s'han guardat a la pila.
   pop rax
   mov rsp, rbp
   pop rbp
   ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Mostrar el caràcter de la matriu (marks) a la posicionar del cursor de
; la fila (row) i la columna (col), rebuts com a paràmetre, a la pantalla.
; Per a posicionar el cursor s'ha de cridar a la subrutina posCursorP2.
; i per a mostrar el caràcter a la subrutina printchP2.

; Variables globals utilitzades:	
; Cap
; 
; Paràmetres d'entrada : 
; (marks):rdi(rdi): Adreça de la matriu amb les mines marcades i les mines de les caselles obertes.
; (row)  :rsi(esi): Fila on està el cursor dins les matrius mines i marks.
; (col)  :rdx(edx): Columna on està el cursor dins les matrius mines i marks.
; 
; Paràmetres de sortida: 
; Cap.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; 
showMarkP2:
   push rbp
   mov  rbp, rsp
   push rax
   push rcx
   ;guardem l'estat dels registres del processador perquè
   ;les funcions de C no mantenen l'estat dels registres.  
   
   ;Guardem la ubicacio de la matriu i el valor de la fila
   mov rax, rdi
   mov ecx, esi
   
   ;Movem el cursor a la posició que s'ha de marcar
   mov edi, esi
   mov esi, edx
   call posCursorP2
   
   ;Ara obtenim la posicio de la matriu que volem marcar
   imul ecx, DIMMATRIX 
   add ecx, edx ; Fem row*NCOLS + col per a accedir una per una a les posicions de l'array bidimensional
   mov dil, BYTE[rax+rcx] ; Rax pos inicial, rcx desplaçament
   call printchP2
   
   smk_end:
   ;restaurar l'estat dels registres que s'han guardat a la pila.
   pop rcx
   pop rax
   mov rsp, rbp
   pop rbp
   ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;		
; Actualitzar la posició del cursor al tauler, que està a la fila (row)
; i la columna (col), en funció de la tecla premuda (c),
; rebuts com a parametre.
; Si es surt fora del tauler no actualitzar la posició del cursor.
; ( i:amunt, j:esquerra, k:avall, l:dreta)
; Amunt i avall: ( row--/++ ) 
; Esquerra i Dreta: ( col--/++ )
; Retornar l'index que indica la posició del cursor dins la matriu
; amb la següent fórmula: index = row*DIMMATRIX+col
; No s'ha de posicionar el cursor a la pantalla.
;  
; Variables globals utilitzades:	
; Cap   
; 
; Paràmetres d'entrada : 
; (c)     :rdi(dil): Caràcter llegit de teclat.
; (row)   :rsi(esi): Fila on està el cursor dins les matrius mines i marks.
; (col)   :rdx(edx): Columna on està el cursor dins les matrius mines i marks.
; 
; Paràmetres de sortida: 
; (index) :rax(eax): Índex que indica la posició del cursor dins la matriu.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; 
moveCursorP2:
   push rbp
   mov  rbp, rsp
   push rcx
   ;guardem l'estat dels registres del processador perquè
   ;les funcions de C no mantenen l'estat dels registres.
   
   up:
   cmp dil, 'i'
   jne left
   cmp esi, 0
   jle calculate_index ;si no podem fer el moviment perque sortim del tauler, acabem l'execucio
   dec esi
   jmp calculate_index ;break
   
   left:
   cmp dil, 'j'
   jne down
   cmp edx, 0
   jle calculate_index
   dec edx
   jmp calculate_index
   
   down:
   ;Fem DIMMATRIX -1 i ho guardem per als dos posibles casos posteriors
   mov ecx, DIMMATRIX
   dec ecx
   
   cmp dil, 'k'
   jne right
   cmp esi, ecx
   jge calculate_index
   inc esi
   jmp calculate_index
   
   right:
   cmp dil, 'l'
   jne calculate_index
   cmp edx, ecx
   jge calculate_index
   inc edx
   
   calculate_index:
   imul esi, DIMMATRIX 
   add esi, edx 
   mov eax, esi 
   
   mc_end:
   ;restaurar l'estat dels registres que s'han guardat a la pila.
   pop rcx
   mov rsp, rbp
   pop rbp
   ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; 
; Marcar/desmarcar una mina a la matriu (marks) a la posició actual
; del cursor indicada per les variables (row) fila i (col) columna,
; rebuts com a paràmetre
; Si en aquella posició de la matriu (marks) hi ha un espai en blanc i 
; no hem marcat totes les mines, marquem una mina posant una 'M' a la 
; matriu (marks) i decrementem el nombre de mines que queden per 
; marcar (numMines), si en aquella posició de la matriu (marks) hi ha 
; una 'M', posarem un espai (' ') a la matriu (marks) i incrementem 
; el nombre de mines que queden per marcar (numMines).
; Si hi ha un altre valor no canviarem res.
; Retornar el nombre de mines (numMines) que queden per marcar actualitzat .
; Mostrar el canvi fet a la matriu (marks) cridant la subrutina showMarkP2.
; 
; Variables globals utilitzades:	
; Cap
; 
; Paràmetres d'entrada : 
; (marks)   :rdi(rdi): Adreça de la matriu amb les mines marcades i les mines de les caselles obertes.
; (row)     :rsi(esi): Fila on està el cursor dins les matrius mines i marks.
; (col)     :rdx(edx): Columna on està el cursor dins les matrius mines i marks.
; (numMines):rcx(rcx): Mines que queden per marcar.
; 
; Paràmetres de sortida: 
; (numMines) : rax(ax) : Mines que queden per marcar.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;  
markMineP2:
   push rbp
   mov  rbp, rsp
   push r8
   push r9
   ;guardem l'estat dels registres del processador perquè
   ;les funcions de C no mantenen l'estat dels registres.
   
   mov r8d, esi
   imul r8d, DIMMATRIX ; Multipliquem la fila per el nombre de columnes i sumem la columna 
   add r8d, edx 
   mov r9b, BYTE[rdi+r8] ;Obtenim el caràcter que volem comparar
   
   cmp r9b, ' '
   jne else_if
   cmp rcx, 0
   jle else_if
   mov r9b, 'M' ;Marquem la posicio
   mov BYTE[rdi+r8], r9b ;Modifiquem l'array
   dec rcx
   jmp mm_out ;fem aquest salt per a evitar que ara ens entri en el segon if i anuli el canvi
   
   
   else_if:
   cmp r9b, 'M'
   jne mm_out
   mov r9b, ' ' ;Desmarquem la posicio
   mov BYTE[rdi+r8], r9b ;Modifiquem l'array
   inc rcx
   
   
   mm_out:
   call showMarkP2
   mov rax, rcx ;numMines es una variable del tipus long, per tant, li correspon un mode d'adrçament QWORD
   
   
   mm_end:
   ;restaurar l'estat dels registres que s'han guardat a la pila.    
   pop r9
   pop r8
   mov rsp, rbp
   pop rbp
   ret
		

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;  
; Obrir casella. Mirar quantes mines hi ha al voltant de la 
; posició actual del cursor a la fila (row) i la columna (col)
; de la matriu (mines), rebuts com a paràmetre.
; Si a la posició actual de la matriu (marks) hi ha un espai (' ') 
;   Mirar si a la matriu (mines) hi ha una mina ('*').
;   Si hi ha una mina canviar l'estat (state) a 3 "Explosió".
;	 Sinó, comptar quantes mines hi ha al voltant de la posició 
;     actual i actualitzar la posició de la matriu (marks) amb 
;     el nombre de mines (caràcter ASCII del valor), per fer-ho, cal 
;     sumar 48 ('0') al valor.
; Mostrar el canvi fet a la matriu (marks) cridant la subrutina showMarkP2.
; Si no hi ha un espai, vol dir que hi ha una mina marcada ('M') o la 
; casella ja s'ha obert (hi ha el nombre de mines que ja s'ha calculat 
; anteriorment), no fer res.
; Retornar l'estat del joc actualitzat (state).
;  
; Variables globals utilitzades:	
; Cap
; 
; Paràmetres d'entrada :
; (marks):rdi(rdi) : Adreça de la matriu amb les mines marcades i les mines de les caselles obertes.
; (row)  :rsi(esi): Fila on està el cursor dins les matrius mines i marks.
; (col)  :rdx(edx): Columna on està el cursor dins les matrius mines i marks.
; (mines):rcx(rcx): Adreça de la matriu amb les mines.
; (state):r8 (r8w): Estat del joc. 
; 
; Paràmetres de sortida: 
; (state):rax(ax) : Estat del joc.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;  
searchMinesP2:
   push rbp
   mov  rbp, rsp
   ;guardem l'estat dels registres del processador perquè
   ;les funcions de C no mantenen l'estat dels registres.
   
   push r9 ; neigbors
   push r10 ; row -1
   push r11 ; col -1
   push r12 ; charac
   push r13 ; index
   push r14 ; DIMMATRIX - 1
   push r15 ; row +1
   push rbx ; col +1
   
   mov r9d, 0 ; int neighbors = 0;
   mov r13d, esi
   imul r13d, DIMMATRIX 
   add r13d, edx 
   mov r12b, BYTE[rdi+r13] 
   
   cmp r12b, ' ' ; if (marks[row][col]==' ')
   jne end
   
   mov r12b, BYTE[rcx+r13] ; if (mines[row][col]!=' ')
   cmp r12b, ' '
   je else
   mov r8w, 3
   jmp end
   
   else:
   mov r10d, esi
   mov r11d, edx
   mov r14d, DIMMATRIX
   mov r15d, esi
   mov ebx, edx
   dec r10d ; row -1
   dec r11d ; col -1
   dec r14d ; DIMMATRIX - 1
   inc r15d ; row +1
   inc ebx ; col +1
   
   up_if:
   cmp esi, 0 ; if (row > 0)
   jle center_left
   
   up_left:
   cmp edx, 0 ; if (col > 0)
   jle up_center
   mov r13d, r10d
   imul r13d, DIMMATRIX
   add r13d, r11d
   mov r12b, BYTE[rcx+r13]
   cmp r12b, '*' ; if (mines[row-1][col-1]=='*')) 
   jne up_center
   inc r9d ; neighbors++
   
   up_center:
   mov r13d, r10d 
   imul r13d, DIMMATRIX 
   add r13d, edx 
   mov r12b, BYTE[rcx+r13]
   cmp r12b, '*' ;if (mines[row-1][col]=='*')
   jne up_right
   inc r9d ; neighbors++
   
   up_right:
   cmp edx, r14d ; if (col < DIMMATRIX-1)
   jge center_left 
   mov r13d, r10d
   imul r13d, DIMMATRIX
   add r13d, ebx
   mov r12b, BYTE[rcx+r13] 
   cmp r12b, '*' ; if (mines[row-1][col+1]=='*')
   jne center_left
   inc r9d ; neighbors++
   
   center_left:
   cmp edx, 0 ;if (col > 0)
   jle center_right
   mov r13d, esi
   imul r13d, DIMMATRIX
   add r13d, r11d
   mov r12b, BYTE[rcx+r13] ;if(mines[row][col-1]=='*')
   cmp r12b, '*'
   jne center_right
   inc r9d ; neighbors++
   
   center_right:
   cmp edx, r14d ;if (col < DIMMATRIX-1)
   jge down_if
   mov r13d, esi
   imul r13d, DIMMATRIX
   add r13d, ebx
   mov r12b, BYTE[rcx+r13] ;if(mines[row][col+1]=='*')
   cmp r12b, '*'
   jne down_if
   inc r9d ; neighbors++
   
   down_if:
   cmp esi, r14d ; if (row < DIMMATRIX-1)
   jge end_if
   
   down_left:
   cmp edx, 0 ;if (col > 0)
   jle down_center
   mov r13d, r15d
   imul r13d, DIMMATRIX
   add r13d, r11d
   mov r12b, BYTE[rcx+r13] ;(mines[row+1][col-1]=='*')
   cmp r12b, '*'
   jne down_center
   inc r9d ; neighbors++
   
   down_center:
   mov r13d, r15d
   imul r13d, DIMMATRIX
   add r13d, edx
   mov r12b, BYTE[rcx+r13] ;(mines[row+1][col]=='*')
   cmp r12b, '*'
   jne down_right
   inc r9d ; neighbors++
   
   down_right:
   cmp edx, r14d ; if (col < DIMMATRIX-1)
   jge end_if
   mov r13d, r15d
   imul r13d, DIMMATRIX
   add r13d, ebx
   mov r12b, BYTE[rcx+r13] ;(mines[row+1][col+1]=='*')
   cmp r12b, '*'
   jne end_if
   inc r9d ; neighbors++
   
   end_if:
   mov r13d, esi
   imul r13d, DIMMATRIX
   add r13d, edx
   add r9b, '0'
   mov BYTE[rdi+r13], r9b ;marks[row][col] = (char)neighbors+'0';
   call showMarkP2 
   
   end:
   mov ax, r8w

   sm_end:
   ;restaurar l'estat dels registres que s'han guardat a la pila.
   pop rbx
   pop r15
   pop r14
   pop r13
   pop r12
   pop r11
   pop r10
   pop r9
   mov rsp, rbp
   pop rbp
   ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; 
; Verificar si hem marcat totes les mines (numMines=0) i hem obert o
; marcat amb una mina a totes les altres caselles i no hi ha cap espai 
; en blanc (' ') a la matriu (marks), si és així, canviar l'estat 
; (state) a 2 "Guanya la partida".
; Retornar l'estat del joc actualitzat (state).
; 
; Variables globals utilitzades:	
; Cap
; 
; Paràmetres d'entrada : 
; (marks)   :rdi(rdi): Adreça de la matriu amb les mines marcades i les mines de les caselles obertes.
; (numMines):rsi(rsi): Mines que queden per marcar.
; (state)   :rdx(dx) : Estat del joc. 
; 
; Paràmetres de sortida: 
; (state)   :rax(ax) : Estat del joc.  
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;  
checkEndP2:
   push rbp
   mov  rbp, rsp
   ;guardem l'estat dels registres del processador perquè
   ;les funcions de C no mantenen l'estat dels registres.
   push r8
   push r9
   
   mov r8w, 0 ; notOpenMarks = 0
   mov r9, 0 ;Index en mode 64 bits, ja que sino despres no podem fer l'adreçament relatiu
   
   cmp rsi, 0
   jne che_ret
   
   loop_chk:
	cmp BYTE[rdi+r9*1], ' ' ;Comprovem si la posició de l'array es buida
	jne not_add
	inc r8w
   
   not_add:
	inc r9 ;Incrementem l'index
	cmp r9, SIZEMATRIX
	jl loop_chk
   
   cmp r8w, 0
   jne che_ret
   mov dx, 2
   
   
   che_ret:
   mov ax, dx
   
   pop r9
   pop r8
   che_end:
   ;restaurar l'estat dels registres que s'han guardat a la pila. 
   
   mov rsp, rbp
   pop rbp
   ret
   

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Joc del Buscamines
; Subrutina principal del joc
; Permet jugar al joc del buscamines cridant totes les funcionalitats.
;
;Pseudo codi:
; Mostrar el tauler de joc (cridar la funció PrintBoardP2_C).
; Comptar el nombre de mines (numMines) que hi ha la matriu (mines) 
; cridant la subrutina countMinesP2.
; Marcar una mina a la fila (row)=8, columna (col)=8 
; cridant a la subrutina markMineP2.
; Mostrar el nombre de mines (numMines) cridant la subrutina showNumMinesP2.
; 
; Inicialitzar l'estat del joc, (state=1) per començar a jugar.
; Fixar la posició inicial del cursor a la fila (row)=5, columna (col)=4.
; 
; Mentre (state=1) fer:
;   Posicionar el cursor a la pantalla dins del tauler, en funció de
;   la fila (row) i la columna(col) cridant la subrutina posCursorP2.
;   Llegir una tecla i guardar-la a la variable local (charac) cridant
;     a la subrutina getchP2.  
;   Segons la tecla llegida cridarem a la subrutina corresponent.
;     - ['i','j','k' o 'l']  cridar a la subrutina moveCursorP2 i 
;                            actualitzar la fila (row) i la columa (col)
;                            a partir del valor retornat (indexMat).
;     - 'm'                  cridar a la subrutina markMineP2 i 
;                            la subrutina showNumMinesP2 per actualitzar
;                            el valor de (numMines) al tauler.
;     - '<espace>'(codi ASCII 32) cridar a la subrutina searchMinesP2.
;     - '<ESC>'   (codi ASCII 27) posar (state = 0) per a sortir.   
;   Verificar si hem marcat totes les mines i si hem obert totes  
;   les caselles cridant a la subrutina checkEndP2_C.
; Fi mentre.
; Sortir:
; Si s'ha obert una mina (state==3) mostrar totes les mines de 
; la matriu (mines) cridant la subrutina showMarkP2.
; Mostrar el missatge de sortida que correspongui cridant a la funció
; printMessageP2_C.
; S'acabat el joc.
; 
; Variables globals utilitzades:
; Cap
; 
; Paràmetres d'entrada : 
; (marks) :rdi(rdi): Adreça de la matriu amb les mines marcades i les mines de les caselles obertes.
; (mines) :rsi(rsi): Adreça de la matriu amb les mines.
; 
; Paràmetres de sortida: 
; Cap.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
playP2:
   push rbp
   mov  rbp, rsp
   ;guardem l'estat dels registres del processador perquè
   ;les funcions de C no mantenen l'estat dels registres.
   push rax
   push rbx
   push rcx
   push rdx
   push rdi
   push rsi
   push r8
   push r9
   push r10
   push r11
   push r12
   push r13
   push r14
   
   push rdi
   push rsi
   call printBoardP2_C   ;printBoard2_C();
   pop  rsi
   pop  rdi
   
   mov  r12, rdi         ;marks
   mov  r13, rsi         ;mines
   mov  rdi, r13
   call countMinesP2     ;numMines = countMinesP2_C(mines);
   mov  r14,  rax
   mov  r10d, 8          ;row=8; 
   mov  r11d, 8          ;col=8;
   mov  rdi, r12
   mov  esi, r10d
   mov  edx, r11d
   mov  rcx, r14
   call markMineP2       ;numMines = markMineP2_C(marks, row, col, numMines);
   mov  r14, rax
   mov  r9w, 1           ;state = 1;
   mov  r10d, 5          ;row=5;
   mov  r11d, 4          ;col=4;

   mov  rdi, r14
   call showNumMinesP2   ;showNumMinesP2_C(numMines);
   
   p_while               ;bucle principal del joc.
   cmp  r9w, 1           ;while (state == 1) {            
   jne  p_printMessage

   mov  edi, r10d        ;row
   mov  esi, r11d        ;col
   call posCursorP2      ;posCursorP2_C(row, col); 

   call getchP2          ;(al)charac = getchP2_C();

   p_move:
   cmp al, 'i'		     ;if (charac>='i'
   jl  p_mark
   cmp al, 'l'		     ;&& charac<='l')
   jle p_moveCursor
   p_mark:
   cmp al, 'm'		     ;if (charac=='m') { 
   je  p_mineMarker
   p_open:
   cmp al, ' '           ;if (charac==' ') {
   je  p_searchMines
   p_esc:
   cmp al, 27		     ;if (charac==27) {  
   je  p_exit
   jmp p_check  
    
   p_moveCursor:
   mov  dil, al          ;charac
   mov  esi, r10d        ;row
   mov  edx, r11d        ;col
   call moveCursorP2     ;indexMat = moveCursorP2_C(charac, row, col);
   mov  edx, 0
   mov  ebx, DIMMATRIX          
   div  ebx              ;EAX:row=(posCursor/DIMMATRIX) EDX:col=(posCursor%DIMMATRIX)
   mov  r10d, eax
   mov  r11d, edx
   jmp  p_check

   p_mineMarker:
   mov  rdi, r12         ;marks
   mov  esi, r10d        ;row
   mov  edx, r11d        ;col
   mov  rcx, r14         ;numMines
   call markMineP2       ;numMines = markMineP2_C(marks, row, col, numMines);
   mov  r14, rax
   mov  rdi, r14
   call showNumMinesP2   ;showNumMinesP2_C(numMines);
   jmp  p_check

   p_searchMines:
   mov  rdi, r12         ;marks
   mov  esi, r10d        ;row
   mov  edx, r11d        ;col
   mov  rcx, r13         ;mines
   mov  r8w, r9w         ;state
   call searchMinesP2    ;state = searchMinesP2_C(marks, row, col, mines, state);
   mov  r9w, ax
   jmp  p_check

   p_exit:
   mov  r9w, 0           ;state = 0;
 
   p_check:
   mov  rdi, r12         ;marks
   mov  rsi, r14         ;numMines
   mov  dx,  r9w         ;state
   call checkEndP2       ;state = checkEndP2_C(marks, numMines, state);
   mov  r9w, ax
   
   jmp  p_while

   p_printMessage:
       
   cmp r9w, 3             ;if(state==3){
   jne p_endif1
     mov r10b, 0          ;row=0;
     p_for1:             
	 cmp r10b, DIMMATRIX  ;for (row=0;row<DIMMATRIX;row++){
	 jge p_endfor1
	   mov r11b, 0        ;col=0;
	   p_for2:
	   cmp r11b, DIMMATRIX;for (col=0;col<DIMMATRIX;col++){
	   jge p_endfor2 
         mov  rdi, r13     ;mines
         mov  esi, r10d    ;row
         mov  edx, r11d    ;col   
         call showMarkP2   ;showMarkP2_C(mines,row, col);
         inc r11b          ;col++
         jmp p_for2
       p_endfor2:            ;}
       inc r10b              ;row++
       jmp p_for1
     p_endfor1:
   p_endif1:               ;}

   mov  di, r9w
   call printMessageP2_C ;printMessageP2_C(state);
      
   p_end:
   ;restaurar l'estat dels registres que s'han guardat a la pila.
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
