
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

/*
//Drum Patterns Program --------------------------------------------------------------------//
void sendSysExDrum(int n) {
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
//-------------------------------------------------------------------------------------------//

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

//Tempos
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

void OSC1waves(byte byte15, byte byte16) {
  
  osc1Waves[15] = byte15;
  osc1Waves[16] = byte16;

  MIDI.sendSysEx(18, osc1Waves, true);
}

void OSC2waves(byte byte15, byte byte16) {
  
  osc2Waves[15] = byte15;
  osc2Waves[16] = byte16;

  MIDI.sendSysEx(18, osc2Waves, true);
}
// LFO1 Osc1
void Lfo1osc1waves(byte byte16) {
  
  Lfo1osc1Wav[16] = byte16;

  MIDI.sendSysEx(18, Lfo1osc1Wav, true);
}
// LFO1 Osc2
void Lfo1osc2waves(byte byte16) {
  
  Lfo1osc2Wav[16] = byte16;

  MIDI.sendSysEx(18, Lfo1osc2Wav, true);
}
// LFO2 Osc1
void Lfo2osc1waves(byte byte16) {
  
  Lfo2osc1Wav[16] = byte16;

  MIDI.sendSysEx(18, Lfo2osc1Wav, true);
}
// LFO2 Osc2
void Lfo2osc2waves(byte byte16) {
  
  Lfo2osc2Wav[16] = byte16;

  MIDI.sendSysEx(18, Lfo2osc2Wav, true);
}

void sendSysExNeg4(byte byte9, byte byte11, byte byte15, byte byte16) {
  
  NegArray4[9] = byte9;
  NegArray4[11] = byte11;
  NegArray4[15] = byte15;
  NegArray4[16] = byte16;

  MIDI.sendSysEx(18, NegArray4, true);
}

void Filters(byte byte9, byte byte16) {

  FiltsArray[9] = byte9;
  FiltsArray[16] = byte16;

  MIDI.sendSysEx(18, FiltsArray, true);
}

//Def IFX
void sendIFX( byte byte11, byte byte16) {
  
  defIFX[11] = byte11;
  defIFX[16] = byte16;

  MIDI.sendSysEx(18, defIFX, true);
}

//Def OSC1
void OSC1def( byte byte11, byte byte16) {
  
  defOSC1[11] = byte11;
  defOSC1[16] = byte16;

  MIDI.sendSysEx(18, defOSC1, true);
}

//Def OSC2
void OSC2def( byte byte11, byte byte16) {
  
  defOSC2[11] = byte11;
  defOSC2[16] = byte16;

  MIDI.sendSysEx(18, defOSC2, true);
}

void sendDef702(byte byte9, byte byte11, byte byte16) {
  
  def702[9] = byte9;
  def702[11] = byte11;
  def702[16] = byte16;

  MIDI.sendSysEx(18, def702, true);
}

void sendDef703(byte byte9, byte byte11, byte byte16) {
  
  def703[9] = byte9;
  def703[11] = byte11;
  def703[16] = byte16;

  MIDI.sendSysEx(18, def703, true);
}

//Layer OSC1
void OSC1Layer( byte byte15, byte byte16) {
  
  LayerWavOSC1[15] = byte15;
  LayerWavOSC1[16] = byte16;

  MIDI.sendSysEx(18, LayerWavOSC1, true);
}

//Layer OSC2
void OSC2Layer( byte byte15, byte byte16) {
  
  LayerWavOSC2[15] = byte15;
  LayerWavOSC2[16] = byte16;

  MIDI.sendSysEx(18, LayerWavOSC2, true);
}




/*
void sendSysExOSC1(int value) {
    // Dividir el valor en dos bytes
    byte lowByte = value & 0x7F; // 7 bits inferiores
    byte highByte = (value >> 7) & 0x03; // 2 bits superiores
byte sysExArray[18] = {0xf0, 0x42, 0x30, 0x00, 0x01, 0x49, 0x41, 0x03, 0x00, 0x03, 0x00, 0x02, 0x00, 0x00, 0x00, highByte, lowByte, 0xf7};

   MIDI.sendSysEx(18, sysExArray, true);
}

void sendSysExOSC2(int value) {
    // Dividir el valor en dos bytes
    byte lowByte = value & 0x7F; // 7 bits inferiores
    byte highByte = (value >> 7) & 0x03; // 2 bits superiores
byte sysExArray[18] = {0xf0, 0x42, 0x30, 0x00, 0x01, 0x49, 0x41, 0x03, 0x00, 0x13, 0x00, 0x02, 0x00, 0x00, 0x00, highByte, lowByte, 0xf7};
  
   MIDI.sendSysEx(18, sysExArray, true);
}
*/



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



