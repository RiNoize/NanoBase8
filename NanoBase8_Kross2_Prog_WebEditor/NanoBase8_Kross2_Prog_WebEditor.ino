/***************************************************************************************************************************
  NanoBase8 Kross2 Prog&combi es un sketch Arduino de un Controlador MIDI hecho con un teclado 
  QWERTY PS2 spanish y 8 potenciometros conectados a un Arduino Nano.
  La inspiracion de crear este controlador MIDI fué la necesidad de controlar 
  via CCs y via SysEx los cientos de parametros y funciones que tiene 
  un sintetizador como el Korg Kross 2

  Este sketch se puede modificar para controlar cualquier aparato que tenga interfaz MIDI IN

  ** Hecho por RiNoize ** (2023)
 
   NanoBase8 Kross2 Prog&combi       Version 1.0     Licencia MIT
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
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);

byte NUMBER_POTS = 8;

//ooo Definir los potenciometros conectados oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo//
//Pot (Pin Number, Command, CC Control, Channel Number)
//**El parametro Command es para diferentes mapeos y usos de potenciometro**

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

// Sostenuto ------------------- BloqDesplazar key ------------ Program y Combi -------//
   case 258: 
   case 4354: 
        MIDI.sendControlChange(66, 127, 1); // Sostenuto On
        break; 
   case 33026:
   case 37122:
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
        PO1.Potec[7]={0x02}; PO1.Potec[9]={0x0a}; PO1.Potec[11]={0x19}; 
        PO2.newValue(1, 71, 6);
        PO2.Potec[7]={0x02}; PO2.Potec[9]={0x0a}; PO2.Potec[11]={0x1a}; 
        PO3.newValue(1, 72, 6);
        PO3.Potec[7]={0x02}; PO3.Potec[9]={0x0a}; PO3.Potec[11]={0x1b}; 
        PO4.newValue(1, 73, 6);
        PO4.Potec[7]={0x02}; PO4.Potec[9]={0x0a}; PO4.Potec[11]={0x1c};  
        PO5.newValue(1, 75, 6);
        PO5.Potec[7]={0x02}; PO5.Potec[9]={0x0a}; PO5.Potec[11]={0x1d}; 
        PO6.newValue(0, 7, 1);
        PO7.newValue(0, 7, 10);
        PO8.newValue(0, 7, 16);
        break;     
    case 360: //bank F8           ON/OFF inserts FX 
        PO1.newValue(7, 22, 1); //  IFX 1
        PO1.Potec[7]={0x02}; PO1.Potec[9]={0x0a}; PO1.Potec[11]={0x05};           
        PO2.newValue(7, 23, 1); //  IFX 2
        PO2.Potec[7]={0x02}; PO2.Potec[9]={0x0a}; PO2.Potec[11]={0x06};         
        PO3.newValue(7, 24, 1); //  IFX 3
        PO3.Potec[7]={0x02}; PO3.Potec[9]={0x0a}; PO3.Potec[11]={0x07};         
        PO4.newValue(7, 93, 1); //  IFX 4
        PO4.Potec[7]={0x02}; PO4.Potec[9]={0x0a}; PO4.Potec[11]={0x08};         
        PO5.newValue(7, 91, 1); //  IFX 5
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
    if (POTS[i]->Pcommand == 3) // ------------------------ PitchBend
    {
      if (POTS[i]->pitchbend(POTS[i]->getValueBits(100))) // leo el potenciometro y mapeo a 14 bits, si hubo cambio...
        MIDI.sendPitchBend(POTS[i]->pitchvalue(), POTS[i]->Pchannel); // envío el int de 14 bits a .sendPitchBend
    }   
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











