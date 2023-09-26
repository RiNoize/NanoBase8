void updatePots()
{
  for (int i = 0; i < NUMBER_POTS; i = i + 1)
  {
    if (POTS[i]->Pcommand == 0) //---------------------------- 0-127 CCs values
    {
      byte potmessage = POTS[i]->getValue();
      if (potmessage != 255)
        MIDI.sendControlChange(POTS[i]->Pcontrol, potmessage, POTS[i]->Pchannel);
    }
    if (POTS[i]->Pcommand == 1) //---------------------------SysEx 127 value
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
    if (POTS[i]->Pcommand == 11)
      {
        if (POTS[i]->potec2(false, 16, -12, 12)) //    -12 to 12 value 
          MIDI.sendSysEx(18, POTS[i]->Potec1, true);
      }   
    if (POTS[i]->Pcommand == 12)
      {
        if (POTS[i]->potec3(false, 16, 0, 99)) //   0 - 99 value   
          MIDI.sendSysEx(18, POTS[i]->Potec, true);
      }  
    if (POTS[i]->Pcommand == 13)
      {
        if (POTS[i]->potec2(false, 16, -20, 20)) //  -20, 0, +20  value 
          MIDI.sendSysEx(18, POTS[i]->Potec1, true);
      }  
    if (POTS[i]->Pcommand == 14) // Doble array OSC 1 y 2 LFO1 Freq
      {
        if (POTS[i]->potec3(false, 16, 0, 99)) //   0 - 99 value   
        {
        // Enviamos el primer array
        POTS[i]->Potec[9] = {0x04};
        MIDI.sendSysEx(18, POTS[i]->Potec, true);
        
        // Enviamos el segundo array
        POTS[i]->Potec[9] = {0x14};
        MIDI.sendSysEx(18, POTS[i]->Potec, true);
        }
      }  
    if (POTS[i]->Pcommand == 15) // Doble array OSC 1 y 2 LFO2 Freq
      {
        if (POTS[i]->potec3(false, 16, 0, 99)) //   0 - 99 value   
        {
        // Enviamos el primer array
        POTS[i]->Potec[9] = {0x05};
        MIDI.sendSysEx(18, POTS[i]->Potec, true);
        
        // Enviamos el segundo array
        POTS[i]->Potec[9] = {0x15};
        MIDI.sendSysEx(18, POTS[i]->Potec, true);
        }        
      }  
    if (POTS[i]->Pcommand == 16)
      {
        if (POTS[i]->potec3(false, 16, 0, 6)) //   0 - 6 value   
          MIDI.sendSysEx(18, POTS[i]->Potec, true);
      }      
    if (POTS[i]->Pcommand == 17)
      {
        if (POTS[i]->potec3(false, 16, 0, 100)) //   0 - 100 value   
          MIDI.sendSysEx(18, POTS[i]->Potec, true);
      }     
    if (POTS[i]->Pcommand == 18)
      {
        if (POTS[i]->potec2(false, 16, -48, 48)) //  -48, 0, +48  value 
          MIDI.sendSysEx(18, POTS[i]->Potec1, true);
      }  
    if (POTS[i]->Pcommand == 19)
      {
        if (POTS[i]->potec3(false, 16, 0, 8)) //   0 - 8 value   
          MIDI.sendSysEx(18, POTS[i]->Potec, true);
      }  
    if (POTS[i]->Pcommand == 20)
      {
        if (POTS[i]->potec3(false, 16, 0, 104)) //   0 - 104 value   
          MIDI.sendSysEx(18, POTS[i]->Potec, true);
      }  
    if (POTS[i]->Pcommand == 21)
      {
        if (POTS[i]->potec3(false, 16, 0, 3)) //   0 - 3 value   
          MIDI.sendSysEx(18, POTS[i]->Potec, true);
      }  

    if (POTS[i]->Pcommand == 22) // 2 Bytes
      {
        if (POTS[i]->potec7(false, 16, 5, 208)) //   5 - 208 (2 bytes value)   
          MIDI.sendSysEx(18, POTS[i]->Potec2, true);
      }  
    if (POTS[i]->Pcommand == 23)
      {
        if (POTS[i]->potec3(false, 16, 0, 1)) //   0 - 1 value   
          MIDI.sendSysEx(18, POTS[i]->Potec, true);
      }    
    if (POTS[i]->Pcommand == 24) // Doble array OSC 1 y 2 LFO1 Waveform
      {
        if (POTS[i]->potec3(false, 16, 0, 17)) //   0 - 17 value   
        {
        // Enviamos el primer array
        POTS[i]->Potec[9] = {0x04};
        MIDI.sendSysEx(18, POTS[i]->Potec, true);
        
        // Enviamos el segundo array
        POTS[i]->Potec[9] = {0x14};
        MIDI.sendSysEx(18, POTS[i]->Potec, true);
        }
      }  
    if (POTS[i]->Pcommand == 25) // Doble array OSC 1 y 2 LFO2 Waveform
      {
        if (POTS[i]->potec3(false, 16, 0, 17)) //   0 - 17 value   
        {
        // Enviamos el primer array
        POTS[i]->Potec[9] = {0x05};
        MIDI.sendSysEx(18, POTS[i]->Potec, true);
        
        // Enviamos el segundo array
        POTS[i]->Potec[9] = {0x15};
        MIDI.sendSysEx(18, POTS[i]->Potec, true);
        }        
      }  
    if (POTS[i]->Pcommand == 26) // Doble array OSC 1 y 2 LFO1 Shape
      {
        if (POTS[i]->potec2(false, 16, -99, 99)) //   0 - 17 value   
        {
        // Enviamos el primer array
        POTS[i]->Potec1[9] = {0x04};
        MIDI.sendSysEx(18, POTS[i]->Potec1, true);
        
        // Enviamos el segundo array
        POTS[i]->Potec1[9] = {0x14};
        MIDI.sendSysEx(18, POTS[i]->Potec1, true);
        }
      }  
    if (POTS[i]->Pcommand == 27) // Doble array OSC 1 y 2 LFO2 Shape
      {
        if (POTS[i]->potec2(false, 16, -99, 99)) //   0 - 17 value   
        {
        // Enviamos el primer array
        POTS[i]->Potec1[9] = {0x05};
        MIDI.sendSysEx(18, POTS[i]->Potec1, true);
        
        // Enviamos el segundo array
        POTS[i]->Potec1[9] = {0x15};
        MIDI.sendSysEx(18, POTS[i]->Potec1, true);
        }        
      }  
    if (POTS[i]->Pcommand == 28)
      {
        if (POTS[i]->potec2(false, 16, -100, 100)) //  -100, 0, +100  value 
          MIDI.sendSysEx(18, POTS[i]->Potec1, true);
      }  

// Waves --------------------------------------------------------------------------------------------------------------------------//      
    if (POTS[i]->Pcommand == 29) // DWGS Waves
      {
        if (POTS[i]->potec7(false, 16, 350, 380)) //   350 - 380 (2 bytes value)   
          MIDI.sendSysEx(18, POTS[i]->Potec2, true);
      }  
    if (POTS[i]->Pcommand == 30) // Organ Waves
      {
        if (POTS[i]->potec7(false, 16, 70, 109)) //    70 - 109 (2 bytes value)   
          MIDI.sendSysEx(18, POTS[i]->Potec2, true);
      }  
    if (POTS[i]->Pcommand == 31) // Waves Sostenidas
      {
        if (POTS[i]->potec7(false, 16, 167, 207)) //   167 - 207 (2 bytes value)   
          MIDI.sendSysEx(18, POTS[i]->Potec2, true);
      }  
    if (POTS[i]->Pcommand == 32) // Waves caños
      {
        if (POTS[i]->potec7(false, 16, 134, 166)) //   134 - 166 (2 bytes value)   
          MIDI.sendSysEx(18, POTS[i]->Potec2, true);
      }  
    if (POTS[i]->Pcommand == 33) // Waves Percusion
      {
        if (POTS[i]->potec7(false, 16, 459, 495)) //   459 - 495 (2 bytes value)   
          MIDI.sendSysEx(18, POTS[i]->Potec2, true);
      }  
    if (POTS[i]->Pcommand == 34) // Waves Bells
      {
        if (POTS[i]->potec7(false, 16, 110, 133)) //   110 - 133 (2 bytes value)   
          MIDI.sendSysEx(18, POTS[i]->Potec2, true);
      }  
    if (POTS[i]->Pcommand == 35) // Waves Synth 1
      {
        if (POTS[i]->potec7(false, 16, 271, 300)) //   271 - 300 (2 bytes value)   
          MIDI.sendSysEx(18, POTS[i]->Potec2, true);
      }  
    if (POTS[i]->Pcommand == 36) // Waves Hits
      {
        if (POTS[i]->potec7(false, 16, 381, 408)) //   381 - 408 (2 bytes value)   
          MIDI.sendSysEx(18, POTS[i]->Potec2, true);
      }  
    if (POTS[i]->Pcommand == 37) // Waves Basic
      {
        if (POTS[i]->potec7(false, 16, 311, 341)) //   311 - 341 (2 bytes value)   
          MIDI.sendSysEx(18, POTS[i]->Potec2, true);
      }  
    if (POTS[i]->Pcommand == 38) // Waves KG2 Mono
      {
        if (POTS[i]->potec3(false, 16, 0, 31)) //   0 - 31 value   
          MIDI.sendSysEx(18, POTS[i]->Potec, true);
      }     
    if (POTS[i]->Pcommand == 39) // Waves KG4 Mono
      {
        if (POTS[i]->potec3(false, 16, 0, 33)) //   0 - 33 value   
          MIDI.sendSysEx(18, POTS[i]->Potec, true);
      }     
    if (POTS[i]->Pcommand == 40) // Waves KG4 Stereo
      {
        if (POTS[i]->potec3(false, 16, 0, 5)) //   0 - 5 value   
          MIDI.sendSysEx(18, POTS[i]->Potec, true);
      }     
    if (POTS[i]->Pcommand == 41) // Waves Guit-Bass
      {
        if (POTS[i]->potec7(false, 16, 208, 250)) //   208 - 250 (2 bytes value)   
          MIDI.sendSysEx(18, POTS[i]->Potec2, true);
      }  
    if (POTS[i]->Pcommand == 42) // Waves Pluk-SynthBass
      {
        if (POTS[i]->potec7(false, 16, 251, 270)) //   251 - 270 (2 bytes value)   
          MIDI.sendSysEx(18, POTS[i]->Potec2, true);
      }  
    if (POTS[i]->Pcommand == 43) // Waves Pianos-E.Pianos
      {
        if (POTS[i]->potec7(false, 16, 10, 39)) //   10 - 39 (2 bytes value)   
          MIDI.sendSysEx(18, POTS[i]->Potec2, true);
      }  
    if (POTS[i]->Pcommand == 44) // Waves Clavis y E.Ps
      {
        if (POTS[i]->potec7(false, 16, 40, 69)) //   40 - 69 (2 bytes value)   
          MIDI.sendSysEx(18, POTS[i]->Potec2, true);
      }        
    if (POTS[i]->Pcommand == 45) // Waves Stereo
      {
        if (POTS[i]->potec7(false, 16, 0, 6)) //   0 - 6 (2 bytes value)   
          MIDI.sendSysEx(18, POTS[i]->Potec2, true);
      }              
    if (POTS[i]->Pcommand == 46)
      {
        if (POTS[i]->potec2(false, 16, -18, 18)) //    -18 to 18 value 
          MIDI.sendSysEx(18, POTS[i]->Potec1, true);
      }   



  }
}
