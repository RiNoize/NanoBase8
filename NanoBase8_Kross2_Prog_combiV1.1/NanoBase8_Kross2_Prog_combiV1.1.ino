/***************************************************************************************************************************
  NanoBase8 Kross2 Prog&combi es un sketch Arduino de un Controlador MIDI hecho con un teclado 
  QWERTY PS2 spanish y 8 potenciometros conectados a un Arduino Nano.
  La inspiracion de crear este controlador MIDI fué la necesidad de controlar 
  via CCs y via SysEx los cientos de parametros y funciones que tiene 
  un sintetizador como el Korg Kross 2

  Este sketch se puede modificar para controlar cualquier aparato que tenga interfaz MIDI IN

  ** Hecho por RiNoize ** (2023)
 
   NanoBase8 Kross2 Prog&combi       Version 1.1     Licencia MIT
 ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo

La biblioteca PS2KeyAdvanced se utiliza para leer la entrada del teclado PS2, 
mientras que la biblioteca Arduino MIDI Library se utiliza para generar y enviar mensajes MIDI a través de un puerto MIDI.

La biblioteca PS2KeyAdvanced está bajo la Licencia Pública General de GNU Lesser General Public License v2.1, 
y la biblioteca Arduino MIDI Library está bajo la Licencia MIT.

Para obtener más información sobre las licencias, consulte los archivos "LICENSE.txt" incluidas en cada biblioteca.

También debo nombrar el trabajo y Videos de https://www.notesandvolts.com, 
el principio de este proyecto fué basado en el sketch de controlador midi de sus videos publicados en Youtube,
una serie de videos donde Dave explica con mucha dedicación el protocolo Midi y cómo hacer un controlador basado en Arduino. 
Recomiendo encarecidamente ver sus videos.
*****************************************************************************************************************************/


#include <MIDI.h>
#include "potenciometros.h"
#include "ArraysTables.h"
#include <PS2KeyAdvanced.h>

//------------------------------------------------  Definicion de los pines conectados al teclado PS2-------------------------------------------------------//
#define DATAPIN 2
#define IRQPIN 3

uint16_t c;
uint8_t breaks = 0;
uint8_t repeats = 0;


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
  c = keyboard.read();

  
}

void loop() 
{  
//  MIDI.read();

// Activacion de los codigos del teclado con SWITCH y CASE donde cada CASE pertenece a un número perteneciente a cada tecla y sus combinaciones -----------------//

    // read the next key
    c = keyboard.read();

      switch (c)
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
        if (startStop)//true
        {
          startStop = !startStop;          
//        sendSysExDef(0x02, 0x02, 0x02, 0x00); // startInmediatly  
//        sendSysExDef(0x02, 0x02, 0x04, 0x01); // SyncOn 
        sendSysExDef(0x02, 0x02, 0x0a, 0x01); // start       
        }
        else//false
        {
          startStop = !startStop;         
        sendSysExDef(0x02, 0x02, 0x0a, 0x00); // Stop         
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
        sendSysExDef(0x02, 0x01, 0x08, 0x01); // on Program
        sendSysExDef(0x00, 0x01, 0x08, 0x01); // on Combi
        MIDI.sendControlChange(80, 127, 1);
        }
        else//false
        {
          Boton1 = !Boton1; 
        sendSysExDef(0x02, 0x01, 0x08, 0x00); // off Program
        sendSysExDef(0x00, 0x01, 0x08, 0x00); // off Combi
        MIDI.sendControlChange(80, 1, 1);
        }
        break;

//Button 2 ------------------------------------------ Pause inter ----------------- MODO PROGRAM Y COMBI -----------------------------------------------------------//
   case 6:
        if (Boton2)//true
        {
          Boton2 = !Boton2;
        sendSysExDef(0x02, 0x01, 0x09, 0x01); // on Program
        sendSysExDef(0x00, 0x01, 0x09, 0x01); // on Combi
        MIDI.sendControlChange(81, 127, 1);
        }
        else//false
        {
          Boton2 = !Boton2; 
        sendSysExDef(0x02, 0x01, 0x09, 0x00); // off Program
        sendSysExDef(0x00, 0x01, 0x09, 0x00); // off Combi
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
        sendSysExDef(0x02, 0x12, 0x08, 0x00); // SRC off
        sendSysExDef(0x02, 0x12, 0x07, 0x64); // Reverb105Wet
        sendSysExDef(0x02, 0x12, 0x09, 0x64); // Reverb107Wet
        sendSysExDef(0x02, 0x12, 0x04, 0x64); // Reverb109Wet
        sendSysExDef(0x02, 0x12, 0x06, 0x64); // Reverb113Wet
        sendSysExDef(0x02, 0x12, 0x0b, 0x64); // Reverb120Wet
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
      sendSysExDef(0x03, 0x00, 0x02, 0x00); // Poly
      break;

   case 282: //  Supr    
      sendSysExDef(0x03, 0x00, 0x02, 0x01); // Mono
      break;      

//FX STEP SEQ LR SEND Full Vel --------------------------------------- Control + inicio ------------------------------------------------------------------------//  
   case 8465: //  
//      sendSysExDef(0x02, 0x09, 0x04, 0x01); // StepSeq_LR
      sendSysExDef(0x02, 0x03, 0x01, 0x3f); // StepSeqVelFull
      break;

//DISTORTED  MFX1
   case 8468: // ------------------------------------------------- Control + AvPag ----------------------------------------------------------------------------//
      MIDI.sendControlChange(93, 127, 10);  
      MIDI.sendControlChange(93, 127, 16);  
      MIDI.sendControlChange(93, 0, 1);  
      sendSysExDef(0x02, 0x09, 0x04, 0x01); // ssLR
      sendSysExDef(0x02, 0x08, 0x04, 0x01); // DrumBusLR
      sendSysExDef(0x02, 0x10, 0x00, 0x38); // m1fxS56
      sendSysExDef(0x02, 0x10, 0x02, 0x01); // MFX1 On
      sendSysExDef(0x02, 0x10, 0x04, 0x7f); // MFX1 return 127
      break;  

//FX2 Reverb Hall --------------------------------------- Control + Fin ----------------------------------------------------------------------------------------//  
   case 8466: //  
      sendSysExDef(0x02, 0x10, 0x01, 0x69); // m2fx105
      break;

//Boton RealTime Select en Tempo --------------------------------------- Control + Hoja ---------------------------------------------------------------------------------------//  
   case 8462: //  
      sendSysExDef(0x02, 0x01, 0x14, 0x05); // RealTime Select en Tempo
      break;

// Abierto/Cerrado

//Filtro Cerrado  ------------------- Ctrl + Win -------------- Program y Combi -----//
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

//Filtro Todo Abierto  ------------------- Ctrl + Alt --------- Program y Combi ----------//
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
      sendSysExArp0010
        break;                   
// 7
   case 8247:
      sendSysExArp0026
        break;                   
// 8
   case 8248:
      sendSysExArp0025
        break;                   
// 9
   case 8249:
      sendSysExArp0095
        break;                   
// 0
   case 8240:
      sendSysExArp0118
        break;                   
// ?
   case 8252:
      sendSysExArp0139
        break;                   
// ¡
   case 8287:
      sendSysExArp0346
        break;                   
// }
   case 8284:
      sendSysExArp0350
        break;                   
// Back
   case 8476:
      sendSysExArp0370
        break;                   
// Q
   case 8273:
      sendSysExArp0390
        break;                   
// W
   case 8279:
      sendSysExArp0396
        break;                   
//  E
   case 8261:
      sendSysExArp0405
        break;                   
// R
   case 8274:
      sendSysExArp0488
        break;                   
// T
   case 8276:
      sendSysExArp0473
        break;                   
// Y
   case 8281:
      sendSysExArp0474
        break;      
// U
   case 8277:
      sendSysExArp0478
        break;                   
// I
   case 8265:
      sendSysExArp0482
        break;                   
// O
   case 8271:
      sendSysExArp0487
        break;                   
// P
   case 8272:
      sendSysExArp0488
        break;                   
// [
   case 8285:
      sendSysExArp0489
        break;                   
// ]
   case 8286:
      sendSysExArp0497
        break;                   
// A
   case 8257:
      sendSysExArp0498
        break;                   
// S
   case 8275:
      sendSysExArp0517
        break;                   
// D
   case 8260:
      sendSysExArp0981
        break;                   
// F
   case 8262:
      sendSysExArp0985
        break;                   
// G
   case 8263:
      sendSysExArp0991
        break;                   
// H
   case 8264:
      sendSysExArp1038
        break;                 
// J
   case 8266:
      sendSysExArp1068
        break;                   
// K
   case 8267:
      sendSysExArp1080
        break;                   
// L
   case 8268:
      sendSysExArp0014
        break;                   
// Ñ
   case 8283:
      sendSysExArp0023
        break;      
// {
   case 8250:
      sendSysExArp0077
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

//LATCH ARP ---------------------------------------------------------------------------//
  case 32: // 0
      arpControls(0x07, 0x01); //  onLatch
      break;
  case 42: // .
      arpControls(0x07, 0x00); // offLatch
      break;

//ARP SORT
  case 36: // 
      arpControls(0x06, 0x01); // sortOn
      break;
  case 37: // 
      arpControls(0x06, 0x00); // sortOff
      break;

//ARP KEY SYNC
  case 39: // 
      arpControls(0x08, 0x01); // onKeySync
      break;
  case 40: // 
      arpControls(0x08, 0x00); // offKeySync
      break;

//ARP KEYBOARD
  case 33: // 
      arpControls(0x09, 0x01); // onKeyboard
      break;
  case 34: // 
      arpControls(0x09, 0x00); // offKeyboard
      break;

//ARP ON/OFF
   case 43: // on
      MIDI.sendControlChange(14, 127, 1);
      break;
   case 35: // off
      MIDI.sendControlChange(14, 00, 1);
      break;
            
//ARP  OCTAVE 
   case 47: // /
      arpControls(0x02, 0x00);  // Arp octave 1
      break;
   case 46: // *
      arpControls(0x02, 0x01);  // Arp octave 2
      break;
   case 45: // -
      arpControls(0x02, 0x02);  // Arp octave 3
      break;    

//ARP  RESOLUTION 
   case 41: //  9 
      arpControls(0x01, 0x03);  // Arp Res SemiCorchea
      break;
   case 38: //  6
      arpControls(0x01, 0x07);  // Arp Res Negra
      break;        
   case 44: //  +
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
      sendSysExDef(0x02, 0x02, 0x02, 0x01); // waitKeyboard
      break;
   case 285: // 
      sendSysExDef(0x02, 0x02, 0x02, 0x00); // startInmediat
      sendSysExDef(0x02, 0x02, 0x04, 0x01); // drumsSyncOn    
      break;

//DRUM Patterns A -----------------------------------------------------ALT key ------------------------------------------------------------------------//

// 1
   case 2097:
        sendSysExDrum794
        break;                   
// 2
   case 2098:
        sendSysExDrum745
        break;                   
// 3
   case 2099:
        sendSysExDrum734
        break;                   
// 4
   case 2100:
        sendSysExDrum730
        break;      
// 5
   case 2101:
        sendSysExDrum731
        break;                   
// 6
   case 2102:
        sendSysExDrum382
        break;                   
// 7
   case 2103:
        sendSysExDrum383
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



//oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo//
//ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo//
//ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo MODO COMBI ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo//

//BANK CHANGE------------------------------------------Control key --------------------------------------------------------------------------//
    case 12566:
          MIDI.sendControlChange(0, bankSelect, 1);
        if (bankNum < 6 ) // cantidad de bancos
        {
          bankNum++;
          MIDI.sendControlChange( 32, bankNum, 1);
          MIDI.sendProgramChange(0, 1);
        }
        break;

    case 12565:
          MIDI.sendControlChange(0, bankSelect, 1);
        if (bankNum > 0)
        {
          bankNum--;
          MIDI.sendControlChange( 32, bankNum, 1);
          MIDI.sendProgramChange(0, 1);
        }
        break;       

// Screen -----------------------------------------------------------------------------------------------//
   case 4366:
        defArray9bytes( 0x4e, 0x01);  // Ultima Pantalla ------ Hoja      
        break;
   case 4878:
        defArray9bytes( 0x4e , 0x00);  // Retorno ----- Win + Hoja
        break;

    //Start Stop Drum Track ----------------------------------------------------- Barra espaciadora ---------------------------------------------------------------------------//
    case 4383:
        if (startStop)//true
        {
          startStop = !startStop;
//          sendSysExDef(0x00, 0x05, 0x04, 0x01); // DrumTrkSyncOnCombi    
//          sendSysExDef(0x00, 0x05, 0x02, 0x00); // startInmediat Combi
          sendSysExDef(0x00, 0x05, 0x0a, 0x01); // startButtonCombi    
        }
        else//false
        {
          startStop = !startStop; 
          sendSysExDef(0x00, 0x05, 0x0a, 0x00);  // stopButtonCombi 
        }
        break;  

 //Boton RealTime Select en Tempo --------------------------------------- Control + Hoja ---------------------------------------------------------------------------------------//  
   case 12558: //  
      sendSysExDef(0x00, 0x01, 0x10, 0x05); // RealTime Select en Tempo
      break;       


//FX STEP SEQ LR SEND Full Vel --------------------------------------- Shift + Hoja --------------------//  
   case 20750: //  
//      sendSysExDef(0x02, 0x18, 0x2b, 0x01); // StepSeq_LR
      sendSysExDef(0x00, 0x06, 0x01, 0x3f); // StepSeqVelFull
      break;       
        
 //DRUM TRIGGER MODE 
   case 4160: // 
      sendSysExDef(0x00, 0x05, 0x02, 0x01); // waitKeyboard Combi
      break;
   case 4381: // 
      sendSysExDef(0x00, 0x05, 0x02, 0x00); // startInmediat Combi
      sendSysExDef(0x00, 0x05, 0x04, 0x01); // drumsSyncOn  Combi   
      break;                               
/*
//Boton 1 ------------------------------------------------------- Boton 1 y Boton 2 comparten codigo con modo Program -----------------------------------------------//
    case 4356:
        if (Boton1)//true
        {
          Boton1 = !Boton1;
        sendSysExDef(0x00, 0x01, 0x08, 0x01);  // onButton1Combi
        MIDI.sendControlChange(80, 127, 1); 
        }
        else//false
        {
          Boton1 = !Boton1; 
        sendSysExDef(0x00, 0x01, 0x08, 0x00);  // offButton1Combi
        MIDI.sendControlChange(80, 1, 1);
        }
        break;

//Boton 2
    case 6:
        if (Boton2)//true
        {
          Boton2 = !Boton2;
        defarray[7]={0x00}; defarray[9]={0x01}; defarray[11]={0x09}; defarray[16]={0x01};  // on
        MIDI.sendSysEx(18, defarray, true);
        MIDI.sendControlChange(81, 127, 1);
        }
        else//false
        {
          Boton2 = !Boton2; 
        defarray[7]={0x00}; defarray[9]={0x01}; defarray[11]={0x09}; defarray[16]={0x00};  // off
        MIDI.sendSysEx(18, defarray, true);
        MIDI.sendControlChange(81, 1, 1);
        }
        break;  */

//DRUM NOTES --------------------------------------------------------------------------------------------------------------------------------------------------//
//Play Notes Kit 1 Channel 12
  case 4496: //  Despetar
    Trk12Controls(0x04, 0x0b);  // Ch12Midi12
    Trk12Controls(0x21, 0x00);  // ch12Dplay
    Trk12DrumKit(0x05, 0x2a);   // ch12DkitBasic1
    Trk12Controls(0x2b, 0x01);  // ch12LR
    Trk12Controls(0x02, 0x7f);  // ch12vol127
        break;
//Play Notes Kit 2 Channel 12
  case 4493: // Dormir
    Trk12Controls(0x04, 0x0b);  // Ch12Midi12
    Trk12Controls(0x21, 0x00);  // ch12Dplay
    Trk12DrumKit(0x05, 0x21);   // ch12DkitRock1
    Trk12Controls(0x2b, 0x01);  // ch12LR
    Trk12Controls(0x02, 0x7f);  // ch12vol127
        break;   
//Play Notes Kit 3 Channel 12
  case 4492: // Apagar
    Trk12Controls(0x04, 0x0b);  // Ch12Midi12
    Trk12Controls(0x21, 0x00);  // ch12Dplay
    Trk12DrumKit(0x09, 0x6a);   // ch12 User 490
    Trk12Controls(0x2b, 0x01);  // ch12LR
    Trk12Controls(0x02, 0x7f);  // ch12vol127
        break;    


// MIDI NOTES----------------------------------------------------------------------------------//
   case 4377:
     MIDI.sendNoteOn(60, 120, 12);    // Send a Note (pitch 61, velo 120 on channel 12)
     delay(100);               
     MIDI.sendNoteOff(60, 0, 12);     // Note off
     break; 
   case 4369:
     MIDI.sendNoteOn(57, 120, 12);    
     delay(100);               
     MIDI.sendNoteOff(57, 0, 12);         
     break; 
   case 4371:
     MIDI.sendNoteOn(53, 120, 12);   
     delay(100);               
     MIDI.sendNoteOff(53, 0, 12);     
     break;
   case 4378:
     MIDI.sendNoteOn(69, 120, 12);    
     delay(100);               
     MIDI.sendNoteOff(69, 0, 12);     
     break;
   case 4370:
     MIDI.sendNoteOn(61, 120, 12);    
     delay(100);               
     MIDI.sendNoteOff(61, 0, 12);     
     break;
   case 4372:
     MIDI.sendNoteOn(51, 127, 12);    
     delay(100);               
     MIDI.sendNoteOff(51, 0, 12);     
     break;






// ARPs  A y B ------------- Control -----------------------------------------------------------------------------------------------------------------------------------//

//  1
   case 12337:
      sendSysExArpAUp
        break;                   
// 2
   case 12338:
      sendSysExArpADown
        break;                   
// 3
   case 12339:
      sendSysExArpAAlt1
        break;                   
// 4
   case 12340:
      sendSysExArpAAlt2
        break;      
// 5
   case 12341:
      sendSysExArpARndm
        break;                   
// 6
   case 12342:
      sendSysExArpA0002
        break;                   
// 7
   case 12343:
      sendSysExArpA0003
        break;                   
// 8
   case 12344:
      sendSysExArpA0004
        break;                   
// 9
   case 12345:
      sendSysExArpA0005
        break;                   
// 0
   case 12336:
      sendSysExArpA0006
        break;                   
// ?
   case 12348:
      sendSysExArpA0009
        break;                   
// ¡
   case 12383:
      sendSysExArpA0010
        break;                   
// }
   case 12380:
      sendSysExArpA0012
        break;                   
// Back
   case 12572:
      sendSysExArpA0013
        break;                   
// Q
   case 12369:
      sendSysExArpA0014
        break;                   
// W
   case 12375:
      sendSysExArpA0015
        break;                   
//  E
   case 12357:
      sendSysExArpA0023
        break;                   
// R
   case 12370:
      sendSysExArpA0028
        break;                   
// T
   case 12372:
      sendSysExArpA0077
        break;                   
// Y
   case 12377:
      sendSysExArpA0035
        break;      
// U
   case 12373:
      sendSysExArpA0037
        break;                   
// I
   case 12361:
      sendSysExArpA0071
        break;                   
// O
   case 12367:
      sendSysExArpA0072
        break;                   
// P
   case 12368:
      sendSysExArpA0032
        break;                   
// [
   case 12381:
      sendSysExArpA0018
        break;                   
// ]
   case 12382:
      sendSysExArpA0024
        break;                   
// A
   case 12353:
      sendSysExArpBUp
        break;                   
// S
   case 12371:
      sendSysExArpBDown
        break;                   
// D
   case 12356:
      sendSysExArpBAlt1
        break;                   
// F
   case 12358:
      sendSysExArpBAlt2
        break;                   
// G
   case 12359:
      sendSysExArpBRndm
        break;                   
// H
   case 12360:
      sendSysExArpB0002
        break;                 
// J
   case 12362:
      sendSysExArpB0003
        break;                   
// K
   case 12363:
      sendSysExArpB0004
        break;                   
// L
   case 12364:
      sendSysExArpB0005
        break;                   
// Ñ
   case 12379:
      sendSysExArpB0006
        break;      
// {
   case 12346:
      sendSysExArpB0009
        break;                   
// <>
   case 12427:
      sendSysExArpB0010
        break;                   
// Z
   case 12378:
      sendSysExArpB0012
        break;                   
// X
   case 12376:
      sendSysExArpB0013
        break;                   
// C
   case 12355:
      sendSysExArpB0014
        break;                   
// V
   case 12374:
      sendSysExArpB0015
        break;                   
// B
   case 12354:
      sendSysExArpB0023
        break;                   
// N
   case 12366:
      sendSysExArpB0028
        break;                   
// M
   case 12365:
      sendSysExArpB0077
        break;                   
// ,
   case 12347:
      sendSysExArpB0035
        break;                   
// .
   case 12349:
      sendSysExArpB0037
        break;                   
// _
   case 12350:
      sendSysExArpB0071
        break; 

// ARP On/Off CC control --------------------------------------------------- Shift ----------------------------------------------------------------//
   case 4354:
        MIDI.sendControlChange(14, 127, 1); // Arp On
        break; 
   case 37122:
        MIDI.sendControlChange(14, 01, 1);  // Arp Off
        break; 


// ARP A ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//ARP A ON/OFF 
   case 4139: // Intro
      ArpAcontrols(0x0a, 0x01);  // Arp A on
      break;
   case 4131: // 3
      ArpAcontrols(0x0a, 0x00);  // Arp A Off
      break;                        

//ARP A SORT 
   case 4135: // 7
      ArpAcontrols(0x06, 0x01);  // Arp A Sort On
      break;
   case 4136: // 8
      ArpAcontrols(0x06, 0x00);  // Arp A Sort Off
      break;
      
//LATCH A ARP 
   case 4128: // 0
      ArpAcontrols(0x07, 0x01);  // Arp A Latch on
      break;
   case 4138: // Supr 
      ArpAcontrols(0x07, 0x00);  // Arp A Latch Off
      break;
      
//ARP A KEY SYNC 
   case 4132: // 4
      ArpAcontrols(0x08, 0x01);  // Arp A KeySync On
      break;
   case 4133: // 5
      ArpAcontrols(0x08, 0x00);  // Arp A KeySync Off
      break;

//ARP A KEYBOARD 
   case 4129: // 1
      ArpAcontrols(0x09, 0x01);  // Arp A Keyboard On
      break;
   case 4130: // 2
      ArpAcontrols(0x09, 0x00);  // Arp A Keyboard Off
      break;
            
//ARP A  OCTAVE 
   case 4143: // /
      ArpAcontrols(0x02, 0x00);  // Arp A octave 1
      break;
   case 4142: // *
      ArpAcontrols(0x02, 0x01);  // Arp A octave 2
      break;
   case 4141: // -
      ArpAcontrols(0x02, 0x02);  // Arp A octave 3
      break;    

//ARP A  RESOLUTION 
   case 4137: //  9 
      ArpAcontrols(0x01, 0x03);  // Arp A Res Semicorchea
      break;
   case 4134: //  6
      ArpAcontrols(0x01, 0x07);  // Arp A Res Negra
      break;        
   case 4140: //  +
      ArpAcontrols(0x01, 0x05);  // Arp A Res Corchea
      break;

// ARP B ---------------------------------- CONTROL -----------------------------------------------------------------------------------------------------------------------------------------//
//ARP B ON/OFF 
   case 12331: // Intro
      ArpBcontrols(0x0a, 0x01);  // Arp B on
      break;
   case 12323: // 3
      ArpBcontrols(0x0a, 0x00);  // Arp B Off
      break;                        

//ARP B SORT 
   case 12327: // 7
      ArpBcontrols(0x06, 0x01);  // Arp B Sort On
      break;
   case 12328: // 8
      ArpBcontrols(0x06, 0x00);  // Arp B Sort Off
      break;
      
//LATCH B ARP 
   case 12320: // 0
      ArpBcontrols(0x07, 0x01);  // Arp B Latch on
      break;
   case 12330: // Supr 
      ArpBcontrols(0x07, 0x00);  // Arp B Latch Off
      break;
      
//ARP B KEY SYNC 
   case 12324: // 4
      ArpBcontrols(0x08, 0x01);  // Arp B KeySync On
      break;
   case 12325: // 5
      ArpBcontrols(0x08, 0x00);  // Arp B KeySync Off
      break;

//ARP B KEYBOARD 
   case 12321: // 1
      ArpBcontrols(0x09, 0x01);  // Arp B Keyboard On
      break;
   case 12322: // 2
      ArpBcontrols(0x09, 0x00);  // Arp B Keyboard Off
      break;
            
//ARP B  OCTAVE 
   case 12335: // /
      ArpBcontrols(0x02, 0x00);  // Arp B octave 1
      break;
   case 12334: // *
      ArpBcontrols(0x02, 0x01);  // Arp B octave 2
      break;
   case 12333: // -
      ArpBcontrols(0x02, 0x02);  // Arp B octave 3
      break;    

//ARP B  RESOLUTION 
   case 12329: //  9 
      ArpBcontrols(0x01, 0x03);  // Arp B Res Semicorchea
      break;
   case 12326: //  6
      ArpBcontrols(0x01, 0x07);  // Arp B Res Negra
      break;        
   case 12332: //  +
      ArpBcontrols(0x01, 0x05);  // Arp B Res Corchea
      break;



//SHIFT DRUMS------------------------------------DEF----------------------------------------------------------------------------------------------------------------------------------------//
/*
//ShiftDrum-24
  case 4145: // 1
    ShiftDrumsT924C
    break;
//ShiftDrum-23
  case 4146: // 2
    ShiftDrumsT923C
    break;
//ShiftDrum-22
  case 4147: // 3
    ShiftDrumsT922C
    break;
//ShiftDrum-21
  case 4148: // 4
    ShiftDrumsT921C
    break;
//ShiftDrum-20
  case 4149: // 5
    ShiftDrumsT920C
    break;
//ShiftDrum-19
  case 4150: // 6
    ShiftDrumsT919C
    break;
//ShiftDrum-18
  case 4151: // 7
    ShiftDrumsT918C
    break;
//ShiftDrum-17
  case 4152: // 8
    ShiftDrumsT917C
    break;
//ShiftDrum-16
  case 4153: // 9 
    ShiftDrumsT916C
    break;
//ShiftDrum-15
  case 4144: // 0
    ShiftDrumsT915C
    break;
*/    
//ShiftDrum-14
  case 4156: // ?
    ShiftDrumsT914C
    break;
//ShiftDrum-13
  case 4191: // ¿
    ShiftDrumsT913C
    break;
//ShiftDrum-12
  case 4188: // }
    ShiftDrumsT912C
    break;
//ShiftDrum-11
  case 4380: // Back
    ShiftDrumsT911C
    break;
//ShiftDrum-10
  case 4177: // Q
    ShiftDrumsT910C
    break;
//ShiftDrum-09
  case 4183: // W
    ShiftDrumsT909C
    break;
//ShiftDrum-08
  case 4165: // E
    ShiftDrumsT908C
    break;
//ShiftDrum-07
  case 4178: // R
    ShiftDrumsT907C
    break;
//ShiftDrum-06
  case 4180: // T
    ShiftDrumsT906C
    break;
//ShiftDrum-05
  case 4185: // Y
    ShiftDrumsT905C
    break;
//ShiftDrum-04
  case 4181: // U
    ShiftDrumsT904C
    break;
//ShiftDrum-03
  case 4169: // I
    ShiftDrumsT903C
    break;
//ShiftDrum-02
  case 4175: // O
    ShiftDrumsT902C
    break;
//ShiftDrum-01
  case 4176: // P
    ShiftDrumsT901C
    break;
//ShiftDrum 00
  case 4382: // Enter
    ShiftDrumsT000C
    break;
//ShiftDrum 01
  case 4189: // [
    ShiftDrumsT001C
    break;
//ShiftDrum 02
  case 4190: // ]
    ShiftDrumsT002C
    break;
//ShiftDrum 03
  case 4161: // A
    ShiftDrumsT003C
    break;
//ShiftDrum 04
  case 4179: // S
    ShiftDrumsT004C
    break;
//ShiftDrum 05
  case 4164: // D
    ShiftDrumsT005C
    break;
//ShiftDrum 06
  case 4166: // F
    ShiftDrumsT006C
    break;
//ShiftDrum 07
  case 4167: // G
    ShiftDrumsT007C
    break;
//ShiftDrum 08
  case 4168: // H
    ShiftDrumsT008C
    break;
//ShiftDrum 09
  case 4170: // J
    ShiftDrumsT009C
    break;
//ShiftDrum 10
  case 4171: // K
    ShiftDrumsT010C
    break;
//ShiftDrum 11
  case 4172: // L
    ShiftDrumsT011C
    break;
//ShiftDrum 12
  case 4187: // Ñ
    ShiftDrumsT012C
    break;
//ShiftDrum 13
  case 4154: // {
    ShiftDrumsT013C
    break;
//ShiftDrum 14
  case 4235: // <>
    ShiftDrumsT014C
    break;
//ShiftDrum 15
  case 4186: // Z
    ShiftDrumsT015C
    break;
//ShiftDrum 16
  case 4184: // X
    ShiftDrumsT016C
    break;
//ShiftDrum 17
  case 4163: // C
    ShiftDrumsT017C
    break;
//ShiftDrum 18
  case 4182: // V
    ShiftDrumsT018C
    break;
//ShiftDrum 19
  case 4162: // B
    ShiftDrumsT019C
    break;
//ShiftDrum 20
  case 4174: // N
    ShiftDrumsT020C
    break;
//ShiftDrum 21
  case 4173: // M
    ShiftDrumsT021C
    break;
//ShiftDrum 22
  case 4155: // ,
    ShiftDrumsT022C
    break;
//ShiftDrum 23
  case 4157: // .
    ShiftDrumsT023C
    break;
//ShiftDrum 24
  case 4158: // -
    ShiftDrumsT024C
    break; 

  //Step Seq DRUM KITS Combi ------------------------- Shift ----------------------------------------------------------------------------------------------------------------------//

//ElectronicSSDrumKit 1
   case 20529: // 1
        sendSysExSSdrumKitC015
        break;                   
//ElectronicSSDrumKit 2
   case 20530: // 2
        sendSysExSSdrumKitC016
        break;                   
//ElectronicSSDrumKit 3
   case 20531: // 3
        sendSysExSSdrumKitC032
        break;                   
//ElectronicSSDrumKit 4
   case 20532: // 4
        sendSysExSSdrumKitC033
        break;      
//ElectronicSSDrumKit 5
   case 20533: // 5
        sendSysExSSdrumKitC013
        break;                   
//ElectronicSSDrumKit 6
   case 20534: // 6
        sendSysExSSdrumKitC021
        break;                   
//ElectronicSSDrumKit 7
   case 20535: // 7
        sendSysExSSdrumKitC023
        break;                   
//ElectronicSSDrumKit 8
   case 20536: // 8
        sendSysExSSdrumKitC026
        break;                   
//ElectronicSSDrumKit 9
   case 20537: // 9
        sendSysExSSdrumKitC027
        break;                   
//ElectronicSSDrumKit 10
   case 20528: // 0
        sendSysExSSdrumKitC028
        break;                   
//ElectronicSSDrumKit 11
   case 20540: // ?
        sendSysExSSdrumKitC029
        break;                   
//ElectronicSSDrumKit 12
   case 20575: // ¿
        sendSysExSSdrumKitC030
        break;                   
//ElectronicSSDrumKit 13
   case 20572: // }
        sendSysExSSdrumKitC024
        break;                   
//ElectronicSSDrumKit 14
   case 20764: // Back
        sendSysExSSdrumKitC025
        break;                           
//AcousticSSDrumKit 1
   case 20561: // Q
        sendSysExSSdrumKitC000
        break;                   
//AcousticSSDrumKit 2
   case 20567: // W
        sendSysExSSdrumKitC002
        break;
//AcousticSSDrumKit 3
   case 20549: // E
        sendSysExSSdrumKitC003
        break;
//AcousticSSDrumKit 4
   case 20562: // R
        sendSysExSSdrumKitC005
        break;
//AcousticSSDrumKit 5
   case 20564: // T
        sendSysExSSdrumKitC006
        break;
//AcousticSSDrumKit 6
   case 20569: // Y
        sendSysExSSdrumKitC007
        break;
//AcousticSSDrumKit 7
   case 20565: // U
        sendSysExSSdrumKitC008
        break;
//AcousticSSDrumKit 8
   case 20553: // I
        sendSysExSSdrumKitC009
        break;
//AcousticSSDrumKit 9
   case 20559: // O
        sendSysExSSdrumKitC010
        break;     
//AcousticSSDrumKit 10
   case 20560: // P
        sendSysExSSdrumKitC011
        break;
//AcousticSSDrumKit 11
   case 20573: // 
        sendSysExSSdrumKitC018
        break;
//AcousticSSDrumKit 12
   case 20574: // *
        sendSysExSSdrumKitC019
        break; 

 

 // BUS SELECT Track 8 ---------------------------------------------------Control + Alt key----------------------------------------------------//        

   case 14697: // DKIT---------F9
        ch8DBus(0x00); //
        break;
   case 14698: // L/R---------F10
        ch8DBus(0x01); //
        break;
   case 14699: // IFX1-------F11
        ch8DBus(0x02); //
        break;
   case 14700: // IFX2-------F12
        ch8DBus(0x03); //
        break;
   case 14736: // IFX3------Desp
        ch8DBus(0x04); //
        break;
//   case 5005: // IFX4------Dormir
//        ch8DBus(0x05); //
//        break;
   case 14732: // IFX5------Apagar
        ch8DBus(0x06); //
        break;                        

 // BUS SELECT Track 12 ---------------------------------------------------WIN key----------------------------------------------------//        

   case 4969: // DKIT---------F9
        ch12DBus(0x00); //
        break;
   case 4970: // L/R---------F10
        ch12DBus(0x01); //
        break;
   case 4971: // IFX1-------F11
        ch12DBus(0x02); //
        break;
   case 4972: // IFX2-------F12
        ch12DBus(0x03); //
        break;
   case 5008: // IFX3------Desp
        ch12DBus(0x04); //
        break;
   case 5005: // IFX4------Dormir
        ch12DBus(0x05); //
        break;
   case 5004: // IFX5------Apagar
        ch12DBus(0x06); //
        break;       

 // STEP SEQ BUS SELECT COMBI ------------------- Shift +  F9 to Apagar -------------------------------//        

   case 20841: // DKIT
        SSBusSelectC(0x00); //
        break;
   case 20842: // L/R
        SSBusSelectC(0x01); //
        break;
   case 20843: // IFX1
        SSBusSelectC(0x02); //
        break;
   case 20844: // IFX2
        SSBusSelectC(0x03); //
        break;
   case 20880: // IFX3
        SSBusSelectC(0x04); //
        break;
   case 20877: // IFX4
        SSBusSelectC(0x05); //
        break;
   case 20876: // IFX5
        SSBusSelectC(0x06); //
        break;              

//DRUM Patterns Combi -----------------------------------------------------WIN ----------------------------------------------------------------//


   case 4657://1
        sendSysExDrumC382
        break;                   
   case 4658://2
        sendSysExDrumC383
        break;                   
   case 4659://3
        sendSysExDrumC384
        break;                   
   case 4660://4
        sendSysExDrumC385
        break;      
   case 4661://5
        sendSysExDrumC386
        break;                   
   case 4662://6
        sendSysExDrumC387
        break;                   
   case 4663://7
        sendSysExDrumC388
        break;                   
   case 4664://8
        sendSysExDrumC390
        break;                   
   case 4665://9
        sendSysExDrumC391
        break;                   
   case 4656://0
        sendSysExDrumC730
        break;                   
   case 4668://?
        sendSysExDrumC731
        break;                   
   case 4703://¿
        sendSysExDrumC732
        break;                   
   case 4700://}
        sendSysExDrumC734
        break;                   
   case 4892://Back
        sendSysExDrumC744
        break;                   
   case 4689://Q
        sendSysExDrumC745
        break;                   
   case 4695://W
        sendSysExDrumC791
        break;                   
   case 4677://E
        sendSysExDrumC794
        break;                   
   case 4690://R
        sendSysExDrumC795
        break;                   
   case 4692://T
        sendSysExDrumC402
        break;                   
   case 4697://Y
        sendSysExDrumC403
        break;                   
   case 4693://U
        sendSysExDrumC404
        break;                   
   case 4681://I
        sendSysExDrumC405
        break;                   
   case 4687://O
        sendSysExDrumC406
        break;                   
   case 4688://P
        sendSysExDrumC407
        break;   
   case 4701://
        sendSysExDrumC408
        break;                   
   case 4702://*
        sendSysExDrumC409
        break;                   
   case 4673://A
        sendSysExDrumC480
        break;                   
   case 4691://S
        sendSysExDrumC481
        break;
   case 4676://D
        sendSysExDrumC482
        break;
   case 4678://F
        sendSysExDrumC545
        break;
   case 4679://G
        sendSysExDrumC558
        break;
   case 4680://H
        sendSysExDrumC563
        break;
   case 4682://J
        sendSysExDrumC569
        break;
   case 4683://K
        sendSysExDrumC570
        break;
   case 4684://L
        sendSysExDrumC572
        break;     
   case 4699://Ñ
        sendSysExDrumC628
        break;
   case 4666://{
        sendSysExDrumC422
        break;
   case 4747://<>
        sendSysExDrumC423
        break;
   case 4698://Z
        sendSysExDrumC424
        break;
   case 4696://X
        sendSysExDrumC425
        break;
   case 4675://C
        sendSysExDrumC426
        break;
   case 4694://V
        sendSysExDrumC427
        break;
   case 4674://B
        sendSysExDrumC428
        break;
   case 4686://N
        sendSysExDrumC429
        break;     
   case 4685://M
        sendSysExDrumC430
        break;
   case 4667://;
        sendSysExDrumC431
        break;        
   case 4669://:
        sendSysExDrumC450
        break;
   case 4670://-
        sendSysExDrumC451
        break;        

 //SS BassDrums Combi
   case 20619://<>
        BassDrum01C
        break;
   case 20570://Z
        BassDrum02C
        break;
   case 20568://X
        BassDrum03C
        break;
   case 20547://C
        BassDrum04C
        break;
   case 20566://V
        BassDrum05C
        break;
   case 20546://B
        BassDrum06C
        break;
   case 20558://N
        BassDrum07C
        break; 
   case 20557://M
        BassDrum08C
        break;
   case 20539://;
        BassDrum09C
        break; 
   case 20541://:
        BassDrum10C
        break;
   case 20542://-
        BassDrum11C
        break;        

//SS Snare1Drums Combi
   case 20545://<>
        Snare1Drum01C
        break;
   case 20563://Z
        Snare1Drum02C
        break;
   case 20548://X
        Snare1Drum03C
        break;
   case 20550://C
        Snare1Drum04C
        break;
   case 20551://V
        Snare1Drum05C
        break;
   case 20552://B
        Snare1Drum06C
        break;
   case 20554://N
        Snare1Drum07C
        break;     
   case 20555://M
        Snare1Drum08C
        break;
   case 20556://;
        Snare1Drum09C
        break;   
   case 20571://:
        Snare1Drum10C
        break;
   case 20538://-
        Snare1Drum11C
        break;                     

// 12 Combis ------------------------------------------------------------- Control + F1 a F12 ------ Combi 116 a 127 del ultimo banco USER-----------------------------------------------------------------------// 
   case 12641: // F1
          sendMIDIcombiChange(116);
          break;         
   case 12642: // F2
          sendMIDIcombiChange(117);    
          break;                     
   case 12643: // F3
          sendMIDIcombiChange(118);   
          break;           
   case 12644: // F4
          sendMIDIcombiChange(119);    
          break; 
   case 12645: // F5
          sendMIDIcombiChange(120);
          break;           
   case 12646: // F6
          sendMIDIcombiChange(121); 
          break;                     
   case 12647: // F7
          sendMIDIcombiChange(122);  
          break;           
   case 12648: // F8
          sendMIDIcombiChange(123);
          break; 
   case 12649: // F9
          sendMIDIcombiChange(124);
          break;           
   case 12650: // F10
          sendMIDIcombiChange(125);
          break;                     
   case 12651: // F11
          sendMIDIcombiChange(126); 
          break;           
   case 12652: // F12
          sendMIDIcombiChange(127);
          break;   

// TEMPOS COMBI ----------------------------------- Win Key + teclado numeral ------------------------------------------------------------------------------------------------//
// 0
   case 4640:
      tempoC120
        break;           
//  1
   case 4641: 
      tempoC80 
        break;                   
// 2
   case 4642:
      tempoC90
        break;                   
// 3
   case 4643:
      tempoC100
        break;                   
// 4
   case 4644:
      tempoC110
        break;      
// 5
   case 4645:
      tempoC126
        break;                   
// 6
   case 4646:
      tempoC128
        break;                   
// 7
   case 4647:
      tempoC135
        break;                   
// 8
   case 4648:
      tempoC140
        break;                   
// 9
   case 4649:
      tempoC145
        break;                   

//PLAY/MUTE TRACKs--------------------- ALT y AltGr + Fs Keys -------------------------------------------------------------------------------------//
//Play-Mute Track-------1
  case 6497: // Play F1
    PlayMuteTrks(0x09, 0x00);
    break;
  case 5473: // Mute F1
    PlayMuteTrks(0x09, 0x01);
    break;  
//Play-Mute Track-------2
  case 6498: // Play F2
    PlayMuteTrks(0x0a, 0x00);
    break;
  case 5474: // Mute F2
    PlayMuteTrks(0x0a, 0x01);
    break;
//Play-Mute Track-------3
  case 6499: // Play F3
    PlayMuteTrks(0x0b, 0x00);
    break;
  case 5475: // Mute F3
    PlayMuteTrks(0x0b, 0x01);
    break;
//Play-Mute Track-------4
  case 6500: // Play F4
    PlayMuteTrks(0x0c, 0x00);
    break;
  case 5476: // Mute F4
    PlayMuteTrks(0x0c, 0x01);
    break;
//Play-Mute Track-------5
  case 6501: // Play F5
    PlayMuteTrks(0x0d, 0x00);
    break;
  case 5477: // Mute F5
    PlayMuteTrks(0x0d, 0x01);
    break;
//Play-Mute Track-------6
  case 6502: // Play F6
    PlayMuteTrks(0x0e, 0x00);
    break;
  case 5478: // Mute F6
    PlayMuteTrks(0x0e, 0x01);
    break;
//Play-Mute Track-------7
  case 6503: // Play F7
    PlayMuteTrks(0x0f, 0x00);
    break;
  case 5479: // Mute F7
    PlayMuteTrks(0x0f, 0x01);
    break;
//Play-Mute Track-------8
  case 6504: // Play F8
    PlayMuteTrks(0x10, 0x00);
    break;
  case 5480: // Mute F8
    PlayMuteTrks(0x10, 0x01);
    break;
//Play-Mute Track-------9
  case 6505: // Play F9
    PlayMuteTrks(0x11, 0x00);
    break;
  case 5481: // Mute F9
    PlayMuteTrks(0x11, 0x01);
    break;
//Play-Mute Track-------10
  case 6506: // Play F10
    PlayMuteTrks(0x12, 0x00);
    break;
  case 5482: // Mute F10
    PlayMuteTrks(0x12, 0x01);
    break;
//Play-Mute Track-------11
  case 6507: // Play F11
    PlayMuteTrks(0x13, 0x00);
    break;
  case 5483: // Mute F11
    PlayMuteTrks(0x13, 0x01);
    break;
//Play-Mute Track-------12
  case 6508: // Play F12
    PlayMuteTrks(0x14, 0x00);
    break;
  case 5484: // Mute F12
    PlayMuteTrks(0x14, 0x01);
    break;
//Play-Mute Track-------StepSeq
  case 6427: // Play Esc
    PlayMuteTrks(0x18, 0x00);
    break;
  case 5403: // Mute Esc
    PlayMuteTrks(0x18, 0x01);
    break;

//ARP B   Drum Patterns -----------------------  del 1 al 0  ---------------------------------------------------------------------------------------------------------------------//

//  Establecer ajustes 
   case 5902: // AltGr + Win + Hoja
      sendSysExDef(0x00, 0x10, 0x26, 0x00);  // Track 8 rango from C-1
      sendSysExDef(0x00, 0x10, 0x09, 0x02);  // Track 8 ArpB
      sendSysExDef(0x00, 0x16, 0x09, 0x02);  // Track 14 ArpB      
      sendSysExDef(0x00, 0x10, 0x04, 0x03);  // Track 8 Midi Channel 4
      sendSysExDef(0x00, 0x10, 0x0b, 0x00);  // Track 8 Transpose +00
      sendSysExDef5(0x00, 0x10, 0x00, 0x05, 0x2a);  // Track 8 Basic1 DrumKit
      PlayMuteTrks(0x10, 0x00);  // Play trk 8
      MIDI.sendControlChange(14, 127, 1);
      ArpBcontrols(0x0a, 0x01);  // Arp B on
      ArpBcontrols(0x07, 0x01);  // Arp B Latch on
      ArpBcontrols(0x08, 0x00);  // Arp B KeySync Off
      ArpBcontrols(0x01, 0x03);  // Arp B semicorchea
      ArpBcontrols(0x03, 0x65);  // Arp B Gate: Step
      sendSysExDef5(0x00, 0x04, 0x04, 0x01, 0x01);  // Arp B Vel: Step
      ArpBcontrols(0x05, 0x00);  // Arp B Swing: +000
//      delay(10);
//      ArpBcontrols(0x0c, 0x00);  // Arp B Rango C-1
//      ArpBcontrols(0x0b, 0x3b);  // Arp B Rango B 3

        break;   
//  1
   case 4145: 
      sendSysExArpB1201 
        break;                   
// 2
   case 4146:
      sendSysExArpB1202
        break;                   
// 3
   case 4147:
      sendSysExArpB1203
        break;                   
// 4
   case 4148:
      sendSysExArpB1204
        break;      
// 5
   case 4149:
      sendSysExArpB1205
        break;                   
// 6
   case 4150:
      sendSysExArpB1206
        break;                   
// 7
   case 4151:
      sendSysExArpB1207
        break;                   
// 8
   case 4152:
      sendSysExArpB1208
        break;                   
// 9
   case 4153:
      sendSysExArpA1209
        break;                   
// 0
   case 4144:
      sendSysExArpA1210
        break;                   



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
        PO1.newValue(5, 73, 1);
        PO1.Potec1[7]={0x02};PO1.Potec1[9]={0x15}; PO1.Potec1[11]={0x06};
        PO2.newValue(5, 75, 1);
        PO2.Potec1[7]={0x02};PO2.Potec1[9]={0x15}; PO2.Potec1[11]={0x07};
        PO3.newValue(5, 70, 1);
        PO3.Potec1[7]={0x02};PO3.Potec1[9]={0x15}; PO3.Potec1[11]={0x08};
        PO4.newValue(5, 72, 1);
        PO4.Potec1[7]={0x02};PO4.Potec1[9]={0x15}; PO4.Potec1[11]={0x09};
        PO5.newValue(5, 79, 1);
        PO5.Potec1[7]={0x02};PO5.Potec1[9]={0x15}; PO5.Potec1[11]={0x04};
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
    case 357: //bank F5  FX         MFX1send, MFX2send, Reverb Time, MFX1 master, MFX2 master  
        PO1.newValue(0, 93, 1);
        PO2.newValue(0, 91, 1);
        PO3.newValue(6, 92, 1);
        //{0xf0, 0x42, 0x30, 0x00, 0x01, 0x49, 0x41, 0x02, 0x00, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf7};
        PO3.Potec[7]={0x02};PO3.Potec[9]={0x12}; PO3.Potec[11]={0x00};
        PO4.newValue(8, 93, 1);
        //{0xf0, 0x42, 0x30, 0x00, 0x01, 0x49, 0x41, 0x02, 0x00, 0x10, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf7};
        PO4.Potec[7]={0x02}; PO4.Potec[9]={0x10}; PO4.Potec[11]={0x04};    
        PO5.newValue(1, 91, 1);
        //{0xf0, 0x42, 0x30, 0x00, 0x01, 0x49, 0x41, 0x02, 0x00, 0x10, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf7};
        PO5.Potec[7]={0x02}; PO5.Potec[9]={0x10}; PO5.Potec[11]={0x05};
        PO6.newValue(0, 7, 1);
        PO7.newValue(0, 7, 10);
        PO8.newValue(0, 7, 16);
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
    case 361: //bank F9           CC: 12, 13, 16, 17, 19 
        PO1.newValue(0, 12, 1);
        PO2.newValue(0, 13, 1);
        PO3.newValue(0, 16, 1);
        PO4.newValue(0, 17, 1);
        PO5.newValue(0, 19, 1);
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
          sendSysExDef(0x03, 0x00, 0x09, 0x00); // MonoUnisonOFF
        }
        else//false
        {
          MonoUnison = !MonoUnison; 
        
          sendSysExDef(0x03, 0x00, 0x09, 0x01); // MonoUnisonON     
          sendSysExDef(0x03, 0x00, 0x08, 0x04); // voices4
          sendSysExDef(0x03, 0x00, 0x0b, 0x03); // thickness3
        }  
        PO1.newValue(1, 74, 1);   //OSC 1  Vol
         //{0xf0, 0x42, 0x30, 0x00, 0x01, 0x49, 0x41, 0x02, 0x00, 0x01, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf7};
        PO1.Potec[7]={0x02}; PO1.Potec[9]={0x01}; PO1.Potec[11]={0x0c}; 
        PO2.newValue(1, 71, 1);   //OSC 2  Vol
         //{0xf0, 0x42, 0x30, 0x00, 0x01, 0x49, 0x41, 0x02, 0x00, 0x01, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf7};
        PO2.Potec[7]={0x02}; PO2.Potec[9]={0x01}; PO2.Potec[11]={0x0d}; 
        PO3.defineSySex( 66 );   //OSC 1 octave   
        PO3.newValue(9, 73, 1); 
        PO4.defineSySex( 67 );   //OSC 2 Octave
        PO4.newValue(9, 72, 1);
        PO5.newValue(6, 83, 1);   //Mono Unison Detune cents 
         //{0xf0, 0x42, 0x30, 0x00, 0x01, 0x49, 0x41, 0x03, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf7};
        PO5.Potec[7]={0x03}; PO5.Potec[9]={0x00}; PO5.Potec[11]={0x0a};
        PO6.newValue(0, 7, 1);
        PO7.newValue(0, 7, 10);
        PO8.newValue(0, 7, 16);     
        break;

//oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo//
//--------------------------------------------------------- POTEs MODO COMBI --------------------------------------------------------------------------------//

// Pots default modo Combi 
    case 4355: // CapsLock
        PO1.newValue(0, 7, 1);    // Vol
        PO2.newValue(0, 73, 1);    // Attack
        PO3.newValue(0, 75, 1);    // Decay
        PO4.newValue(0, 70, 1);    // Sustain
        PO5.newValue(0, 72, 1);    // Release
        PO6.newValue(0, 79, 1);    // EGintensity
        PO7.newValue(0, 74, 1);    // CutOff
        PO8.newValue(0, 71, 1);    // Resonance 
        break;

// Pots A
   case 4449: //bancoA F1 -------------------MIDI CHANNEL 1  ----------------------------------------------------------------Normal//     
        PO1.newValue(0, 7, 1);  
        PO2.newValue(0, 73, 1);  
        PO3.newValue(0, 75, 1);  
        PO4.newValue(0, 70, 1);  
        PO5.newValue(0, 72, 1);  
        PO6.newValue(0, 79, 1);  
        PO7.newValue(0, 74, 1);  
        PO8.newValue(0, 71, 1);  
        break;
   case 4450: //bancoA F2 -------------------MIDI CHANNEL 2          
        PO1.newValue(0, 7, 2);
        PO2.newValue(0, 73, 2);
        PO3.newValue(0, 75, 2);
        PO4.newValue(0, 70, 2);
        PO5.newValue(0, 72, 2);
        PO6.newValue(0, 79, 2);
        PO7.newValue(0, 74, 2);
        PO8.newValue(0, 71, 2);
        break;
   case 4451: //bancoA F3 ------------------MIDI CHANNEL 3          
        PO1.newValue(0, 7, 3);
        PO2.newValue(0, 73, 3);
        PO3.newValue(0, 75, 3);
        PO4.newValue(0, 70, 3);
        PO5.newValue(0, 72, 3);
        PO6.newValue(0, 79, 3);
        PO7.newValue(0, 74, 3);
        PO8.newValue(0, 71, 3);
        break;    
   case 4452: //bancoA F4 ------------------MIDI CHANNEL 4          
        PO1.newValue(0, 7, 4);
        PO2.newValue(0, 73, 4);
        PO3.newValue(0, 75, 4);
        PO4.newValue(0, 70, 4);
        PO5.newValue(0, 72, 4);
        PO6.newValue(0, 79, 4);
        PO7.newValue(0, 74, 4);
        PO8.newValue(0, 71, 4);
        break;
   case 4453: //bancoB F5 ------------------MIDI CHANNEL 1 B          
        PO1.newValue(0, 1, 1);    // Mod1
        PO2.newValue(0, 2, 1);    // Mod2
        PO3.newValue(0, 16, 1);    // Mod 16
        PO4.newValue(0, 10, 1);    // PAN
        PO5.newValue(0, 8, 1);    // PAN insert FX
        PO6.newValue(0, 5, 1);     // Portamento
        PO7.newValue(10, 76, 1);   // LFO Rate
        PO8.newValue(0, 77, 1);    // LFO Depth
        break; 
   case 4454: //bancoB F6 ------------------MIDI CHANNEL 2 B          
        PO1.newValue(0, 1, 2);
        PO2.newValue(0, 2, 2);
        PO3.newValue(0, 16, 2);
        PO4.newValue(0, 10, 2);
        PO5.newValue(0, 8, 2);
        PO6.newValue(0, 5, 2);
        PO7.newValue(10, 76, 2);
        PO8.newValue(0, 77, 2);
        break; 
   case 4455: //bancoB F7 -------------------MIDI CHANNEL 3 B          
        PO1.newValue(0, 1, 3);
        PO2.newValue(0, 2, 3);
        PO3.newValue(0, 16, 3);
        PO4.newValue(0, 10, 3);
        PO5.newValue(0, 8, 3);
        PO6.newValue(0, 5, 3);
        PO7.newValue(0, 76, 3);
        PO8.newValue(0, 77, 3);
        break;
   case 4456: //bancoB F8 -------------------MIDI CHANNEL 4 B          
        PO1.newValue(0, 1, 4);
        PO2.newValue(0, 2, 4);
        PO3.newValue(3, 22, 1);   // ARP Gate
        PO4.newValue(0, 23, 1);   // ARP Vel
        PO5.newValue(0, 24, 1);   // ARP Swing
        PO6.newValue(3, 1, 4);    // Pitch
        PO7.newValue(0, 76, 4);
        PO8.newValue(0, 77, 4);
        break; 
   case 4457: //bancoA F9 -------------------MIDI CHANNEL 10 A          
        PO1.newValue(0, 7, 10);
        PO2.newValue(0, 73, 10);
        PO3.newValue(0, 75, 10);
        PO4.newValue(0, 70, 10);
        PO5.newValue(0, 72, 10);
        PO6.newValue(0, 79, 10);
        PO7.newValue(0, 74, 10);
        PO8.newValue(0, 71, 10);
        break; 
   case 4458: //bancoB F10 ------------------MIDI CHANNEL 10 B         
        PO1.newValue(0, 1, 10);
        PO2.newValue(0, 2, 10);
        PO3.newValue(0, 16, 10);    
        PO4.newValue(0, 10, 10);
        PO5.newValue(0, 8, 10);
        PO6.newValue(3, 1, 10);     // Pitch
        PO7.newValue(10, 76, 10);
        PO8.newValue(0, 77, 10);
        break;   
        

   case 4459: //bancoA F11 -----------------MIDI CHANNEL 12 A          
        PO1.newValue(0, 07, 12);
        PO2.newValue(0, 73, 12);
        PO3.newValue(0, 75, 12);
        PO4.newValue(0, 70, 12);
        PO5.newValue(0, 72, 12);
        PO6.newValue(0, 79, 12);
        PO7.newValue(0, 74, 12);
        PO8.newValue(0, 71, 12);
        break;   
   case 4460: //bancoB F12 -----------------MIDI CHANNEL 12 B         
        PO1.newValue(0, 1, 12);
        PO2.newValue(0, 2, 12);
        PO3.newValue(0, 16, 12);   
        PO4.newValue(0, 10, 12);
        PO5.newValue(0, 8, 12);
        PO6.newValue(3, 1, 12);    // Pitch
        PO7.newValue(10, 76, 12);
        PO8.newValue(0, 77, 12);
        break;  
        

   case 4379: //StepSeq ESC -----------------MIDI CHANNEL 16          
        PO1.newValue(0, 07, 16);
        PO2.newValue(0, 73, 16);
        PO3.newValue(0, 75, 16);
        PO4.newValue(0, 74, 16);
        PO5.newValue(0, 71, 16);
        PO6.newValue(3, 72, 16);     // Pitch
        PO7.newValue(0, 93, 16);
        PO8.newValue(0, 91, 16);
        break;  
/*
        
// Pots  B  ------------------------- Control + Fs ----------------------------------------------------------------------------------------------//
   case 12641: //bancoB F1  ---------------MIDI CHANNEL 1          
        PO1.newValue(0, 01, 1);    // Mod1
        PO2.newValue(0, 02, 1);    // Mod2
        PO3.newValue(0, 05, 1);    // Portamento
        PO4.newValue(0, 22, 1);    // Arp-Gate
        PO5.newValue(0, 23, 1);    // Arp-Vel
        PO6.newValue(0, 24, 1);    // Arp-Swing
        PO7.newValue(0, 93, 1);    // MFX 1 Send
        PO8.newValue(0, 91, 1);    // MFX 2 Send
        break;     
   case 12642: //bancoB F2  ---------------MIDI CHANNEL 2          
        PO1.newValue(0, 01, 2);
        PO2.newValue(0, 02, 2);
        PO3.newValue(0, 05, 2);
        PO4.newValue(0, 22, 1);
        PO5.newValue(0, 23, 1);
        PO6.newValue(0, 24, 1);
        PO7.newValue(0, 93, 2);
        PO8.newValue(0, 91, 2);
        break;     
   case 12643: //bancoB F3  ---------------MIDI CHANNEL 3          
        PO1.newValue(0, 01, 3);
        PO2.newValue(0, 02, 3);
        PO3.newValue(0, 05, 3);
        PO4.newValue(0, 22, 1);
        PO5.newValue(0, 23, 1);
        PO6.newValue(0, 24, 1);
        PO7.newValue(0, 93, 3);
        PO8.newValue(0, 91, 3);
        break;     
   case 12644: //bancoB F4 --------------- MIDI CHANNEL 4          
        PO1.newValue(0, 01, 4);
        PO2.newValue(0, 02, 4);
        PO3.newValue(0, 05, 4);
        PO4.newValue(0, 22, 1);
        PO5.newValue(0, 23, 1);
        PO6.newValue(0, 24, 1);
        PO7.newValue(0, 93, 4);
        PO8.newValue(0, 91, 4);
        break;     
   case 12645: //bancoB F5  ---------------MIDI CHANNEL 5          
        PO1.newValue(0, 01, 5);
        PO2.newValue(0, 02, 5);
        PO3.newValue(0, 05, 5);
        PO4.newValue(0, 22, 1);
        PO5.newValue(0, 23, 1);
        PO6.newValue(0, 24, 1);
        PO7.newValue(0, 93, 5);
        PO8.newValue(0, 91, 5);
        break;     
   case 12646: //bancoB F6  ---------------MIDI CHANNEL 6          
        PO1.newValue(0, 01, 6);
        PO2.newValue(0, 02, 6);
        PO3.newValue(0, 05, 6);
        PO4.newValue(0, 22, 1);
        PO5.newValue(0, 23, 1);
        PO6.newValue(0, 24, 1);
        PO7.newValue(0, 93, 6);
        PO8.newValue(0, 91, 6);
        break;     
   case 12647: //bancoB F7 ---------------- MIDI CHANNEL 7          
        PO1.newValue(0, 01, 7);
        PO2.newValue(0, 02, 7);
        PO3.newValue(0, 05, 7);
        PO4.newValue(0, 22, 1);
        PO5.newValue(0, 23, 1);
        PO6.newValue(0, 24, 1);
        PO7.newValue(0, 93, 7);
        PO8.newValue(0, 91, 7);
        break;     
   case 12648: //bancoB F8  ----------------MIDI CHANNEL 8          
        PO1.newValue(0, 01, 8);
        PO2.newValue(0, 02, 8);
        PO3.newValue(0, 05, 8);
        PO4.newValue(0, 22, 1);
        PO5.newValue(0, 23, 1);
        PO6.newValue(0, 24, 1);
        PO7.newValue(0, 93, 8);
        PO8.newValue(0, 91, 8);
        break;     
   case 12649: //bancoB F9  -----------------MIDI CHANNEL 9          
        PO1.newValue(0, 01, 9);
        PO2.newValue(0, 02, 9);
        PO3.newValue(0, 05, 9);
        PO4.newValue(0, 22, 1);
        PO5.newValue(0, 23, 1);
        PO6.newValue(0, 24, 1);
        PO7.newValue(0, 93, 9);
        PO8.newValue(0, 91, 9);
        break;     
   case 12650: //bancoB F10  ----------------MIDI CHANNEL 10         
        PO1.newValue(0, 01, 10);
        PO2.newValue(0, 02, 10);
        PO3.newValue(3, 05, 10);   // Pitch 
        PO4.newValue(0, 22, 1);
        PO5.newValue(0, 23, 1);
        PO6.newValue(0, 24, 1);
        PO7.newValue(0, 93, 10);
        PO8.newValue(0, 91, 10);
        break;     
   case 12651: //bancoB F11 ---------------- MIDI CHANNEL 11          
        PO1.newValue(0, 01, 11);
        PO2.newValue(0, 02, 11);
        PO3.newValue(0, 05, 11);
        PO4.newValue(0, 22, 1);
        PO5.newValue(0, 23, 1);
        PO6.newValue(0, 24, 1);
        PO7.newValue(0, 93, 11);
        PO8.newValue(0, 91, 11);
        break;     
   case 12652: //bancoB F12 --------------- MIDI CHANNEL 12          
        PO1.newValue(0, 01, 12);
        PO2.newValue(0, 02, 12);
        PO3.newValue(0, 05, 12);
        PO4.newValue(0, 22, 1);
        PO5.newValue(0, 23, 1);
        PO6.newValue(0, 24, 1);
        PO7.newValue(0, 93, 12);
        PO8.newValue(0, 91, 12);
        break;     
   case 12571: //bancoB ESC --------------- MIDI CHANNEL 16          
        PO1.newValue(0, 01, 16);
        PO2.newValue(0, 02, 16);
        PO3.newValue(3, 05, 16);   // Pitch
        PO4.newValue(0, 22, 1);
        PO5.newValue(0, 23, 1);
        PO6.newValue(0, 24, 1);
        PO7.newValue(0, 93, 16);
        PO8.newValue(0, 91, 16);
        break;    

 */                                        
//Grupos Pots
   case 4961: //banco F1   GROUP POTS VOLUMEN ------------------- Win + Fs -----------------------------------------------------------Win Key       
        PO1.newValue(0, 7, 1);
        PO2.newValue(0, 7, 2);
        PO3.newValue(0, 7, 3);
        PO4.newValue(0, 7, 4);
        PO5.newValue(0, 7, 5);
        PO6.newValue(0, 7, 10);
        PO7.newValue(0, 7, 12);
        PO8.newValue(0, 7, 16);
        break;    
   case 4962: //banco F2   GROUP POTS PAN       
        PO1.newValue(0, 10, 1);
        PO2.newValue(0, 10, 2);
        PO3.newValue(0, 10, 3);
        PO4.newValue(0, 10, 4);
        PO5.newValue(0, 10, 5);
        PO6.newValue(0, 10, 10);
        PO7.newValue(0, 10, 12);
        PO8.newValue(0, 10, 16);
        break;
   case 4963: //banco F3   GROUP POTS ATTACK       
        PO1.newValue(0, 73, 1);
        PO2.newValue(0, 73, 2);
        PO3.newValue(0, 73, 3);
        PO4.newValue(0, 73, 4);
        PO5.newValue(0, 73, 5);
        PO6.newValue(0, 73, 10);
        PO7.newValue(0, 73, 12);
        PO8.newValue(0, 73, 16);
        break; 
   case 4964: //banco F4   GROUP POTS Decay-Release       
        PO1.newValue(0, 72, 1);
        PO2.newValue(0, 72, 2);
        PO3.newValue(0, 72, 3);
        PO4.newValue(0, 72, 4);
        PO5.newValue(0, 72, 5);
        PO6.newValue(0, 75, 10);
        PO7.newValue(0, 75, 12);
        PO8.newValue(0, 75, 16);
        break;  
   case 4965: //banco F5   GROUP POTS FILTER CUTOFF       
        PO1.newValue(0, 74, 1);
        PO2.newValue(0, 74, 2);
        PO3.newValue(0, 74, 3);
        PO4.newValue(0, 74, 4);
        PO5.newValue(0, 74, 5);
        PO6.newValue(0, 74, 10);
        PO7.newValue(0, 74, 12);
        PO8.newValue(0, 74, 16);
        break; 
   case 4966: //banco F6   GROUP POTS MOD WHEEL       
        PO1.newValue(0, 1, 1);
        PO2.newValue(0, 1, 2);
        PO3.newValue(0, 1, 3);
        PO4.newValue(0, 1, 4);
        PO5.newValue(0, 1, 5);
        PO6.newValue(0, 1, 10);
        PO7.newValue(0, 1, 12);
        PO8.newValue(0, 1, 16);
        break;
   case 4967: //banco F7   GROUP POTS FX1 send       
        PO1.newValue(0, 93, 1);
        PO2.newValue(0, 93, 2);
        PO3.newValue(0, 93, 3);
        PO4.newValue(0, 93, 4);
        PO5.newValue(0, 93, 5);
        PO6.newValue(0, 93, 10);
        PO7.newValue(0, 93, 12);
        PO8.newValue(0, 93, 16);
        break;
   case 4968: //banco F8   GROUP POTS FX2 send       
        PO1.newValue(0, 91, 1);
        PO2.newValue(0, 91, 2);
        PO3.newValue(0, 91, 3);
        PO4.newValue(0, 91, 4);
        PO5.newValue(0, 91, 5);
        PO6.newValue(0, 91, 10);
        PO7.newValue(0, 91, 12);
        PO8.newValue(0, 91, 16);
        break;  


// Groups Tracks SysEx ------------------------------------------------ Shift + Fs ---------------------------------------------------//                                                                                                                                                                                                                                                                                       

   case 20833: // F1 VOLUMEN TRACK CONTROLS  -----------------SysEx POTES ----------------------------------------      
        PO1.newValue(1, 7, 1);  // trk 1
        PO1.Potec[7]={0x00}; PO1.Potec[9]={0x09}; PO1.Potec[11]={0x02};
        PO2.newValue(1, 10, 1); // trk 2
        PO2.Potec[7]={0x00}; PO2.Potec[9]={0x0a}; PO2.Potec[11]={0x02};
        PO3.newValue(1, 73, 1); // trk 3
        PO3.Potec[7]={0x00}; PO3.Potec[9]={0x0b}; PO3.Potec[11]={0x02};
        PO4.newValue(1, 72, 1); // trk 4
        PO4.Potec[7]={0x00}; PO4.Potec[9]={0x0c}; PO4.Potec[11]={0x02};
        PO5.newValue(1, 74, 1); // trk 5
        PO5.Potec[7]={0x00}; PO5.Potec[9]={0x0d}; PO5.Potec[11]={0x02};
        PO6.newValue(1, 71, 1); // trk 6
        PO6.Potec[7]={0x00}; PO6.Potec[9]={0x0e}; PO6.Potec[11]={0x02};
        PO7.newValue(1, 93, 1); // trk 7
        PO7.Potec[7]={0x00}; PO7.Potec[9]={0x0f}; PO7.Potec[11]={0x02};
        PO8.newValue(1, 93, 1); // trk 8
        PO8.Potec[7]={0x00}; PO8.Potec[9]={0x10}; PO8.Potec[11]={0x02};  // trk 8
        break;          
   case 20834: //F2   PAN TRACK CONTROLS         
        PO1.newValue(1, 7, 1);
        PO1.Potec[7]={0x00}; PO1.Potec[9]={0x09}; PO1.Potec[11]={0x01};
        PO2.newValue(1, 10, 1);
        PO2.Potec[7]={0x00}; PO2.Potec[9]={0x0a}; PO2.Potec[11]={0x01};
        PO3.newValue(1, 73, 1);
        PO3.Potec[7]={0x00}; PO3.Potec[9]={0x0b}; PO3.Potec[11]={0x01};
        PO4.newValue(1, 72, 1);
        PO4.Potec[7]={0x00}; PO4.Potec[9]={0x0c}; PO4.Potec[11]={0x01};
        PO5.newValue(1, 74, 1);
        PO5.Potec[7]={0x00}; PO5.Potec[9]={0x0d}; PO5.Potec[11]={0x01};
        PO6.newValue(1, 71, 1);
        PO6.Potec[7]={0x00}; PO6.Potec[9]={0x0e}; PO6.Potec[11]={0x01};
        PO7.newValue(1, 93, 1);
        PO7.Potec[7]={0x00}; PO7.Potec[9]={0x0f}; PO7.Potec[11]={0x01};
        PO8.newValue(1, 93, 1);
        PO8.Potec[7]={0x00}; PO8.Potec[9]={0x10}; PO8.Potec[11]={0x01}; // trk 8
        break;  
   case 20835: // F3 CUTOFF TRACK CONTROLS        
        PO1.defineSySex( 1 );          
        PO1.newValue(5, 7, 16);        // trk 1
        PO2.defineSySex( 2 );
        PO2.newValue(5, 10, 16);       // trk 2
        PO3.defineSySex( 3 );
        PO3.newValue(5, 73, 16);       // trk 3
        PO4.defineSySex( 4 );
        PO4.newValue(5, 75, 16);       // trk 4
        PO5.defineSySex( 5 );
        PO5.newValue(5, 74, 16);       // trk 5
        PO6.defineSySex( 6 );
        PO6.newValue(5, 71, 16);       // trk 6
        PO7.defineSySex( 7 );
        PO7.newValue(5, 93, 16);       // trk 7
        PO8.defineSySex( 8 );
        PO8.newValue(5, 93, 16);       // trk 8
        break;
   case 20836: // F4 RESONANCE TRACK CONTROLS        
        PO1.defineSySex( 9 );
        PO1.newValue(5, 7, 16);
        PO2.defineSySex( 10 );
        PO2.newValue(5, 10, 16);
        PO3.defineSySex( 11 );
        PO3.newValue(5, 73, 16);
        PO4.defineSySex( 12 );
        PO4.newValue(5, 75, 16);
        PO5.defineSySex( 13 );
        PO5.newValue(5, 74, 16);
        PO6.defineSySex( 14 );
        PO6.newValue(5, 71, 16);
        PO7.defineSySex( 15 );
        PO7.newValue(5, 93, 16);
        PO8.defineSySex( 16 );
        PO8.newValue(5, 93, 16);
        break;        
   case 20837: // F5 EG INTENSITY TRACK CONTROLS        
        PO1.defineSySex( 17 );
        PO1.newValue(5, 7, 16);
        PO2.defineSySex( 18 );
        PO2.newValue(5, 10, 16);
        PO3.defineSySex( 19 );
        PO3.newValue(5, 73, 16);
        PO4.defineSySex( 20 );
        PO4.newValue(5, 75, 16);
        PO5.defineSySex( 21 );
        PO5.newValue(5, 74, 16);
        PO6.defineSySex( 22 );
        PO6.newValue(5, 71, 16);
        PO7.defineSySex( 23 );
        PO7.newValue(5, 93, 16);
        PO8.defineSySex( 24 );
        PO8.newValue(5, 93, 16);
        break;                             
   case 20838: // F6 ATTACK TRACK CONTROLS       
        PO1.defineSySex( 33 );
        PO1.newValue(5, 7, 16);
        PO2.defineSySex( 34 );
        PO2.newValue(5, 10, 16);
        PO3.defineSySex( 35 );
        PO3.newValue(5, 73, 16);
        PO4.defineSySex( 36 );
        PO4.newValue(5, 75, 16);
        PO5.defineSySex( 37 );
        PO5.newValue(5, 74, 16);
        PO6.defineSySex( 38 );
        PO6.newValue(5, 71, 16);
        PO7.defineSySex( 39 );
        PO7.newValue(5, 93, 16);
        PO8.defineSySex( 40 );
        PO8.newValue(5, 93, 16);
        break;                                                   
   case 20839: // F7 DECAY TRACK CONTROLS    
        PO1.defineSySex( 41 );
        PO1.newValue(5, 7, 16);
        PO2.defineSySex( 42 );
        PO2.newValue(5, 10, 16);
        PO3.defineSySex( 43 );
        PO3.newValue(5, 73, 16);
        PO4.defineSySex( 44 );
        PO4.newValue(5, 75, 16);
        PO5.defineSySex( 45 );
        PO5.newValue(5, 74, 16);
        PO6.defineSySex( 46 );
        PO6.newValue(5, 71, 16);
        PO7.defineSySex( 47 );
        PO7.newValue(5, 93, 16);
        PO8.defineSySex( 48 );
        PO8.newValue(5, 93, 16);
        break;   
   case 20840: // F8 release TRACK CONTROLS         
        PO1.defineSySex( 57 );
        PO1.newValue(5, 7, 16);
        PO2.defineSySex( 58 );
        PO2.newValue(5, 10, 16);
        PO3.defineSySex( 59 );
        PO3.newValue(5, 73, 16);
        PO4.defineSySex( 60 );
        PO4.newValue(5, 75, 16);
        PO5.defineSySex( 61 );
        PO5.newValue(5, 74, 16);
        PO6.defineSySex( 62 );
        PO6.newValue(5, 71, 16);
        PO7.defineSySex( 63 );
        PO7.newValue(5, 93, 16);
        PO8.defineSySex( 64 );
        PO8.newValue(5, 93, 16);
        break;  

    case 20763: //  Shift + ESC  ---------- FX: ifx1 On/Off, ifx2 On/Off, ifx3 On/Off, ifx4 On/Off, ifx5 On/Off, Reverb Time, MFX 1 Return, MFX 2 Return ------//        
        PO1.newValue(7, 22, 1); //  IFX 1
        //{0xf0, 0x42, 0x30, 0x00, 0x01, 0x49, 0x41, 0x00, 0x00, 0x19, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf7};
        PO1.Potec[7]={0x00}; PO1.Potec[9]={0x19}; PO1.Potec[11]={0x05};           
        PO2.newValue(7, 23, 1); //  IFX 2
        //{0xf0, 0x42, 0x30, 0x00, 0x01, 0x49, 0x41, 0x00, 0x00, 0x19, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf7};
        PO2.Potec[7]={0x00}; PO2.Potec[9]={0x19}; PO2.Potec[11]={0x06};         
        PO3.newValue(7, 24, 1); //  IFX 3
        //{0xf0, 0x42, 0x30, 0x00, 0x01, 0x49, 0x41, 0x00, 0x00, 0x19, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf7};
        PO3.Potec[7]={0x00}; PO3.Potec[9]={0x19}; PO3.Potec[11]={0x07};         
        PO4.newValue(7, 93, 1); //  IFX 4
        //{0xf0, 0x42, 0x30, 0x00, 0x01, 0x49, 0x41, 0x00, 0x00, 0x19, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf7};
        PO4.Potec[7]={0x00}; PO4.Potec[9]={0x19}; PO4.Potec[11]={0x08};         
        PO5.newValue(7, 91, 1); //  IFX 5
        //{0xf0, 0x42, 0x30, 0x00, 0x01, 0x49, 0x41, 0x00, 0x00, 0x19, 0x00, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf7};
        PO5.Potec[7]={0x00};PO5.Potec[9]={0x19}; PO5.Potec[11]={0x09};      
        PO6.newValue(6, 7, 1); // Reverb Time
        //{0xf0, 0x42, 0x30, 0x00, 0x01, 0x49, 0x41, 0x00, 0x00, 0x21, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf7};
        PO6.Potec[7]={0x00};PO6.Potec[9]={0x21}; PO6.Potec[11]={0x00};        
        PO7.newValue(8, 7, 10); // MFX1 Return
        //{0xf0, 0x42, 0x30, 0x00, 0x01, 0x49, 0x41, 0x00, 0x00, 0x1f, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf7};
        PO7.Potec[7]={0x00};PO7.Potec[9]={0x1f}; PO7.Potec[11]={0x04};        
        PO8.newValue(1, 7, 16); // MFX2 Return
        //{0xf0, 0x42, 0x30, 0x00, 0x01, 0x49, 0x41, 0x00, 0x00, 0x1f, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf7};
        PO8.Potec[7]={0x00};PO8.Potec[9]={0x1f}; PO8.Potec[11]={0x05};        
        break;  


  }
  updatePots();



}                   
//-------------------------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------------------------//
void updatePots()
{
  for (int i = 0; i < NUMBER_POTS; i = i + 1)
  {
    if (POTS[i]->Pcommand == 0) //---------------------------- 0-127 values
    {
      byte potmessage = POTS[i]->getValue();
      if (potmessage != 255)
        MIDI.sendControlChange(POTS[i]->Pcontrol, potmessage, POTS[i]->Pchannel);
    }
    if (POTS[i]->Pcommand == 1) //---------------------------SysEx 1 value
    {
      if (POTS[i]->potec6(false, 16))
        MIDI.sendSysEx(18, POTS[i]->Potec, true);
    }
   /*
    if (POTS[i]->Pcommand == 2) //------------------------- 2 values SysEx
    {
      if (POTS[i]->potec(true, 15)) // envias la menor posicion e internamente le suma 1
        MIDI.sendSysEx(18, POTS[i]->Potec, true);
    }  */
    if (POTS[i]->Pcommand == 3) // ------------------------ PitchBend
    {
      if (POTS[i]->pitchbend(POTS[i]->getValueBits(100))) // leo el potenciometro y mapeo a 14 bits, si hubo cambio...
        MIDI.sendPitchBend(POTS[i]->pitchvalue(), POTS[i]->Pchannel); // envío el int de 14 bits a .sendPitchBend
    }   
    /*      
    if (POTS[i]->Pcommand == 4) //------------------------- Pote Custom value
    {
      byte potmessage3 = POTS[i]->getValue();
      if (potmessage3 != 255)
        MIDI.sendControlChange(POTS[i]->Pcontrol, potmessage3 +455 >>3, POTS[i]->Pchannel);  //+ - 7 con centro en 64 = +455 >>3, + - 14 con centro en 64 = +200 >>2
    } */
    if (POTS[i]->Pcommand == 5)
      {
        if (POTS[i]->potec2(false, 16, -99, 99)) //  -99, 0, +99  value 
          MIDI.sendSysEx(18, POTS[i]->Potec1, true);
      }  
    if (POTS[i]->Pcommand == 6)
      {
        if (POTS[i]->potec3(false, 16, 1, 100)) //   1 - 100 value   
          MIDI.sendSysEx(18, POTS[i]->Potec, true);
      } 
    if (POTS[i]->Pcommand == 7)
      {
        if (POTS[i]->potec4(false, 16)) //     
          MIDI.sendSysEx(18, POTS[i]->Potec, true);
      }  
    if (POTS[i]->Pcommand == 8) //---------------------------SysEx 1 value
      {
        if (POTS[i]->potec5(false, 16))
          MIDI.sendSysEx(18, POTS[i]->Potec, true);
      } 
    if (POTS[i]->Pcommand == 9)
      {
        if (POTS[i]->potec2(false, 16, -2, 2)) //    -2 to 2 value 
          MIDI.sendSysEx(18, POTS[i]->Potec1, true);
      }   
    if (POTS[i]->Pcommand == 10) //-------------------------Pote for LFO Depth
    {
      byte potmessage2 = POTS[i]->getValue();
      if (potmessage2 != 255)
        MIDI.sendControlChange(POTS[i]->Pcontrol, potmessage2 +250 >>2, POTS[i]->Pchannel);  // +250>>2  es: de 62 a 94 //+ - 7 con centro en 64 = +455 >>3, + - 14 con centro en 64 = +200 >>2
    } 


  }
}

//---------------------------------------------------------------------------------------------------------------------------------------------------//











