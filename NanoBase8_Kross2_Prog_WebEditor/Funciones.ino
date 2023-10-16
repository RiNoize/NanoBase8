// default Array 
void sendSysExDef(byte byte7, byte byte9, byte byte11, byte byte16) {
  
  defarray[7] = byte7;
  defarray[9] = byte9;
  defarray[11] = byte11;
  defarray[16] = byte16;

  MIDI.sendSysEx(18, defarray, true);
}

//Drum Patterns Program
void sendSysExDrum( byte byte15, byte byte16) {
  
  defDrum[15] = byte15;
  defDrum[16] = byte16;

  MIDI.sendSysEx(18, defDrum, true);
}

// Arpegios Program
void sendSysExArp( byte byte15, byte byte16) {
  
  defArp[15] = byte15;
  defArp[16] = byte16;

  MIDI.sendSysEx(18, defArp, true);
}

// Control de Arpegios
void arpControls( byte byte11, byte byte16) {
  
  arpCont[11] = byte11;
  arpCont[16] = byte16;

  MIDI.sendSysEx(18, arpCont, true);
}

// DrumTrack Bus select
void DrumTrackBusSelect( byte byte16) {
  
  DrumTrackBus[16] = byte16;

  MIDI.sendSysEx(18, DrumTrackBus, true);
}

// Step Seq Bus select
void SSBusSelect( byte byte16) {
  
  StepSeqBus[16] = byte16;

  MIDI.sendSysEx(18, StepSeqBus, true);
}

//Drum Kits Program
void sendSysExDrumKit( byte byte15, byte byte16) {
  
  defDrumKit[15] = byte15;
  defDrumKit[16] = byte16;

  MIDI.sendSysEx(18, defDrumKit, true);
}



//Array de 9 bytes
void defArray9bytes( byte byte6, byte byte7) {
  
  defArray9[6] = byte6;
  defArray9[7] = byte7;

  MIDI.sendSysEx(9, defArray9, true);
}


// ShiftDrums Negativos Program
void ShiftDrumsNegative( byte byte16) {
  
  ShiftDrumNeg[16] = byte16;

  MIDI.sendSysEx(18, ShiftDrumNeg, true);
}

// ShiftDrums Positivos Program
void ShiftDrumsPositive( byte byte16) {
  
  ShiftDrumPos[16] = byte16;

  MIDI.sendSysEx(18, ShiftDrumPos, true);
}

//Step seq DrumKits Program
void sendSysExSSdrumKit( byte byte15, byte byte16) {
  
  SSdrumKitDef[15] = byte15;
  SSdrumKitDef[16] = byte16;

  MIDI.sendSysEx(18, SSdrumKitDef, true);
}

// SS Bass Drum Modo Program
void SSBassDrm( byte byte16) {
  
  BassDrum[16] = byte16;

  MIDI.sendSysEx(18, BassDrum, true);
}

// SS Snare1 Drum Modo Program
void SSsnare1Drm( byte byte16) {
  
  Snare1Drum[16] = byte16;

  MIDI.sendSysEx(18, Snare1Drum, true);
}



void sendTempo(byte byte14, byte byte15, byte byte16) {
  
  TempoDef[14] = byte14;
  TempoDef[15] = byte15;
  TempoDef[16] = byte16;

  MIDI.sendSysEx(18, TempoDef, true);
}



// memorias de Cambio de Propgrama (program change) ----------------------------------------------------------------------------------------------------------------//
void sendMIDIProgramChange(byte programNumber) {
    MIDI.sendControlChange(0, bankSelect, 1);
    MIDI.sendControlChange(32, 9, 1); 
    MIDI.sendProgramChange(programNumber, 1);    
}

// memorias de Cambio de Combi (program change) --------------------------------------------------------------------------------------------------------------------//
void sendMIDIcombiChange(byte programNumber) {
    MIDI.sendControlChange(0, bankSelect, 1);
    MIDI.sendControlChange(32, 6, 1); 
    MIDI.sendProgramChange(programNumber, 1);    
}







/*
//Drum Patterns Program
void sendSysExDrums(int n) {
    if (n < 0 || n > 899) {
        // Número fuera de rango
        return;
    }
    
    byte sysExDrum[18] = {0xf0, 0x42, 0x30, 0x00, 0x01, 0x49, 0x41, 0x02, 0x00, 0x02, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf7};
    
    // Calcula los dos penúltimos bytes
    sysExDrum[15] = (n >> 7) & 0x7F; // Parte alta del número
    sysExDrum[16] = n & 0x7F;        // Parte baja del número

    // Envía el mensaje SysEx
    MIDI.sendSysEx(18, sysExDrum, true);
}

*/



