/***************************************************************************************************************************
  NanoBase8_Kross2_combiV1 es un sketch Arduino de un Controlador MIDI hecho con un teclado 
  QWERTY PS2 spanish y 8 potenciometros conectados a un Arduino Nano.
  La inspiracion de crear este controlador MIDI fué la necesidad de controlar 
  via CCs y via SysEx los cientos de parametros y funciones que tiene 
  un sintetizador como el Korg Kross 2

  Este sketch se puede modificar para controlar cualquier aparato que tenga interfaz MIDI IN

  ** Hecho por RiNoize ** (2023)
 
   NanoBase8_Kross2_combiV1       Version 1.0     Licencia MIT
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

//ooo Definir los potenciometros conectados oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo//
//Pot (Pin Number, Command, CC Control, Channel Number)
// El parametro Command es para diferentes mapeos y usos de potenciometro 

//Default
Pot PO1(A0, 0, 07, 1);  // Vol
Pot PO2(A1, 0, 10, 1);  // Pan
Pot PO3(A2, 0, 73, 1);  // Attack
Pot PO4(A3, 0, 75, 1);  // Decay
Pot PO5(A4, 0, 70, 1);  // Sustain
Pot PO6(A5, 0, 72, 1);  // Release
Pot PO7(A6, 0, 74, 1);  // CutOff
Pot PO8(A7, 0, 71, 1);  // Resonance

//0000000000000000000000000000000000000000000000000000000000000000000
Pot *POTS[]{&PO1, &PO2, &PO3, &PO4, &PO5, &PO6, &PO7, &PO8};
//0000000000000000000000000000000000000000000000000000000000000000000




void setup() {

  MIDI.begin(MIDI_CHANNEL_OMNI);
//  MIDI.turnThruOn();

// inicio y setup del teclado -----------------------------------------------------------------------------------------------------------------------------------------------------//
  keyboard.begin(DATAPIN, IRQPIN);
  delay( 1000 );
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
//ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo MODO COMBI en el Kross2 oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo//

//PROGRAM CHANGE de a 1 -------------------------- Flecha arriba, flecha abajo -------------------------------------------------------------------------//
    case 279:
        if (patchNum < 127)
        {
          patchNum++;
          MIDI.sendProgramChange(patchNum, 1);
        }
        break;
    case 280:
        if (patchNum > 0)
        {
          patchNum--;
          MIDI.sendProgramChange(patchNum, 1);
         }
        break;
//PROGRAM CHANGE de a 10 ------------------------------------- Flecha Izq, Flecha Der -------------------------------------------------------------//
    case 278://flecha der
        if (patchNum <= 117)
        {
          patchNum=patchNum+10;
          MIDI.sendProgramChange(patchNum, 1);
        }
        break;
    case 277://flecha izq
        if (patchNum >=11)
        {
          patchNum=patchNum-10;
          MIDI.sendProgramChange(patchNum, 1);
         }
        break;
//BANK CHANGE------------------------------------------Control Key + Flecha Izq, Flecha Der ---------------------------------------------------------------------------//
    case 8470:
          MIDI.sendControlChange(0, bankSelect, 1);
        if (bankNum < 6 ) // cantidad de bancos
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

// Start / Stop Arps con led------------------- BloqDesplazar key -------------------//
   case 258: 
        MIDI.sendControlChange(14, 127, 1); // Arp On
        break; 
   case 33026:
        MIDI.sendControlChange(14, 01, 1);  // Arp Off
        break; 

        
//Start Stop Drum Track -------------------------------------------------------- Barra espaciadora --------------------------------------------------------------------------//
   case 287:
   case 8479:
        if (startStop)//true
        {
          startStop = !startStop;          
  //      sendSysExDef(0x00, 0x05, 0x02, 0x00); // startInmediatly  
  //      sendSysExDef(0x00, 0x05, 0x04, 0x01); // SyncOn 
        sendSysExDef(0x00, 0x05, 0x0a, 0x01); // start       
        }
        else//false
        {
          startStop = !startStop;         
        sendSysExDef(0x00, 0x05, 0x0a, 0x00); // Stop         
        }
        break;

//Button 1 -------------------------------------- imp pant Pet Sis----------------- MODO PROGRAM Y COMBI ------------------------------------------------------//
   case 260:
        if (Boton1)//true
        {
          Boton1 = !Boton1;
        sendSysExDef(0x00, 0x01, 0x08, 0x01); // on Combi
        MIDI.sendControlChange(80, 127, 1);
        }
        else//false
        {
          Boton1 = !Boton1; 
        sendSysExDef(0x00, 0x01, 0x08, 0x00); // off Combi
        MIDI.sendControlChange(80, 1, 1);
        }
        break;

//Button 2 ------------------------------------------ Pause inter ----------------- MODO PROGRAM Y COMBI -----------------------------------------------------------//
   case 6:
        if (Boton2)//true
        {
          Boton2 = !Boton2;
        sendSysExDef(0x00, 0x01, 0x09, 0x01); // on Combi
        MIDI.sendControlChange(81, 127, 1);
        }
        else//false
        {
          Boton2 = !Boton2; 
        sendSysExDef(0x00, 0x01, 0x09, 0x00); // off Combi
        MIDI.sendControlChange(81, 1, 1);
        }
        break;  

// Screen ----------------------------------------------------------------------------------------------------------------------------------------------------------//
   case 270:
        defArray9bytes( 0x4e, 0x01);  // on screen ------ Hoja      
        break;
   case 782:
        defArray9bytes( 0x4e , 0x00);  // off screen ----- Win + Hoja

//Portamento On ------------------- Win + insert -------------------//
   case 723:
        MIDI.sendControlChange(65, 127, 1);
        break; 
//Portamento Off ------------------- Win + Supr -------------------//
   case 794:
        MIDI.sendControlChange(65, 01, 1);
        break; 

// todos reverbs WET        
   case 787: // Todos los reverbs WET.   All the reverbs WET----------- Win + RePag ----------------------------------------------//
        sendSysExDef(0x00, 0x21, 0x08, 0x00); // SRC off combi
        sendSysExDef(0x00, 0x21, 0x07, 0x64); // Reverb105Wet combi
        sendSysExDef(0x00, 0x21, 0x09, 0x64); // Reverb107Wet combi
        sendSysExDef(0x00, 0x21, 0x04, 0x64); // Reverb109Wet combi
        sendSysExDef(0x00, 0x21, 0x06, 0x64); // Reverb113Wet combi
        sendSysExDef(0x00, 0x21, 0x0b, 0x64); // Reverb120Wet combi
        break;                   



//FX STEP SEQ LR SEND Full Vel --------------------------------------- Win + inicio ------------------------------------------------------------------------//  
   case 785: //  
      sendSysExDef(0x00, 0x18, 0x2b, 0x01); // StepSeq_LR
      sendSysExDef(0x00, 0x06, 0x01, 0x3f); // StepSeqVelFull
      break;

//DISTORTED  MFX1
   case 788: // ------------------------------------------------- Win + AvPag ----------------------------------------------------------------------------//
      MIDI.sendControlChange(93, 127, 10);  
      MIDI.sendControlChange(93, 127, 16);  
      MIDI.sendControlChange(93, 0, 1);  
      sendSysExDef(0x00, 0x18, 0x2b, 0x01); // ssLR
      sendSysExDef(0x00, 0x0e, 0x2b, 0x01); // DrumBusLR track 6
      sendSysExDef(0x00, 0x0f, 0x2b, 0x01); // DrumBusLR track 7
      sendSysExDef(0x00, 0x10, 0x2b, 0x01); // DrumBusLR track 8
      sendSysExDef(0x00, 0x1f, 0x00, 0x38); // m1fxS56
      sendSysExDef(0x00, 0x1f, 0x02, 0x01); // MFX1 On
      sendSysExDef(0x00, 0x1f, 0x04, 0x7f); // MFX1 return 127
      break;  

//FX2 Reverb Hall --------------------------------------- Win + Fin ----------------------------------------------------------------------------------------//  
   case 786: //  
      sendSysExDef(0x00, 0x1f, 0x01, 0x69); // m2fx105
      break;

//Boton RealTime Select en Tempo --------------------------------------- Control + Hoja ---------------------------------------------------------------------------------------//  
   case 8462: //  
      sendSysExDef(0x00, 0x01, 0x10, 0x05); // RealTime Select en Tempo
      break;

//DRUM TRIGGER MODE 
   case 64: // 
      sendSysExDef(0x00, 0x05, 0x02, 0x01); // waitKeyboard
      break;
   case 285: // 
      sendSysExDef(0x00, 0x05, 0x02, 0x00); // startInmediat
      sendSysExDef(0x00, 0x05, 0x04, 0x01); // drumsSyncOn    
      break;


// Abierto/Cerrado
//Filtro Cerrado  ------------------- Control + Alt -------------------//
   case 8972:
        MIDI.sendControlChange(74, 10, 1);
        break; 

//Filtro Abierto ----------------------------------------------//
   case 41228:
        MIDI.sendControlChange(74, 64, 1);
        break; 

// Abierto/Todo Abierto
//Filtro TodoAbierto  ------------------- Control + AltGr -------------------//
   case 10506:
        MIDI.sendControlChange(74, 127, 1);
        break; 

//Filtro Abierto --------------------------------------------------------//
   case 41226:
        MIDI.sendControlChange(74, 63, 1);
        break; 


//DRUM KITs Play NOTES --------------------------------------------------------------------------------------------------------------------------------------------------//
//Play Notes Kit 1 Channel 12
  case 400: // 
    sendSysExDef(0x00, 0x14, 0x04, 0x0b);  // Ch12Midi12
    sendSysExDef(0x00, 0x14, 0x21, 0x00);  // ch12Dplay
    ch12DkitsC(0x05, 0x2a);                // ch12DkitBasic1
    sendSysExDef(0x00, 0x14, 0x2b, 0x01);  // ch12LR
    sendSysExDef(0x00, 0x14, 0x02, 0x7f);  // ch12vol127
        break;
//Play Notes Kit 2 Channel 12
  case 397: // 
    sendSysExDef(0x00, 0x14, 0x04, 0x0b);  // Ch12Midi12
    sendSysExDef(0x00, 0x14, 0x21, 0x00);  // ch12Dplay
    ch12DkitsC(0x05, 0x21);                // ch12DkitRock1
    sendSysExDef(0x00, 0x14, 0x2b, 0x01);  // ch12LR
    sendSysExDef(0x00, 0x14, 0x02, 0x7f);  // ch12vol127
        break;   
//Play Notes Kit 3 Channel 12
  case 396: // 
    sendSysExDef(0x00, 0x14, 0x04, 0x0b);  // Ch12Midi12
    sendSysExDef(0x00, 0x14, 0x21, 0x00);  // ch12Dplay
    ch12DkitsC(0x05, 0x23);                // ch12DkitRock2
    sendSysExDef(0x00, 0x14, 0x2b, 0x01);  // ch12LR
    sendSysExDef(0x00, 0x14, 0x02, 0x7f);  // ch12vol127
        break;    

//Play Notes Kit 1B Channel 12 -------------------------------------------------------------------------------------------//
  case 8473: // 
    sendSysExDef(0x00, 0x14, 0x04, 0x0b);  // Ch12Midi12
    sendSysExDef(0x00, 0x14, 0x21, 0x00);  // ch12Dplay
    ch12DkitsC(0x05, 0x27);                // ch12DkitWild
    sendSysExDef(0x00, 0x14, 0x2b, 0x01);  // ch12LR
    sendSysExDef(0x00, 0x14, 0x02, 0x7f);  // ch12vol127
        break;

//Play Notes Kit 2B Channel 12
  case 8465: // 
    sendSysExDef(0x00, 0x14, 0x04, 0x0b);  // Ch12Midi12
    sendSysExDef(0x00, 0x14, 0x21, 0x00);  // ch12Dplay
    ch12DkitsC(0x05, 0x2b);                // ch12DkitBasic2
    sendSysExDef(0x00, 0x14, 0x2b, 0x01);  // ch12LR
    sendSysExDef(0x00, 0x14, 0x02, 0x7f);  // ch12vol127
        break;

//Play Notes Kit 3B Channel 12
  case 8467: // 
    sendSysExDef(0x00, 0x14, 0x04, 0x0b);  // Ch12Midi12
    sendSysExDef(0x00, 0x14, 0x21, 0x00);  // ch12Dplay
    ch12DkitsC(0x05, 0x28);                // ch12DkitPower1
    sendSysExDef(0x00, 0x14, 0x2b, 0x01);  // ch12LR
    sendSysExDef(0x00, 0x14, 0x02, 0x7f);  // ch12vol127
        break;

//Play Notes Kit 4B Channel 12 
  case 8474: // 
    sendSysExDef(0x00, 0x14, 0x04, 0x0b);  // Ch12Midi12
    sendSysExDef(0x00, 0x14, 0x21, 0x00);  // ch12Dplay
    ch12DkitsC(0x09, 0x4c);                // ch12Dkit----- user 460
    sendSysExDef(0x00, 0x14, 0x2b, 0x01);  // ch12LR
    sendSysExDef(0x00, 0x14, 0x02, 0x7f);  // ch12vol127
        break;

//Play Notes Kit 5B Channel 12
  case 8466: // 
    sendSysExDef(0x00, 0x14, 0x04, 0x0b);  // Ch12Midi12
    sendSysExDef(0x00, 0x14, 0x21, 0x00);  // ch12Dplay
    ch12DkitsC(0x09, 0x4d);                // ch12Dkit----- user 461
    sendSysExDef(0x00, 0x14, 0x2b, 0x01);  // ch12LR
    sendSysExDef(0x00, 0x14, 0x02, 0x7f);  // ch12vol127
        break;

//Play Notes Kit 6B Channel 12
  case 8468: // 
    sendSysExDef(0x00, 0x14, 0x04, 0x0b);  // Ch12Midi12
    sendSysExDef(0x00, 0x14, 0x21, 0x00);  // ch12Dplay
    ch12DkitsC(0x09, 0x4e);                // ch12Dkit----- user 462
    sendSysExDef(0x00, 0x14, 0x2b, 0x01);  // ch12LR
    sendSysExDef(0x00, 0x14, 0x02, 0x7f);  // ch12vol127
        break;


// MIDI NOTES----------------------------------------------------------------------------------//
   case 281: // insert
     MIDI.sendNoteOn(60, 110, 12);    // Send a Note (pitch 61, velo 120 on channel 12)
     delay(100);               
     MIDI.sendNoteOff(60, 0, 12);     // Note off
     break; 
   case 273: // inicio
     MIDI.sendNoteOn(57, 110, 12);    
     delay(100);               
     MIDI.sendNoteOff(57, 0, 12);         
     break; 
   case 275: // RePag
     MIDI.sendNoteOn(53, 110, 12);   
     delay(100);               
     MIDI.sendNoteOff(53, 0, 12);     
     break;
   case 282: // Supr
     MIDI.sendNoteOn(69, 120, 12);    
     delay(100);               
     MIDI.sendNoteOff(69, 0, 12);     
     break;
   case 274: // Fin
     MIDI.sendNoteOn(61, 120, 12);    
     delay(100);               
     MIDI.sendNoteOff(61, 0, 12);     
     break;
   case 276: // AvPag
     MIDI.sendNoteOn(51, 127, 12);    
     delay(100);               
     MIDI.sendNoteOff(51, 0, 12);     
     break;

//PLAY/MUTE TRACKs--------------------- ALT y AltGr + Fs Keys -------------------------------------------------------------------------------------//
//Play-Mute Track-------1
  case 2401: // Play F1
    PlayMuteTrks(0x09, 0x00);
    break;
  case 1377: // Mute F1
    PlayMuteTrks(0x09, 0x01);
    break;
//Play-Mute Track-------2
  case 2402: // Play F2
    PlayMuteTrks(0x0a, 0x00);
    break;
  case 1378: // Mute F2
    PlayMuteTrks(0x0a, 0x01);
    break;
//Play-Mute Track-------3
  case 2403: // Play F3
    PlayMuteTrks(0x0b, 0x00);
    break;
  case 1379: // Mute F3
    PlayMuteTrks(0x0b, 0x01);
    break;
//Play-Mute Track-------4
  case 2404: // Play F4
    PlayMuteTrks(0x0c, 0x00);
    break;
  case 1380: // Mute F4
    PlayMuteTrks(0x0c, 0x01);
    break;
//Play-Mute Track-------5
  case 2405: // Play F5
    PlayMuteTrks(0x0d, 0x00);
    break;
  case 1381: // Mute F5
    PlayMuteTrks(0x0d, 0x01);
    break;
//Play-Mute Track-------6
  case 2406: // Play F6
    PlayMuteTrks(0x0e, 0x00);
    break;
  case 1382: // Mute F6
    PlayMuteTrks(0x0e, 0x01);
    break;
//Play-Mute Track-------7
  case 2407: // Play F7
    PlayMuteTrks(0x0f, 0x00);
    break;
  case 1383: // Mute F7
    PlayMuteTrks(0x0f, 0x01);
    break;
//Play-Mute Track-------8
  case 2408: // Play F8
    PlayMuteTrks(0x10, 0x00);
    break;
  case 1384: // Mute F8
    PlayMuteTrks(0x10, 0x01);
    break;
//Play-Mute Track-------9
  case 2409: // Play F9
    PlayMuteTrks(0x11, 0x00);
    break;
  case 1385: // Mute F9
    PlayMuteTrks(0x11, 0x01);
    break;
//Play-Mute Track-------10
  case 2410: // Play F10
    PlayMuteTrks(0x12, 0x00);
    break;
  case 1386: // Mute F10
    PlayMuteTrks(0x12, 0x01);
    break;
//Play-Mute Track-------11
  case 2411: // Play F11
    PlayMuteTrks(0x13, 0x00);
    break;
  case 1387: // Mute F11
    PlayMuteTrks(0x13, 0x01);
    break;
//Play-Mute Track-------12
  case 2412: // Play F12
    PlayMuteTrks(0x14, 0x00);
    break;
  case 1388: // Mute F12
    PlayMuteTrks(0x14, 0x01);
    break;
//Play-Mute Track-------StepSeq
  case 2331: // Play Esc
    PlayMuteTrks(0x18, 0x00);
    break;
  case 1307: // Mute Esc
    PlayMuteTrks(0x18, 0x01);
    break;

//PLAY/MUTE TRACKs la misma Tecla --------------------- Bloqueo Mayuscula -------------------------------------------------------------------------------------//
//Play-Mute Track-------1
  case 37217: // Play F1
    PlayMuteTrks(0x09, 0x00);
    break;
  case 4449: // Mute F1
    PlayMuteTrks(0x09, 0x01);
    break;
//Play-Mute Track-------2
  case 37218: // Play F2
    PlayMuteTrks(0x0a, 0x00);
    break;
  case 4450: // Mute F2
    PlayMuteTrks(0x0a, 0x01);
    break;
//Play-Mute Track-------3
  case 37219: // Play F3
    PlayMuteTrks(0x0b, 0x00);
    break;
  case 4451: // Mute F3
    PlayMuteTrks(0x0b, 0x01);
    break;
//Play-Mute Track-------4
  case 37220: // Play F4
    PlayMuteTrks(0x0c, 0x00);
    break;
  case 4452: // Mute F4
    PlayMuteTrks(0x0c, 0x01);
    break;
//Play-Mute Track-------5
  case 37221: // Play F5
    PlayMuteTrks(0x0d, 0x00);
    break;
  case 4453: // Mute F5
    PlayMuteTrks(0x0d, 0x01);
    break;
//Play-Mute Track-------6
  case 37222: // Play F6
    PlayMuteTrks(0x0e, 0x00);
    break;
  case 4454: // Mute F6
    PlayMuteTrks(0x0e, 0x01);
    break;
//Play-Mute Track-------7
  case 37223: // Play F7
    PlayMuteTrks(0x0f, 0x00);
    break;
  case 4455: // Mute F7
    PlayMuteTrks(0x0f, 0x01);
    break;
//Play-Mute Track-------8
  case 37224: // Play F8
    PlayMuteTrks(0x10, 0x00);
    break;
  case 4456: // Mute F8
    PlayMuteTrks(0x10, 0x01);
    break;
//Play-Mute Track-------9
  case 37225: // Play F9
    PlayMuteTrks(0x11, 0x00);
    break;
  case 4457: // Mute F9
    PlayMuteTrks(0x11, 0x01);
    break;
//Play-Mute Track-------10
  case 37226: // Play F10
    PlayMuteTrks(0x12, 0x00);
    break;
  case 4458: // Mute F10
    PlayMuteTrks(0x12, 0x01);
    break;
//Play-Mute Track-------11
  case 37227: // Play F11
    PlayMuteTrks(0x13, 0x00);
    break;
  case 4459: // Mute F11
    PlayMuteTrks(0x13, 0x01);
    break;
//Play-Mute Track-------12
  case 37228: // Play F12
    PlayMuteTrks(0x14, 0x00);
    break;
  case 4460: // Mute F12
    PlayMuteTrks(0x14, 0x01);
    break;
//Play-Mute Track-------StepSeq
  case 37147: // Play Esc
    PlayMuteTrks(0x18, 0x00);
    break;
  case 4379: // Mute Esc
    PlayMuteTrks(0x18, 0x01);
    break;



//ARPs A ----------------------- Control Key---------------------------------------------------------------------------------------------------------------------//

//  1
   case 8241: 
      sendSysExArpAUp 
        break;                   
// 2
   case 8242:
      sendSysExArpADown
        break;                   
// 3
   case 8243:
      sendSysExArpAAlt1
        break;                   
// 4
   case 8244:
      sendSysExArpAAlt2
        break;      
// 5
   case 8245:
      sendSysExArpARndm
        break;                   
// 6
   case 8246:
      sendSysExArpA0010
        break;                   
// 7
   case 8247:
      sendSysExArpA0026
        break;                   
// 8
   case 8248:
      sendSysExArpA0025
        break;                   
// 9
   case 8249:
      sendSysExArpA0095
        break;                   
// 0
   case 8240:
      sendSysExArpA0118
        break;                   
// ?
   case 8252:
      sendSysExArpA0139
        break;                   
// ¡
   case 8287:
      sendSysExArpA0346
        break;                   
// }
   case 8284:
      sendSysExArpA0350
        break;                   
// Back
   case 8476:
      sendSysExArpA0370
        break;                   
// Q
   case 8273:
      sendSysExArpA0390
        break;                   
// W
   case 8279:
      sendSysExArpA0396
        break;                   
//  E
   case 8261:
      sendSysExArpA0405
        break;                   
// R
   case 8274:
      sendSysExArpA0488
        break;                   
// T
   case 8276:
      sendSysExArpA0473
        break;                   
// Y
   case 8281:
      sendSysExArpA0474
        break;      
// U
   case 8277:
      sendSysExArpA0478
        break;                   
// I
   case 8265:
      sendSysExArpA0482
        break;                   
// O
   case 8271:
      sendSysExArpA0487
        break;                   
// P
   case 8272:
      sendSysExArpA0488
        break;                   
// [
   case 8285:
      sendSysExArpA0489
        break;                   
// ]
   case 8286:
      sendSysExArpA0497
        break;                   
// A
   case 8257:
      sendSysExArpA1024
        break;                   
// S
   case 8275:
      sendSysExArpA1025
        break;                   
// D
   case 8260:
      sendSysExArpA1026
        break;                   
// F
   case 8262:
      sendSysExArpA1027
        break;                   
// G
   case 8263:
      sendSysExArpA1028
        break;                   
// H
   case 8264:
      sendSysExArpA1029
        break;                 
// J
   case 8266:
      sendSysExArpA1030
        break;                   
// K
   case 8267:
      sendSysExArpA1031
        break;                   
// L
   case 8268:
      sendSysExArpA1032
        break;                   
// Ñ
   case 8283:
      sendSysExArpA1033
        break;      
// {
   case 8250:
      sendSysExArpA1034
        break;                           
// <>
   case 8331:
      sendSysExArpA1081
        break;                   
// Z
   case 8282:
      sendSysExArpA1082
        break;                   
// X
   case 8280:
      sendSysExArpA1083
        break;                   
// C
   case 8259:
      sendSysExArpA1084 
        break;                   
// V
   case 8278:
      sendSysExArpA1085
        break;                   
// B
   case 8258:
      sendSysExArpA1086
        break;                   
// N
   case 8270:
      sendSysExArpA1087
        break;                   
// M
   case 8269:
      sendSysExArpA1088
        break;                   
// ,
   case 8251:
      sendSysExArpA1089
        break;                   
// .
   case 8253:
      sendSysExArpA1090
        break;                   
// _
   case 8254:
      sendSysExArpA1091
        break; 
        
//ARPs B ----------------------- Control Key + Shift key ---------------------------------------------------------------------------------------------------------------------//

//  1
   case 24625: 
      sendSysExArpBUp 
        break;                   
// 2
   case 24626:
      sendSysExArpBDown
        break;                   
// 3
   case 24627:
      sendSysExArpBAlt1
        break;                   
// 4
   case 24628:
      sendSysExArpBAlt2
        break;      
// 5
   case 24629:
      sendSysExArpBRndm
        break;                   
// 6
   case 24630:
      sendSysExArpB0010
        break;                   
// 7
   case 24631:
      sendSysExArpB0026
        break;                   
// 8
   case 24632:
      sendSysExArpB0025
        break;                   
// 9
   case 24633:
      sendSysExArpB0095
        break;                   
// 0
   case 24624:
      sendSysExArpB0118
        break;                   
// ?
   case 24636:
      sendSysExArpB0139
        break;                   
// ¡
   case 24671:
      sendSysExArpB0346
        break;                   
// }
   case 24668:
      sendSysExArpB0350
        break;                   
// Back
   case 24860:
      sendSysExArpB0370
        break;                   
// Q
   case 24657:
      sendSysExArpB0390
        break;                   
// W
   case 24663:
      sendSysExArpB0396
        break;                   
//  E
   case 24645:
      sendSysExArpB0405
        break;                   
// R
   case 24658:
      sendSysExArpB0488
        break;                   
// T
   case 24660:
      sendSysExArpB0473
        break;                   
// Y
   case 24665:
      sendSysExArpB0474
        break;      
// U
   case 24661:
      sendSysExArpB0478
        break;                   
// I
   case 24649:
      sendSysExArpB0482
        break;                   
// O
   case 24655:
      sendSysExArpB0487
        break;                   
// P
   case 24656:
      sendSysExArpB0488
        break;                   
// [
   case 24669:
      sendSysExArpB0489
        break;                   
// ]
   case 24670:
      sendSysExArpB0497
        break;                   
// A
   case 24641:
      sendSysExArpB1024
        break;                   
// S
   case 24659:
      sendSysExArpB1025
        break;                   
// D
   case 24644:
      sendSysExArpB1026
        break;                   
// F
   case 24646:
      sendSysExArpB1027
        break;                   
// G
   case 24647:
      sendSysExArpB1028
        break;                   
// H
   case 24648:
      sendSysExArpB1029
        break;                 
// J
   case 24650:
      sendSysExArpB1030
        break;                   
// K
   case 24651:
      sendSysExArpB1031
        break;                   
// L
   case 24652:
      sendSysExArpB1032
        break;                   
// Ñ
   case 24667:
      sendSysExArpB1033
        break;      
// {
   case 24634:
      sendSysExArpB1034
        break;                           
// <>
   case 24715:
      sendSysExArpB1081
        break;                   
// Z
   case 24666:
      sendSysExArpB1082
        break;                   
// X
   case 24664:
      sendSysExArpB1083
        break;                   
// C
   case 24643:
      sendSysExArpB1084 
        break;                   
// V
   case 24662:
      sendSysExArpB1085
        break;                   
// B
   case 24642:
      sendSysExArpB1086
        break;                   
// N
   case 24654:
      sendSysExArpB1087
        break;                   
// M
   case 24653:
      sendSysExArpB1088
        break;                   
// ,
   case 24635:
      sendSysExArpB1089
        break;                   
// .
   case 24637:
      sendSysExArpB1090
        break;                   
// _
   case 24638:
      sendSysExArpB1091
        break; 


// ARP A ----------------------------- CONTROL Key ----------------------------------------------------------------------------------------------------------------------------------------------//
//ARP A ON/OFF 
   case 8235: // Intro
      ArpAcontrols(0x0a, 0x01);  // Arp A on
      break;
   case 8227: // 3
      ArpAcontrols(0x0a, 0x00);  // Arp A Off
      break;                        

//ARP A SORT 
   case 8231: // 7
      ArpAcontrols(0x06, 0x01);  // Arp A Sort On
      break;
   case 8232: // 8
      ArpAcontrols(0x06, 0x00);  // Arp A Sort Off
      break;
      
//LATCH A ARP 
   case 8224: // 0
      ArpAcontrols(0x07, 0x01);  // Arp A Latch on
      break;
   case 8234: // Supr 
      ArpAcontrols(0x07, 0x00);  // Arp A Latch Off
      break;
      
//ARP A KEY SYNC 
   case 8228: // 4
      ArpAcontrols(0x08, 0x01);  // Arp A KeySync On
      break;
   case 8229: // 5
      ArpAcontrols(0x08, 0x00);  // Arp A KeySync Off
      break;

//ARP A KEYBOARD 
   case 8225: // 1
      ArpAcontrols(0x09, 0x01);  // Arp A Keyboard On
      break;
   case 8226: // 2
      ArpAcontrols(0x09, 0x00);  // Arp A Keyboard Off
      break;
            
//ARP A  OCTAVE 
   case 8239: // /
      ArpAcontrols(0x02, 0x00);  // Arp A octave 1
      break;
   case 8238: // *
      ArpAcontrols(0x02, 0x01);  // Arp A octave 2
      break;
   case 8237: // -
      ArpAcontrols(0x02, 0x02);  // Arp A octave 3
      break;    

//ARP A  RESOLUTION 
   case 8233: //  9 
      ArpAcontrols(0x01, 0x03);  // Arp A Res Semicorchea
      break;
   case 8230: //  6
      ArpAcontrols(0x01, 0x07);  // Arp A Res Negra
      break;        
   case 8236: //  +
      ArpAcontrols(0x01, 0x05);  // Arp A Res Corchea
      break;

// ARP B ---------------------------------- WIN Key -----------------------------------------------------------------------------------------------------------------------------------------//
//ARP B ON/OFF 
   case 555: // Intro
      ArpBcontrols(0x0a, 0x01);  // Arp B on
      break;
   case 547: // 3
      ArpBcontrols(0x0a, 0x00);  // Arp B Off
      break;                        

//ARP B SORT 
   case 551: // 7
      ArpBcontrols(0x06, 0x01);  // Arp B Sort On
      break;
   case 552: // 8
      ArpBcontrols(0x06, 0x00);  // Arp B Sort Off
      break;
      
//LATCH B ARP 
   case 544: // 0
      ArpBcontrols(0x07, 0x01);  // Arp B Latch on
      break;
   case 554: // Supr 
      ArpBcontrols(0x07, 0x00);  // Arp B Latch Off
      break;
      
//ARP B KEY SYNC 
   case 548: // 4
      ArpBcontrols(0x08, 0x01);  // Arp B KeySync On
      break;
   case 549: // 5
      ArpBcontrols(0x08, 0x00);  // Arp B KeySync Off
      break;

//ARP B KEYBOARD 
   case 545: // 1
      ArpBcontrols(0x09, 0x01);  // Arp B Keyboard On
      break;
   case 546: // 2
      ArpBcontrols(0x09, 0x00);  // Arp B Keyboard Off
      break;
            
//ARP B  OCTAVE 
   case 559: // /
      ArpBcontrols(0x02, 0x00);  // Arp B octave 1
      break;
   case 558: // *
      ArpBcontrols(0x02, 0x01);  // Arp B octave 2
      break;
   case 557: // -
      ArpBcontrols(0x02, 0x02);  // Arp B octave 3
      break;    

//ARP B  RESOLUTION 
   case 553: //  9 
      ArpBcontrols(0x01, 0x03);  // Arp B Res Semicorchea
      break;
   case 550: //  6
      ArpBcontrols(0x01, 0x07);  // Arp B Res Negra
      break;        
   case 556: //  +
      ArpBcontrols(0x01, 0x05);  // Arp B Res Corchea
      break;



//DRUM Patterns Banco A -----------------------------------------------------ALT key ------------------------------------------------------------------------//

// 1
   case 2097:
        sendSysExDrumC794
        break;                   
// 2
   case 2098:
        sendSysExDrumC745
        break;                   
// 3
   case 2099:
        sendSysExDrumC734
        break;                   
// 4
   case 2100:
        sendSysExDrumC730
        break;      
// 5
   case 2101:
        sendSysExDrumC731
        break;                   
// 6
   case 2102:
        sendSysExDrumC382
        break;                   
// 7
   case 2103:
        sendSysExDrumC383
        break;                   
// 8
   case 2104:
        sendSysExDrumC384
        break;                   
// 9
   case 2105:
        sendSysExDrumC393
        break;                   
// 0
   case 2096:
        sendSysExDrumC394
        break;                   
// ?
   case 2108:
        sendSysExDrumC396
        break;                   
// ¡
   case 2143:
        sendSysExDrumC398
        break;                   
// }
   case 2140:
        sendSysExDrumC400
        break;                   
// Back
   case 2332:
        sendSysExDrumC404
        break;                   
// Q
   case 2129:
        sendSysExDrumC405
        break;                   
// W
   case 2135:
        sendSysExDrumC628
        break;                   
// E
   case 2117:
        sendSysExDrumC634
        break;                   
// R
   case 2130:
        sendSysExDrumC640
        break;                   
// T
   case 2132:
        sendSysExDrumC641
        break;                   
// Y
   case 2137:
        sendSysExDrumC642
        break;                   
// U
   case 2133:
        sendSysExDrumC643
        break;                   
// I
   case 2121:
        sendSysExDrumC652
        break;                   
// O
   case 2127:
        sendSysExDrumC667
        break;                   
// P
   case 2128:
        sendSysExDrumC675
        break;   
// [
   case 2141:
        sendSysExDrumC422
        break;                   
// ]
   case 2142:
        sendSysExDrumC423
        break;                   
// A
   case 2113:
        sendSysExDrumC438
        break;                   
// S
   case 2131:
        sendSysExDrumC451
        break;
// D
   case 2116:
        sendSysExDrumC460
        break;
// F
   case 2118:
        sendSysExDrumC485
        break;
// G
   case 2119:
        sendSysExDrumC489
        break;
// H
   case 2120:
        sendSysExDrumC504
        break;
// J
   case 2122:
        sendSysExDrumC558
        break;
// K
   case 2123:
        sendSysExDrumC569
        break;
// L
   case 2124:
        sendSysExDrumC570
        break;     
// Ñ
   case 2139:
        sendSysExDrumC572
        break;
// {
   case 2106:
        sendSysExDrumC764
        break;
// <>
   case 2187:
        sendSysExDrumC763
        break;
// Z
   case 2138:
        sendSysExDrumC765
        break;
// X
   case 2136:
        sendSysExDrumC766
        break;
// C
   case 2115:
        sendSysExDrumC768
        break;
// V
   case 2134:
        sendSysExDrumC769
        break;
// B
   case 2114:
        sendSysExDrumC773
        break;
// N
   case 2126:
        sendSysExDrumC774
        break;     
// M
   case 2125:
        sendSysExDrumC776
        break;
// ,
   case 2107:
        sendSysExDrumC782
        break;        
// .
   case 2109:
        sendSysExDrumC783
        break;
// -
   case 2110:
        sendSysExDrumC784
        break;

//DRUM Patterns Banco B ----------------------------------------------------- Win + ALT key ------------------------------------------------------------------------//

// 1
   case 2609:
        sendSysExDrumC413
        break;                 
// 2
   case 2610:
        sendSysExDrumC414
        break;                   
// 3
   case 2611:
        sendSysExDrumC415
        break;                   
// 4
   case 2612:
        sendSysExDrumC416
        break;      
// 5
   case 2613:
        sendSysExDrumC417
        break;                   
// 6
   case 2614:
        sendSysExDrumC418
        break;                   
// 7
   case 2615:
        sendSysExDrumC436
        break;                   
// 8
   case 2616:
        sendSysExDrumC432
        break;                   
// 9
   case 2617:
        sendSysExDrumC433
        break;                   
// 0
   case 2608:
        sendSysExDrumC434
        break;                   
// ?
   case 2620:
        sendSysExDrumC435
        break;                   
// ¡
   case 2655:
        sendSysExDrumC437
        break;                   
// }
   case 2652:
        sendSysExDrumC438
        break;                   
// Back
   case 2844:
        sendSysExDrumC439
        break;                   
// Q
   case 2641:
        sendSysExDrumC440
        break;                   
// W
   case 2647:
        sendSysExDrumC441
        break;                   
// E
   case 2629:
        sendSysExDrumC442
        break;                   
// R
   case 2642:
        sendSysExDrumC443
        break;                   
// T
   case 2644:
        sendSysExDrumC444
        break;                   
// Y
   case 2649:
        sendSysExDrumC449
        break;                   
// U
   case 2645:
        sendSysExDrumC399
        break;                   
// I
   case 2633:
        sendSysExDrumC696
        break;                   
// O
   case 2639:
        sendSysExDrumC697
        break;                   
// P
   case 2640:
        sendSysExDrumC698
        break;   
// [
   case 2653:
        sendSysExDrumC700
        break;                   
// ]
   case 2654:
        sendSysExDrumC775
        break;                   
// A
   case 2625:
        sendSysExDrumC776
        break;                   
// S
   case 2643:
        sendSysExDrumC777
        break;
// D
   case 2628:
        sendSysExDrumC778
        break;
// F
   case 2630:
        sendSysExDrumC779
        break;
// G
   case 2631:
        sendSysExDrumC796
        break;
// H
   case 2632:
        sendSysExDrumC261
        break;
// J
   case 2634:
        sendSysExDrumC262
        break;
// K
   case 2635:
        sendSysExDrumC263
        break;
// L
   case 2636:
        sendSysExDrumC264
        break;     
// Ñ
   case 2651:
        sendSysExDrumC265
        break;
// {
   case 2618:
        sendSysExDrumC266
        break;
// <>
   case 2699:
        sendSysExDrumC267
        break;
// Z
   case 2650:
        sendSysExDrumC268
        break;
// X
   case 2648:
        sendSysExDrumC269
        break;
// C
   case 2627:
        sendSysExDrumC270
        break;
// V
   case 2646:
        sendSysExDrumC272
        break;
// B
   case 2626:
        sendSysExDrumC274
        break;
// N
   case 2638:
        sendSysExDrumC278
        break;     
// M
   case 2637:
        sendSysExDrumC280
        break;
// ,
   case 2619:
        sendSysExDrumC785
        break;        
// .
   case 2621:
        sendSysExDrumC786
        break;
// -
   case 2622:
        sendSysExDrumC787
        break;        

//DRUM KITS-----------------------------------------------------WIN key Track 7--------------------------------------------------------------------------------------------------------------------//

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
    ShiftDrumsT924C
    break;
//ShiftDrum-23
  case 50: //
    ShiftDrumsT923C
    break;
//ShiftDrum-22
  case 51: // 
    ShiftDrumsT922C
    break;
//ShiftDrum-21
  case 52: //
    ShiftDrumsT921C
    break;
//ShiftDrum-20
  case 53: // 
    ShiftDrumsT920C
    break;
//ShiftDrum-19
  case 54: //
    ShiftDrumsT919C
    break;
//ShiftDrum-18
  case 55: // 
    ShiftDrumsT918C
    break;
//ShiftDrum-17
  case 56: //
    ShiftDrumsT917C
    break;
//ShiftDrum-16
  case 57: //  
    ShiftDrumsT916C
    break;
//ShiftDrum-15
  case 48: //
    ShiftDrumsT915C
    break;
//ShiftDrum-14
  case 60: // 
    ShiftDrumsT914C
    break;
//ShiftDrum-13
  case 95: // 
    ShiftDrumsT913C
    break;
//ShiftDrum-12
  case 92: //
    ShiftDrumsT912C
    break;
//ShiftDrum-11
  case 284: // 
    ShiftDrumsT911C
    break;
//ShiftDrum-10
  case 81: // 
    ShiftDrumsT910C
    break;
//ShiftDrum-09
  case 87: // 
    ShiftDrumsT909C
    break;
//ShiftDrum-08
  case 69: // 
    ShiftDrumsT908C
    break;
//ShiftDrum-07
  case 82: // 
    ShiftDrumsT907C
    break;
//ShiftDrum-06
  case 84: // 
    ShiftDrumsT906C
    break;
//ShiftDrum-05
  case 89: // 
    ShiftDrumsT905C
    break;
//ShiftDrum-04
  case 85: // 
    ShiftDrumsT904C
    break;
//ShiftDrum-03
  case 73: // 
    ShiftDrumsT903C
    break;
//ShiftDrum-02
  case 79: // 
    ShiftDrumsT902C
    break;
//ShiftDrum-01
  case 80: // 
    ShiftDrumsT901C
    break;
//ShiftDrum 00
  case 286: //
    ShiftDrumsT000C
    break;
//ShiftDrum 01
  case 93: // 
    ShiftDrumsT001C
    break;
//ShiftDrum 02
  case 94: // 
    ShiftDrumsT002C
    break;
//ShiftDrum 03
  case 65: // 
    ShiftDrumsT003C
    break;
//ShiftDrum 04
  case 83: // 
    ShiftDrumsT004C
    break;
//ShiftDrum 05
  case 68: // 
    ShiftDrumsT005C
    break;
//ShiftDrum 06
  case 70: // 
    ShiftDrumsT006C
    break;
//ShiftDrum 07
  case 71: // 
    ShiftDrumsT007C
    break;
//ShiftDrum 08
  case 72: // 
    ShiftDrumsT008C
    break;
//ShiftDrum 09
  case 74: // 
    ShiftDrumsT009C
    break;
//ShiftDrum 10
  case 75: // 
    ShiftDrumsT010C
    break;
//ShiftDrum 11
  case 76: //
    ShiftDrumsT011C
    break;
//ShiftDrum 12
  case 91: // 
    ShiftDrumsT012C
    break;
//ShiftDrum 13
  case 58: // 
    ShiftDrumsT013C
    break;
//ShiftDrum 14
  case 139: //
    ShiftDrumsT014C
    break;
//ShiftDrum 15
  case 90: //
    ShiftDrumsT015C
    break;
//ShiftDrum 16
  case 88: //
    ShiftDrumsT016C
    break;
//ShiftDrum 17
  case 67: // 
    ShiftDrumsT017C
    break;
//ShiftDrum 18
  case 86: // 
    ShiftDrumsT018C
    break;
//ShiftDrum 19
  case 66: // 
    ShiftDrumsT019C
    break;
//ShiftDrum 20
  case 78: // 
    ShiftDrumsT020C
    break;
//ShiftDrum 21
  case 77: //
    ShiftDrumsT021C
    break;
//ShiftDrum 22
  case 59: //
    ShiftDrumsT022C
    break;
//ShiftDrum 23
  case 61: //
    ShiftDrumsT023C
    break;
//ShiftDrum 24
  case 62: // 
    ShiftDrumsT024C
    break; 



//Step Seq DRUM KITS  --------------------------Shift ------------------------------------------------//

//ElectronicSSDrumKit 1
   case 16433:
        sendSysExSSdrumKitC015 
        break;                   
//ElectronicSSDrumKit 2
   case 16434:
        sendSysExSSdrumKitC016 
        break;                   
//ElectronicSSDrumKit 3
   case 16435:
        sendSysExSSdrumKitC032 
        break;                   
//ElectronicSSDrumKit 4
   case 16436:
        sendSysExSSdrumKitC033 
        break;      
//ElectronicSSDrumKit 5
   case 16437:
        sendSysExSSdrumKitC013 
        break;                   
//ElectronicSSDrumKit 6
   case 16438:
        sendSysExSSdrumKitC021 
        break;                   
//ElectronicSSDrumKit 7
   case 16439:
        sendSysExSSdrumKitC023 
        break;                   
//ElectronicSSDrumKit 8
   case 16440:
        sendSysExSSdrumKitC026 
        break;                   
//ElectronicSSDrumKit 9
   case 16441:
        sendSysExSSdrumKitC027 
        break;                   
//ElectronicSSDrumKit 10
   case 16432:
        sendSysExSSdrumKitC028 
        break;                   
//ElectronicSSDrumKit 11
   case 16444:
        sendSysExSSdrumKitC029 
        break;                   
//ElectronicSSDrumKit 12
   case 16479:
        sendSysExSSdrumKitC030 
        break;                   
//ElectronicSSDrumKit 13
   case 16476:
        sendSysExSSdrumKitC024 
        break;                   
//ElectronicSSDrumKit 14
   case 16668:
        sendSysExSSdrumKitC025 
        break;                           
//AcousticSSDrumKit 1
   case 16465:
        sendSysExSSdrumKitC000 
        break;                   
//AcousticSSDrumKit 2
   case 16471:
        sendSysExSSdrumKitC002 
        break;
//AcousticSSDrumKit 3
   case 16453:
        sendSysExSSdrumKitC003 
        break;
//AcousticSSDrumKit 4
   case 16466:
        sendSysExSSdrumKitC005 
        break;
//AcousticSSDrumKit 5
   case 16468:
        sendSysExSSdrumKitC006 
        break;
//AcousticSSDrumKit 6
   case 16473:
        sendSysExSSdrumKitC007 
        break;
//AcousticSSDrumKit 7
   case 16469:
        sendSysExSSdrumKitC008 
        break;
//AcousticSSDrumKit 8
   case 16457:
        sendSysExSSdrumKitC009 
        break;
//AcousticSSDrumKit 9
   case 16463:
        sendSysExSSdrumKitC010 
        break;     
//AcousticSSDrumKit 10
   case 16464:
        sendSysExSSdrumKitC011 
        break;
//AcousticSSDrumKit 11
   case 16477:
        sendSysExSSdrumKitC018 
        break;
//AcousticSSDrumKit 12
   case 16478:
        sendSysExSSdrumKitC017 
        break;           

//SS BassDrums
   case 16523://<>
        BassDrum01C
        break;
   case 16474://Z
        BassDrum02C
        break;
   case 16472://X
        BassDrum03C
        break;
   case 16451://C
        BassDrum04C
        break;
   case 16470://V
        BassDrum05C
        break;
   case 16450://B
        BassDrum06C
        break;
   case 16462://N
        BassDrum07C
        break;     
   case 16461://M
        BassDrum08C
        break;
   case 16443://;
        BassDrum09C
        break;        
   case 16445://:
        BassDrum10C
        break;
   case 16446://-
        BassDrum11C
        break;        

//SS Snare1Drums
   case 16449://<>
        Snare1Drum01C
        break;
   case 16467://Z
        Snare1Drum02C
        break;
   case 16452://X
        Snare1Drum03C
        break;
   case 16454://C
        Snare1Drum04C
        break;
   case 16455://V
        Snare1Drum05C
        break;
   case 16456://B
        Snare1Drum06C
        break;
   case 16458://N
        Snare1Drum07C
        break;     
   case 16459://M
        Snare1Drum08C
        break;
   case 16460://;
        Snare1Drum09C
        break;        
   case 16475://:
        Snare1Drum10C
        break;
   case 16442://-
        Snare1Drum11C
        break;                


// STEP SEQ BUS SELECT ------------------- Shift +  F9 to Apagar -------------------------------//        

   case 16745: // DKIT
        sendSysExDef(0x00, 0x18, 0x2b, 0x00); //
        break;
   case 16746: // L/R
        sendSysExDef(0x00, 0x18, 0x2b, 0x01); //
        break;
   case 16747: // IFX1
        sendSysExDef(0x00, 0x18, 0x2b, 0x02); //
        break;
   case 16748: // IFX2
        sendSysExDef(0x00, 0x18, 0x2b, 0x03); //
        break;
   case 16784: // IFX3
        sendSysExDef(0x00, 0x18, 0x2b, 0x04); //
        break;
   case 16781: // IFX4
        sendSysExDef(0x00, 0x18, 0x2b, 0x05); //
        break;
   case 16780: // IFX5
        sendSysExDef(0x00, 0x18, 0x2b, 0x06); //
        break;       

//SHIFT STEP SEQ ------------------------------------ Control + Win ----------------------------------------------------------------//

  case 8753: // 1
    ShiftStepSeqPositiveC(0x01); // +1
    break;
  case 8754: // 2
    ShiftStepSeqPositiveC(0x02); // +2
    break;
  case 8755: // 3
    ShiftStepSeqPositiveC(0x03); // +3
    break;
  case 8756: // 4
    ShiftStepSeqPositiveC(0x04); // +4
    break;
  case 8757: // 5
    ShiftStepSeqPositiveC(0x05); // +5
    break;
  case 8758: // 6
    ShiftStepSeqPositiveC(0x06); // +6
    break;
  case 8759: // 7 
    ShiftStepSeqPositiveC(0x07); // +7
    break;
  case 8760: // 8
    ShiftStepSeqPositiveC(0x08); // +8
    break;
  case 8761: // 9
    ShiftStepSeqPositiveC(0x09); // +9
    break;
  case 8752: // 0
    ShiftStepSeqPositiveC(0x0a); // +10
    break;
  case 8764: // ?
    ShiftStepSeqPositiveC(0x0b); // +11
    break;
  case 8799: // ¿
    ShiftStepSeqPositiveC(0x0c); // +12
    break;
  case 8796: // }
    ShiftStepSeqPositiveC(0x0d); // +13
    break;
  case 8988: // Back
    ShiftStepSeqPositiveC(0x0e); // +14
    break;
  case 8785: // Q
    ShiftStepSeqPositiveC(0x0f); // +15
    break;
  case 8791: // W
    ShiftStepSeqPositiveC(0x10); // +16
    break;
  case 8773: // E
    ShiftStepSeqPositiveC(0x11); // +17
    break;
  case 8786: // R
    ShiftStepSeqPositiveC(0x12); // +18
    break;
  case 8788: // T
    ShiftStepSeqPositiveC(0x13); // +19
    break;
  case 8793: // Y
    ShiftStepSeqPositiveC(0x14); // +20
    break;
  case 8789: // U
    ShiftStepSeqPositiveC(0x15); // +21
    break;
  case 8777: // i
    ShiftStepSeqPositiveC(0x16); // +22
    break;
  case 8990: // ENTER
    ShiftStepSeqPositiveC(0x00); // +00
    break;
  case 8783: // O
    ShiftStepSeqPositiveC(0x17); // +23
    break;
  case 8784: // P
    ShiftStepSeqPositiveC(0x18); // +24
    break;
  case 8797: // [
    ShiftStepSeqPositiveC(0x19); // +25
    break;
  case 8798: // +
    ShiftStepSeqPositiveC(0x1a); // +26
    break;
  case 8769: // A
    ShiftStepSeqPositiveC(0x1b); // +27
    break;
  case 8787: // S
    ShiftStepSeqPositiveC(0x1c); // +28
    break;
  case 8772: // D
    ShiftStepSeqPositiveC(0x1d); // +29
    break;
  case 8774: // F
    ShiftStepSeqPositiveC(0x1e); // +30
    break;
  case 8775: // G
    ShiftStepSeqPositiveC(0x1f); // +31
    break;
  case 8776: // H
    ShiftStepSeqPositiveC(0x20); // +32
    break;
  case 8778: // J
    ShiftStepSeqPositiveC(0x21); // +33
    break;
  case 8779: // K
    ShiftStepSeqPositiveC(0x22); // +34
    break;
  case 8780: // L
    ShiftStepSeqPositiveC(0x23); // +35
    break;
  case 8795: // Ñ
    ShiftStepSeqPositiveC(0x24); // +36
    break;
  case 8762: // {
    ShiftStepSeqPositiveC(0x25); // +37
    break;
  case 8843: // <>
    ShiftStepSeqPositiveC(0x26); // +38
    break;
  case 8794: // Z
    ShiftStepSeqPositiveC(0x27); // +39
    break;
  case 8792: // X
    ShiftStepSeqPositiveC(0x28); // +40
    break;
  case 8771: // C
    ShiftStepSeqPositiveC(0x29); // +41
    break;
  case 8790: // V
    ShiftStepSeqPositiveC(0x2a); // +42
    break;
  case 8770: // B
    ShiftStepSeqPositiveC(0x2b); // +43
    break;
  case 8782: // N
    ShiftStepSeqPositiveC(0x2c); // +44
    break;
  case 8781: // M
    ShiftStepSeqPositiveC(0x2d); // +45
    break;
  case 8763: // ,
    ShiftStepSeqPositiveC(0x2e); // +46
    break; 
  case 8765: // .
    ShiftStepSeqPositiveC(0x2f); // +47
    break; 
  case 8766: // -
    ShiftStepSeqPositiveC(0x30); // +48
    break; 





// Step Seq Pads Patterns

   case 42: //---------------- ( . )  ----------Hi part Muted-----------------------//
        sendSysExDef(0x00, 0x08, 0x01, 0x10); // SSLength16C
        sendSysExDef(0x00, 0x07, 0x00, 0x03); // CseemicorcheaSS
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

   case 32: //----------------  0  ----------All Muted-----------------------//
        sendSysExDef(0x00, 0x08, 0x01, 0x10); // SSLength16C
        sendSysExDef(0x00, 0x07, 0x00, 0x03); // CseemicorcheaSS
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
   //     sendSysExDef(0x00, 0x08, 0x01, 0x10); // SSLength16C
   //     sendSysExDef(0x00, 0x07, 0x00, 0x03); // CseemicorcheaSS
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
   //     sendSysExDef(0x00, 0x08, 0x01, 0x10); // SSLength16C
   //     sendSysExDef(0x00, 0x07, 0x00, 0x03); // CseemicorcheaSS
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
   //     sendSysExDef(0x00, 0x08, 0x01, 0x10); // SSLength16C
   //     sendSysExDef(0x00, 0x07, 0x00, 0x03); // CseemicorcheaSS
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
//        sendSysExDef(0x00, 0x08, 0x01, 0x10); // SSLength16C
//        sendSysExDef(0x00, 0x07, 0x00, 0x03); // CseemicorcheaSS
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
//        sendSysExDef(0x00, 0x08, 0x01, 0x10); // SSLength16C
//        sendSysExDef(0x00, 0x07, 0x00, 0x03); // CseemicorcheaSS
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
//        sendSysExDef(0x00, 0x08, 0x01, 0x10); // SSLength16C
//        sendSysExDef(0x00, 0x07, 0x00, 0x03); // CseemicorcheaSS
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
 //        sendSysExDef(0x00, 0x08, 0x01, 0x10); // SSLength16C
//        sendSysExDef(0x00, 0x07, 0x00, 0x03); // CseemicorcheaSS
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
//        sendSysExDef(0x00, 0x08, 0x01, 0x10); // SSLength16C
//        sendSysExDef(0x00, 0x07, 0x00, 0x03); // CseemicorcheaSS
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
   case 41: //----------------  9  ----------HiHat 1,3,5,7,9,11,13,15 -----------------------//
        sendSysExDef(0x00, 0x08, 0x19, 0x00); // SFX en All
        sendSysExDef(0x00, 0x08, 0x18, 0x36); // F#3
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
   case 47: //----------------  /  ----------HiHat 1,3,5,7,9,11,13,15-----------------------//
   //     sendSysExDef(0x00, 0x08, 0x01, 0x10); // SSLength16C
   //     sendSysExDef(0x00, 0x07, 0x00, 0x03); // CseemicorcheaSS
        sendSysExDef(0x00, 0x08, 0x19, 0x00); // SFX en All
        sendSysExDef(0x00, 0x08, 0x18, 0x36); // F#3
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
   //     sendSysExDef(0x00, 0x08, 0x01, 0x10); // SSLength16C
   //     sendSysExDef(0x00, 0x07, 0x00, 0x03); // CseemicorcheaSS
        sendSysExDef(0x00, 0x08, 0x19, 0x00); // SFX en All
        sendSysExDef(0x00, 0x08, 0x18, 0x36); // F#3
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
   case 45: //---------------- ( - ) ----------HiHat 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 -----------------------//
   //     sendSysExDef(0x00, 0x08, 0x01, 0x10); // SSLength16C
   //     sendSysExDef(0x00, 0x07, 0x00, 0x03); // CseemicorcheaSS
        sendSysExDef(0x00, 0x08, 0x19, 0x00); // SFX en All
        sendSysExDef(0x00, 0x08, 0x18, 0x41); // F#3
        sendSysExDef(0x00, 0x08, 0x15, 0x00); // Perc1 en All
        sendSysExDef(0x00, 0x08, 0x14, 0x43); // F#3        
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
   case 44: //----------------  /  ----------HiHat 1,3,5,7,9,11,13,15-----------------------//
   //     sendSysExDef(0x00, 0x08, 0x01, 0x10); // SSLength16C
   //     sendSysExDef(0x00, 0x07, 0x00, 0x03); // CseemicorcheaSS
        sendSysExDef(0x00, 0x08, 0x19, 0x00); // SFX en All
        sendSysExDef(0x00, 0x08, 0x18, 0x36); // F#3
        sendSysExDef(0x00, 0x08, 0x15, 0x00); // Perc1 en All
        sendSysExDef(0x00, 0x08, 0x14, 0x43); // F#3    
        SSpatternsPad1Hi  (0x10); // 1
        SSpatternsPad2Hi  (0x00); // 2
        SSpatternsPad3Hi  (0x14); // 3
        SSpatternsPad4Hi  (0x00); // 4
        SSpatternsPad5Hi  (0x10); // 5
        SSpatternsPad6Hi  (0x00); // 6
        SSpatternsPad7Hi  (0x14); // 7
        SSpatternsPad8Hi  (0x00); // 8
        SSpatternsPad9Hi  (0x10); // 9
        SSpatternsPad10Hi (0x00); // 10
        SSpatternsPad11Hi (0x14); // 11
        SSpatternsPad12Hi (0x00); // 12
        SSpatternsPad13Hi (0x10); // 13
        SSpatternsPad14Hi (0x00); // 14
        SSpatternsPad15Hi (0x14); // 15
        SSpatternsPad16Hi (0x00); // 16 
        break;                                                 

   case 43: //----------------  /  ----------HiHat 1,3,5,7,9,11,13,15-----------------------//
//     sendSysExDef(0x00, 0x08, 0x01, 0x10); // SSLength16C
   //     sendSysExDef(0x00, 0x07, 0x00, 0x03); // CseemicorcheaSS
        sendSysExDef(0x00, 0x08, 0x19, 0x00); // SFX en All
        sendSysExDef(0x00, 0x08, 0x18, 0x36); // F#3
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


// 37 Programs (letras y numeros)--------------- AltGr------------------// 
   case 1073: // 1
          sendMIDIcombiChange(91); 
          break;           
   case 1074: // 2
          sendMIDIcombiChange(92);  
          break;                     
   case 1075: // 3
          sendMIDIcombiChange(93); 
          break;           
   case 1076: // 4
          sendMIDIcombiChange(94);  
          break; 
   case 1077: // 5
          sendMIDIcombiChange(95);    
          break;           
   case 1078: // 6
          sendMIDIcombiChange(96);  
          break;                     
   case 1079: // 7
          sendMIDIcombiChange(97);    
          break;           
   case 1080: // 8
          sendMIDIcombiChange(98);     
          break; 
   case 1081: // 9
          sendMIDIcombiChange(99); 
          break;           
   case 1072: // 0
          sendMIDIcombiChange(100);    
          break;                     
   case 1105: // Q
          sendMIDIcombiChange(101);    
          break;           
   case 1111: // W
          sendMIDIcombiChange(102);   
          break; 
   case 1093: // E
          sendMIDIcombiChange(103);    
          break;           
   case 1106: // R
          sendMIDIcombiChange(104);   
          break;                     
   case 1108: // T
         sendMIDIcombiChange(105); 
          break;           
   case 1113: // Y
          sendMIDIcombiChange(106);  
          break;  
   case 1109: // U
          sendMIDIcombiChange(107);  
          break;           
   case 1097: // I
          sendMIDIcombiChange(108); 
          break;                     
   case 1103: // O
          sendMIDIcombiChange(109);  
          break;           
   case 1104: // P
          sendMIDIcombiChange(110);   
          break; 
   case 1089: // A
          sendMIDIcombiChange(111);  
          break;           
   case 1107: // S
          sendMIDIcombiChange(112);   
          break;                     
   case 1092: // D
          sendMIDIcombiChange(113);    
          break;           
   case 1094: // F
          sendMIDIcombiChange(114);   
          break; 
   case 1095: // G
          sendMIDIcombiChange(115);    
          break;           
   case 1096: // H
          sendMIDIcombiChange(116); 
          break;                     
   case 1098: // J
          sendMIDIcombiChange(117);   
          break;           
   case 1099: // K
          sendMIDIcombiChange(118);   
          break; 
   case 1100: // L
          sendMIDIcombiChange(119);     
          break;           
   case 1115: // Ñ
          sendMIDIcombiChange(120);  
          break;                     
   case 1114: // Z
          sendMIDIcombiChange(121); 
          break;           
   case 1112: // X
          sendMIDIcombiChange(122); 
          break; 
   case 1091: // C
          sendMIDIcombiChange(123); 
          break;           
   case 1110: // V
          sendMIDIcombiChange(124);   
          break;                     
   case 1090: // B
          sendMIDIcombiChange(125);    
          break;           
   case 1102: // N
          sendMIDIcombiChange(126);  
          break; 
   case 1101: // M
          sendMIDIcombiChange(127);   
          break;                   
  

// TEMPOS ------------------------------------- ALT Key ----------------------------------------------------------------------------------------------//
// 0
   case 2080:
      tempoC120
        break;           
//  1
   case 2081: 
      tempoC80 
        break;                   
// 2
   case 2082:
      tempoC90
        break;                   
// 3
   case 2083:
      tempoC100
        break;                   
// 4
   case 2084:
      tempoC110
        break;      
// 5
   case 2085:
      tempoC126
        break;                   
// 6
   case 2086:
      tempoC128
        break;                   
// 7
   case 2087:
      tempoC135
        break;                   
// 8
   case 2088:
      tempoC140
        break;                   
// 9
   case 2089:
      tempoC145
        break;                   





//ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo//
 
//oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo//
//--------------------------------------------------------- POTEs MODO COMBI --------------------------------------------------------------------------------//


// Pots A
   case 353: //bancoA F1 -------------------MIDI CHANNEL 1  ----------------------------------------------------------------Normal//     
        PO1.newValue(0, 07, 1);    // Vol
        PO2.newValue(0, 10, 1);    // Pan
        PO3.newValue(0, 73, 1);    // Attack
        PO4.newValue(0, 75, 1);    // Decay
        PO5.newValue(0, 70, 1);    // Sustain
        PO6.newValue(0, 72, 1);    // Release
        PO7.newValue(0, 74, 1);    // CutOff
        PO8.newValue(0, 71, 1);    // Resonance 
        break;
   case 354: //bancoA F2 -------------------MIDI CHANNEL 2          
        PO1.newValue(0, 07, 2);
        PO2.newValue(0, 10, 2);
        PO3.newValue(0, 73, 2);
        PO4.newValue(0, 75, 2);
        PO5.newValue(0, 70, 2);
        PO6.newValue(0, 72, 2);
        PO7.newValue(0, 74, 2);
        PO8.newValue(0, 71, 2);
        break;
   case 355: //bancoA F3 ------------------MIDI CHANNEL 3          
        PO1.newValue(0, 07, 3);
        PO2.newValue(0, 10, 3);
        PO3.newValue(0, 73, 3);
        PO4.newValue(0, 75, 3);
        PO5.newValue(0, 70, 3);
        PO6.newValue(0, 72, 3);
        PO7.newValue(0, 74, 3);
        PO8.newValue(0, 71, 3);
        break;    
   case 356: //bancoA F4 ------------------MIDI CHANNEL 4          
        PO1.newValue(0, 07, 4);
        PO2.newValue(0, 10, 4);
        PO3.newValue(0, 73, 4);
        PO4.newValue(0, 75, 4);
        PO5.newValue(0, 70, 4);
        PO6.newValue(0, 72, 4);
        PO7.newValue(0, 74, 4);
        PO8.newValue(0, 71, 4);
        break;
   case 357: //bancoA F5 ------------------MIDI CHANNEL 5          
        PO1.newValue(0, 07, 5);
        PO2.newValue(0, 10, 5);
        PO3.newValue(0, 73, 5);
        PO4.newValue(0, 75, 5);
        PO5.newValue(0, 70, 5);
        PO6.newValue(0, 72, 5);
        PO7.newValue(0, 74, 5);
        PO8.newValue(0, 71, 5);
        break; 
   case 358: //bancoA F6 ------------------MIDI CHANNEL 6          
        PO1.newValue(0, 07, 6);
        PO2.newValue(0, 10, 6);
        PO3.newValue(0, 73, 6);
        PO4.newValue(0, 75, 6);
        PO5.newValue(0, 70, 6);
        PO6.newValue(0, 72, 6);
        PO7.newValue(0, 74, 6);
        PO8.newValue(0, 71, 6);
        break; 
   case 359: //bancoA F7 -------------------MIDI CHANNEL 7          
        PO1.newValue(0, 07, 7);
        PO2.newValue(0, 10, 7);
        PO3.newValue(0, 73, 7);
        PO4.newValue(0, 75, 7);
        PO5.newValue(0, 70, 7);
        PO6.newValue(0, 72, 7);
        PO7.newValue(0, 74, 7);
        PO8.newValue(0, 71, 7);
        break;
   case 360: //bancoA F8 -------------------MIDI CHANNEL 8          
        PO1.newValue(0, 07, 8);
        PO2.newValue(0, 10, 8);
        PO3.newValue(0, 73, 8);
        PO4.newValue(0, 75, 8);
        PO5.newValue(0, 70, 8);
        PO6.newValue(0, 72, 8);
        PO7.newValue(0, 74, 8);
        PO8.newValue(0, 71, 8);
        break; 
   case 361: //bancoA F9 -------------------MIDI CHANNEL 9          
        PO1.newValue(0, 07, 9);
        PO2.newValue(0, 10, 9);
        PO3.newValue(0, 73, 9);
        PO4.newValue(0, 75, 9);
        PO5.newValue(0, 70, 9);
        PO6.newValue(0, 72, 9);
        PO7.newValue(0, 74, 9);
        PO8.newValue(0, 71, 9);
        break; 
   case 362: //bancoA F10 ------------------MIDI CHANNEL 10          
        PO1.newValue(0, 07, 10);
        PO2.newValue(0, 10, 10);
        PO3.newValue(0, 73, 10);
        PO4.newValue(0, 75, 10);
        PO5.newValue(0, 70, 10);
        PO6.newValue(0, 72, 10);
        PO7.newValue(0, 74, 10);
        PO8.newValue(0, 71, 10);
        break;   
   case 363: //bancoA F11 -----------------MIDI CHANNEL 11          
        PO1.newValue(0, 07, 11);
        PO2.newValue(0, 10, 11);
        PO3.newValue(0, 73, 11);
        PO4.newValue(0, 75, 11);
        PO5.newValue(0, 70, 11);
        PO6.newValue(0, 72, 11);
        PO7.newValue(0, 74, 11);
        PO8.newValue(0, 71, 11);
        break;   
   case 364: //bancoA F12 -----------------MIDI CHANNEL 12          
        PO1.newValue(0, 07, 12);
        PO2.newValue(0, 10, 12);
        PO3.newValue(0, 73, 12);
        PO4.newValue(0, 75, 12);
        PO5.newValue(0, 70, 12);
        PO6.newValue(0, 72, 12);
        PO7.newValue(0, 74, 12);
        PO8.newValue(0, 71, 12);
        break;  
   case 283: //bancoA ESC -----------------MIDI CHANNEL 16          
        PO1.newValue(0, 07, 16);
        PO2.newValue(0, 10, 16);
        PO3.newValue(0, 73, 16);
        PO4.newValue(0, 75, 16);
        PO5.newValue(0, 70, 16);
        PO6.newValue(0, 72, 16);
        PO7.newValue(0, 74, 16);
        PO8.newValue(0, 71, 16);
        break;  

        
// Pots  B  -------------------------Control + Fs ----------------------------------------------------------------------------------------------//
   case 8545: //bancoB F1  ---------------MIDI CHANNEL 1          
        PO1.newValue(0, 01, 1);    // Mod1
        PO2.newValue(0, 02, 1);    // Mod2
        PO3.newValue(0, 05, 1);    // Portamento
        PO4.newValue(0, 22, 1);    // Arp-Gate
        PO5.newValue(0, 23, 1);    // Arp-Vel
        PO6.newValue(0, 24, 1);    // Arp-Swing
        PO7.newValue(0, 93, 1);    // MFX 1 Send
        PO8.newValue(0, 91, 1);    // MFX 2 Send
        break;     
   case 8546: //bancoB F2  ---------------MIDI CHANNEL 2          
        PO1.newValue(0, 01, 2);
        PO2.newValue(0, 02, 2);
        PO3.newValue(0, 05, 2);
        PO4.newValue(0, 22, 1);
        PO5.newValue(0, 23, 1);
        PO6.newValue(0, 24, 1);
        PO7.newValue(0, 93, 2);
        PO8.newValue(0, 91, 2);
        break;     
   case 8547: //bancoB F3  ---------------MIDI CHANNEL 3          
        PO1.newValue(0, 01, 3);
        PO2.newValue(0, 02, 3);
        PO3.newValue(0, 05, 3);
        PO4.newValue(0, 22, 1);
        PO5.newValue(0, 23, 1);
        PO6.newValue(0, 24, 1);
        PO7.newValue(0, 93, 3);
        PO8.newValue(0, 91, 3);
        break;     
   case 8548: //bancoB F4 --------------- MIDI CHANNEL 4          
        PO1.newValue(0, 01, 4);
        PO2.newValue(0, 02, 4);
        PO3.newValue(0, 05, 4);
        PO4.newValue(0, 22, 1);
        PO5.newValue(0, 23, 1);
        PO6.newValue(0, 24, 1);
        PO7.newValue(0, 93, 4);
        PO8.newValue(0, 91, 4);
        break;     
   case 8549: //bancoB F5  ---------------MIDI CHANNEL 5          
        PO1.newValue(0, 01, 5);
        PO2.newValue(0, 02, 5);
        PO3.newValue(0, 05, 5);
        PO4.newValue(0, 22, 1);
        PO5.newValue(0, 23, 1);
        PO6.newValue(0, 24, 1);
        PO7.newValue(0, 93, 5);
        PO8.newValue(0, 91, 5);
        break;     
   case 8550: //bancoB F6  ---------------MIDI CHANNEL 6          
        PO1.newValue(0, 01, 6);
        PO2.newValue(0, 02, 6);
        PO3.newValue(0, 05, 6);
        PO4.newValue(0, 22, 1);
        PO5.newValue(0, 23, 1);
        PO6.newValue(0, 24, 1);
        PO7.newValue(0, 93, 6);
        PO8.newValue(0, 91, 6);
        break;     
   case 8551: //bancoB F7 ---------------- MIDI CHANNEL 7          
        PO1.newValue(0, 01, 7);
        PO2.newValue(0, 02, 7);
        PO3.newValue(0, 05, 7);
        PO4.newValue(0, 22, 1);
        PO5.newValue(0, 23, 1);
        PO6.newValue(0, 24, 1);
        PO7.newValue(0, 93, 7);
        PO8.newValue(0, 91, 7);
        break;     
   case 8552: //bancoB F8  ----------------MIDI CHANNEL 8          
        PO1.newValue(0, 01, 8);
        PO2.newValue(0, 02, 8);
        PO3.newValue(0, 05, 8);
        PO4.newValue(0, 22, 1);
        PO5.newValue(0, 23, 1);
        PO6.newValue(0, 24, 1);
        PO7.newValue(0, 93, 8);
        PO8.newValue(0, 91, 8);
        break;     
   case 8553: //bancoB F9  -----------------MIDI CHANNEL 9          
        PO1.newValue(0, 01, 9);
        PO2.newValue(0, 02, 9);
        PO3.newValue(0, 05, 9);
        PO4.newValue(0, 22, 1);
        PO5.newValue(0, 23, 1);
        PO6.newValue(0, 24, 1);
        PO7.newValue(0, 93, 9);
        PO8.newValue(0, 91, 9);
        break;     
   case 8554: //bancoB F10  ----------------MIDI CHANNEL 10         
        PO1.newValue(0, 01, 10);
        PO2.newValue(0, 02, 10);
        PO3.newValue(3, 05, 10);   // Pitch 
        PO4.newValue(0, 22, 1);
        PO5.newValue(0, 23, 1);
        PO6.newValue(0, 24, 1);
        PO7.newValue(0, 93, 10);
        PO8.newValue(0, 91, 10);
        break;     
   case 8555: //bancoB F11 ---------------- MIDI CHANNEL 11          
        PO1.newValue(0, 01, 11);
        PO2.newValue(0, 02, 11);
        PO3.newValue(0, 05, 11);
        PO4.newValue(0, 22, 1);
        PO5.newValue(0, 23, 1);
        PO6.newValue(0, 24, 1);
        PO7.newValue(0, 93, 11);
        PO8.newValue(0, 91, 11);
        break;     
   case 8556: //bancoB F12 --------------- MIDI CHANNEL 12          
        PO1.newValue(0, 01, 12);
        PO2.newValue(0, 02, 12);
        PO3.newValue(0, 05, 12);
        PO4.newValue(0, 22, 1);
        PO5.newValue(0, 23, 1);
        PO6.newValue(0, 24, 1);
        PO7.newValue(0, 93, 12);
        PO8.newValue(0, 91, 12);
        break;     
   case 8475: //bancoB ESC --------------- MIDI CHANNEL 16          
        PO1.newValue(0, 01, 16);
        PO2.newValue(0, 02, 16);
        PO3.newValue(3, 05, 16);   // Pitch
        PO4.newValue(0, 22, 1);
        PO5.newValue(0, 23, 1);
        PO6.newValue(0, 24, 1);
        PO7.newValue(0, 93, 16);
        PO8.newValue(0, 91, 16);
        break;    

                                         
//Grupos Pots
   case 865: //banco F1   GROUP POTS VOLUMEN -------------------Win + Fs-----------------------------------------------------------Win Key       
        PO1.newValue(0, 7, 1);
        PO2.newValue(0, 7, 2);
        PO3.newValue(0, 7, 3);
        PO4.newValue(0, 7, 4);
        PO5.newValue(0, 7, 5);
        PO6.newValue(0, 7, 6);
        PO7.newValue(0, 7, 10);
        PO8.newValue(0, 7, 16);
        break;    
   case 866: //banco F2   GROUP POTS PAN       
        PO1.newValue(0, 10, 1);
        PO2.newValue(0, 10, 2);
        PO3.newValue(0, 10, 3);
        PO4.newValue(0, 10, 4);
        PO5.newValue(0, 10, 5);
        PO6.newValue(0, 10, 6);
        PO7.newValue(0, 10, 10);
        PO8.newValue(0, 10, 16);
        break;
   case 867: //banco F3   GROUP POTS ATTACK       
        PO1.newValue(0, 73, 1);
        PO2.newValue(0, 73, 2);
        PO3.newValue(0, 73, 3);
        PO4.newValue(0, 73, 4);
        PO5.newValue(0, 73, 5);
        PO6.newValue(0, 73, 6);
        PO7.newValue(0, 73, 10);
        PO8.newValue(0, 73, 16);
        break; 
   case 868: //banco F4   GROUP POTS Decay-Release       
        PO1.newValue(0, 72, 1);
        PO2.newValue(0, 72, 2);
        PO3.newValue(0, 72, 3);
        PO4.newValue(0, 72, 4);
        PO5.newValue(0, 72, 5);
        PO6.newValue(0, 72, 6);
        PO7.newValue(0, 75, 10);
        PO8.newValue(0, 75, 16);
        break;  
   case 869: //banco F5   GROUP POTS FILTER CUTOFF       
        PO1.newValue(0, 74, 1);
        PO2.newValue(0, 74, 2);
        PO3.newValue(0, 74, 3);
        PO4.newValue(0, 74, 4);
        PO5.newValue(0, 74, 5);
        PO6.newValue(0, 74, 6);
        PO7.newValue(0, 74, 10);
        PO8.newValue(0, 74, 16);
        break; 
   case 870: //banco F6   GROUP POTS MOD WHEEL       
        PO1.newValue(0, 1, 1);
        PO2.newValue(0, 1, 2);
        PO3.newValue(0, 1, 3);
        PO4.newValue(0, 1, 4);
        PO5.newValue(0, 1, 5);
        PO6.newValue(0, 1, 6);
        PO7.newValue(0, 1, 10);
        PO8.newValue(0, 1, 16);
        break;
   case 871: //banco F7   GROUP POTS FX1 send       
        PO1.newValue(0, 93, 1);
        PO2.newValue(0, 93, 2);
        PO3.newValue(0, 93, 3);
        PO4.newValue(0, 93, 4);
        PO5.newValue(0, 93, 5);
        PO6.newValue(0, 93, 6);
        PO7.newValue(0, 93, 10);
        PO8.newValue(0, 93, 16);
        break;
   case 872: //banco F8   GROUP POTS FX2 send       
        PO1.newValue(0, 91, 1);
        PO2.newValue(0, 91, 2);
        PO3.newValue(0, 91, 3);
        PO4.newValue(0, 91, 4);
        PO5.newValue(0, 91, 5);
        PO6.newValue(0, 91, 6);
        PO7.newValue(0, 91, 10);
        PO8.newValue(0, 91, 16);
        break;  


// Groups Tracks SysEx ------------------------------------------------Shift + Fs ---------------------------------------------------//                                                                                                                                                                                                                                                                                       

   case 16737: // F1 VOLUMEN TRACK CONTROLS  -----------------SysEx POTES ----------------------------------------      
        PO1.newValue(1, 7, 1);  // trk 1
        //{0xf0, 0x42, 0x30, 0x00, 0x01, 0x49, 0x41, 0x00, 0x00, 0x09, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf7};
        PO1.Potec[7]={0x00}; PO1.Potec[9]={0x09}; PO1.Potec[11]={0x02};
        PO2.newValue(1, 10, 1); // trk 2
        //{0xf0, 0x42, 0x30, 0x00, 0x01, 0x49, 0x41, 0x00, 0x00, 0x0a, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf7};
        PO2.Potec[7]={0x00}; PO2.Potec[9]={0x0a}; PO2.Potec[11]={0x02};
        PO3.newValue(1, 73, 1); // trk 3
        //{0xf0, 0x42, 0x30, 0x00, 0x01, 0x49, 0x41, 0x00, 0x00, 0x0b, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf7};
        PO3.Potec[7]={0x00}; PO3.Potec[9]={0x0b}; PO3.Potec[11]={0x02};
        PO4.newValue(1, 72, 1); // trk 4
        //{0xf0, 0x42, 0x30, 0x00, 0x01, 0x49, 0x41, 0x00, 0x00, 0x0c, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf7};
        PO4.Potec[7]={0x00}; PO4.Potec[9]={0x0c}; PO4.Potec[11]={0x02};
        PO5.newValue(1, 74, 1); // trk 5
        //{0xf0, 0x42, 0x30, 0x00, 0x01, 0x49, 0x41, 0x00, 0x00, 0x0d, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf7};
        PO5.Potec[7]={0x00}; PO5.Potec[9]={0x0d}; PO5.Potec[11]={0x02};
        PO6.newValue(1, 71, 1); // trk 6
        //{0xf0, 0x42, 0x30, 0x00, 0x01, 0x49, 0x41, 0x00, 0x00, 0x0e, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf7};
        PO6.Potec[7]={0x00}; PO6.Potec[9]={0x0e}; PO6.Potec[11]={0x02};
        PO7.newValue(1, 93, 1); // trk 7
        //{0xf0, 0x42, 0x30, 0x00, 0x01, 0x49, 0x41, 0x00, 0x00, 0x0f, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf7};
        PO7.Potec[7]={0x00}; PO7.Potec[9]={0x0f}; PO7.Potec[11]={0x02};
        PO8.newValue(1, 93, 1); // trk 16
        //{0xf0, 0x42, 0x30, 0x00, 0x01, 0x49, 0x41, 0x00, 0x00, 0x10, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf7};
        PO8.Potec[7]={0x00}; PO8.Potec[9]={0x18}; PO8.Potec[11]={0x02};  // trk 16
        break;          
   case 16738: //F2   PAN TRACK CONTROLS         
        PO1.newValue(1, 7, 1);
        //{0xf0, 0x42, 0x30, 0x00, 0x01, 0x49, 0x41, 0x00, 0x00, 0x09, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf7};
        PO1.Potec[7]={0x00}; PO1.Potec[9]={0x09}; PO1.Potec[11]={0x01};
        PO2.newValue(1, 10, 1);
        //{0xf0, 0x42, 0x30, 0x00, 0x01, 0x49, 0x41, 0x00, 0x00, 0x0a, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf7};
        PO2.Potec[7]={0x00}; PO2.Potec[9]={0x0a}; PO2.Potec[11]={0x01};
        PO3.newValue(1, 73, 1);
        //{0xf0, 0x42, 0x30, 0x00, 0x01, 0x49, 0x41, 0x00, 0x00, 0x0b, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf7};
        PO3.Potec[7]={0x00}; PO3.Potec[9]={0x0b}; PO3.Potec[11]={0x01};
        PO4.newValue(1, 72, 1);
        //{0xf0, 0x42, 0x30, 0x00, 0x01, 0x49, 0x41, 0x00, 0x00, 0x0c, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf7};
        PO4.Potec[7]={0x00}; PO4.Potec[9]={0x0c}; PO4.Potec[11]={0x01};
        PO5.newValue(1, 74, 1);
        //{0xf0, 0x42, 0x30, 0x00, 0x01, 0x49, 0x41, 0x00, 0x00, 0x0d, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf7};
        PO5.Potec[7]={0x00}; PO5.Potec[9]={0x0d}; PO5.Potec[11]={0x01};
        PO6.newValue(1, 71, 1);
        //{0xf0, 0x42, 0x30, 0x00, 0x01, 0x49, 0x41, 0x00, 0x00, 0x0e, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf7};
        PO6.Potec[7]={0x00}; PO6.Potec[9]={0x0e}; PO6.Potec[11]={0x01};
        PO7.newValue(1, 93, 1);
        //{0xf0, 0x42, 0x30, 0x00, 0x01, 0x49, 0x41, 0x00, 0x00, 0x0f, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf7};
        PO7.Potec[7]={0x00}; PO7.Potec[9]={0x0f}; PO7.Potec[11]={0x01};
        PO8.newValue(1, 93, 1);
        //{0xf0, 0x42, 0x30, 0x00, 0x01, 0x49, 0x41, 0x00, 0x00, 0x10, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf7};
        PO8.Potec[7]={0x00}; PO8.Potec[9]={0x18}; PO8.Potec[11]={0x01}; // trk 16
        break;  
   case 16739: // F3 CUTOFF TRACK CONTROLS        
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
        PO8.newValue(5, 93, 16);       // trk 16
        break;
   case 16740: // F4 RESONANCE TRACK CONTROLS        
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
   case 16741: // F5 EG INTENSITY TRACK CONTROLS        
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
   case 16742: // F6 ATTACK TRACK CONTROLS       
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
   case 16743: // F7 DECAY TRACK CONTROLS    
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
   case 16744: // F8 release TRACK CONTROLS         
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

    case 16667: //  Shift + ESC  ---------- FX: ifx1 On/Off, ifx2 On/Off, ifx3 On/Off, ifx4 On/Off, ifx5 On/Off, Reverb Time, MFX 1 Return, MFX 2 Return ------//        
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

// default Array 
void sendSysExDef(byte byte7, byte byte9, byte byte11, byte byte16) {
  
  defarray[7] = byte7;
  defarray[9] = byte9;
  defarray[11] = byte11;
  defarray[16] = byte16;

  MIDI.sendSysEx(18, defarray, true);
}

// Arpegios A Combi
void sendSysExArpA( byte byte15, byte byte16) {
  
  defArpA[15] = byte15;
  defArpA[16] = byte16;

  MIDI.sendSysEx(18, defArpA, true);
}

// Arpegios A Controls
void ArpAcontrols( byte byte11, byte byte16) {
  
  arpAcont[11] = byte11;
  arpAcont[16] = byte16;

  MIDI.sendSysEx(18, arpAcont, true);
}

// Arpegios B Combi
void sendSysExArpB( byte byte15, byte byte16) {
  
  defArpB[15] = byte15;
  defArpB[16] = byte16;

  MIDI.sendSysEx(18, defArpB, true);
}

// Arpegios B Controls
void ArpBcontrols( byte byte11, byte byte16) {
  
  arpBcont[11] = byte11;
  arpBcont[16] = byte16;

  MIDI.sendSysEx(18, arpBcont, true);
}

// Drum Kits Combi
void sendSysExSSdrumKitC( byte byte15, byte byte16) {
  
  SSdrumKitDefC[15] = byte15;
  SSdrumKitDefC[16] = byte16;

  MIDI.sendSysEx(18, SSdrumKitDefC, true);
}

// Drum Patterns Combi
void sendSysExDrumC( byte byte15, byte byte16) {
  
  defDrumC[15] = byte15;
  defDrumC[16] = byte16;

  MIDI.sendSysEx(18, defDrumC, true);
}

// ShiftDrums Negativos Combi
void ShiftDrumsNegativeC( byte byte16) {
  
  ShiftDrumNegC[16] = byte16;

  MIDI.sendSysEx(18, ShiftDrumNegC, true);
}

// ShiftDrums Positivos Combi
void ShiftDrumsPositiveC( byte byte16) {
  
  ShiftDrumPosC[16] = byte16;

  MIDI.sendSysEx(18, ShiftDrumPosC, true);
}

//Array de 9 bytes
void defArray9bytes( byte byte6, byte byte7) {
  
  defArray9[6] = byte6;
  defArray9[7] = byte7;

  MIDI.sendSysEx(9, defArray9, true);
}

// DrumKits canal 12 Combi
void ch12DkitsC( byte byte15, byte byte16) {
  
  ch12Dkits[15] = byte15;
  ch12Dkits[16] = byte16;

  MIDI.sendSysEx(18, ch12Dkits, true);
}


// SS Bass Drum Modo Combi
void SSBassDrmC( byte byte16) {
  
  BassDrumC[16] = byte16;

  MIDI.sendSysEx(18, BassDrumC, true);
}

// SS Snare1 Drum Modo Combi
void SSsnare1DrmC( byte byte16) {
  
  Snare1DrumC[16] = byte16;

  MIDI.sendSysEx(18, Snare1DrumC, true);
}

void sendTempoC(byte byte14, byte byte15, byte byte16) {
  
  TempoDefC[14] = byte14;
  TempoDefC[15] = byte15;
  TempoDefC[16] = byte16;

  MIDI.sendSysEx(18, TempoDefC, true);
}

//Play-Mute Tracks Combi
void PlayMuteTrks( byte byte9, byte byte16) {
  
  TrkPlayMute[9] = byte9;
  TrkPlayMute[16] = byte16;

  MIDI.sendSysEx(18, TrkPlayMute, true);
}

//------------------------------------------------------------------------------------------------------------------------------//
// Shift StepSeq Negativos Combi
void ShiftSTepSeqNegativeC( byte byte16) {
  
  ShiftStepSeqNegC[16] = byte16;

  MIDI.sendSysEx(18, ShiftStepSeqNegC, true);
}

// Shift StepSeq Positivos Combi
void ShiftStepSeqPositiveC( byte byte16) {
  
  ShiftStepSeqPosC[16] = byte16;

  MIDI.sendSysEx(18, ShiftStepSeqPosC, true);
}

// memorias de Cambio de Combi (program change) ----------------------------------------------------------------------------------//
void sendMIDIcombiChange(byte programNumber) {
    MIDI.sendControlChange(0, bankSelect, 1);
    MIDI.sendControlChange(32, 6, 1); 
    MIDI.sendProgramChange(programNumber, 1);    
}

//Drum Kits Track 7
void sendSysExDrumKit( byte byte15, byte byte16) {
  
  defDrumKitTrk7[15] = byte15;
  defDrumKitTrk7[16] = byte16;

  MIDI.sendSysEx(18, defDrumKitTrk7, true);
}


//Patterns lo step Seq ------------------------------------------------------------------------------------------------------------------------------------------------------//
void SSpatternsPad1Lo(byte byte16) {
  
  ssPatterns1[16] = byte16;

  MIDI.sendSysEx(18, ssPatterns1, true);
}

void SSpatternsPad2Lo(byte byte16) {
  
  ssPatterns2[16] = byte16;

  MIDI.sendSysEx(18, ssPatterns2, true);
}
void SSpatternsPad3Lo(byte byte16) {
  
  ssPatterns3[16] = byte16;

  MIDI.sendSysEx(18, ssPatterns3, true);
}
void SSpatternsPad4Lo(byte byte16) {
  
  ssPatterns4[16] = byte16;

  MIDI.sendSysEx(18, ssPatterns4, true);
}
void SSpatternsPad5Lo(byte byte16) {
  
  ssPatterns5[16] = byte16;

  MIDI.sendSysEx(18, ssPatterns5, true);
}
void SSpatternsPad6Lo(byte byte16) {
  
  ssPatterns6[16] = byte16;

  MIDI.sendSysEx(18, ssPatterns6, true);
}
void SSpatternsPad7Lo(byte byte16) {
  
  ssPatterns7[16] = byte16;

  MIDI.sendSysEx(18, ssPatterns7, true);
}
void SSpatternsPad8Lo(byte byte16) {
  
  ssPatterns8[16] = byte16;

  MIDI.sendSysEx(18, ssPatterns8, true);
}
void SSpatternsPad9Lo(byte byte16) {
  
  ssPatterns9[16] = byte16;

  MIDI.sendSysEx(18, ssPatterns9, true);
}
void SSpatternsPad10Lo(byte byte16) {
  
  ssPatterns10[16] = byte16;

  MIDI.sendSysEx(18, ssPatterns10, true);
}
void SSpatternsPad11Lo(byte byte16) {
  
  ssPatterns11[16] = byte16;

  MIDI.sendSysEx(18, ssPatterns11, true);
}
void SSpatternsPad12Lo(byte byte16) {
  
  ssPatterns12[16] = byte16;

  MIDI.sendSysEx(18, ssPatterns12, true);
}
void SSpatternsPad13Lo(byte byte16) {
  
  ssPatterns13[16] = byte16;

  MIDI.sendSysEx(18, ssPatterns13, true);
}
void SSpatternsPad14Lo(byte byte16) {
  
  ssPatterns14[16] = byte16;

  MIDI.sendSysEx(18, ssPatterns14, true);
}
void SSpatternsPad15Lo(byte byte16) {
  
  ssPatterns15[16] = byte16;

  MIDI.sendSysEx(18, ssPatterns15, true);
}
void SSpatternsPad16Lo(byte byte16) {
  
  ssPatterns16[16] = byte16;

  MIDI.sendSysEx(18, ssPatterns16, true);
}



//Patterns Hi step Seq -----------------------------------------------------------------------------------------------------------------------------------------------------------//
void SSpatternsPad1Hi(byte byte15) {
  
  ssPatterns1[15] = byte15;

  MIDI.sendSysEx(18, ssPatterns1, true);
}

void SSpatternsPad2Hi(byte byte15) {
  
  ssPatterns2[15] = byte15;

  MIDI.sendSysEx(18, ssPatterns2, true);
}
void SSpatternsPad3Hi(byte byte15) {
  
  ssPatterns3[15] = byte15;

  MIDI.sendSysEx(18, ssPatterns3, true);
}
void SSpatternsPad4Hi(byte byte15) {
  
  ssPatterns4[15] = byte15;

  MIDI.sendSysEx(18, ssPatterns4, true);
}
void SSpatternsPad5Hi(byte byte15) {
  
  ssPatterns5[15] = byte15;

  MIDI.sendSysEx(18, ssPatterns5, true);
}
void SSpatternsPad6Hi(byte byte15) {
  
  ssPatterns6[15] = byte15;

  MIDI.sendSysEx(18, ssPatterns6, true);
}
void SSpatternsPad7Hi(byte byte15) {
  
  ssPatterns7[15] = byte15;

  MIDI.sendSysEx(18, ssPatterns7, true);
}
void SSpatternsPad8Hi(byte byte15) {
  
  ssPatterns8[15] = byte15;

  MIDI.sendSysEx(18, ssPatterns8, true);
}
void SSpatternsPad9Hi(byte byte15) {
  
  ssPatterns9[15] = byte15;

  MIDI.sendSysEx(18, ssPatterns9, true);
}
void SSpatternsPad10Hi(byte byte15) {
  
  ssPatterns10[15] = byte15;

  MIDI.sendSysEx(18, ssPatterns10, true);
}
void SSpatternsPad11Hi(byte byte15) {
  
  ssPatterns11[15] = byte15;

  MIDI.sendSysEx(18, ssPatterns11, true);
}
void SSpatternsPad12Hi(byte byte15) {
  
  ssPatterns12[15] = byte15;

  MIDI.sendSysEx(18, ssPatterns12, true);
}
void SSpatternsPad13Hi(byte byte15) {
  
  ssPatterns13[15] = byte15;

  MIDI.sendSysEx(18, ssPatterns13, true);
}
void SSpatternsPad14Hi(byte byte15) {
  
  ssPatterns14[15] = byte15;

  MIDI.sendSysEx(18, ssPatterns14, true);
}
void SSpatternsPad15Hi(byte byte15) {
  
  ssPatterns15[15] = byte15;

  MIDI.sendSysEx(18, ssPatterns15, true);
}
void SSpatternsPad16Hi(byte byte15) {
  
  ssPatterns16[15] = byte15;

  MIDI.sendSysEx(18, ssPatterns16, true);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//







