// default Array 
void sendSysExDef(byte byte7, byte byte9, byte byte11, byte byte16) {
  
  defarray[7] = byte7;
  defarray[9] = byte9;
  defarray[11] = byte11;
  defarray[16] = byte16;

  MIDI.sendSysEx(18, defarray, true);
}

void sendSysExDef5(byte byte7, byte byte9, byte byte11, byte byte15, byte byte16) {
  
  defarray5[7] = byte7;
  defarray5[9] = byte9;
  defarray5[11] = byte11;
  defarray5[15] = byte15;
  defarray5[16] = byte16;

  MIDI.sendSysEx(18, defarray5, true);
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

// Drum Kits SS Combi
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

// track 8 Bus select Combi
void ch8DBus(byte byte16) {
  
  ch8Bus[16] = byte16;

  MIDI.sendSysEx(18, ch8Bus, true);
}

// track 12 Bus select Combi
void ch12DBus(byte byte16) {
  
  ch12Bus[16] = byte16;

  MIDI.sendSysEx(18, ch12Bus, true);
}

// Track 12 Controls
void Trk12Controls( byte byte11, byte byte16) {
  
  ch12Drum[11] = byte11;
  ch12Drum[16] = byte16;

  MIDI.sendSysEx(18, ch12Drum, true);
}

// Track 12 DrumKit
void Trk12DrumKit( byte byte15, byte byte16) {
  
  ch12DrumKit[15] = byte15;
  ch12DrumKit[16] = byte16;

  MIDI.sendSysEx(18, ch12DrumKit, true);
}

// Step Seq Bus select Combi
void SSBusSelectC( byte byte16) {
  
  StepSeqBusC[16] = byte16;

  MIDI.sendSysEx(18, StepSeqBusC, true);
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

void sendTempo(byte byte14, byte byte15, byte byte16) {
  
  TempoDef[14] = byte14;
  TempoDef[15] = byte15;
  TempoDef[16] = byte16;

  MIDI.sendSysEx(18, TempoDef, true);
}

void sendTempoC(byte byte14, byte byte15, byte byte16) {
  
  TempoDefC[14] = byte14;
  TempoDefC[15] = byte15;
  TempoDefC[16] = byte16;

  MIDI.sendSysEx(18, TempoDefC, true);
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

//Play-Mute Tracks Combi
void PlayMuteTrks( byte byte9, byte byte16) {
  
  TrkPlayMute[9] = byte9;
  TrkPlayMute[16] = byte16;

  MIDI.sendSysEx(18, TrkPlayMute, true);
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



