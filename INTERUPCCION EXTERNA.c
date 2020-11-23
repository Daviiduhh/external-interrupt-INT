/*
PRACTICA 12 - “CALCULADORA ARITMÉTICA DE 2 DÍGITOS”
EQUIPO V 
8H1 - ELECTRONICA DIGITAL V
DAVID MADRID NAPOLES 17100201
JOSE GUILLERMO CALLEROS RODRIGUEZ 17100054
ERNESTO TORRES PEREZ 17100326
20/10/2020

EN ESTE PROGRAMA HAREMOS UN CONTADOR CON UN GRAN DELAY PARA OBSERVAR LAS 
DIFERENCIAS ENTRE UNA INTERRUPCION Y UN METODO DE PAUSA NORMAL, EL OBJETIVO ES 
VER LAS DIFERENCIAS, Y NOTAR LAS INTERRUPCIONES
*/

#INCLUDE <16F887.H>
#USE DELAY(CLOCK=4000000)
#FUSES INTRC_IO,NOWDT,NOPUT,MCLR,NOPROTECT,NOCPD,NOBROWNOUT
#FUSES NOIESO,NOFCMEN,NOLVP,NODEBUG,NOWRT,BORV21

#USE FAST_IO(A)
#USE FAST_IO(B)
#USE FAST_IO(C)
#USE FAST_IO(D)

#INCLUDE <LCD1.C>

INT Z = 0; //ETAPAS
INT I = 0, J = 0, AUX = 0, CUENTA = 0, L = 0; //CUENTAS
LONG S = 0, MS = 0; //SEGUNDOS Y MILISEGUNDOS

INT RV [16] = {1, 2, 3, 101, 4, 5, 6, 102, 7, 8, 9, 103, 200, 0, 201, 104};

VOID INICIO (){ //MUESTRA LOS MENSAJES DE INICIO
   LCD_GOTOXY(1,1);
   PRINTF(LCD_PUTC, "      CETI      \n     COLOMOS    ");         
   DELAY_MS(500);
   
   LCD_GOTOXY(1,1);
   PRINTF(LCD_PUTC, "TGO. EN CONTROL \nAUTOMATICO E INS");         
   DELAY_MS(500);
   
   LCD_GOTOXY(1,1);
   PRINTF(LCD_PUTC, "   ELECTRONICA  \n    DIGITAL V   ");         
   DELAY_MS(500);
   
   FOR(J = 0; J <= 2; J++){ // EN CADA ITERACION MUESTRA EL R Y N DE UN ALM
      SWITCH (J){
         CASE 0:
            LCD_GOTOXY(1,1);
            PRINTF(LCD_PUTC, "DAVID MADRID     \n17100201            ");
            BREAK;
               
         CASE 1:
            LCD_GOTOXY(1,1);
            PRINTF(LCD_PUTC, "TORRES PEREZ    \n17100326          ");
            BREAK;
                     
         CASE 2:
            LCD_GOTOXY(1,1);
            PRINTF(LCD_PUTC, "JOSE CALLEROS   \n17100054         ");
            BREAK;
            }
      DELAY_MS(200);
   }
   
   LCD_GOTOXY(1,1);
   PRINTF(LCD_PUTC, "  INTERRUPCION  \n EXTERNA (INT)  ");         
   DELAY_MS(500);
}
VOID TECLADO (){ //SE ENCARGA DE DETECTAR PULSACIONES EN EL TECLADO
   AUX = 0XEF;
   CUENTA = 0; //LLEVARA LA CUENTA, DETECTANDO LA CUENTA PRESIONADO
   FOR(I=0;I<4;I++){//EN CADA ITERACION REVISA UNA FILA DIFERENTE
         
      AUX=AUX>>1; //DESPLAZA UN CERO A LA DERECHA
      OUTPUT_A((INPUT_A()&0XF0)|(AUX&0X0F));
         
      IF(INPUT(PIN_A7)==0) //REVISDA ESTADO COLUMNA A7
      BREAK;
      CUENTA++;
      IF(INPUT(PIN_A6)==0) //REVISDA ESTADO COLUMNA A6
      BREAK;
      CUENTA++;
      IF(INPUT(PIN_A5)==0) //REVISDA ESTADO COLUMNA A5
      BREAK;
      CUENTA++;
      IF(INPUT(PIN_A4)==0) //REVISDA ESTADO COLUMNA A4
      BREAK;
      CUENTA++;
         
      DELAY_MS(20); //TIEMPO DE ESPERA NECESARIO PARA DETECTAR CAMBIOS
   }
}

#INT_EXT //Directiva para definir la interrupción a utilizar 
VOID EXT_ISR( ){ //Función de la interrupción
   LCD_GOTOXY(1,1);
   PRINTF(LCD_PUTC, "  INTERRUPCION  \n    EXTERNA     "); 
   DELAY_MS(1000);
   //INDICA QUE INTERRUMPE DURANTE 1 s
}

VOID MAIN (){

   SET_TRIS_A(0XF0); //DECLARAR SALIDAS
   SET_TRIS_B(0XFF); //DECLARAR ENTRADAS INTERRUPCIONES
   OUTPUT_A(INPUT_A()&0XF0); //LIMPIAR BITS DE SALIDA
   LCD_INIT(); //INICIAR LCD
   
   ENABLE_INTERRUPTS(GLOBAL); //Función para habilitar el uso de las interrupciones 
   ENABLE_INTERRUPTS(INT_EXT); //Función para habilitar la interrupción a utilizar
   EXT_INT_EDGE(L_TO_H); //INTERRUMPE DE LOW A HIGH
   
   INICIO ();
   
   LCD_GOTOXY(1,1);
   PRINTF(LCD_PUTC, "VELOCIDAD 1-5 S \n'A' CONFIRMACION"); 
   
   WHILE(TRUE){

      WHILE(Z == 0){ //ETAPA 1      
         TECLADO ();
         IF(CUENTA<16){ //SI SE PRESIONA EL TECLADO MATRICIAL
            IF(RV[CUENTA]<10){ //SI SE PRESIONA TECLA DIFERENTE A LAS DE NUMERO            
               S = RV[CUENTA]; //ASIGAN EL VALOR LEIDO A U
               LCD_GOTOXY(1,1);
               PRINTF(LCD_PUTC, "VELOCIDAD = %Lu   \n'A' CONFIRMACION"S);      
               MS = S*1000; //GUARDA EL VALOR EN MS DE S
               IF(S>5){ //SI ESTA FUERA DE RANGO
                  LCD_GOTOXY(1,2);
                  PRINTF(LCD_PUTC, " FUERA DE RANGO ");
               }           
            }
            
            IF(CUENTA==3){ //SI SE PRESIONA A ARRANCA CUENTA
               Z=1; //PASA A ETAPA 2
            }
            DO{
               TECLADO();
            }WHILE(CUENTA<16);
         }
      }
      
      WHILE(Z == 1){ //ETAPA 2     
         LCD_GOTOXY(1,1);
         PRINTF(LCD_PUTC, "CUENTA = %u      \n                "L);
         //DETENER SIN INTERRUPCION
         WHILE(INPUT(PIN_B7)==0){ //CUANDO SE PRESIONA B7
               WHILE(INPUT(PIN_B7)==0){ //MIENTRAS SE SIGUE PRESIONANDO B7
                  LCD_GOTOXY(1,1);
                  PRINTF(LCD_PUTC, "    DEJA DE     \n  PRESIONAR B7  "); 
               }
               WHILE(INPUT(PIN_B7)==1){ //CUANDO SE SUELTA B7
                  LCD_GOTOXY(1,1);
                  PRINTF(LCD_PUTC, "    DETENIDO    \nSIN INTERRUPCION"); 
                  DELAY_MS(1000);
                  BREAK;
               }
            }
         TECLADO ();
         IF(CUENTA<16){           
            IF(CUENTA==15){ //SI SE PRESIONA D (RESET)
               LCD_GOTOXY(1,1);
               PRINTF(LCD_PUTC, "VELOCIDAD 1-5 S \n'A' CONFIRMACION");
               L = -1; //PARA INICIAR EN 0
               Z = 0; //PASA A ETAPA 1
               DO{
                  TECLADO();
               }WHILE(CUENTA<16);
            }            
         }
         DELAY_MS(MS); //DELAY DEL TIEMPO PROPORCIONADO POR USUARIO EN MS
         L++; //AUMENTA L EN 1
         IF(L > 50){ // REINICIA LA CUENTA SI L PASA DE 50
            L = 0;             
         }
      }
   }
}
