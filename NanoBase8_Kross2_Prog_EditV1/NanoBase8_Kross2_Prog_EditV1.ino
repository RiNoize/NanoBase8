
#include <MIDI.h>
#include "potenciometros.h"
#include "ArraysTables.h"
#include <PS2KeyAdvanced.h>

//------------------------------------------------  Definicion de los pines conectados al teclado PS2-------------------------------------------------------//
#define DATAPIN 2
#define IRQPIN 3

uint16_t keyValue;
uint8_t breaks = 0;
uint8_t repeats = 0;
int currentIndex = 0;

PS2KeyAdvanced keyboard;
//------------------------------------------------- Hasta aqui definiciones del teclado --------------------------------------------------------------------//
//int aux = 0;
bool startStop = true;
bool Boton1 = true;
bool Boton2 = true;
bool MonoUnison = true;

byte patchNum = 0;
byte bankNum = 0;
byte bankSelect = 0;

// Se crea la instancia MIDI
MIDI_CREATE_INSTANCE(HardwareSerial, Serial, MIDI);

byte NUMBER_POTS = 8;

//ooo   Definir los potenciometros conectados  oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo//

// Pot (Pin Number, Command, CC Control, Channel Number)
//** El parametro Command es para diferentes mapeos y usos de potenciometro

//Default
Pot PO1(A0, 0, 73, 1); // Attack
Pot PO2(A1, 0, 72, 1); // Release
Pot PO3(A2, 0, 74, 1); // CutOff
Pot PO4(A3, 0, 71, 1); // Resonance
Pot PO5(A4, 0, 5, 1);  // Portamento Time
Pot PO6(A5, 0, 7, 1);  // Vol ch 1
Pot PO7(A6, 0, 7, 10); // Vol ch 10
Pot PO8(A7, 0, 7, 16); // Vol ch 16

//0000000000000000000000000000000000000000000000000000000000000000000
Pot *POTS[]{&PO1, &PO2, &PO3, &PO4, &PO5, &PO6, &PO7, &PO8};
//0000000000000000000000000000000000000000000000000000000000000000000


void setup() {

  MIDI.begin(MIDI_CHANNEL_OMNI);
//  MIDI.turnThruOn();

// inicio y setup del teclado -----------------------------------------------------------------------------------------------------------------------------------------------------//
  keyboard.begin(DATAPIN, IRQPIN);
  delay( 500 );
  keyboard.setNoRepeat(1);
//  keyboard.setNoBreak(1);
  keyboard.setLock(PS2_LOCK_NUM);
  keyValue = keyboard.read();

  
}

void loop() 
{  
//  MIDI.read();

// Activacion de los codigos del teclado con SWITCH y CASE donde cada CASE pertenece a un número perteneciente a cada tecla y sus combinaciones -----------------//

    // read the next key
    keyValue = keyboard.read();

      switch (keyValue)
      {
//ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo MODO PROGRAM oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo//

//PROGRAM CHANGE de a 1 -------------------------- Flecha arriba, flecha abajo -------------------------------------------------------------------------//
    case 279:
    case 4375:
        if (patchNum < 127)
        {
          patchNum++;
          MIDI.sendProgramChange(patchNum, 1);
        }
        break;
    case 280:
    case 4376:
        if (patchNum > 0)
        {
          patchNum--;
          MIDI.sendProgramChange(patchNum, 1);
         }
        break;
//PROGRAM CHANGE de a 10 ------------------------------------- Flecha Izq, Flecha Der -------------------------------------------------------------//
    case 278://flecha der
    case 4374:
        if (patchNum <= 117)
        {
          patchNum=patchNum+10;
          MIDI.sendProgramChange(patchNum, 1);
        }
        break;
    case 277://flecha izq
    case 4373:
        if (patchNum >=11)
        {
          patchNum=patchNum-10;
          MIDI.sendProgramChange(patchNum, 1);
         }
        break;
//BANK CHANGE------------------------------------------Control Key + Flecha Izq, Flecha Der ---------------------------------------------------------------------------//
    case 8470:
          MIDI.sendControlChange(0, bankSelect, 1);
        if (bankNum < 9 ) // cantidad de bancos
        {
          bankNum++;
          MIDI.sendControlChange( 32, bankNum, 1);
          MIDI.sendProgramChange(0, 1);
        }
        break;
        
    case 8469:
          MIDI.sendControlChange(0, bankSelect, 1);
        if (bankNum > 0)
        {
          bankNum--;
          MIDI.sendControlChange( 32, bankNum, 1);
          MIDI.sendProgramChange(0, 1);
        }
        break;


        
//Start Stop Drum Track -------------------------------------------------------- Barra espaciadora --------------------------------------------------------------------------//
   case 287:
   case 4383:
        if (startStop)//true
        {
          startStop = !startStop;          
//        sendSysExDef(0x02, 0x02, 0x02, 0x00); // startInmediatly  
//        sendSysExDef(0x02, 0x02, 0x04, 0x01); // SyncOn 
        sendDef702(0x02, 0x0a, 0x01); // start       
        }
        else//false
        {
          startStop = !startStop;         
        sendDef702(0x02, 0x0a, 0x00); // Stop         
        }
        break;

//Portamento On -------------------Control + insert-------------------//
   case 8473:
        MIDI.sendControlChange(65, 127, 1);
        break; 
//Portamento Off -------------------Control + Supr-------------------//
   case 8474:
        MIDI.sendControlChange(65, 01, 1);
        break; 

//Button 1 -------------------------------------- imp pant Pet Sis----------------- MODO PROGRAM Y COMBI ------------------------------------------------------//
   case 260:
   case 4356:   
        if (Boton1)//true
        {
          Boton1 = !Boton1;
        sendDef702(0x01, 0x08, 0x01); // on Program
        MIDI.sendControlChange(80, 127, 1);
        }
        else//false
        {
          Boton1 = !Boton1; 
        sendDef702(0x01, 0x08, 0x00); // off Program
        MIDI.sendControlChange(80, 1, 1);
        }
        break;

//Button 2 ------------------------------------------ Pause inter ----------------- MODO PROGRAM Y COMBI -----------------------------------------------------------//
   case 6:
        if (Boton2)//true
        {
          Boton2 = !Boton2;
        sendDef702(0x01, 0x09, 0x01); // on Program
        MIDI.sendControlChange(81, 127, 1);
        }
        else//false
        {
          Boton2 = !Boton2; 
        sendDef702(0x01, 0x09, 0x00); // off Program
        MIDI.sendControlChange(81, 1, 1);
        }
        break;  

// Screen ----------------------------------------------------------------------------------------------------------------------------------------------------------//
   case 270:
        defArray9bytes( 0x4e, 0x03);  // on screen ------ Hoja      
        break;
   case 782:
        defArray9bytes( 0x4e , 0x02);  // off screen ----- Win + Hoja

// todos reverbs WET        
   case 8467: // Todos los reverbs WET.   All the reverbs WET----------- Control + RePag ----------------------------------------------//
 
        break;                   

//Limitar ARP hasta. Limit ARP up to --------------------------------------------------------------------------------------------------------------------------//
   case 275: // RePag
      arpControls(0x0b, 0x3b); // B3
      break;

   case 276: // AvPag
      arpControls(0x0b, 0x47); // B4
      
      break;
   case 273: // Inicio
      arpControls(0x0b, 0x7f); // G9
      
      break;
   case 274: // Fin
      arpControls(0x0b, 0x2f); // B2
      break;      
               
//POLY / MONO ---------------------------------------------------------------------------------------------------------------------------------------------------//
   case 281: //  Insert  
      sendDef703(0x00, 0x02, 0x00); // Poly
      break;

   case 282: //  Supr    
      sendDef703(0x00, 0x02, 0x01); // Mono
      break;      

//FX STEP SEQ LR SEND Full Vel --------------------------------------- Control + inicio ------------------------------------------------------------------------//  
   case 8465: //  
//      sendSysExDef(0x02, 0x09, 0x04, 0x01); // StepSeq_LR
      sendDef702(0x03, 0x01, 0x3f); // StepSeqVelFull
      break;
/*
//DISTORTED  MFX1
   case 8468: // ------------------------------------------------- Control + AvPag ----------------------------------------------------------------------------//

      break;  

//FX2 Reverb Hall --------------------------------------- Control + Fin ----------------------------------------------------------------------------------------//  
   case 8466: //  
 
      break;
*/
//Boton RealTime Select en Tempo --------------------------------------- Control + Hoja ---------------------------------------------------------------------------------------//  
   case 8462: //  
      sendDef702(0x01, 0x14, 0x05); // RealTime Select en Tempo
      break;

// Abierto/Cerrado

//Filtro Cerrado  ------------------- Ctrl + Win -------------- Program y Modo Edit -----//
   case 8972:
   case 13068:
        MIDI.sendControlChange(74, 10, 1);
        break; 

//Filtro Abierto ----------------------------------------------//
   case 41228:
   case 45324:
        MIDI.sendControlChange(74, 64, 1);
        break; 

// Abierto/Todo Abierto

//Filtro Todo Abierto  ------------------- Ctrl + Alt --------- Program y Modo edit ----------//
   case 10506:
   case 14602:
        MIDI.sendControlChange(74, 127, 1);
        break; 

//Filtro Abierto --------------------------------------------------------//
   case 41226:
   case 45322:
        MIDI.sendControlChange(74, 63, 1);
        break; 

// Sostenuto ------------------- BloqDesplazar key ------------ Program -------//
   case 258: 
        MIDI.sendControlChange(66, 127, 1); // Sostenuto On
        break; 
   case 33026:
        MIDI.sendControlChange(66, 00, 1);  // Sostenuto Off
        break; 


//ARPs ----------------------- Control Key---------------------------------------------------------------------------------------------------------------------//

//  1
   case 8241: 
      sendSysExArpUp 
        break;                   
// 2
   case 8242:
      sendSysExArpDown
        break;                   
// 3
   case 8243:
      sendSysExArpAlt1
        break;                   
// 4
   case 8244:
      sendSysExArpAlt2
        break;      
// 5
   case 8245:
      sendSysExArpRndm
        break;                   
// 6
   case 8246:
      sendSysExArp0003
        break;                   
// 7
   case 8247:
      sendSysExArp0004
        break;                   
// 8
   case 8248:
      sendSysExArp0006
        break;                   
// 9
   case 8249:
      sendSysExArp0009
        break;                   
// 0
   case 8240:
      sendSysExArp0019
        break;                   
// ?
   case 8252:
      sendSysExArp0020
        break;                   
// ¡
   case 8287:
      sendSysExArp0023
        break;                   
// }
   case 8284:
      sendSysExArp0027
        break;                   
// Back
   case 8476:
      sendSysExArp0028
        break;                   
// Q
   case 8273:
      sendSysExArp0531
        break;                   
// W
   case 8279:
      sendSysExArp0532
        break;                   
//  E
   case 8261:
      sendSysExArp0533
        break;                   
// R
   case 8274:
      sendSysExArp0534
        break;                   
// T
   case 8276:
      sendSysExArp0535
        break;                   
// Y
   case 8281:
      sendSysExArp0536
        break;      
// U
   case 8277:
      sendSysExArp0537
        break;                   
// I
   case 8265:
      sendSysExArp0538
        break;                   
// O
   case 8271:
      sendSysExArp0539
        break;                   
// P
   case 8272:
      sendSysExArp0540
        break;                   
// [
   case 8285:
      sendSysExArp0541
        break;                   
// ]
   case 8286:
      sendSysExArp0542
        break;                   
// A
   case 8257:
      sendSysExArp0543
        break;                   
// S
   case 8275:
      sendSysExArp0544
        break;                   
// D
   case 8260:
      sendSysExArp0545
        break;                   
// F
   case 8262:
      sendSysExArp0546
        break;                   
// G
   case 8263:
      sendSysExArp0547
        break;                   
// H
   case 8264:
      sendSysExArp0548
        break;                 
// J
   case 8266:
      sendSysExArp0549
        break;                   
// K
   case 8267:
      sendSysExArp0550
        break;                   
// L
   case 8268:
      sendSysExArp0551
        break;                   
// Ñ
   case 8283:
      sendSysExArp0552
        break;      
// {
   case 8250:
      sendSysExArp0553
        break;                          
// <>
   case 8331:
      sendSysExArp0019
        break;                   
// Z
   case 8282:
      sendSysExArp0106
        break;                   
// X
   case 8280:
      sendSysExArp0108
        break;                   
// C
   case 8259:
      sendSysExArp0009 
        break;                   
// V
   case 8278:
      sendSysExArp0186
        break;                   
// B
   case 8258:
      sendSysExArp0188
        break;                   
// N
   case 8270:
      sendSysExArp0191
        break;                   
// M
   case 8269:
      sendSysExArp0201
        break;                   
// ,
   case 8251:
      sendSysExArp0202
        break;                   
// .
   case 8253:
      sendSysExArp0203
        break;                   
// _
   case 8254:
      sendSysExArp0206
        break; 

//LATCH ARP -------------------------------- Control Key -------------------------------------------//
  case 8224: // 0
      arpControls(0x07, 0x01); //  onLatch
      break;
  case 8234: // .
      arpControls(0x07, 0x00); // offLatch
      break;

//ARP SORT
  case 8228: // 4
      arpControls(0x06, 0x01); // sortOn
      break;
  case 8229: // 5
      arpControls(0x06, 0x00); // sortOff
      break;

//ARP KEY SYNC
  case 8231: // 7
      arpControls(0x08, 0x01); // onKeySync
      break;
  case 8232: // 8
      arpControls(0x08, 0x00); // offKeySync
      break;

//ARP KEYBOARD
  case 8225: // 1
      arpControls(0x09, 0x01); // onKeyboard
      break;
  case 8226: // 2
      arpControls(0x09, 0x00); // offKeyboard
      break;

//ARP ON/OFF
   case 8235: // intro
      MIDI.sendControlChange(14, 127, 1);  // on
      break;
   case 8227: // 3
      MIDI.sendControlChange(14, 00, 1);  //  Off
      break;
            
//ARP  OCTAVE 
   case 8239: // /
      arpControls(0x02, 0x00);  // Arp octave 1
      break;
   case 8238: // *
      arpControls(0x02, 0x01);  // Arp octave 2
      break;
   case 8237: // -
      arpControls(0x02, 0x02);  // Arp octave 3
      break;    


//ARP  RESOLUTION 
   case 8233: //  9 
      arpControls(0x01, 0x03);  // Arp Res SemiCorchea
      break;
   case 8230: //  6
      arpControls(0x01, 0x07);  // Arp Res Negra
      break;        
   case 8236: //  +
      arpControls(0x01, 0x05);  // Arp Res Corchea
      break;

// DRUM TRACK BUS SELECT --------------------------------------------------- WIN key ------------------------------//        

   case 873: // DKIT---------F9
        DrumTrackBusSelect(0x00); //
        break;
   case 874: // L/R---------F10
        DrumTrackBusSelect(0x01); //
        break;
   case 875: // IFX1-------F11
        DrumTrackBusSelect(0x02); //
        break;
   case 876: // IFX2-------F12
        DrumTrackBusSelect(0x03); //
        break;
   case 912: // IFX3------Desp
        DrumTrackBusSelect(0x04); //
        break;
   case 909: // IFX4------Dormir
        DrumTrackBusSelect(0x05); //
        break;
   case 908: // IFX5------Apagar
        DrumTrackBusSelect(0x06); //
        break;

//DRUM TRIGGER MODE 
   case 64: // 
   case 4160:
      sendDef702(0x02, 0x02, 0x01); // waitKeyboard
      break;
   case 285: // 
   case 4381:
      sendDef702(0x02, 0x02, 0x00); // startInmediat
      sendDef702(0x02, 0x04, 0x01); // drumsSyncOn    
      break;

//DRUM Patterns A -----------------------------------------------------ALT key ------------------------------------------------------------------------//

// 1
   case 2097:
        sendSysExDrum270
        break;                        
// 2
   case 2098:
        sendSysExDrum272
        break;                
// 3
   case 2099:
        sendSysExDrum274
        break;                
// 4
   case 2100:
        sendSysExDrum269
        break;      
// 5
   case 2101:
        sendSysExDrum267
        break;                
// 6
   case 2102:
        sendSysExDrum260
        break;                
// 7
   case 2103:
        sendSysExDrum258
        break;                
// 8
   case 2104:
        sendSysExDrum384
        break;                
// 9
   case 2105:
        sendSysExDrum393
        break;        
// 0
   case 2096:
        sendSysExDrum394
        break;      
// ?
   case 2108:
        sendSysExDrum396
        break;         
// ¡
   case 2143:
        sendSysExDrum398
        break;          
// }        
   case 2140:
        sendSysExDrum400
        break;          
// Back        
   case 2332:
        sendSysExDrum404
        break;          
// Q
   case 2129:
        sendSysExDrum405
        break;  
// W                    
   case 2135:
        sendSysExDrum628
        break;         
// E
   case 2117:
        sendSysExDrum634
        break;       
// R
   case 2130:
        sendSysExDrum640
        break;        
// T
   case 2132:
        sendSysExDrum641
        break;       
// Y        
   case 2137:
        sendSysExDrum642
        break;     
// U
   case 2133:
        sendSysExDrum643
        break;       
// I        
   case 2121:
        sendSysExDrum652
        break;        
// O
   case 2127:
        sendSysExDrum667
        break;        
// P
   case 2128:
        sendSysExDrum675
        break;   
// [
   case 2141:
        sendSysExDrum422
        break;  
// ]                
   case 2142:
        sendSysExDrum423
        break;        
// A
   case 2113:
        sendSysExDrum438
        break;       
// S
   case 2131:
        sendSysExDrum451
        break;
// D
   case 2116:
        sendSysExDrum460
        break;
// F
   case 2118:
        sendSysExDrum485
        break;
// G
   case 2119:
        sendSysExDrum489
        break;
// H
   case 2120:
        sendSysExDrum504
        break;
// J
   case 2122:
        sendSysExDrum558
        break;
// K
   case 2123:
        sendSysExDrum569
        break;
// L
   case 2124:
        sendSysExDrum570
        break;     
// Ñ
   case 2139:
        sendSysExDrum572
        break;
// {
   case 2106:
        sendSysExDrum764
        break;
// <>
   case 2187:
        sendSysExDrum001
        break;
// Z
   case 2138:
        sendSysExDrum002
        break;
// X
   case 2136:
        sendSysExDrum003
        break;
// C
   case 2115:
        sendSysExDrum004
        break;
// V
   case 2134:
        sendSysExDrum005
        break;
// B
   case 2114:
        sendSysExDrum006
        break;
// N
   case 2126:
        sendSysExDrum007
        break;     
// M
   case 2125:
        sendSysExDrum008
        break;
// ,
   case 2107:
        sendSysExDrum782
        break;       
// .
   case 2109:
        sendSysExDrum783
        break;
// -
   case 2110:
        sendSysExDrum784
        break;

//DRUM KITS-----------------------------------------------------WIN key-----------------------------------------------------------------------------------------------------------------------//

//ElectronicDrumKit 1
   case 561:
        sendSysExDrumKit015
        break;                   
//ElectronicDrumKit 2
   case 562:
        sendSysExDrumKit016
        break;                   
//ElectronicDrumKit 3
   case 563:
        sendSysExDrumKit032
        break;                   
//ElectronicDrumKit 4
   case 564:
        sendSysExDrumKit033
        break;      
//ElectronicDrumKit 5
   case 565:
        sendSysExDrumKit013
        break;                   
//ElectronicDrumKit 6
   case 566:
        sendSysExDrumKit021
        break;                   
//ElectronicDrumKit 7
   case 567:
        sendSysExDrumKit023
        break;                   
//ElectronicDrumKit 8
   case 568:
        sendSysExDrumKit026
        break;                   
//ElectronicDrumKit 9
   case 569:
        sendSysExDrumKit027
        break;                   
//ElectronicDrumKit 0
   case 560:
        sendSysExDrumKit028
        break;                   
//ElectronicDrumKit ?
   case 572:
        sendSysExDrumKit029
        break;                   
//ElectronicDrumKit ¡
   case 607:
        sendSysExDrumKit030
        break;                   
//ElectronicDrumKit }
   case 604:
        sendSysExDrumKit024
        break;                   
//ElectronicDrumKit Back
   case 796:
        sendSysExDrumKit025
        break;                   
//ElectronicDrumKit Q
   case 593:
        sendSysExDrumKit012
        break;                   
//ElectronicDrumKit W
   case 599:
        sendSysExDrumKit014
        break;                   
//ElectronicDrumKit E
   case 581:
        sendSysExDrumKit022
        break;                   
//ElectronicDrumKit R
   case 594:
        sendSysExDrumKit031
        break;                   
//ElectronicDrumKit T
   case 596:
        sendSysExDrumKit034
        break;                   
//ElectronicDrumKit Y
   case 601:
        sendSysExDrumKit153
        break;                   
//ElectronicDrumKit U
   case 597:
        sendSysExDrumKit152
        break;                   
//ElectronicDrumKit I
   case 585:
        sendSysExDrumKit161
        break;                   
//ElectronicDrumKit O
   case 591:
        sendSysExDrumKit162
        break;                   
//ElectronicDrumKit P
   case 592:
        sendSysExDrumKit168
        break;   
//ElectronicDrumKit [
   case 605:
        sendSysExDrumKit166
        break;                   
//ElectronicDrumKit ]
   case 606:
        sendSysExDrumKit167
        break;                   
//AcousticDrumKit A
   case 577:
        sendSysExDrumKit000
        break;                   
//AcousticDrumKit S
   case 595:
        sendSysExDrumKit002
        break;
//AcousticDrumKit D
   case 580:
        sendSysExDrumKit003
        break;
//AcousticDrumKit F
   case 582:
        sendSysExDrumKit005
        break;
//AcousticDrumKit G
   case 583:
        sendSysExDrumKit006
        break;
//AcousticDrumKit H
   case 584:
        sendSysExDrumKit007
        break;
//AcousticDrumKit J
   case 586:
        sendSysExDrumKit008
        break;
//AcousticDrumKit K
   case 587:
        sendSysExDrumKit009
        break;
//AcousticDrumKit L
   case 588:
        sendSysExDrumKit010
        break;     
//AcousticDrumKit Ñ
   case 603:
        sendSysExDrumKit011
        break;
//AcousticDrumKit {
   case 570:
        sendSysExDrumKit018
        break;
//VariosDrumKit   <>
   case 651:
        sendSysExDrumKit036
        break;
//VariosDrumKit   Z
   case 602:
        sendSysExDrumKit026
        break;
//VariosDrumKit   X
   case 600:
        sendSysExDrumKit037
        break;
//VariosDrumKit   C
   case 579:
        sendSysExDrumKit038
        break;
//VariosDrumKit   V
   case 598:
        sendSysExDrumKit149
        break;
//VariosDrumKit   B
   case 578:
        sendSysExDrumKit150
        break;
//VariosDrumKit   N
   case 590:
        sendSysExDrumKit151
        break;     
//VariosDrumKit   M
   case 589:
        sendSysExDrumKit154
        break;
//VariosDrumKit   ,
   case 571:
        sendSysExDrumKit155
        break;        
//VariosDrumKit   .
   case 573:
        sendSysExDrumKit047
        break;
//VariosDrumKit   -
   case 574:
        sendSysExDrumKit043
        break;



//SHIFT DRUMS------------------------------------DEF----------------------------------------------------------------//

//ShiftDrum-24
  case 49: //
    ShiftDrumsT924
    break;
//ShiftDrum-23
  case 50: //
    ShiftDrumsT923
    break;
//ShiftDrum-22
  case 51: // 
    ShiftDrumsT922
    break;
//ShiftDrum-21
  case 52: //
    ShiftDrumsT921
    break;
//ShiftDrum-20
  case 53: // 
    ShiftDrumsT920
    break;
//ShiftDrum-19
  case 54: //
    ShiftDrumsT919
    break;
//ShiftDrum-18
  case 55: // 
    ShiftDrumsT918
    break;
//ShiftDrum-17
  case 56: //
    ShiftDrumsT917
    break;
//ShiftDrum-16
  case 57: //  
    ShiftDrumsT916
    break;
//ShiftDrum-15
  case 48: //
    ShiftDrumsT915
    break;
//ShiftDrum-14
  case 60: // 
    ShiftDrumsT914
    break;
//ShiftDrum-13
  case 95: // 
    ShiftDrumsT913
    break;
//ShiftDrum-12
  case 92: //
    ShiftDrumsT912
    break;
//ShiftDrum-11
  case 284: // 
    ShiftDrumsT911
    break;
//ShiftDrum-10
  case 81: // 
    ShiftDrumsT910
    break;
//ShiftDrum-09
  case 87: // 
    ShiftDrumsT909
    break;
//ShiftDrum-08
  case 69: // 
    ShiftDrumsT908
    break;
//ShiftDrum-07
  case 82: // 
    ShiftDrumsT907
    break;
//ShiftDrum-06
  case 84: // 
    ShiftDrumsT906
    break;
//ShiftDrum-05
  case 89: // 
    ShiftDrumsT905
    break;
//ShiftDrum-04
  case 85: // 
    ShiftDrumsT904
    break;
//ShiftDrum-03
  case 73: // 
    ShiftDrumsT903
    break;
//ShiftDrum-02
  case 79: // 
    ShiftDrumsT902
    break;
//ShiftDrum-01
  case 80: // 
    ShiftDrumsT901
    break;
//ShiftDrum 00
  case 286: //
    ShiftDrumsT000
    break;
//ShiftDrum 01
  case 93: // 
    ShiftDrumsT001
    break;
//ShiftDrum 02
  case 94: // 
    ShiftDrumsT002
    break;
//ShiftDrum 03
  case 65: // 
    ShiftDrumsT003
    break;
//ShiftDrum 04
  case 83: // 
    ShiftDrumsT004
    break;
//ShiftDrum 05
  case 68: // 
    ShiftDrumsT005
    break;
//ShiftDrum 06
  case 70: // 
    ShiftDrumsT006
    break;
//ShiftDrum 07
  case 71: // 
    ShiftDrumsT007
    break;
//ShiftDrum 08
  case 72: // 
    ShiftDrumsT008
    break;
//ShiftDrum 09
  case 74: // 
    ShiftDrumsT009
    break;
//ShiftDrum 10
  case 75: // 
    ShiftDrumsT010
    break;
//ShiftDrum 11
  case 76: //
    ShiftDrumsT011
    break;
//ShiftDrum 12
  case 91: // 
    ShiftDrumsT012
    break;
//ShiftDrum 13
  case 58: // 
    ShiftDrumsT013
    break;
//ShiftDrum 14
  case 139: //
    ShiftDrumsT014
    break;
//ShiftDrum 15
  case 90: //
    ShiftDrumsT015
    break;
//ShiftDrum 16
  case 88: //
    ShiftDrumsT016
    break;
//ShiftDrum 17
  case 67: // 
    ShiftDrumsT017
    break;
//ShiftDrum 18
  case 86: // 
    ShiftDrumsT018
    break;
//ShiftDrum 19
  case 66: // 
    ShiftDrumsT019
    break;
//ShiftDrum 20
  case 78: // 
    ShiftDrumsT020
    break;
//ShiftDrum 21
  case 77: //
    ShiftDrumsT021
    break;
//ShiftDrum 22
  case 59: //
    ShiftDrumsT022
    break;
//ShiftDrum 23
  case 61: //
    ShiftDrumsT023
    break;
//ShiftDrum 24
  case 62: // 
    ShiftDrumsT024
    break; 



//Step Seq DRUM KITS  --------------------------Shift ------------------------------------------------//

//ElectronicSSDrumKit 1
   case 16433:
        sendSysExSSdrumKit015 
        break;                   
//ElectronicSSDrumKit 2
   case 16434:
        sendSysExSSdrumKit016 
        break;                   
//ElectronicSSDrumKit 3
   case 16435:
        sendSysExSSdrumKit032 
        break;                   
//ElectronicSSDrumKit 4
   case 16436:
        sendSysExSSdrumKit033 
        break;      
//ElectronicSSDrumKit 5
   case 16437:
        sendSysExSSdrumKit013 
        break;                   
//ElectronicSSDrumKit 6
   case 16438:
        sendSysExSSdrumKit021 
        break;                   
//ElectronicSSDrumKit 7
   case 16439:
        sendSysExSSdrumKit023 
        break;                   
//ElectronicSSDrumKit 8
   case 16440:
        sendSysExSSdrumKit026 
        break;                   
//ElectronicSSDrumKit 9
   case 16441:
        sendSysExSSdrumKit027 
        break;                   
//ElectronicSSDrumKit 10
   case 16432:
        sendSysExSSdrumKit028 
        break;                   
//ElectronicSSDrumKit 11
   case 16444:
        sendSysExSSdrumKit029 
        break;                   
//ElectronicSSDrumKit 12
   case 16479:
        sendSysExSSdrumKit030 
        break;                   
//ElectronicSSDrumKit 13
   case 16476:
        sendSysExSSdrumKit024 
        break;                   
//ElectronicSSDrumKit 14
   case 16668:
        sendSysExSSdrumKit025 
        break;                           
//AcousticSSDrumKit 1
   case 16465:
        sendSysExSSdrumKit000 
        break;                   
//AcousticSSDrumKit 2
   case 16471:
        sendSysExSSdrumKit002 
        break;
//AcousticSSDrumKit 3
   case 16453:
        sendSysExSSdrumKit003 
        break;
//AcousticSSDrumKit 4
   case 16466:
        sendSysExSSdrumKit005 
        break;
//AcousticSSDrumKit 5
   case 16468:
        sendSysExSSdrumKit006 
        break;
//AcousticSSDrumKit 6
   case 16473:
        sendSysExSSdrumKit007 
        break;
//AcousticSSDrumKit 7
   case 16469:
        sendSysExSSdrumKit008 
        break;
//AcousticSSDrumKit 8
   case 16457:
        sendSysExSSdrumKit009 
        break;
//AcousticSSDrumKit 9
   case 16463:
        sendSysExSSdrumKit010 
        break;     
//AcousticSSDrumKit 10
   case 16464:
        sendSysExSSdrumKit011 
        break;
//AcousticSSDrumKit 11
   case 16477:
        sendSysExSSdrumKit018 
        break;
//AcousticSSDrumKit 12
   case 16478:
        sendSysExSSdrumKit017 
        break;           

//SS BassDrums
   case 16523://<>
        BassDrum01
        break;
   case 16474://Z
        BassDrum02
        break;
   case 16472://X
        BassDrum03
        break;
   case 16451://C
        BassDrum04
        break;
   case 16470://V
        BassDrum05
        break;
   case 16450://B
        BassDrum06
        break;
   case 16462://N
        BassDrum07
        break;     
   case 16461://M
        BassDrum08
        break;
   case 16443://;
        BassDrum09
        break;        
   case 16445://:
        BassDrum10
        break;
   case 16446://-
        BassDrum11
        break;        

//SS Snare1Drums
   case 16449://<>
        Snare1Drum01
        break;
   case 16467://Z
        Snare1Drum02
        break;
   case 16452://X
        Snare1Drum03
        break;
   case 16454://C
        Snare1Drum04
        break;
   case 16455://V
        Snare1Drum05
        break;
   case 16456://B
        Snare1Drum06
        break;
   case 16458://N
        Snare1Drum07
        break;     
   case 16459://M
        Snare1Drum08
        break;
   case 16460://;
        Snare1Drum09
        break;        
   case 16475://:
        Snare1Drum10
        break;
   case 16442://-
        Snare1Drum11
        break;                


// STEP SEQ BUS SELECT ------------------- Shift +  F9 to Apagar -------------------------------//        

   case 16745: // DKIT
        SSBusSelect(0x00); //
        break;
   case 16746: // L/R
        SSBusSelect(0x01); //
        break;
   case 16747: // IFX1
        SSBusSelect(0x02); //
        break;
   case 16748: // IFX2
        SSBusSelect(0x03); //
        break;
   case 16784: // IFX3
        SSBusSelect(0x04); //
        break;
   case 16781: // IFX4
        SSBusSelect(0x05); //
        break;
   case 16780: // IFX5
        SSBusSelect(0x06); //
        break;       




// 12 Programs ----------------------------------------- Control + Shift + F1 a F12 ----------------------------- Programs  de 500 a 511 de USER----------------------------------------------------// 
   case 24929: // F1
          sendMIDIProgramChange(116);
          break;           
   case 24930: // F2
          sendMIDIProgramChange(117);   
          break;                     
   case 24931: // F3
          sendMIDIProgramChange(118);   
          break;           
   case 24932: // F4
          sendMIDIProgramChange(119); 
          break; 
   case 24933: // F5
          sendMIDIProgramChange(120);
          break;           
   case 24934: // F6
          sendMIDIProgramChange(121);  
          break;                     
   case 24935: // F7
          sendMIDIProgramChange(122);
          break;           
   case 24936: // F8
          sendMIDIProgramChange(123);
          break; 
   case 24937: // F9
          sendMIDIProgramChange(124);
          break;           
   case 24938: // F10
          sendMIDIProgramChange(125);
          break;                     
   case 24939: // F11
          sendMIDIProgramChange(126);
          break;           
   case 24940: // F12
          sendMIDIProgramChange(127); 
          break;   

// TEMPOS ---------------------------------------------------- Win Key + teclado numeral -------------------------------------------------------------------------------//
// 0
   case 544:
      tempo120
        break;           
//  1
   case 545: 
      tempo80 
        break;                   
// 2
   case 546:
      tempo90
        break;                   
// 3
   case 547:
      tempo100
        break;                   
// 4
   case 548:
      tempo110
        break;      
// 5
   case 549:
      tempo126
        break;                   
// 6
   case 550:
      tempo128
        break;                   
// 7
   case 551:
      tempo135
        break;                   
// 8
   case 552:
      tempo140
        break;                   
// 9
   case 553:
      tempo145
        break;                   

// Step Seq Pads Patterns

 case 257: // Configuracion para los Patterns del Step Seq ------------------------------ ( Bloq Num )  ------------------------------------------------------------------------------------------//
        sendDef702(0x05, 0x01, 0x10); // SSLength16C
        sendDef702(0x04, 0x00, 0x03); // CsemicorcheaSS
        sendDef702(0x05, 0x19, 0x00); // instrumento 12 (SFX) en All
        sendDef702(0x05, 0x15, 0x00); // instrumento 10 (Perc1) en All
        sendDef702(0x05, 0x0f, 0x00); // Instrumento 7 (Ride) en All
        break;                                               


   case 42: //---------------- ( . )  ----------Hi part Muted-----------------------//
        SSpatternsPad1Hi  (0x00); // 1
        SSpatternsPad2Hi  (0x00); // 2
        SSpatternsPad3Hi  (0x00); // 3
        SSpatternsPad4Hi  (0x00); // 4
        SSpatternsPad5Hi  (0x00); // 5
        SSpatternsPad6Hi  (0x00); // 6
        SSpatternsPad7Hi  (0x00); // 7
        SSpatternsPad8Hi  (0x00); // 8
        SSpatternsPad9Hi  (0x00); // 9
        SSpatternsPad10Hi (0x00); // 10
        SSpatternsPad11Hi (0x00); // 11
        SSpatternsPad12Hi (0x00); // 12
        SSpatternsPad13Hi (0x00); // 13
        SSpatternsPad14Hi (0x00); // 14
        SSpatternsPad15Hi (0x00); // 15
        SSpatternsPad16Hi (0x00); // 16 
        break;                                               

   case 32: //----------------  0  ----------Lo Muted-----------------------//
        SSpatternsPad1Lo  (0x00); // 1
        SSpatternsPad2Lo  (0x00); // 2
        SSpatternsPad3Lo  (0x00); // 3
        SSpatternsPad4Lo  (0x00); // 4
        SSpatternsPad5Lo  (0x00); // 5
        SSpatternsPad6Lo  (0x00); // 6
        SSpatternsPad7Lo  (0x00); // 7
        SSpatternsPad8Lo  (0x00); // 8
        SSpatternsPad9Lo  (0x00); // 9
        SSpatternsPad10Lo (0x00); // 10
        SSpatternsPad11Lo (0x00); // 11
        SSpatternsPad12Lo (0x00); // 12
        SSpatternsPad13Lo (0x00); // 13
        SSpatternsPad14Lo (0x00); // 14
        SSpatternsPad15Lo (0x00); // 15
        SSpatternsPad16Lo (0x00); // 16
        break;            

   case 33: //----------------  1  ----------Kick drum 1,5,9,13-----------------------//
        SSpatternsPad1Lo  (0x01); // 1
        SSpatternsPad2Lo  (0x00); // 2
        SSpatternsPad3Lo  (0x00); // 3
        SSpatternsPad4Lo  (0x00); // 4
        SSpatternsPad5Lo  (0x01); // 5
        SSpatternsPad6Lo  (0x00); // 6
        SSpatternsPad7Lo  (0x00); // 7
        SSpatternsPad8Lo  (0x00); // 8
        SSpatternsPad9Lo  (0x01); // 9
        SSpatternsPad10Lo (0x00); // 10
        SSpatternsPad11Lo (0x00); // 11
        SSpatternsPad12Lo (0x00); // 12
        SSpatternsPad13Lo (0x01); // 13
        SSpatternsPad14Lo (0x00); // 14
        SSpatternsPad15Lo (0x00); // 15
        SSpatternsPad16Lo (0x00); // 16
        break;            

   case 34: //----------------  2  ----------Kick drum 1,3,5,7,9,11,13,15-----------------------//
        SSpatternsPad1Lo  (0x01); // 1
        SSpatternsPad2Lo  (0x00); // 2
        SSpatternsPad3Lo  (0x01); // 3
        SSpatternsPad4Lo  (0x00); // 4
        SSpatternsPad5Lo  (0x01); // 5
        SSpatternsPad6Lo  (0x00); // 6
        SSpatternsPad7Lo  (0x01); // 7
        SSpatternsPad8Lo  (0x00); // 8
        SSpatternsPad9Lo  (0x01); // 9
        SSpatternsPad10Lo (0x00); // 10
        SSpatternsPad11Lo (0x01); // 11
        SSpatternsPad12Lo (0x00); // 12
        SSpatternsPad13Lo (0x01); // 13
        SSpatternsPad14Lo (0x00); // 14
        SSpatternsPad15Lo (0x01); // 15
        SSpatternsPad16Lo (0x00); // 16
        break;                    

   case 35: //----------------  3  ----------Kick drum all-----------------------//
        SSpatternsPad1Lo  (0x01); // 1
        SSpatternsPad2Lo  (0x01); // 2
        SSpatternsPad3Lo  (0x01); // 3
        SSpatternsPad4Lo  (0x01); // 4
        SSpatternsPad5Lo  (0x01); // 5
        SSpatternsPad6Lo  (0x01); // 6
        SSpatternsPad7Lo  (0x01); // 7
        SSpatternsPad8Lo  (0x01); // 8
        SSpatternsPad9Lo  (0x01); // 9
        SSpatternsPad10Lo (0x01); // 10
        SSpatternsPad11Lo (0x01); // 11
        SSpatternsPad12Lo (0x01); // 12
        SSpatternsPad13Lo (0x01); // 13
        SSpatternsPad14Lo (0x01); // 14
        SSpatternsPad15Lo (0x01); // 15
        SSpatternsPad16Lo (0x01); // 16
        break; 
         
   case 36: //----------------  4  ----------Kick drum 1,5,9,13 and snare 5,13-----------------------//
        SSpatternsPad1Lo  (0x01); // 1
        SSpatternsPad2Lo  (0x00); // 2
        SSpatternsPad3Lo  (0x00); // 3
        SSpatternsPad4Lo  (0x00); // 4
        SSpatternsPad5Lo  (0x03); // 5
        SSpatternsPad6Lo  (0x00); // 6
        SSpatternsPad7Lo  (0x00); // 7
        SSpatternsPad8Lo  (0x00); // 8
        SSpatternsPad9Lo  (0x01); // 9
        SSpatternsPad10Lo (0x00); // 10
        SSpatternsPad11Lo (0x00); // 11
        SSpatternsPad12Lo (0x00); // 12
        SSpatternsPad13Lo (0x03); // 13
        SSpatternsPad14Lo (0x00); // 14
        SSpatternsPad15Lo (0x00); // 15
        SSpatternsPad16Lo (0x00); // 16
        break;            
    case 37: //----------------  5  ----------Kick drum 1,5,9,13 and snare 1,5,9,13-----------------------//
        SSpatternsPad1Lo  (0x03); // 1
        SSpatternsPad2Lo  (0x00); // 2
        SSpatternsPad3Lo  (0x00); // 3
        SSpatternsPad4Lo  (0x00); // 4
        SSpatternsPad5Lo  (0x03); // 5
        SSpatternsPad6Lo  (0x00); // 6
        SSpatternsPad7Lo  (0x00); // 7
        SSpatternsPad8Lo  (0x00); // 8
        SSpatternsPad9Lo  (0x03); // 9
        SSpatternsPad10Lo (0x00); // 10
        SSpatternsPad11Lo (0x00); // 11
        SSpatternsPad12Lo (0x00); // 12
        SSpatternsPad13Lo (0x03); // 13
        SSpatternsPad14Lo (0x00); // 14
        SSpatternsPad15Lo (0x00); // 15
        SSpatternsPad16Lo (0x00); // 16
        break;                                      
    case 38: //----------------  6  ----------Kick drum 1,5,9,13 and snare 1,3,5,7,9,11,13,15-----------------------//
        SSpatternsPad1Lo  (0x03); // 1
        SSpatternsPad2Lo  (0x00); // 2
        SSpatternsPad3Lo  (0x02); // 3
        SSpatternsPad4Lo  (0x00); // 4
        SSpatternsPad5Lo  (0x03); // 5
        SSpatternsPad6Lo  (0x00); // 6
        SSpatternsPad7Lo  (0x02); // 7
        SSpatternsPad8Lo  (0x00); // 8
        SSpatternsPad9Lo  (0x03); // 9
        SSpatternsPad10Lo (0x00); // 10
        SSpatternsPad11Lo (0x02); // 11
        SSpatternsPad12Lo (0x00); // 12
        SSpatternsPad13Lo (0x03); // 13
        SSpatternsPad14Lo (0x00); // 14
        SSpatternsPad15Lo (0x02); // 15
        SSpatternsPad16Lo (0x00); // 16 
        break;                                              
    case 39: //----------------  7  ----------Kick drum 1,5,9,13 and snare 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 -----------------------//
        SSpatternsPad1Lo  (0x03); // 1
        SSpatternsPad2Lo  (0x02); // 2
        SSpatternsPad3Lo  (0x02); // 3
        SSpatternsPad4Lo  (0x02); // 4
        SSpatternsPad5Lo  (0x03); // 5
        SSpatternsPad6Lo  (0x02); // 6
        SSpatternsPad7Lo  (0x02); // 7
        SSpatternsPad8Lo  (0x02); // 8
        SSpatternsPad9Lo  (0x03); // 9
        SSpatternsPad10Lo (0x02); // 10
        SSpatternsPad11Lo (0x02); // 11
        SSpatternsPad12Lo (0x02); // 12
        SSpatternsPad13Lo (0x03); // 13
        SSpatternsPad14Lo (0x02); // 14
        SSpatternsPad15Lo (0x02); // 15
        SSpatternsPad16Lo (0x02); // 16 
        break; 
    case 40: //----------------  8  ----------Kick drum 1,5,9,13 and snare 3,7,11,15-----------------------//
        SSpatternsPad1Lo  (0x01); // 1
        SSpatternsPad2Lo  (0x00); // 2
        SSpatternsPad3Lo  (0x02); // 3
        SSpatternsPad4Lo  (0x00); // 4
        SSpatternsPad5Lo  (0x01); // 5
        SSpatternsPad6Lo  (0x00); // 6
        SSpatternsPad7Lo  (0x02); // 7
        SSpatternsPad8Lo  (0x00); // 8
        SSpatternsPad9Lo  (0x01); // 9
        SSpatternsPad10Lo (0x00); // 10
        SSpatternsPad11Lo (0x02); // 11
        SSpatternsPad12Lo (0x00); // 12
        SSpatternsPad13Lo (0x01); // 13
        SSpatternsPad14Lo (0x00); // 14
        SSpatternsPad15Lo (0x02); // 15
        SSpatternsPad16Lo (0x00); // 16 
        break;   
    case 41: //----------------  9  ----------Kick drum 1,5,9,13 y Bordona 5,13 -----------------------//

        sendDef702(0x05, 0x0e, 0x31); // Instrumento 7 Bordona Nota C#3
        SSpatternsPad1Lo  (0x01); // 1
        SSpatternsPad2Lo  (0x00); // 2
        SSpatternsPad3Lo  (0x00); // 3
        SSpatternsPad4Lo  (0x00); // 4
        SSpatternsPad5Lo  (0x41); // 5
        SSpatternsPad6Lo  (0x00); // 6
        SSpatternsPad7Lo  (0x00); // 7
        SSpatternsPad8Lo  (0x00); // 8
        SSpatternsPad9Lo  (0x01); // 9
        SSpatternsPad10Lo (0x00); // 10
        SSpatternsPad11Lo (0x00); // 11
        SSpatternsPad12Lo (0x00); // 12
        SSpatternsPad13Lo (0x41); // 13
        SSpatternsPad14Lo (0x00); // 14
        SSpatternsPad15Lo (0x00); // 15
        SSpatternsPad16Lo (0x00); // 16
        break; 
    case 47: //----------------  /  ----------HiHat 1,3,5,7,9,11,13,15-----------------------//
        sendDef702(0x05, 0x18, 0x36); // F#3
        SSpatternsPad1Hi  (0x10); // 1
        SSpatternsPad2Hi  (0x00); // 2
        SSpatternsPad3Hi  (0x10); // 3
        SSpatternsPad4Hi  (0x00); // 4
        SSpatternsPad5Hi  (0x10); // 5
        SSpatternsPad6Hi  (0x00); // 6
        SSpatternsPad7Hi  (0x10); // 7
        SSpatternsPad8Hi  (0x00); // 8
        SSpatternsPad9Hi  (0x10); // 9
        SSpatternsPad10Hi (0x00); // 10
        SSpatternsPad11Hi (0x10); // 11
        SSpatternsPad12Hi (0x00); // 12
        SSpatternsPad13Hi (0x10); // 13
        SSpatternsPad14Hi (0x00); // 14
        SSpatternsPad15Hi (0x10); // 15
        SSpatternsPad16Hi (0x00); // 16 
        break;                                                    
    case 46: //----------------  *  ----------HiHat 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 -----------------------//
        sendDef702(0x05, 0x18, 0x36); // F#3
        SSpatternsPad1Hi  (0x10); // 1
        SSpatternsPad2Hi  (0x10); // 2
        SSpatternsPad3Hi  (0x10); // 3
        SSpatternsPad4Hi  (0x10); // 4
        SSpatternsPad5Hi  (0x10); // 5
        SSpatternsPad6Hi  (0x10); // 6
        SSpatternsPad7Hi  (0x10); // 7
        SSpatternsPad8Hi  (0x10); // 8
        SSpatternsPad9Hi  (0x10); // 9
        SSpatternsPad10Hi (0x10); // 10
        SSpatternsPad11Hi (0x10); // 11
        SSpatternsPad12Hi (0x10); // 12
        SSpatternsPad13Hi (0x10); // 13
        SSpatternsPad14Hi (0x10); // 14
        SSpatternsPad15Hi (0x10); // 15
        SSpatternsPad16Hi (0x10); // 16 
        break;   
    case 45: //---------------- ( - ) ----------Ride 1,3,5,7,9,11,13,15 -----------------------//
        sendDef702(0x05, 0x18, 0x41); // Instrumento 12 nota F4        
        SSpatternsPad1Hi  (0x10); // 1
        SSpatternsPad2Hi  (0x00); // 2
        SSpatternsPad3Hi  (0x10); // 3
        SSpatternsPad4Hi  (0x00); // 4
        SSpatternsPad5Hi  (0x10); // 5
        SSpatternsPad6Hi  (0x00); // 6
        SSpatternsPad7Hi  (0x10); // 7
        SSpatternsPad8Hi  (0x00); // 8
        SSpatternsPad9Hi  (0x10); // 9
        SSpatternsPad10Hi (0x00); // 10
        SSpatternsPad11Hi (0x10); // 11
        SSpatternsPad12Hi (0x00); // 12
        SSpatternsPad13Hi (0x10); // 13
        SSpatternsPad14Hi (0x00); // 14
        SSpatternsPad15Hi (0x10); // 15
        SSpatternsPad16Hi (0x00); // 16 
        break;                                                                               
    case 44: //---------------- (+) ----------HiHatClosed 1,3,5,7,9,11,13,15 y Open 3,7,11,15 -----------------------//
        sendDef702(0x05, 0x18, 0x36); // Instrumento 12 nota F#3
        sendDef702(0x05, 0x14, 0x3a); // Instrumento 10 nota A#3    
        SSpatternsPad1Hi  (0x10); // 1
        SSpatternsPad2Hi  (0x00); // 2
        SSpatternsPad3Hi  (0x04); // 3
        SSpatternsPad4Hi  (0x00); // 4
        SSpatternsPad5Hi  (0x10); // 5
        SSpatternsPad6Hi  (0x00); // 6
        SSpatternsPad7Hi  (0x04); // 7
        SSpatternsPad8Hi  (0x00); // 8
        SSpatternsPad9Hi  (0x10); // 9
        SSpatternsPad10Hi (0x00); // 10
        SSpatternsPad11Hi (0x04); // 11
        SSpatternsPad12Hi (0x00); // 12
        SSpatternsPad13Hi (0x10); // 13
        SSpatternsPad14Hi (0x00); // 14
        SSpatternsPad15Hi (0x04); // 15
        SSpatternsPad16Hi (0x00); // 16 
        break;                                                 

    case 43: //----------------  Intro  ----------HiHat 1,3,5,7,9,11,13,15-----------------------//
        sendDef702(0x05, 0x18, 0x36); // Instrumento 12 nota F#3
        SSpatternsPad1Hi  (0x00); // 1
        SSpatternsPad2Hi  (0x00); // 2
        SSpatternsPad3Hi  (0x10); // 3
        SSpatternsPad4Hi  (0x00); // 4
        SSpatternsPad5Hi  (0x00); // 5
        SSpatternsPad6Hi  (0x00); // 6
        SSpatternsPad7Hi  (0x10); // 7
        SSpatternsPad8Hi  (0x00); // 8
        SSpatternsPad9Hi  (0x00); // 9
        SSpatternsPad10Hi (0x00); // 10
        SSpatternsPad11Hi (0x10); // 11
        SSpatternsPad12Hi (0x00); // 12
        SSpatternsPad13Hi (0x00); // 13
        SSpatternsPad14Hi (0x00); // 14
        SSpatternsPad15Hi (0x10); // 15
        SSpatternsPad16Hi (0x00); // 16 
        break;                                                         

 // Master MFX1 ---------------------------- Alt  Key -------------------------------------------------------//

    case 2080: // 0 ------  MFX1 Off -------------------------------------//
        sendDef702(0x10, 0x02, 0x01); // MFX1 on        
        sendDef702(0x10, 0x00, 0x00); // MFX1 000 No Effect                
        break;


    case 2081: // 1 -------------------------------------------------------//
        sendDef702(0x10, 0x02, 0x01); // MFX1 On
        sendDef702(0x10, 0x00, 0x40); // s64 Stereo Chorus
        PO8.newValue(28, 71, 1); // Dry/Wet
        PO8.Potec1[7]={0x02}; PO8.Potec1[9]={0x11}; PO8.Potec1[11]={0x12}; 
        break;

    case 2082: // 2 -------------------------------------------------------//
        sendDef702(0x10, 0x02, 0x01); // MFX1 On
        sendDef702(0x10, 0x00, 0x50); // s80 Pitch Shifter
        PO8.newValue(17, 71, 1); // Dry/Wet
        PO8.Potec[7]={0x02}; PO8.Potec[9]={0x11}; PO8.Potec[11]={0x0c};  
        break;

    case 2083: // 3 ----------------- -------------------------------------//
        sendDef702(0x10, 0x02, 0x01); // MFX1 On
        sendDef702(0x10, 0x00, 0x4f); // s79 Detune
        PO8.newValue(17, 71, 1); // Dry/Wet
        PO8.Potec[7]={0x02}; PO8.Potec[9]={0x11}; PO8.Potec[11]={0x08};  
        break;

    case 2084: // 4 ---------------------------------------------------------//
        sendDef702(0x10, 0x02, 0x01); // MFX1 On
        sendDef702(0x10, 0x00, 0x46); // s 70 Ensemble
        PO8.newValue(17, 71, 1); // Dry/Wet
        PO8.Potec[7]={0x02}; PO8.Potec[9]={0x11}; PO8.Potec[11]={0x07};  
        break;

    case 2085: // 5 ---------------------------------------------------------//
        sendDef702(0x10, 0x02, 0x01); // MFX1 On
        sendDef702(0x10, 0x00, 0x45); // s69 Multitap Chorus/Delay
        PO8.newValue(17, 71, 1); // Dry/Wet
        PO8.Potec[7]={0x02}; PO8.Potec[9]={0x11}; PO8.Potec[11]={0x13};  
        break;    

    case 2086: // 6 ---------------------------------------------------------//
        sendDef702(0x10, 0x02, 0x01); // MFX1 On
        sendDef702(0x10, 0x00, 0x3f); // s63 Rotary Speaker
        PO8.newValue(17, 71, 1); // Dry/Wet
        PO8.Potec[7]={0x02}; PO8.Potec[9]={0x11}; PO8.Potec[11]={0x0e};  
        break;                              

    case 2087: // 7 ----------------------------------------------------------//
        sendDef702(0x10, 0x02, 0x01); // MFX1 On
        sendDef702(0x10, 0x00, 0x48); // s72 Stereo Flanger
        PO8.newValue(28, 71, 1); // Dry/Wet
        PO8.Potec1[7]={0x02}; PO8.Potec1[9]={0x11}; PO8.Potec1[11]={0x0f};  
        break;

    case 2088: // 8 ----------------------------------------------------------//
        sendDef702(0x10, 0x02, 0x01); // MFX1 On
        sendDef702(0x10, 0x00, 0x0e); // s14 Stereo Random Filter
        PO8.newValue(28, 71, 1); // Dry/Wet
        PO8.Potec1[7]={0x02}; PO8.Potec1[9]={0x11}; PO8.Potec1[11]={0x13}; 
        break;        

    case 2089: // 9 ------DISTORTED  MFX1 -----------------------------------//
      sendDef702(0x09, 0x04, 0x01); // ssLR
      sendDef702(0x08, 0x04, 0x01); // DrumBusLR
      sendDef702(0x10, 0x02, 0x01); // MFX1 On
      sendDef702(0x10, 0x00, 0x38); // S56 OD/HG-Amp Sim
      sendDef702(0x10, 0x04, 0x7f); // MFX1 return 127
      break;  

 // Master MFX2 ---------------------------- AltGr Key -------------------------------------------------------//

    case 1057: // 1 -------------------------------------------- Reverb Hall -------------------------------------//
        sendDef702(0x10, 0x01, 0x69); // Reverb Hall
        PO8.newValue(17, 71, 1); // Dry/Wet
        PO8.Potec[7]={0x02}; PO8.Potec[9]={0x12}; PO8.Potec[11]={0x07}; 
        break;

    case 1058: // 2 -------------------------------------------- Reverb Plate -------------------------------------//
        sendDef702(0x10, 0x01, 0x6a); // Reverb Plate
        PO8.newValue(17, 71, 1); // Dry/Wet
        PO8.Potec[7]={0x02}; PO8.Potec[9]={0x12}; PO8.Potec[11]={0x07}; 
        break;

    case 1059: // 3 -------------------------------------------- Reverb Room -------------------------------------//
        sendDef702(0x10, 0x01, 0x6b); // Reverb Room
        PO8.newValue(17, 71, 1); // Dry/Wet
        PO8.Potec[7]={0x02}; PO8.Potec[9]={0x12}; PO8.Potec[11]={0x09}; 
        break;

    case 1060: // 4 -------------------------------------------- Early Reflections -------------------------------------//
        sendDef702(0x10, 0x01, 0x71); // Early Reflections
        PO8.newValue(17, 71, 1); // Dry/Wet
        PO8.Potec[7]={0x02}; PO8.Potec[9]={0x12}; PO8.Potec[11]={0x06}; 
        break;

    case 1061: // 5 -------------------------------------------- Reverb Gate -------------------------------------//
        sendDef702(0x10, 0x01, 0x78); // Reverb Gate
        PO8.newValue(17, 71, 1); // Dry/Wet
        PO8.Potec[7]={0x02}; PO8.Potec[9]={0x12}; PO8.Potec[11]={0x0b}; 
        break;    

    case 1062: // 6 -------------------------------------------- L/C/R BPM Delay -------------------------------------//
        sendDef702(0x10, 0x01, 0x63); // L/C/R BPM Delay
        PO8.newValue(17, 71, 1); // Dry/Wet
        PO8.Potec[7]={0x02}; PO8.Potec[9]={0x12}; PO8.Potec[11]={0x14}; 
        break;                              

    case 1063: // 7 ---------------------------------------------------------------------------------//
        sendDef702(0x10, 0x01, 0x59); // L/C/R Delay
        PO8.newValue(17, 71, 1); // Dry/Wet
        PO8.Potec[7]={0x02}; PO8.Potec[9]={0x12}; PO8.Potec[11]={0x0e};         
        break;

    case 1064: // 8 ------------------------------------------------------------------------------------//
        sendDef702(0x10, 0x01, 0x68); // Tape Echo BPM
        PO8.newValue(17, 71, 1); // Dry/Wet
        PO8.Potec[7]={0x02}; PO8.Potec[9]={0x12}; PO8.Potec[11]={0x1b};         
        break;        

    case 1065: // 9 ----------------------------------------------------------------------------------//
        sendDef702(0x10, 0x01, 0x5f); // Tape Echo
        PO8.newValue(17, 71, 1); // Dry/Wet
        PO8.Potec[7]={0x02}; PO8.Potec[9]={0x12}; PO8.Potec[11]={0x18}; 
      break;        

// Inserts FXs  ----------------------- Alt ---------------------------------------------------------//

    case 2329://insert
      sendIFX(0x08, 0x01); // IFX 4 on
      sendIFX(0x03, 0x0d); // IFX 4 s13 Vox Wah
        PO5.newValue(17, 13, 1);  // Auto Sens  
        PO5.Potec[7]={0x02}; PO5.Potec[9]={0x0e}; PO5.Potec[11]={0x06};       
        PO6.newValue(6, 71, 1);  // Open 
        PO6.Potec[7]={0x02}; PO6.Potec[9]={0x0e}; PO6.Potec[11]={0x01}; 
        PO7.newValue(6, 74, 1);  // Close
        PO7.Potec[7]={0x02}; PO7.Potec[9]={0x0e}; PO7.Potec[11]={0x02}; 
        PO8.newValue(17, 71, 1); // Dry/Wet
        PO8.Potec[7]={0x02}; PO8.Potec[9]={0x0e}; PO8.Potec[11]={0x09};            
        break;
    case 2321://Inicio
      sendIFX(0x08, 0x01); // IFX 4 on
      sendIFX(0x03, 0x20); // IFX 4 s32 Black Phase
        PO5.newValue(22, 13, 1);  // Speed  
        PO5.Potec2[7]={0x02}; PO5.Potec2[9]={0x0e}; PO5.Potec2[11]={0x00};       
        PO6.newValue(17, 71, 1);  // Depth
        PO6.Potec[7]={0x02}; PO6.Potec[9]={0x0e}; PO6.Potec[11]={0x01}; 
        PO7.newValue(12, 74, 1);  // Resonance
        PO7.Potec[7]={0x02}; PO7.Potec[9]={0x0e}; PO7.Potec[11]={0x02}; 
        PO8.newValue(17, 71, 1);  // Dry/Wet
        PO8.Potec[7]={0x02}; PO8.Potec[9]={0x0e}; PO8.Potec[11]={0x04};  
        break;  
    case 2323://RePag
      sendIFX(0x08, 0x01); // IFX 4 on
      sendIFX(0x03, 0x0e); // IFX 4 s14 Random Filter
      sendDef702(0x0e, 0x07, 0x01); // MIDI Sync On
        PO5.newValue(17, 13, 1);  // Manual  
        PO5.Potec[7]={0x02}; PO5.Potec[9]={0x0e}; PO5.Potec[11]={0x0e};       
        PO6.newValue(17, 71, 1);  // Depth
        PO6.Potec[7]={0x02}; PO6.Potec[9]={0x0e}; PO6.Potec[11]={0x0f}; 
        PO7.newValue(17, 74, 1);  // Resonance
        PO7.Potec[7]={0x02}; PO7.Potec[9]={0x0e}; PO7.Potec[11]={0x12}; 
        PO8.newValue(5, 71, 1); //  Dry/Wet
        PO8.Potec1[7]={0x02}; PO8.Potec1[9]={0x0e}; PO8.Potec1[11]={0x13};               
        break;  
    case 2330://Supr
      sendIFX(0x08, 0x01); // IFX 4 on
      sendIFX(0x03, 0x2a); // IFX 4 s42 Ring Modulator
      sendDef702(0x0e, 0x03, 0x0d); // Fixed FX1#CC12
        PO5.newValue(23, 13, 1);  // OSC mode  
        PO5.Potec[7]={0x02}; PO5.Potec[9]={0x0e}; PO5.Potec[11]={0x01};       
        PO6.newValue(17, 71, 1);  // Pre LPF
        PO6.Potec[7]={0x02}; PO6.Potec[9]={0x0e}; PO6.Potec[11]={0x00}; 
        PO7.newValue(18, 74, 1);  // Manual
        PO7.Potec[7]={0x02}; PO7.Potec[9]={0x0e}; PO7.Potec[11]={0x05}; 
        PO8.newValue(17, 71, 1);  // Dry/Wet
        PO8.Potec[7]={0x02}; PO8.Potec[9]={0x0e}; PO8.Potec[11]={0x13};  
        break;   
     case 2322://Fin
      sendSysExDef5(0x02, 0x0a, 0x00, 0x01, 0x02); // IFX1 Vocoder
    // IFX1
      sendDef702(0x0b, 0x0d, 0x01); // FX control 1
      sendDef702(0x0b, 0x05, 0x0d); // Carrier Noise FX#12
      sendDef702(0x0b, 0x06, 0x07); // Amt= 07
    // audio input
      sendDef702(0x14, 0x04, 0x01); // FX Control Bus FC1
      sendDef702(0x14, 0x03, 0x0c); // Bus Off
      sendDef702(0x14, 0x02, 0x00); // Send2= 000
       PO4.newValue(0, 12, 1);  // CC12
       PO5.newValue(9, 13, 1);  // Formant Shift  
       PO5.Potec1[7]={0x02}; PO5.Potec1[9]={0x0b}; PO5.Potec1[11]={0x02}; 
       PO6.newValue(17, 71, 1);  // Carrier Trim
       PO6.Potec[7]={0x02}; PO6.Potec[9]={0x0b}; PO6.Potec[11]={0x00}; 
       PO7.newValue(17, 74, 1);  // Response
       PO7.Potec[7]={0x02}; PO7.Potec[9]={0x0b}; PO7.Potec[11]={0x03}; 
       PO8.newValue(17, 71, 1); // Dry/Wet
       PO8.Potec[7]={0x02}; PO8.Potec[9]={0x0b}; PO8.Potec[11]={0x0a};    
        break;      
    case 2324://AvPag 
      sendIFX(0x08, 0x01); // IFX 4 on
      sendIFX(0x03, 0x38); // IFX 4 s56 OD/HG AmpSim
      sendDef702(0x0e, 0x0d, 0x64); // WET
      sendDef702(0x0e, 0x04, 0x00); // Low 0
      sendDef702(0x0e, 0x01, 0x03); // Drive 3
        PO5.newValue(6, 13, 1);  //  Drive
        PO5.Potec[7]={0x02}; PO5.Potec[9]={0x0e}; PO5.Potec[11]={0x01};       
        PO6.newValue(46, 71, 1);  // Low EQ dB  
        PO6.Potec1[7]={0x02}; PO6.Potec1[9]={0x0e}; PO6.Potec1[11]={0x04}; 
        PO7.newValue(46, 74, 1);  // Mid EQ dB
        PO7.Potec1[7]={0x02}; PO7.Potec1[9]={0x0e}; PO7.Potec1[11]={0x07}; 
        PO8.newValue(46, 71, 1); //  Mid Hi EQ dB
        PO8.Potec1[7]={0x02}; PO8.Potec1[9]={0x0e}; PO8.Potec1[11]={0x0a};    
        break;         
/*               
    case 2322://Fin
      sendIFX(0x08, 0x01); // IFX 4 on
      sendIFX(0x03, 0x29); // IFX 4 s41 St Phaser + Tremolo
      sendDef702(0x0e, 0x03, 0x0d); // LFO FX1#CC12
      sendDef702(0x0e, 0x04, 0x73); // amt
        PO5.newValue(23, 13, 1);  // OSC mode  
        PO5.Potec[7]={0x02}; PO5.Potec[9]={0x0e}; PO5.Potec[11]={0x01};       
        PO6.newValue(17, 71, 1);  // Pre LPF
        PO6.Potec[7]={0x02}; PO6.Potec[9]={0x0e}; PO6.Potec[11]={0x00}; 
        PO7.newValue(18, 74, 1);  // Manual
        PO7.Potec[7]={0x02}; PO7.Potec[9]={0x0e}; PO7.Potec[11]={0x05}; 
        PO8.newValue(17, 71, 1);  // Dry/Wet
        PO8.Potec[7]={0x02}; PO8.Potec[9]={0x0e}; PO8.Potec[11]={0x13};       
        break;             
/*
    case 1063://7
      sendIFX4(0x08, 0x01); // IFX 4 on
      sendIFX4(0x03, 0x06); // IFX 4 s06 Stereo Gate
        PO5.newValue(23, 13, 1);  // OSC mode  
        PO5.Potec[7]={0x02}; PO5.Potec[9]={0x0e}; PO5.Potec[11]={0x01};       
        PO6.newValue(17, 71, 1);  // Pre LPF
        PO6.Potec[7]={0x02}; PO6.Potec[9]={0x0e}; PO6.Potec[11]={0x00}; 
        PO7.newValue(18, 74, 1);  // Manual
        PO7.Potec[7]={0x02}; PO7.Potec[9]={0x0e}; PO7.Potec[11]={0x05}; 
        PO8.newValue(17, 71, 1);  // Dry/Wet
        PO8.Potec[7]={0x02}; PO8.Potec[9]={0x0e}; PO8.Potec[11]={0x13};         
        break;
    case 1064://8
      sendIFX4(0x08, 0x01); // IFX 4 on
      sendIFX4(0x03, 0x1c); // IFX 4 s28 Mic Model + Preamp
        PO5.newValue(19, 13, 1);  // Mic Type  
        PO5.Potec[7]={0x02}; PO5.Potec[9]={0x0e}; PO5.Potec[11]={0x03};       
        PO6.newValue(20, 71, 1);  // L Cut 
        PO6.Potec[7]={0x02}; PO6.Potec[9]={0x0e}; PO6.Potec[11]={0x05}; 
        PO7.newValue(17, 74, 1);  // Saturate
        PO7.Potec[7]={0x02}; PO7.Potec[9]={0x0e}; PO7.Potec[11]={0x08}; 
        PO8.newValue(21, 71, 1);  // Position
        PO8.Potec[7]={0x02}; PO8.Potec[9]={0x0e}; PO8.Potec[11]={0x04};      
        break;  
             
    case 2324://9
      sendIFX4(0x08, 0x01); // IFX 4 on
      sendIFX4(0x03, 0x2a); // IFX 4 s42 Ring Modulator
      sendDef702(0x0e, 0x03, 0x0d); // Fixed FX1#CC12
        PO5.newValue(23, 13, 1);  // OSC mode  
        PO5.Potec[7]={0x02}; PO5.Potec[9]={0x0e}; PO5.Potec[11]={0x01};       
        PO6.newValue(17, 71, 1);  // Pre LPF
        PO6.Potec[7]={0x02}; PO6.Potec[9]={0x0e}; PO6.Potec[11]={0x00}; 
        PO7.newValue(18, 74, 1);  // Manual
        PO7.Potec[7]={0x02}; PO7.Potec[9]={0x0e}; PO7.Potec[11]={0x05}; 
        PO8.newValue(17, 71, 1);  // Dry/Wet
        PO8.Potec[7]={0x02}; PO8.Potec[9]={0x0e}; PO8.Potec[11]={0x13};  
        break;   */

// Mic Audio Fxs
    case 8475: // ESC ------------------------------------- Control Key ----------------------------------------------------------//
        sendDef702(0x01, 0x14, 0x03); // G-Set Off
        sendDef702(0x01, 0x10, 0x00); // Real Time Selector en Audio in 1
        sendDef702(0x14, 0x00, 0x7f); // Level 127
        sendDef702(0x14, 0x03, 0x04); // Bus IFX4
        sendDef702(0x0a, 0x17, 0x00); // IFX4 Send 1 = 000
        sendDef702(0x0a, 0x1c, 0x00); // IFX4 Send 2 = 000
        PO1.newValue(1, 74, 1);  // audio  Level    
        PO1.Potec[7]={0x02}; PO1.Potec[9]={0x14}; PO1.Potec[11]={0x00}; 
        PO2.newValue(1, 71, 1);  // IFX4 Send 1
        PO2.Potec[7]={0x02}; PO2.Potec[9]={0x0a}; PO2.Potec[11]={0x17}; 
        PO3.newValue(1, 74, 1);  // IFX4 Send 2     
        PO3.Potec[7]={0x02}; PO3.Potec[9]={0x0a}; PO3.Potec[11]={0x1c}; 
        PO4.newValue(0, 12, 1);  // CC12
//        PO4.Potec[7]={0x03}; PO4.Potec[9]={0x0d}; PO4.Potec[11]={0x03}; 
        PO5.newValue(0, 13, 1);  // CC13   
//        PO5.Potec[7]={0x03}; PO5.Potec[9]={0x0d}; PO5.Potec[11]={0x04}; 
        PO6.newValue(17, 71, 1);  // Carrier Trim
        PO6.Potec[7]={0x02}; PO6.Potec[9]={0x0b}; PO6.Potec[11]={0x00}; 
        PO7.newValue(17, 74, 1);  // Response
        PO7.Potec[7]={0x02}; PO7.Potec[9]={0x0b}; PO7.Potec[11]={0x03}; 
        PO8.newValue(16, 71, 1); // Bus Select
        PO8.Potec[7]={0x02}; PO8.Potec[9]={0x14}; PO8.Potec[11]={0x03};

        break;  


//oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo//
//ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo//
//ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo MODO Edición oooooo Bloqueo Mayuscula oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo//

//BANK CHANGE------------------------------------------Control key --------------------------------------------------------------------------//
    case 12566:
 
        break;

    case 12565:

        break;       

// Screen -----------------------------------------------------------------------------------------------//
   case 4366:
        defArray9bytes( 0x4e, 0x03);  // Ultima Pantalla ------ Hoja      
        break;
   case 4878:
        defArray9bytes( 0x4e , 0x02);  // Retorno ----- Win + Hoja
        break;


//  ----------------------------------------------------------------------------------//
   case 4377: // ins
     MIDI.sendControlChange(65, 127, 1); // Portamento On  
     break; 
   case 4369: // inicio
     sendDef703(0x00, 0x02, 0x00); // Poly    
     break; 
   case 4371: // RePag
     sendDef702(0x01, 0x0e, 0x00); // Single 
     sendDef702(0x01, 0x0f, 0x00);
     sendDef702(0x01, 0x12, 0x00);
     sendDef702(0x01, 0x13, 0x00);
     sendDef702(0x08, 0x0d, 0x00);
     sendDef702(0x09, 0x0d, 0x00);
     sendDef703(0x00, 0x00, 0x00);   
     break;
   case 4378: // Supr
     MIDI.sendControlChange(65, 1, 1); // Portamento Off      
     break;
   case 4370: // Fin
     sendDef703(0x00, 0x02, 0x01); // Mono     
     break;
   case 4372: // AvPag
     sendDef702(0x01, 0x0e, 0x00); // Double 
     sendDef702(0x01, 0x0f, 0x00);
     sendDef702(0x01, 0x12, 0x00);
     sendDef702(0x01, 0x13, 0x00);
     sendDef702(0x08, 0x0d, 0x00);
     sendDef702(0x09, 0x0d, 0x00);
     sendDef703(0x00, 0x00, 0x01);
     break;

//  OSCs shift --------------------------- Control -------------------------------------//
   case 12569: // ins
     sendDef703(0x06, 0x02, 0x00); // OSC1 tune 0000
     sendDef703(0x06, 0x01, 0x00); // OSC1 Transpose +00
     break; 
   case 12561: // inicio
     sendSysExNeg4(0x06, 0x02, 0x7b, 0x28); // OSC1 tune -0600  
     sendDef703(0x06, 0x01, 0x06); // OSC1 transpose +06   
     break; 
   case 12563: // RePag
     sendSysExNeg4(0x06, 0x02, 0x76, 0x50); // OSC1 tune -1200 
     sendDef703(0x06, 0x01, 0x0c); // OSC1 transpose +12
     break;
   case 12570: // Supr
     sendDef703(0x16, 0x02, 0x00); // OSC2 tune 0000
     sendDef703(0x16, 0x01, 0x00); // OSC2 Transpose +00
     break;
   case 12562: // Fin
     sendSysExNeg4(0x16, 0x02, 0x7b, 0x28); // OSC2 tune -0600  
     sendDef703(0x16, 0x01, 0x06); // OSC2 transpose +06   
     break;
   case 12564: // AvPag
     sendSysExNeg4(0x16, 0x02, 0x76, 0x50); // OSC2 tune -1200 
     sendDef703(0x16, 0x01, 0x0c); // OSC2 transpose +12 
     break;




// Defaults ------------------------------------ DEF ----------------------------------------------------------------------------------------------------------------------------------------//
// Def OSC1
  case 4145: // 1
//        OSC1def(0x01, 0x00); // OSC1 RomM
//        OSC1def(0x04, 0x00); // Offset Off
        OSC1def(0x22, 0x00); // Mode= Key
        OSC1def(0x05, 0x00); // Off MS2
        OSC1def(0x06, 0x00); // Off MS3
        OSC1def(0x0c, 0x00); // Off MS4
        OSC1def(0x12, 0x00);
        OSC1def(0x24, 0x7f); // Vel Top 127
        OSC1def(0x23, 0x01); // Vel Bottom 001
        OSC1def(0x18, 0x01); // Bottom Vel 001
    break;
//Def OSC2
  case 4146: // 2
//        OSC2def(0x01, 0x00); // OSC2 RomM
//        OSC2def(0x04, 0x00);
        OSC2def(0x22, 0x00); // Mode= Key
        OSC2def(0x05, 0x00);
        OSC2def(0x06, 0x00);
        OSC2def(0x0c, 0x00);
        OSC2def(0x12, 0x00);
        OSC2def(0x24, 0x7f); // Vel Top 127
        OSC2def(0x23, 0x01); // Vel Bottom 001
        OSC2def(0x18, 0x01); // Bottom Vel 001
    break;

// Layer Def OSC1
  case 4147: // 3
        OSC1def(0x22, 0x00); // Mode= Key
        OSC1def(0x07, 0x00); // OSC1 RomM
//        OSC1def(0x04, 0x00); // Offset Off
        OSC1def(0x1a, 0x02); // Mode= Layer
        OSC1def(0x06, 0x01); // On MS2
        OSC1def(0x0c, 0x00); // Off MS3
        OSC1def(0x12, 0x00); // Off MS4
        OSC1def(0x09, 0x20); // Level 32
        OSC1def(0x12, 0x00);
        OSC1def(0x19, 0x7f); // Xfade
        OSC1def(0x24, 0x7f); // Vel Top 127
        OSC1def(0x23, 0x01); // Vel Bottom 001
        OSC1def(0x18, 0x01); // Bottom Vel 001
    break;
//Layer Def OSC2
  case 4148: // 4
        OSC2def(0x22, 0x00); // Mode= Key
        OSC2def(0x07, 0x00); // OSC1 RomM
//        OSC2def(0x04, 0x00); // Offset Off
        OSC2def(0x1a, 0x02); // Mode= Layer
        OSC2def(0x06, 0x01); // On MS2
        OSC2def(0x0c, 0x00); // Off MS3
        OSC2def(0x12, 0x00); // Off MS4
        OSC2def(0x09, 0x20); // Level 32
        OSC2def(0x12, 0x00);
        OSC2def(0x19, 0x7f); // Xfade
        OSC2def(0x24, 0x7f); // Vel Top 127
        OSC2def(0x23, 0x01); // Vel Bottom 001
        OSC2def(0x18, 0x01); // Bottom Vel 001
    break;




// OSC 1 ---------   del Q al +  --------------------------------------------------------------------------//
  case 4177: // Q
    OSC1def(0x01, 0x00); // OSC1 RomM
    OSC1def(0x04, 0x00); 
    OSC1def(0x05, 0x00); 
    OSC1romM311  // Saw
    break;
  case 4183: // W
    OSC1romM312  // Saw Boost
    break;
  case 4165: // E
    OSC1romM339  // Saw/Square
    break;
  case 4178: // R
    OSC1romM324  // Square
    break;
  case 4180: // T
    OSC1romM328  // Square Dual
    break;
  case 4185: // Y
    OSC1romM326  // Square mMG
    break;
  case 4181: // U
    OSC1romM335  // Sine
    break;
  case 4169: // I
    OSC1romM337  // Sine JP
    break;
  case 4175: // O
    OSC1romM331  // Triangle MG3
    break;
  case 4176: // P
    OSC1romM321  // Pulse 40
    break;
  case 4189: // [
    OSC1romM334  // Ramp mMG
    break;
  case 4190: // ]
    OSC1romM310  // Noise
    break;

// OSC 2 ---------   del A al { --------------------------------------------------------------------------//
  case 4161: // A
    OSC2def(0x01, 0x00); // OSC2 RomM
    OSC2def(0x04, 0x00); 
    OSC2def(0x05, 0x00); 
    OSC2romM311  // Saw
    break;
  case 4179: // S
    OSC2romM312  // Saw Boost
    break;
  case 4164: // D
    OSC2romM339  // Saw/Square
    break;
  case 4166: // F
    OSC2romM324  // Square
    break;
  case 4167: // G
    OSC2romM328  // Square Dual
    break;
  case 4168: // H
    OSC2romM326  // Square mMG
    break;
  case 4170: // J
    OSC2romM335  // Sine
    break;
  case 4171: // K
    OSC2romM337  // Sine JP
    break;
  case 4172: // L
    OSC2romM331  // Triangle MG3
    break;
  case 4187: // Ñ
    OSC2romM321  // Pulse 40
    break;
  case 4154: // {
    OSC2romM334  // Ramp mMG
    break;

//  Saw Based Waves OSC1 ------------------------------- Control Key --------------------//


   case 12369: // Q
      OSC1romM278 // Detuned Super 2
        break;                   
   case 12375: // W
      OSC1romM279 // Detuned Lead
        break;                   
   case 12357: // E
      OSC1romM281 // Brite Pad
        break;                   
   case 12370: // R
      OSC1romM282 // Detuned A2600
        break;                   
   case 12372: // T
      OSC1romM284 // Detuned PWM
        break;                   
   case 12377: // Y
      OSC1romM289 // WhitePad
        break;      
   case 12373: // U
      OSC1romM314 // Saw Chroma
        break;                   
   case 12361: // I
      OSC1romM285 // Razor Olive pad
        break;                   
   case 12367: // O
      OSC1romM283 // Brite Hoover
        break;                   
   case 12368: // P
      OSC1romM280 // UnisonSaw MS2k
        break;                   
   case 12381: // [
      OSC1romM315 // Saw 5th
        break;                   
   case 12382: // ]
      OSC1romM287 // AnalogStrings2
        break;                   

//  Saw Based Waves OSC2 ----------------------------------------------------//


   case 12353: // A
      OSC2romM278 // Detuned Super 2
        break;                   
   case 12371: // S
      OSC2romM279 // Detuned Lead
        break;                   
   case 12356: // D
      OSC2romM281 // Brite Pad
        break;                   
   case 12358: // F
      OSC2romM282 // Detuned A2600
        break;                   
   case 12359: // G
      OSC2romM284 // Detuned PWM
        break;                   
   case 12360: // H
      OSC2romM289 // WhitePad
        break;                 
   case 12362: // J
      OSC2romM314 // Saw Chroma
        break;                   
   case 12363: // K
      OSC2romM285 // Razor Olive pad
        break;                   
   case 12364: // L
      OSC2romM283 // Brite Hoover
        break;                   
   case 12379: // Ñ
      OSC2romM280 // UnisonSaw MS2k
        break;      
   case 12346: // {
      OSC2romM315 // Saw 5th
        break;  

// Layer Waves MS2 OSC 1 y 2 ------------- Control -----------------------------//
   case 12337: //  1
      OSC1Layer(0x02, 0x4F); // Sine
        break;                   
   case 12338: // 2
      OSC1Layer(0x02, 0x37); // Saw
        break;                   
   case 12339: // 3
      OSC1Layer(0x02, 0x55); // SineWheel 16
        break;                   
   case 12340: // 4
      OSC1Layer(0x02, 0x31); // Cricket Spectrum
        break;      
   case 12341: // 5
      OSC1Layer(0x02, 0x33); // Noise Spectrum
        break;                   
   case 12342: // 6
      OSC1Layer(0x02, 0x44); // Square
        break;                   
   case 12343: // 7
      OSC1Layer(0x02, 0x35); // Razon Riper
        break;                   
   case 12344: // 8
      OSC2Layer(0x02, 0x4F); // Sine
        break;                    
   case 12345: // 9
      OSC2Layer(0x02, 0x37); // Saw
        break;                   
   case 12336: // 0
      OSC2Layer(0x02, 0x55); // SineWheel 16
        break;                   
   case 12348: // ?
      OSC2Layer(0x02, 0x31); // Cricket Spectrum
        break;                   
   case 12383: // ¡
      OSC2Layer(0x02, 0x33); // Noise Spectrum
        break;                   
   case 12380: // }
      OSC2Layer(0x02, 0x44); // Square
        break;                   
   case 12572: // Back
      OSC2Layer(0x02, 0x35); // Razon Riper
        break;                   


// Synth1 Waves OSC 1 ----------------------------------------- Win Key ----------------------------------//

   case 4689://Q
        OSC1romM275 // Wooble Wave 1
        break;                   
   case 4695://W
        OSC1romM271 // Sync
        break;                   
   case 4677://E
        OSC1romM274 // HiReso Lead
        break;                   
   case 4690://R
        OSC1romM275 // Wooble Wave 1
        break;                   
   case 4692://T
        OSC1romM290 // Glass Vox
        break;                   
   case 4697://Y
        OSC1romM292 // SynAir Vox
        break;                   
   case 4693://U
        OSC1romM293 // Syn Ghostly
        break;                   
   case 4681://I
        OSC1romM294 // Syn AirPad
        break;                   
   case 4687://O
        OSC1romM296 // Syn Palawan
        break;                   
   case 4688://P
        OSC1romM300 // Wave Sweep C
        break;   
   case 4701://
        OSC1romM272 // Sync- MS2000
        break;                   
   case 4702://*
        OSC1romM276 // Wobble Wave 2
        break;  

  // Synth1 Waves OSC 2  

   case 4673://A
        OSC2romM275 // Wooble Wave 1
        break;                   
   case 4691://S
        OSC2romM271 // Sync
        break;
   case 4676://D
        OSC2romM274 // HiReso Lead
        break;
   case 4678://F
        OSC2romM275 // Wooble Wave 1
        break;
   case 4679://G
        OSC2romM290 // Glass Vox
        break;
   case 4680://H
        OSC2romM292 // SynAir Vox
        break;
   case 4682://J
        OSC2romM293 // Syn Ghostly
        break;
   case 4683://K
        OSC2romM294 // Syn AirPad
        break;
   case 4684://L
        OSC2romM296 // Syn Palawan
        break;     
   case 4699://Ñ
        OSC2romM300 // Wave Sweep C
        break;
   case 4666://{
        OSC2romM272 // Sync- MS2000
        break;     
/*
// DWGS Waves OSC 1 ---------------------------------- Alt Key ---------------------//

   case 6225://Q
        OSC1romM352 // DWGS EP1
        break;                   
   case 6231://W
        OSC1romM353 // DWGS EP2
        break;                   
   case 6213://E
        OSC1romM355 // DWGS EP4
        break;                   
   case 6226://R
        OSC1romM356 // DWGS EP5
        break;                   
   case 6228://T
        OSC1romM364 // DWGS Bell1
        break;                   
   case 6233://Y
        OSC1romM367 // DWGS Bell4
        break;                   
   case 6229://U
        OSC1romM371 // DWGS Digi1
        break;                   
   case 6217://I
        OSC1romM375 // DWGS Sync1
        break;                   
   case 6223://O
        OSC1romM377 // DWGS Sync3
        break;                   
   case 6224://P
        OSC1romM379 // DWGS Sync5
        break;   
   case 6237://
        OSC1romM370 // DWGS Clav3
        break;                   
   case 6238://*
        OSC1romM350 // DWGS Sine
        break;  

  // DWGS Waves OSC 2  

   case 6209://A
        OSC2romM352 // DWGS EP1
        break;                   
   case 6227://S
        OSC2romM353 // DWGS EP2
        break;
   case 6212://D
        OSC2romM355 // DWGS EP4
        break;
   case 6214://F
        OSC2romM356 // DWGS EP5
        break;
   case 6215://G
        OSC2romM364 // DWGS Bell1
        break;
   case 6216://H
        OSC2romM367 // DWGS Bell4
        break;
   case 6218://J
        OSC2romM371 // DWGS Digi1
        break;
   case 6219://K
        OSC2romM375 // DWGS Sync1
        break;
   case 6220://L
        OSC2romM377 // DWGS Sync3
        break;     
   case 6235://Ñ
        OSC2romM379 // DWGS Sync5
        break;
   case 6202://{
        OSC2romM370 // DWGS Clav3
        break;

// ORGAN Waves OSC 1 ---------------------------------- Alt Key ---------------------//

   case 5201://Q
        OSC1romM070 // E.Organ Perc 1
        break;                   
   case 5207://W
        OSC1romM071 // E.Organ Perc 1
        break;                   
   case 5189://E
        OSC1romM075 // E.Organ M1
        break;                   
   case 5202://R
        OSC1romM076 // E.Organ Dist
        break;                   
   case 5204://T
        OSC1romM073 // E.Organ Perc 4
        break;                   
   case 5209://Y
        OSC1romM074 // E.Organ Perc 5
        break;                   
   case 5205://U
        OSC1romM077 // E.Organ Perc 3rd
        break;                   
   case 5193://I
        OSC1romM078 // E.Organ Gospel 1
        break;                   
   case 5199://O
        OSC1romM084 // E.Organ Full 1
        break;                   
   case 5200://P
        OSC1romM096 // E.Organ Vox 3
        break;   
   case 5213://
        OSC1romM097 // E.Organ Vox 4
        break;                   
   case 5214://*
        OSC1romM102 // PipeOrg Mixture 1
        break;  

  // ORGAN Waves OSC 2  

   case 5185://A
        OSC2romM070 // E.Organ Perc 1
        break;                   
   case 5203://S
        OSC2romM071 // E.Organ Perc 1
        break;
   case 5188://D
        OSC2romM075 // E.Organ M1
        break;
   case 5190://F
        OSC2romM076 // E.Organ Dist
        break;
   case 5191://G
        OSC2romM073 // E.Organ Perc 4
        break;
   case 5192://H
        OSC2romM074 // E.Organ Perc 5
        break;
   case 5194://J
        OSC2romM077 // E.Organ Perc 3rd
        break;
   case 5195://K
        OSC2romM078 // E.Organ Gospel 1
        break;
   case 5196://L
        OSC2romM084 // E.Organ Full 1
        break;     
   case 5211://Ñ
        OSC2romM096 // E.Organ Vox 3
        break;
   case 5178://{
        OSC2romM097 // E.Organ Vox 4
        break;

// OSC1 Piano-E.Piano Waves -------------------------------------------------------------//
   case 20561: // Q
        OSC1romM011 // Kross Mf Mono
        break;                   
   case 20567: // W
        OSC1romM013 // A. Piano Mono
        break;
   case 20549: // E
        OSC1romM014 // Upright Piano
        break;
   case 20562: // R
        OSC1romM015 // M1 Piano
        break;
   case 20564: // T
        OSC1romM016 // SG Piano
        break;
   case 20569: // Y
        OSC1romM019 // E.Grand f
        break;
   case 20565: // U
        OSC1romM023 // EP SuitBright mp
        break;
   case 20553: // I
        OSC1romM024 // EP SuitBright mf
        break;
   case 20559: // O
        OSC1romM025 // EP SuitBright f
        break;     
   case 20560: // P
        OSC1romM022 // EP SuitPure f
        break;
   case 20573: // 
        OSC1def(0x01, 0x01); // OSC1 RomS
        OSC1def(0x02, 0x04); // Acoustic Piano stereo mf
        break;
   case 20574: // *
        OSC1def(0x01, 0x01); // OSC1 RomS
        OSC1def(0x02, 0x01); // Kross Piano stereo mf
        break; 

// OSC1 Piano-E.Piano Waves -------------------------------------------------------------//
   case 20545://A
        OSC1romM035 // EP Wurly p
        break;
   case 20563://S
        OSC1romM037 // EP Wurly mf
        break;
   case 20548://D
        OSC1romM038 // EP Wurly f
        break;
   case 20550://F
        OSC1romM052 // Clavi EXB 1
        break;
   case 20551://G
        OSC1romM053 // Clavi EXB 2
        break;
   case 20552://H
        OSC1romM054 // Clavi EXB 3
        break;
   case 20554://J
        OSC1romM055 // E.Piano FM 1
        break;     
   case 20555://K
        OSC1romM056 // E.Piano FM 2
        break;
   case 20556://L
        OSC1romM057 // E.Piano FM 3
        break;   
   case 20571://Ñ
        OSC1romM060 // E.Piano FM 4 f
        break;
   case 20538://{
        OSC1romM063 // E.Piano FM 5 f
        break;            

// OSC2 Piano-E.Piano Waves -------------------------------------------------------------//
   case 28753: // Q
        OSC2romM011 // Kross Mf Mono
        break;                   
   case 28759: // W
        OSC2romM013 // A. Piano Mono
        break;
   case 28741: // E
        OSC2romM014 // Upright Piano
        break;
   case 28754: // R
        OSC2romM015 // M1 Piano
        break;
   case 28756: // T
        OSC2romM016 // SG Piano
        break;
   case 28761: // Y
        OSC2romM019 // E.Grand f
        break;
   case 28757: // U
        OSC2romM023 // EP SuitBright mp
        break;
   case 28745: // I
        OSC2romM024 // EP SuitBright mf
        break;
   case 28751: // O
        OSC2romM025 // EP SuitBright f
        break;     
   case 28752: // P
        OSC2romM022 // EP SuitPure f
        break;
   case 28765: // 
        OSC2def(0x01, 0x01); // OSC1 RomS
        OSC2def(0x02, 0x04); // Acoustic Piano stereo mf
        break;
   case 28766: // *
        OSC2def(0x01, 0x01); // OSC1 RomS
        OSC2def(0x02, 0x01); // Kross Piano stereo mf
        break; 

// OSC2 Piano-E.Piano Waves -------------------------------------------------------------//
   case 28737://A
        OSC2romM035 // EP Wurly p
        break;
   case 28755://S
        OSC2romM037 // EP Wurly mf
        break;
   case 28740://D
        OSC2romM038 // EP Wurly f
        break;
   case 28742://F
        OSC2romM052 // Clavi EXB 1
        break;
   case 28743://G
        OSC2romM053 // Clavi EXB 2
        break;
   case 28744://H
        OSC2romM054 // Clavi EXB 3
        break;
   case 28746://J
        OSC2romM055 // E.Piano FM 1
        break;     
   case 28747://K
        OSC2romM056 // E.Piano FM 2
        break;
   case 28748://L
        OSC2romM057 // E.Piano FM 3
        break;   
   case 28763://Ñ
        OSC2romM060 // E.Piano FM 4 f
        break;
   case 28730://{
        OSC2romM063 // E.Piano FM 5 f
        break;            
*/
// OSC1 Filter ---------------------- Control Key + F1 a F4 ---------------------------------------//
   case 12641://F1
        Filters(0x07, 0x00); // OSC1 single filter
        break;
   case 12642://F2
        Filters(0x07, 0x01); // OSC1 serial filter
        break;
   case 12643://F3
        Filters(0x07, 0x02); // OSC1 parallel filter
        break;
   case 12644://F4
        Filters(0x07, 0x03); // OSC1 24 dB filter
        break;

// OSC2 Filter ---------------------- Control Key + F5 a F8 ---------------------------------------//
   case 12645://F5
        Filters(0x17, 0x00); // OSC2 single filter
        break;
   case 12646://F6
        Filters(0x17, 0x01); // OSC2 serial filter
        break;
   case 12647://F7
        Filters(0x17, 0x02); // OSC2 parallel filter
        break;
   case 12648://F8
        Filters(0x17, 0x03); // OSC2 24 dB filter
        break;

// OSC1 Filter A Type ---------------------- Win Key + F1 a F4 ---------------------------------------//
   case 4961://F1
        Filters(0x08, 0x00); // OSC1 Low Pass filter
        break;
   case 4962://F2
        Filters(0x08, 0x01); // OSC1 High Pass filter
        break;
   case 4963://F3
        Filters(0x08, 0x02); // OSC1 Band pass filter
        break;
   case 4964://F4
        Filters(0x08, 0x03); // OSC1 Band Reject filter
        break;

// OSC2 Filter A Type ---------------------- Win Key + F5 a F8 ---------------------------------------//
   case 4965://F5
        Filters(0x18, 0x00); // OSC2 Low pass filter
        break;
   case 4966://F6
        Filters(0x18, 0x01); // OSC2 High Pass filter
        break;
   case 4967://F7
        Filters(0x18, 0x02); // OSC2 Band Pass filter
        break;
   case 4968://F8
        Filters(0x18, 0x03); // OSC2 Band Reject filter
        break;

// OSC1 Filter B Type si es Paralelo o Serial ---------------------- Alt Key + F1 a F4 ---------------------------------------//
   case 6497://F1
        Filters(0x09, 0x00); // OSC1 Low Pass filter
        break;
   case 6498://F2
        Filters(0x09, 0x01); // OSC1 High Pass filter
        break;
   case 6499://F3
        Filters(0x09, 0x02); // OSC1 Band Pass filter
        break;
   case 6500://F4
        Filters(0x09, 0x03); // OSC1 Band Reject filter
        break;

// OSC2 Filter B Type si es paralelo o Serial ---------------------- Alt Key + F5 a F8 ---------------------------------------//
   case 6501://F5
        Filters(0x19, 0x00); // OSC2 Low Pass filter
        break;
   case 6502://F6
        Filters(0x19, 0x01); // OSC2 High Pass filter
        break;
   case 6503://F7
        Filters(0x19, 0x02); // OSC2 Band Pass filter
        break;
   case 6504://F8
        Filters(0x19, 0x03); // OSC2 Band Reject filter
        break;



/*
// Waves SysEx loops

// DWGS OSC1
  case 4235: // <>  Decrementar
  case 4186: // Z   Incrementar
    {
        int desiredValues[] = {350,351,352,353,354,355,356,357,358,359,360,361,362,363,364,365,366,367,368,369,370,371,372,373,374,375,376,377,378,379,380};

        if (keyValue == 4186) {
            currentIndex++;
            if (currentIndex >= sizeof(desiredValues)/sizeof(int)) {
                currentIndex = 0; // Volver al principio si se supera el final
            }
        } else if (keyValue == 4235) {
            currentIndex--;
            if (currentIndex < 0) {
                currentIndex = sizeof(desiredValues)/sizeof(int) - 1; // Volver al final si se supera el principio
            }
        }
        sendSysExOSC1(desiredValues[currentIndex]);
        break;
    }

// DWGS OSC2
  case 4184: // <>  Decrementar
  case 4163: // Z   Incrementar
    {
        int desiredValues[] = {350,351,352,353,354,355,356,357,358,359,360,361,362,363,364,365,366,367,368,369,370,371,372,373,374,375,376,377,378,379,380};

        if (keyValue == 4163) {
            currentIndex++;
            if (currentIndex >= sizeof(desiredValues)/sizeof(int)) {
                currentIndex = 0; // Volver al principio si se supera el final
            }
        } else if (keyValue == 4184) {
            currentIndex--;
            if (currentIndex < 0) {
                currentIndex = sizeof(desiredValues)/sizeof(int) - 1; // Volver al final si se supera el principio
            }
        }
        sendSysExOSC2(desiredValues[currentIndex]);
        break;
    }   */


/*

  case 4182: // V
    Lfo1osc1waves(0x05); // ExpTri
    break;
  case 4162: // B
    Lfo1osc1waves(0x06); // SawDown
    break;
  case 4174: // N
    Lfo1osc1waves(0x07); // Saw Up
    break;
  case 4173: // M
    Lfo1osc1waves(0x08); // StepTri 4
    break;
  case 4155: // ,
    Lfo1osc1waves(0x09); // StepTri 6
    break;
  case 4157: // .
    Lfo1osc1waves(0x0c); // Random1 SH
    break;
  case 4158: // -
    Lfo1osc1waves(0x0d); // Random2 SH
    break; 

// LFO1 OSC2 Waves
// <>
   case 12427:
    Lfo1osc2waves(0x00); // Triangle
        break;                   
// Z
   case 12378:
    Lfo1osc2waves(0x01); // Saw
        break;                   
// X
   case 12376:
    Lfo1osc2waves(0x02); // Square
        break;                   
// C
   case 12355:
    Lfo1osc2waves(0x04); // Guit
        break;                   
// V
   case 12374:
    Lfo1osc2waves(0x05); // ExpTri
        break;                   
// B
   case 12354:
    Lfo1osc2waves(0x06); // SawDown
        break;                   
// N
   case 12366:
    Lfo1osc2waves(0x07); // Saw Up
        break;                   
// M
   case 12365:
    Lfo1osc2waves(0x08); // StepTri 4
        break;                   
// ,
   case 12347:
    Lfo1osc2waves(0x09); // StepTri 6
        break;                   
// .
   case 12349:
    Lfo1osc2waves(0x0c); // Random1 SH
        break;                   
// _
   case 12350:
    Lfo1osc2waves(0x0d); // Random2 SH
        break; 

// LFO2 OSC1
   case 4747://<>
    Lfo2osc1waves(0x00); // Triangle
        break;
   case 4698://Z
    Lfo2osc1waves(0x01); // Saw
        break;
   case 4696://X
    Lfo2osc1waves(0x02); // Square
        break;
   case 4675://C
    Lfo2osc1waves(0x04); // Guit
        break;
   case 4694://V
    Lfo2osc1waves(0x05); // ExpTri
        break;
   case 4674://B
    Lfo2osc1waves(0x06); // SawDown
        break;
   case 4686://N
    Lfo2osc1waves(0x07); // Saw Up
        break;     
   case 4685://M
    Lfo2osc1waves(0x08); // StepTri 4
        break;
   case 4667://;
    Lfo2osc1waves(0x09); // StepTri 6
        break;        
   case 4669://:
    Lfo2osc1waves(0x0c); // Random1 SH
        break;
   case 4670://-
    Lfo2osc1waves(0x0d); // Random2 SH
        break;        

// LFO2 OSC2
   case 6283://<>
    Lfo2osc2waves(0x00); // Triangle
        break;
   case 6234://Z
    Lfo2osc2waves(0x01); // Saw
        break;
   case 6232://X
    Lfo2osc2waves(0x02); // Square
        break;
   case 6211://C
    Lfo2osc2waves(0x04); // Guit
        break;
   case 6230://V
    Lfo2osc2waves(0x05); // ExpTri
        break;
   case 6210://B
    Lfo2osc2waves(0x06); // SawDown
        break;
   case 6222://N
    Lfo2osc2waves(0x07); // Saw Up
        break;     
   case 6221://M
    Lfo2osc2waves(0x08); // StepTri 4
        break;
   case 6203://;
    Lfo2osc2waves(0x09); // StepTri 6
        break;        
   case 6205://:
    Lfo2osc2waves(0x0c); // Random1 SH
        break;
   case 6206://-
    Lfo2osc2waves(0x0d); // Random2 SH
        break;        


*/



//ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo//
  //------------------------------------------------------------ POTEs MODO PROGRAM ----------------------------------------------------------------------------//
   case 353: //bank F1            DEFAULT 
        PO1.newValue(0, 73, 1);  // Attack
        PO2.newValue(0, 72, 1);  // Release
        PO3.newValue(5, 74, 1);  // CutOff
        PO3.Potec1[7]={0x02};PO3.Potec1[9]={0x15}; PO3.Potec1[11]={0x02};
        PO4.newValue(5, 71, 1);  // Resonance
        PO4.Potec1[7]={0x02};PO4.Potec1[9]={0x15}; PO4.Potec1[11]={0x03};
        PO5.newValue(0, 5, 1);
        PO6.newValue(0, 7, 1);
        PO7.newValue(0, 7, 10);
        PO8.newValue(0, 7, 16);
        break;     
    case 354: //bank F2           Mod2, Pan, LFO speed,  LFO depth, LFO delay
        PO1.newValue(0, 2, 1);
        PO2.newValue(0, 10, 1);
        PO3.newValue(0, 76, 1);
        PO4.newValue(10, 77, 1);
        PO5.newValue(0, 78, 1);
        PO6.newValue(0, 7, 1);
        PO7.newValue(0, 7, 10);
        PO8.newValue(0, 7, 16);
        break; 
    case 355: //bank F3           Attack, Decay, Sustain, Release, Filter Eg Intensity
        PO1.newValue(0, 73, 1); // command 5 for SysEx
        //PO1.Potec1[7]={0x02};PO1.Potec1[9]={0x15}; PO1.Potec1[11]={0x06};
        PO2.newValue(0, 75, 1); // 5
        //PO2.Potec1[7]={0x02};PO2.Potec1[9]={0x15}; PO2.Potec1[11]={0x07};
        PO3.newValue(0, 70, 1); // 5
        //PO3.Potec1[7]={0x02};PO3.Potec1[9]={0x15}; PO3.Potec1[11]={0x08};
        PO4.newValue(0, 72, 1); // 5
        //PO4.Potec1[7]={0x02};PO4.Potec1[9]={0x15}; PO4.Potec1[11]={0x09};
        PO5.newValue(0, 79, 1); // 5
        //PO5.Potec1[7]={0x02};PO5.Potec1[9]={0x15}; PO5.Potec1[11]={0x04};
        PO6.newValue(0, 7, 1);
        PO7.newValue(0, 7, 10);
        PO8.newValue(0, 7, 16);
        break;     
    case 356: //bank F4           Arp Gate, Arp Vel, Arp Swing, Attack, Decay
        PO1.newValue(0, 22, 1);
        PO2.newValue(0, 23, 1);
        PO3.newValue(0, 24, 1);
        PO4.newValue(0, 73, 1);
        PO5.newValue(0, 75, 1);
        PO6.newValue(0, 7, 1);
        PO7.newValue(0, 7, 10);
        PO8.newValue(0, 7, 16);
        break;     
    case 357: //bank F5  FX         MFX1send, MFX2send, Reverb Time, MFX1 master, MFX2 master, CC16, CC87, CC88  
        PO1.newValue(0, 93, 1);
        PO2.newValue(0, 91, 1);
        PO3.newValue(6, 92, 1);
        PO3.Potec[7]={0x02};PO3.Potec[9]={0x12}; PO3.Potec[11]={0x00};
        PO4.newValue(8, 93, 1);
        PO4.Potec[7]={0x02}; PO4.Potec[9]={0x10}; PO4.Potec[11]={0x04};    
        PO5.newValue(1, 91, 1);
        PO5.Potec[7]={0x02}; PO5.Potec[9]={0x10}; PO5.Potec[11]={0x05};
        PO6.newValue(0, 16, 1);
//        PO6.Potec[7]={0x02};PO6.Potec[9]={0x12}; PO6.Potec[11]={0x01};        
        PO7.newValue(0, 87, 1);
//        PO7.Potec[7]={0x02};PO7.Potec[9]={0x12}; PO7.Potec[11]={0x03};        
        PO8.newValue(0, 88, 1);
//        PO8.Potec[7]={0x02};PO8.Potec[9]={0x12}; PO8.Potec[11]={0x04};        
        break;     
    case 358: //bank F6           FX ROUTING SEND 1
        PO1.newValue(1, 74, 6);
        //{0xf0, 0x42, 0x30, 0x00, 0x01, 0x49, 0x41, 0x02, 0x00, 0x0a, 0x00, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf7};
        PO1.Potec[7]={0x02}; PO1.Potec[9]={0x0a}; PO1.Potec[11]={0x14}; 
        PO2.newValue(1, 71, 6);
        //{0xf0, 0x42, 0x30, 0x00, 0x01, 0x49, 0x41, 0x02, 0x00, 0x0a, 0x00, 0x15, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf7};
        PO2.Potec[7]={0x02}; PO2.Potec[9]={0x0a}; PO2.Potec[11]={0x15}; 
        PO3.newValue(1, 72, 6);
        //{0xf0, 0x42, 0x30, 0x00, 0x01, 0x49, 0x41, 0x02, 0x00, 0x0a, 0x00, 0x16, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf7};
        PO3.Potec[7]={0x02}; PO3.Potec[9]={0x0a}; PO3.Potec[11]={0x16}; 
        PO4.newValue(1, 73, 6);
        //{0xf0, 0x42, 0x30, 0x00, 0x01, 0x49, 0x41, 0x02, 0x00, 0x0a, 0x00, 0x17, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf7};
        PO4.Potec[7]={0x02}; PO4.Potec[9]={0x0a}; PO4.Potec[11]={0x17};  
        PO5.newValue(1, 75, 6);
        //{0xf0, 0x42, 0x30, 0x00, 0x01, 0x49, 0x41, 0x02, 0x00, 0x0a, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf7};
        PO5.Potec[7]={0x02}; PO5.Potec[9]={0x0a}; PO5.Potec[11]={0x18}; 
        PO6.newValue(0, 7, 1);
        PO7.newValue(0, 7, 10);
        PO8.newValue(0, 7, 16);
        break;     
    case 359: //bank F7           FX ROUTING SEND 2
        PO1.newValue(1, 74, 6);
        //{0xf0, 0x42, 0x30, 0x00, 0x01, 0x49, 0x41, 0x02, 0x00, 0x0a, 0x00, 0x19, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf7};
        PO1.Potec[7]={0x02}; PO1.Potec[9]={0x0a}; PO1.Potec[11]={0x19}; 
        PO2.newValue(1, 71, 6);
        //{0xf0, 0x42, 0x30, 0x00, 0x01, 0x49, 0x41, 0x02, 0x00, 0x0a, 0x00, 0x1a, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf7};
        PO2.Potec[7]={0x02}; PO2.Potec[9]={0x0a}; PO2.Potec[11]={0x1a}; 
        PO3.newValue(1, 72, 6);
        //{0xf0, 0x42, 0x30, 0x00, 0x01, 0x49, 0x41, 0x02, 0x00, 0x0a, 0x00, 0x1b, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf7};
        PO3.Potec[7]={0x02}; PO3.Potec[9]={0x0a}; PO3.Potec[11]={0x1b}; 
        PO4.newValue(1, 73, 6);
        //{0xf0, 0x42, 0x30, 0x00, 0x01, 0x49, 0x41, 0x02, 0x00, 0x0a, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf7};
        PO4.Potec[7]={0x02}; PO4.Potec[9]={0x0a}; PO4.Potec[11]={0x1c};  
        PO5.newValue(1, 75, 6);
        //{0xf0, 0x42, 0x30, 0x00, 0x01, 0x49, 0x41, 0x02, 0x00, 0x0a, 0x00, 0x1d, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf7};
        PO5.Potec[7]={0x02}; PO5.Potec[9]={0x0a}; PO5.Potec[11]={0x1d}; 
        PO6.newValue(0, 7, 1);
        PO7.newValue(0, 7, 10);
        PO8.newValue(0, 7, 16);
        break;     
    case 360: //bank F8           ON/OFF inserts FX 
        PO1.newValue(7, 22, 1); //  IFX 1
        //{0xf0, 0x42, 0x30, 0x00, 0x01, 0x49, 0x41, 0x02, 0x00, 0x0a, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf7};
        PO1.Potec[7]={0x02}; PO1.Potec[9]={0x0a}; PO1.Potec[11]={0x05};           
        PO2.newValue(7, 23, 1); //  IFX 2
        //{0xf0, 0x42, 0x30, 0x00, 0x01, 0x49, 0x41, 0x02, 0x00, 0x0a, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf7};
        PO2.Potec[7]={0x02}; PO2.Potec[9]={0x0a}; PO2.Potec[11]={0x06};         
        PO3.newValue(7, 24, 1); //  IFX 3
        //{0xf0, 0x42, 0x30, 0x00, 0x01, 0x49, 0x41, 0x02, 0x00, 0x0a, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf7};
        PO3.Potec[7]={0x02}; PO3.Potec[9]={0x0a}; PO3.Potec[11]={0x07};         
        PO4.newValue(7, 93, 1); //  IFX 4
        //{0xf0, 0x42, 0x30, 0x00, 0x01, 0x49, 0x41, 0x02, 0x00, 0x0a, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf7};
        PO4.Potec[7]={0x02}; PO4.Potec[9]={0x0a}; PO4.Potec[11]={0x08};         
        PO5.newValue(7, 91, 1); //  IFX 5
        //{0xf0, 0x42, 0x30, 0x00, 0x01, 0x49, 0x41, 0x02, 0x00, 0x0a, 0x00, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf7};
        PO5.Potec[7]={0x02}; PO5.Potec[9]={0x0a}; PO5.Potec[11]={0x09};         
        PO6.newValue(0, 7, 1);
        PO7.newValue(0, 7, 10);
        PO8.newValue(0, 7, 16);
        break;     
    case 361: //bank F9           CC: 12, 13, 16, 17, 18 
        PO1.newValue(0, 12, 1);
        PO2.newValue(0, 13, 1);
        PO3.newValue(0, 16, 1);
        PO4.newValue(0, 17, 1);
        PO5.newValue(0, 18, 1);
        PO6.newValue(0, 7, 1);
        PO7.newValue(0, 7, 10);
        PO8.newValue(0, 7, 16);
        break;     
    case 362: //banco F10          DRUM TRACK:  Attack-Decay, FILTERS, Pitch
        PO1.newValue(0, 73, 10);
        PO2.newValue(0, 75, 10);
        PO3.newValue(0, 74, 10);
        PO4.newValue(0, 71, 10);
        PO5.newValue(3, 7, 10);
        PO6.newValue(0, 7, 1);
        PO7.newValue(0, 7, 10);
        PO8.newValue(0, 7, 16);
        break;     
    case 363: //bank F11          DRUM TRACK: Pan,  Mod 1, Mod 2, LFO
        PO1.newValue(0, 10, 10);
        PO2.newValue(0, 1, 10);
        PO3.newValue(0, 2, 10);
        PO4.newValue(0, 76, 10);
        PO5.newValue(0, 77, 10);
        PO6.newValue(0, 7, 1);
        PO7.newValue(0, 7, 10);
        PO8.newValue(0, 7, 16);
        break;     
    case 364: //bank F12          DRUM TRACK:   FX SEND 1, FX SEND 2, IFX on/off, MFX on/off, CC 12
        PO1.newValue(0, 93, 10);
        PO2.newValue(0, 91, 10);
        PO3.newValue(0, 92, 1);
        PO4.newValue(0, 94, 1);
        PO5.newValue(0, 12, 1);
        PO6.newValue(0, 7, 1);
        PO7.newValue(0, 7, 10);
        PO8.newValue(0, 7, 16);
        break;     
    case 400: //Bank key: Despertar     STEP SEQ:  Atack-Decay, FILTERS, Pitch
        PO1.newValue(0, 73, 16);
        PO2.newValue(0, 75, 16);
        PO3.newValue(0, 74, 16);
        PO4.newValue(0, 71, 16);
        PO5.newValue(3, 7, 16);
        PO6.newValue(0, 7, 1);
        PO7.newValue(0, 7, 10);
        PO8.newValue(0, 7, 16);
        break;     
    case 397: //Bank key: Dormir        STEP SEQ:  Pan,  Mod 1,  Mod 2,  LFO
        PO1.newValue(0, 10, 16);
        PO2.newValue(0, 1, 16);
        PO3.newValue(0, 2, 16);
        PO4.newValue(0, 76, 16);
        PO5.newValue(0, 77, 16);
        PO6.newValue(0, 7, 1);
        PO7.newValue(0, 7, 10);
        PO8.newValue(0, 7, 16);
        break;     
    case 396: //Bank key: Apagar        STEP SEQ:  FX SEND 1, FX SEND 2, IFX on/off, MFX on/off, CC 12
        PO1.newValue(0, 93, 16);
        PO2.newValue(0, 91, 16);
        PO3.newValue(0, 92, 1);
        PO4.newValue(0, 94, 1);
        PO5.newValue(0, 12, 1);
        PO6.newValue(0, 7, 1);
        PO7.newValue(0, 7, 10);
        PO8.newValue(0, 7, 16);
        break; 

    case 283: //------------------------banco ESC  Pagina ajustes de audio. Patch Tweek----------------------------------// 

        if (MonoUnison)//true
        {
          MonoUnison = !MonoUnison;
          sendDef703(0x00, 0x09, 0x00); // MonoUnisonOFF
        }
        else//false
        {
          MonoUnison = !MonoUnison; 
        
          sendDef703(0x00, 0x09, 0x01); // MonoUnisonON     
          sendDef703(0x00, 0x08, 0x04); // voices4
          sendDef703(0x00, 0x0b, 0x03); // thickness3
        }  
        PO1.newValue(1, 74, 1);   //OSC 1  Vol
         //{0xf0, 0x42, 0x30, 0x00, 0x01, 0x49, 0x41, 0x02, 0x00, 0x01, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf7};
        PO1.Potec[7]={0x02}; PO1.Potec[9]={0x01}; PO1.Potec[11]={0x0c}; 
        PO2.newValue(1, 71, 1);   //OSC 2  Vol
         //{0xf0, 0x42, 0x30, 0x00, 0x01, 0x49, 0x41, 0x02, 0x00, 0x01, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf7};
        PO2.Potec[7]={0x02}; PO2.Potec[9]={0x01}; PO2.Potec[11]={0x0d}; 
        PO3.newValue(9, 73, 1);   //OSC 1 octave  
        PO3.Potec1[7]={0x03}; PO3.Potec1[9]={0x06}; PO3.Potec1[11]={0x00}; 
        PO4.newValue(9, 72, 1);   //OSC 2 octave
        PO4.Potec1[7]={0x03}; PO4.Potec1[9]={0x16}; PO4.Potec1[11]={0x00};
        PO5.newValue(6, 83, 1);   //Mono Unison Detune cents 
         //{0xf0, 0x42, 0x30, 0x00, 0x01, 0x49, 0x41, 0x03, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf7};
        PO5.Potec[7]={0x03}; PO5.Potec[9]={0x00}; PO5.Potec[11]={0x0a};
        PO6.newValue(0, 7, 1);
        PO7.newValue(0, 7, 10);
        PO8.newValue(0, 7, 16);     
        break;

//oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo//
//--------------------------------------------------------- POTEs MODO Edición --------------------------------------------------------------------------------//

// Pots default modo Edicion
/*    case 4355: // CapsLock

        PO1.newValue(1, 74, 1);   //OSC 1  Vol       
        PO1.Potec[7]={0x03}; PO1.Potec[9]={0x0c}; PO1.Potec[11]={0x00}; 
        PO2.newValue(1, 71, 1);   //OSC 2  Vol
        PO2.Potec[7]={0x03}; PO2.Potec[9]={0x1c}; PO2.Potec[11]={0x00}; 
        PO3.defineSySex( 66 );   //OSC 1 octave   
        PO3.newValue(9, 73, 1); 
        PO4.defineSySex( 67 );   //OSC 2 Octave
        PO4.newValue(9, 72, 1);
        PO5.newValue(1, 74, 1);   //OSC 1  Pan       
        PO5.Potec[7]={0x03}; PO5.Potec[9]={0x0f}; PO5.Potec[11]={0x00}; 
        PO6.newValue(1, 71, 1);   //OSC 2  Pan
        PO6.Potec[7]={0x03}; PO6.Potec[9]={0x1f}; PO6.Potec[11]={0x00}; 
        PO7.newValue(1, 74, 1);  // Vol Drum Track   
        PO7.Potec[7]={0x02}; PO7.Potec[9]={0x08}; PO7.Potec[11]={0x01}; 
        PO8.newValue(1, 71, 1);   // Vol Step Seq 
        PO8.Potec[7]={0x02}; PO8.Potec[9]={0x09}; PO8.Potec[11]={0x01}; 
        break;    */

// Pots A
   case 4449: // Banco F1 ------------------- Default  ----------------------------------------------------------------//     
        PO1.newValue(1, 74, 1);   //OSC 1  Vol       
        PO1.Potec[7]={0x03}; PO1.Potec[9]={0x0c}; PO1.Potec[11]={0x00}; 
        PO2.newValue(1, 71, 1);   //OSC 2  Vol
        PO2.Potec[7]={0x03}; PO2.Potec[9]={0x1c}; PO2.Potec[11]={0x00}; 
        PO3.newValue(9, 73, 1);   //OSC 1 octave  
        PO3.Potec1[7]={0x03}; PO3.Potec1[9]={0x06}; PO3.Potec1[11]={0x00}; 
        PO4.newValue(9, 72, 1);   //OSC 2 octave
        PO4.Potec1[7]={0x03}; PO4.Potec1[9]={0x16}; PO4.Potec1[11]={0x00};
        PO5.newValue(1, 74, 1);   //OSC 1  Pan       
        PO5.Potec[7]={0x03}; PO5.Potec[9]={0x0f}; PO5.Potec[11]={0x00}; 
        PO6.newValue(1, 71, 1);   //OSC 2  Pan
        PO6.Potec[7]={0x03}; PO6.Potec[9]={0x1f}; PO6.Potec[11]={0x00}; 
        PO7.newValue(1, 74, 1);   // Vol Drum Track   
        PO7.Potec[7]={0x02}; PO7.Potec[9]={0x08}; PO7.Potec[11]={0x01}; 
        PO8.newValue(1, 71, 1);   // Vol Step Seq 
        PO8.Potec[7]={0x02}; PO8.Potec[9]={0x09}; PO8.Potec[11]={0x01}; 
        break;
   case 4450: // Banco F2 ------------- Pitch  ----------------------------------------------------------------------//
        sendDef703(0x06, 0x11, 0x0c); // CC16 +12 OSC1
        sendDef703(0x16, 0x11, 0x0c); // CC16 +12 OSC2 
        PO1.newValue(11, 74, 1);   //OSC 1  Transpose       
        PO1.Potec[7]={0x03}; PO1.Potec[9]={0x06}; PO1.Potec[11]={0x01}; 
        PO2.newValue(11, 71, 1);   //OSC 2  Transpose
        PO2.Potec[7]={0x03}; PO2.Potec[9]={0x16}; PO2.Potec[11]={0x01}; 
        PO3.newValue(13, 74, 1);   // Detune OSC1    
        PO3.Potec1[7]={0x03}; PO3.Potec1[9]={0x06}; PO3.Potec1[11]={0x02}; 
        PO4.newValue(13, 71, 1);   // Detune OSC2
        PO4.Potec1[7]={0x03}; PO4.Potec1[9]={0x16}; PO4.Potec1[11]={0x02}; 
        PO5.newValue(0, 16, 1);    // CC16 Pitch +12
        PO6.newValue(0, 5, 1);     // Portamento
//        PO6.Potec[7]={0x03}; PO6.Potec[9]={0x1f}; PO6.Potec[11]={0x00}; 
        PO7.newValue(5, 74, 1);    // OSC1 EG   
        PO7.Potec1[7]={0x03}; PO7.Potec1[9]={0x06}; PO7.Potec1[11]={0x06}; 
        PO8.newValue(5, 71, 1);    // OSC2 EG
        PO8.Potec1[7]={0x03}; PO8.Potec1[9]={0x16}; PO8.Potec1[11]={0x06}; 
        break;
   case 4451: // Banco F3 --------------------------------- Pitch EG -------------------------------------------------------//
        PO1.newValue(5, 74, 1);   // Start Level    
        PO1.Potec1[7]={0x03}; PO1.Potec1[9]={0x01}; PO1.Potec1[11]={0x00}; 
        PO2.newValue(12, 71, 1);  // Attack Time
        PO2.Potec[7]={0x03}; PO2.Potec[9]={0x01}; PO2.Potec[11]={0x01}; 
        PO3.newValue(5, 74, 1);   // Attack Level     
        PO3.Potec1[7]={0x03}; PO3.Potec1[9]={0x01}; PO3.Potec1[11]={0x02}; 
        PO4.newValue(12, 71, 1);  // Decay Time
        PO4.Potec[7]={0x03}; PO4.Potec[9]={0x01}; PO4.Potec[11]={0x03}; 
        PO5.newValue(5, 74, 1);   // Decay Level     
        PO5.Potec1[7]={0x03}; PO5.Potec1[9]={0x01}; PO5.Potec1[11]={0x04}; 
        PO6.newValue(12, 71, 1);  // Slope Time
        PO6.Potec[7]={0x03}; PO6.Potec[9]={0x01}; PO6.Potec[11]={0x05}; 
        PO7.newValue(12, 74, 1);  // Release Time    
        PO7.Potec[7]={0x03}; PO7.Potec[9]={0x01}; PO7.Potec[11]={0x06}; 
        PO8.newValue(5, 71, 1);   // Release level 
        PO8.Potec1[7]={0x03}; PO8.Potec1[9]={0x01}; PO8.Potec1[11]={0x07}; 
        break;    
   case 4452: // Banco F4 --------------- Tone Adjusts -----------------------------------//
        PO1.newValue(5, 74, 1);  // CutOff    
        PO1.Potec1[7]={0x02}; PO1.Potec1[9]={0x15}; PO1.Potec1[11]={0x02}; 
        PO2.newValue(5, 71, 1);  // Resonance
        PO2.Potec1[7]={0x02}; PO2.Potec1[9]={0x15}; PO2.Potec1[11]={0x03}; 
        PO3.newValue(5, 74, 1);  // EG Intensity     
        PO3.Potec1[7]={0x02}; PO3.Potec1[9]={0x15}; PO3.Potec1[11]={0x04}; 
        PO4.newValue(5, 71, 1);  // Vel Intensity
        PO4.Potec1[7]={0x02}; PO4.Potec1[9]={0x15}; PO4.Potec1[11]={0x05}; 
        PO5.newValue(5, 74, 1);  // EG Attack    
        PO5.Potec1[7]={0x02}; PO5.Potec1[9]={0x15}; PO5.Potec1[11]={0x06}; 
        PO6.newValue(5, 71, 1);  // EG Decay
        PO6.Potec1[7]={0x02}; PO6.Potec1[9]={0x15}; PO6.Potec1[11]={0x07}; 
        PO7.newValue(5, 74, 1);  // EG Sustain   
        PO7.Potec1[7]={0x02}; PO7.Potec1[9]={0x15}; PO7.Potec1[11]={0x08}; 
        PO8.newValue(5, 71, 1);  // EG Release 
        PO8.Potec1[7]={0x02}; PO8.Potec1[9]={0x15}; PO8.Potec1[11]={0x09}; 
        break;
   case 4453: // Banco F5 ------------------ Filter ajustes frecuencia y resonancia.         
        PO1.newValue(12, 74, 1);  // OSC1 A Freq    
        PO1.Potec[7]={0x03}; PO1.Potec[9]={0x08}; PO1.Potec[11]={0x06}; 
        PO2.newValue(12, 71, 1);  // OSC1 A Resonance
        PO2.Potec[7]={0x03}; PO2.Potec[9]={0x08}; PO2.Potec[11]={0x07}; 
        PO3.newValue(12, 74, 1);  // OSC2 A Freq   
        PO3.Potec[7]={0x03}; PO3.Potec[9]={0x18}; PO3.Potec[11]={0x06}; 
        PO4.newValue(12, 71, 1);  // OSC2 A Resonance
        PO4.Potec[7]={0x03}; PO4.Potec[9]={0x18}; PO4.Potec[11]={0x07}; 
        PO5.newValue(12, 74, 1);  // OSC1 B Freq    
        PO5.Potec[7]={0x03}; PO5.Potec[9]={0x09}; PO5.Potec[11]={0x06}; 
        PO6.newValue(12, 71, 1);  // OSC1 B Resonance
        PO6.Potec[7]={0x03}; PO6.Potec[9]={0x09}; PO6.Potec[11]={0x07}; 
        PO7.newValue(12, 74, 1);  // OSC2 B Freq   
        PO7.Potec[7]={0x03}; PO7.Potec[9]={0x19}; PO7.Potec[11]={0x06}; 
        PO8.newValue(12, 71, 1);  // OSC2 B Resonance
        PO8.Potec[7]={0x03}; PO8.Potec[9]={0x19}; PO8.Potec[11]={0x07}; 
        break;     
   case 4454: //banco F6 ------------------ Filter envelope OSC1       
        PO1.newValue(5, 74, 1);    // Start Level    
        PO1.Potec1[7]={0x03}; PO1.Potec1[9]={0x0a}; PO1.Potec1[11]={0x00}; 
        PO2.newValue(12, 71, 1);   // Attack Time
        PO2.Potec[7]={0x03}; PO2.Potec[9]={0x0a}; PO2.Potec[11]={0x01}; 
        PO3.newValue(5, 74, 1);    // Attack Level     
        PO3.Potec1[7]={0x03}; PO3.Potec1[9]={0x0a}; PO3.Potec1[11]={0x02}; 
        PO4.newValue(12, 71, 1);   // Decay Time
        PO4.Potec[7]={0x03}; PO4.Potec[9]={0x0a}; PO4.Potec[11]={0x03}; 
        PO5.newValue(5, 74, 1);    // Decay Level     
        PO5.Potec1[7]={0x03}; PO5.Potec1[9]={0x0a}; PO5.Potec1[11]={0x04}; 
        PO6.newValue(12, 71, 1);   // Slope Time
        PO6.Potec[7]={0x03}; PO6.Potec[9]={0x0a}; PO6.Potec[11]={0x05}; 
        PO7.newValue(5, 74, 1);    // Sustain Level   
        PO7.Potec1[7]={0x03}; PO7.Potec1[9]={0x0a}; PO7.Potec1[11]={0x06}; 
        PO8.newValue(12, 71, 1);   // Release Time
        PO8.Potec[7]={0x03}; PO8.Potec[9]={0x0a}; PO8.Potec[11]={0x07};
        break; 
   case 4455: //banco F7 ------------------- Filter envelope OSC2       
        PO1.newValue(5, 74, 1);   // Start Level    
        PO1.Potec1[7]={0x03}; PO1.Potec1[9]={0x1a}; PO1.Potec1[11]={0x00}; 
        PO2.newValue(12, 71, 1);  // Attack Time
        PO2.Potec[7]={0x03}; PO2.Potec[9]={0x1a}; PO2.Potec[11]={0x01}; 
        PO3.newValue(5, 74, 1);   // Attack Level     
        PO3.Potec1[7]={0x03}; PO3.Potec1[9]={0x1a}; PO3.Potec1[11]={0x02}; 
        PO4.newValue(12, 71, 1);  // Decay Time
        PO4.Potec[7]={0x03}; PO4.Potec[9]={0x1a}; PO4.Potec[11]={0x03}; 
        PO5.newValue(5, 74, 1);   // Decay Level     
        PO5.Potec1[7]={0x03}; PO5.Potec1[9]={0x1a}; PO5.Potec1[11]={0x04}; 
        PO6.newValue(12, 71, 1);  // Slope Time
        PO6.Potec[7]={0x03}; PO6.Potec[9]={0x1a}; PO6.Potec[11]={0x05}; 
        PO7.newValue(5, 74, 1);   // Sustain level   
        PO7.Potec1[7]={0x03}; PO7.Potec1[9]={0x1a}; PO7.Potec1[11]={0x06}; 
        PO8.newValue(12, 71, 1);  // Release Time
        PO8.Potec[7]={0x03}; PO8.Potec[9]={0x1a}; PO8.Potec[11]={0x07};
        break;
   case 4456: //bancoB F8 ------------------- FXs --------------------------//        
        PO1.newValue(0, 93, 1);
        PO2.newValue(0, 91, 1);
        PO3.newValue(6, 92, 1);
        PO3.Potec[7]={0x02}; PO3.Potec[9]={0x12}; PO3.Potec[11]={0x00};
        PO4.newValue(8, 93, 1);
        PO4.Potec[7]={0x02}; PO4.Potec[9]={0x10}; PO4.Potec[11]={0x04};    
        PO5.newValue(1, 91, 1);
        PO5.Potec[7]={0x02}; PO5.Potec[9]={0x10}; PO5.Potec[11]={0x05};
        PO6.newValue(0, 16, 1);
//        PO6.Potec[7]={0x02};PO6.Potec[9]={0x12}; PO6.Potec[11]={0x01};        
        PO7.newValue(0, 12, 1);
//        PO7.Potec[7]={0x02};PO7.Potec[9]={0x12}; PO7.Potec[11]={0x03};        
        PO8.newValue(0, 13, 1);
//        PO8.Potec[7]={0x02};PO8.Potec[9]={0x12}; PO8.Potec[11]={0x04};           
        break; 
   case 4457: // F9 ------------------- LFOs ------------------------------//
        PO1.newValue(14, 74, 1);   //LFO1 freq OSC1 y 2      
        PO1.Potec[7]={0x03}; PO1.Potec[11]={0x03}; 
        PO2.newValue(15, 74, 1);   //LFO2 freq OSC1 y 2      
        PO2.Potec[7]={0x03}; PO2.Potec[11]={0x03}; 
        PO3.newValue(24, 74, 1);   //LFO1 Waveform OSC1 y 2 
        PO3.Potec[7]={0x03}; PO3.Potec[11]={0x00}; 
        PO4.newValue(25, 71, 1);   //LFO2 Waveform OSC1 y 2
        PO4.Potec[7]={0x03}; PO4.Potec[11]={0x00}; 
        PO5.newValue(26, 74, 1);   //LFO1 Shape OSC1 y 2      
        PO5.Potec1[7]={0x03}; PO5.Potec1[11]={0x05}; 
        PO6.newValue(27, 71, 1);   //LFO2 Shape OSC1 y 2 
        PO6.Potec1[7]={0x03}; PO6.Potec1[11]={0x05}; 
        PO7.newValue(0, 76, 1);    // Global CC LFO Speed   
//        PO7.Potec[7]={0x03}; PO7.Potec[9]={0x0d}; PO7.Potec[11]={0x06}; 
        PO8.newValue(0, 77, 1);    // Global CC LFO Depth
//        PO8.Potec[7]={0x03}; PO8.Potec[9]={0x0d}; PO8.Potec[11]={0x07};
        
        break; 
   case 4458: //banco F10 ------------------ Amp Envelope OSC1       
        PO1.newValue(12, 74, 1);   // Start Level    
        PO1.Potec[7]={0x03}; PO1.Potec[9]={0x0d}; PO1.Potec[11]={0x00}; 
        PO2.newValue(12, 71, 1);   // Attack Time
        PO2.Potec[7]={0x03}; PO2.Potec[9]={0x0d}; PO2.Potec[11]={0x01}; 
        PO3.newValue(12, 74, 1);   // Attack Level     
        PO3.Potec[7]={0x03}; PO3.Potec[9]={0x0d}; PO3.Potec[11]={0x02}; 
        PO4.newValue(12, 71, 1);   // Decay Time
        PO4.Potec[7]={0x03}; PO4.Potec[9]={0x0d}; PO4.Potec[11]={0x03}; 
        PO5.newValue(12, 74, 1);   // Decay Level     
        PO5.Potec[7]={0x03}; PO5.Potec[9]={0x0d}; PO5.Potec[11]={0x04}; 
        PO6.newValue(12, 71, 1);   // Slope Time
        PO6.Potec[7]={0x03}; PO6.Potec[9]={0x0d}; PO6.Potec[11]={0x05}; 
        PO7.newValue(12, 74, 1);   // Release Time    
        PO7.Potec[7]={0x03}; PO7.Potec[9]={0x0d}; PO7.Potec[11]={0x06}; 
        PO8.newValue(12, 71, 1);   // Release level 
        PO8.Potec[7]={0x03}; PO8.Potec[9]={0x0d}; PO8.Potec[11]={0x07};
        break;   
        

   case 4459: //banco F11 ----------------- Amp Envelope OSC2     
        PO1.newValue(12, 74, 1);   // Start Level    
        PO1.Potec[7]={0x03}; PO1.Potec[9]={0x1d}; PO1.Potec[11]={0x00}; 
        PO2.newValue(12, 71, 1);   // Attack Time
        PO2.Potec[7]={0x03}; PO2.Potec[9]={0x1d}; PO2.Potec[11]={0x01}; 
        PO3.newValue(12, 74, 1);   // Attack Level     
        PO3.Potec[7]={0x03}; PO3.Potec[9]={0x1d}; PO3.Potec[11]={0x02}; 
        PO4.newValue(12, 71, 1);   // Decay Time
        PO4.Potec[7]={0x03}; PO4.Potec[9]={0x1d}; PO4.Potec[11]={0x03}; 
        PO5.newValue(12, 74, 1);   // Decay Level     
        PO5.Potec[7]={0x03}; PO5.Potec[9]={0x1d}; PO5.Potec[11]={0x04}; 
        PO6.newValue(12, 71, 1);   // Slope Time
        PO6.Potec[7]={0x03}; PO6.Potec[9]={0x1d}; PO6.Potec[11]={0x05}; 
        PO7.newValue(12, 74, 1);   // Release Time    
        PO7.Potec[7]={0x03}; PO7.Potec[9]={0x1d}; PO7.Potec[11]={0x06}; 
        PO8.newValue(12, 71, 1);   // Release level 
        PO8.Potec[7]={0x03}; PO8.Potec[9]={0x1d}; PO8.Potec[11]={0x07};
        break;   
   case 4460: //banco F12 ----------------- Waves OSC1 y OSC2 -------------------------//  
        PO1.newValue(29, 74, 1);   // DWGS OSC1   
        PO1.Potec2[7]={0x03}; PO1.Potec2[9]={0x03}; PO1.Potec2[11]={0x02}; 
        PO2.newValue(29, 71, 1);   // DWGS OSC2 
        PO2.Potec2[7]={0x03}; PO2.Potec2[9]={0x13}; PO2.Potec2[11]={0x02}; 
        PO3.newValue(30, 74, 1);   // Organ OSC1    
        PO3.Potec2[7]={0x03}; PO3.Potec2[9]={0x03}; PO3.Potec2[11]={0x02}; 
        PO4.newValue(30, 71, 1);   // Organ OSC2
        PO4.Potec2[7]={0x03}; PO4.Potec2[9]={0x13}; PO4.Potec2[11]={0x02}; 
        PO5.newValue(31, 74, 1);   // Sostenidas OSC1    
        PO5.Potec2[7]={0x03}; PO5.Potec2[9]={0x03}; PO5.Potec2[11]={0x02}; 
        PO6.newValue(31, 71, 1);   // Sostenidas OSC2
        PO6.Potec2[7]={0x03}; PO6.Potec2[9]={0x13}; PO6.Potec2[11]={0x02}; 
        PO7.newValue(37, 74, 1);   // Waves basic OSC1    
        PO7.Potec2[7]={0x03}; PO7.Potec2[9]={0x03}; PO7.Potec2[11]={0x02}; 
        PO8.newValue(37, 71, 1);   // Waves basic OSC2  
        PO8.Potec2[7]={0x03}; PO8.Potec2[9]={0x13}; PO8.Potec2[11]={0x02};        
        break;  
        
   case 4496: //banco Despertar ----------------- Waves OSC1 y OSC2 -------------------------//  
        PO1.newValue(33, 74, 1);   // percusion OSC1   
        PO1.Potec2[7]={0x03}; PO1.Potec2[9]={0x03}; PO1.Potec2[11]={0x02}; 
        PO2.newValue(33, 71, 1);   // percusion OSC2 
        PO2.Potec2[7]={0x03}; PO2.Potec2[9]={0x13}; PO2.Potec2[11]={0x02}; 
        PO3.newValue(34, 74, 1);   // Bells OSC1    
        PO3.Potec2[7]={0x03}; PO3.Potec2[9]={0x03}; PO3.Potec2[11]={0x02}; 
        PO4.newValue(34, 71, 1);   // Bells OSC2
        PO4.Potec2[7]={0x03}; PO4.Potec2[9]={0x13}; PO4.Potec2[11]={0x02}; 
        PO5.newValue(35, 74, 1);   // Synth 1 OSC1    
        PO5.Potec2[7]={0x03}; PO5.Potec2[9]={0x03}; PO5.Potec2[11]={0x02}; 
        PO6.newValue(35, 71, 1);   // Synth 1 OSC2
        PO6.Potec2[7]={0x03}; PO6.Potec2[9]={0x13}; PO6.Potec2[11]={0x02}; 
        PO7.newValue(36, 74, 1);   // Hits OSC1    
        PO7.Potec2[7]={0x03}; PO7.Potec2[9]={0x03}; PO7.Potec2[11]={0x02}; 
        PO8.newValue(36, 71, 1);   // Hits OSC2  
        PO8.Potec2[7]={0x03}; PO8.Potec2[9]={0x13}; PO8.Potec2[11]={0x02};        
        break;  
    case 4493: //banco Dormir ----------------- Waves OSC1 y OSC2 -------------------------//  
        PO1.newValue(1, 74, 1);    // wave level MS1 OSC1   
        PO1.Potec[7]={0x03}; PO1.Potec[9]={0x03}; PO1.Potec[11]={0x03}; 
        PO2.newValue(1, 71, 1);    // wave level MS1 OSC2 
        PO2.Potec[7]={0x03}; PO2.Potec[9]={0x13}; PO2.Potec[11]={0x03}; 
        PO3.newValue(32, 74, 1);   // percusivas y sostenidas OSC1     
        PO3.Potec2[7]={0x03}; PO3.Potec2[9]={0x03}; PO3.Potec2[11]={0x02}; 
        PO4.newValue(32, 71, 1);   // percusivas y sostenidas OSC2 
        PO4.Potec2[7]={0x03}; PO4.Potec2[9]={0x13}; PO4.Potec2[11]={0x02}; 
        PO5.newValue(41, 74, 1);   // Waves Guit-Bass OSC1    
        PO5.Potec2[7]={0x03}; PO5.Potec2[9]={0x03}; PO5.Potec2[11]={0x02}; 
        PO6.newValue(41, 71, 1);   // Waves Guit-Bass OSC2
        PO6.Potec2[7]={0x03}; PO6.Potec2[9]={0x13}; PO6.Potec2[11]={0x02}; 
        PO7.newValue(42, 74, 1);   // Waves Pluk-SynthBass   
        PO7.Potec2[7]={0x03}; PO7.Potec2[9]={0x03}; PO7.Potec2[11]={0x02}; 
        PO8.newValue(42, 71, 1);   // Waves Pluk-SynthBass 
        PO8.Potec2[7]={0x03}; PO8.Potec2[9]={0x13}; PO8.Potec2[11]={0x02};                
        break;  
              
   case 20761: //Waves KG2 M OSC1  ----------------- Shift key + insert -------------------------//  
        OSC1def(0x01, 0x06); 
        OSC1def(0x04, 0x00); 
        OSC1def(0x02, 0x00); 
        OSC1def(0x05, 0x00); 
        PO7.newValue(38, 74, 1);   // KG2 M OSC1    
        PO7.Potec[7]={0x03}; PO7.Potec[9]={0x03}; PO7.Potec[11]={0x02}; 
        break; 
   case 20762: //Waves KG2 M OSC2  ----------------- Shift key + supr -------------------------//  
        OSC2def(0x01, 0x06); 
        OSC2def(0x04, 0x00); 
        OSC2def(0x02, 0x00); 
        OSC2def(0x05, 0x00); 
        PO8.newValue(38, 74, 1);   // KG2 M OSC2    
        PO8.Potec[7]={0x03}; PO8.Potec[9]={0x13}; PO8.Potec[11]={0x02}; 
        break; 

   case 20753: //Waves KG4 M OSC1  ----------------- Shift key + inicio -------------------------//  
        OSC1def(0x01, 0x0a); 
        OSC1def(0x04, 0x00); 
        OSC1def(0x02, 0x00); 
        OSC1def(0x05, 0x00); 
        PO7.newValue(39, 74, 1);   // KG4 M OSC1    
        PO7.Potec[7]={0x03}; PO7.Potec[9]={0x03}; PO7.Potec[11]={0x02}; 
        break; 
   case 20754: //Waves KG4 M OSC2  ----------------- Shift key + fin -------------------------//  
        OSC2def(0x01, 0x0a); 
        OSC2def(0x04, 0x00); 
        OSC2def(0x02, 0x00); 
        OSC2def(0x05, 0x00); 
        PO8.newValue(39, 74, 1);   // KG4 M OSC2    
        PO8.Potec[7]={0x03}; PO8.Potec[9]={0x13}; PO8.Potec[11]={0x02}; 
        break;  

   case 20755: //Waves KG4 S OSC1  ----------------- Shift key + RePag -------------------------//  
        OSC1def(0x01, 0x0b); 
        OSC1def(0x04, 0x00); 
        OSC1def(0x02, 0x00); 
        OSC1def(0x05, 0x00); 
        PO7.newValue(40, 74, 1);   // KG4 S OSC1    
        PO7.Potec[7]={0x03}; PO7.Potec[9]={0x03}; PO7.Potec[11]={0x02}; 
        break; 
   case 20756: //Waves KG4 S OSC2  ----------------- Shift key + AvPag -------------------------//  
        OSC2def(0x01, 0x0b); 
        OSC2def(0x04, 0x00); 
        OSC2def(0x02, 0x00); 
        OSC2def(0x05, 0x00); 
        PO8.newValue(40, 74, 1);   // KG4 S OSC2    
        PO8.Potec[7]={0x03}; PO8.Potec[9]={0x13}; PO8.Potec[11]={0x02}; 
        break;  

   case 4492: //banco Apagar ------ Waves MS2 OSC1 y OSC2 ------------------------------------------//  
        PO1.newValue(1, 74, 1);   // wave level MS2 OSC1   
        PO1.Potec[7]={0x03}; PO1.Potec[9]={0x03}; PO1.Potec[11]={0x09}; 
        PO2.newValue(1, 71, 1);   // wave level MS2 OSC2 
        PO2.Potec[7]={0x03}; PO2.Potec[9]={0x13}; PO2.Potec[11]={0x09}; 
        PO3.newValue(30, 74, 1);   // Organ Waves MS2 OSC1    
        PO3.Potec2[7]={0x03}; PO3.Potec2[9]={0x03}; PO3.Potec2[11]={0x08}; 
        PO4.newValue(30, 71, 1);   // Organ Waves MS2 OSC2
        PO4.Potec2[7]={0x03}; PO4.Potec2[9]={0x13}; PO4.Potec2[11]={0x08}; 
        PO5.newValue(31, 74, 1);   // Waves sostenidas MS2 OSC1    
        PO5.Potec2[7]={0x03}; PO5.Potec2[9]={0x03}; PO5.Potec2[11]={0x08}; 
        PO6.newValue(31, 71, 1);   // Waves sostenidas MS2 OSC2
        PO6.Potec2[7]={0x03}; PO6.Potec2[9]={0x13}; PO6.Potec2[11]={0x08}; 
        PO7.newValue(37, 74, 1);   // Waves basic MS2 OSC1    
        PO7.Potec2[7]={0x03}; PO7.Potec2[9]={0x03}; PO7.Potec2[11]={0x08}; 
        PO8.newValue(37, 71, 1);   // Waves basic MS2 OSC2  
        PO8.Potec2[7]={0x03}; PO8.Potec2[9]={0x13}; PO8.Potec2[11]={0x08};        
        break;  

   case 4889: //Waves Piano - E.Piano OSC1  ----------------- Win key + insert -------------------------//  
        OSC1def(0x01, 0x00); // OSC1 RomM
        OSC1def(0x04, 0x00); 
        OSC1def(0x05, 0x00); 
        PO7.newValue(43, 74, 1);   // OSC1    
        PO7.Potec2[7]={0x03}; PO7.Potec2[9]={0x03}; PO7.Potec2[11]={0x02}; 
        break; 
   case 4890: //Waves Piano - E.Piano OSC2  ----------------- Win key + supr -------------------------//  
        OSC2def(0x01, 0x00); // OSC2 RomM
        OSC2def(0x04, 0x00); 
        OSC2def(0x05, 0x00); 
        PO8.newValue(43, 74, 1);   // OSC2    
        PO8.Potec2[7]={0x03}; PO8.Potec2[9]={0x13}; PO8.Potec2[11]={0x02}; 
        break; 

   case 4881: //Waves Clavis y E.Ps OSC1  -------------------- Win key + inicio -------------------------//  
        OSC1def(0x01, 0x00); // OSC1 RomM
        OSC1def(0x04, 0x00); 
        OSC1def(0x05, 0x00); 
        PO7.newValue(44, 74, 1);   // OSC1    
        PO7.Potec2[7]={0x03}; PO7.Potec2[9]={0x03}; PO7.Potec2[11]={0x02}; 
        break; 
   case 4882: //Waves Clavis y E.Ps OSC2  -------------------- Win key + fin -------------------------//  
        OSC2def(0x01, 0x00); // OSC2 RomM
        OSC2def(0x04, 0x00); 
        OSC2def(0x05, 0x00); 
        PO8.newValue(44, 74, 1);   // OSC2    
        PO8.Potec2[7]={0x03}; PO8.Potec2[9]={0x13}; PO8.Potec2[11]={0x02}; 
        break;  

   case 4883: //Waves Stereo OSC1  ---------------------------- Win key + RePag -------------------------//  
        OSC1def(0x01, 0x01); // OSC1 RomM
        OSC1def(0x04, 0x00); 
        OSC1def(0x02, 0x00); 
        OSC1def(0x05, 0x00); 
        PO7.newValue(45, 74, 1);   // OSC1    
        PO7.Potec2[7]={0x03}; PO7.Potec2[9]={0x03}; PO7.Potec2[11]={0x02}; 
        break; 
   case 4884: //Waves Stereo OSC2  ---------------------------- Win key + AvPag -------------------------//  
        OSC2def(0x01, 0x01); // OSC2 RomS
        OSC2def(0x04, 0x00); 
        OSC2def(0x02, 0x00); 
        OSC2def(0x05, 0x00); 
        PO8.newValue(45, 74, 1);   // OSC2    
        PO8.Potec2[7]={0x03}; PO8.Potec2[9]={0x13}; PO8.Potec2[11]={0x02}; 
        break;  


  }
  updatePots();



}                   
//-------------------------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------------------------//

//---------------------------------------------------------------------------------------------------------------------------------------------------//














