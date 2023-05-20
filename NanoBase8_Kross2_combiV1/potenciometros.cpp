#include "potenciometros.h"

Pot::Pot(byte pin, byte command, byte control, byte channel)
{
  _pin = pin;
  _control = control;
  _value = analogRead(_pin);
  _value = _value >> 3;
  _oldValue = _value << 3;
  _value = _value << 3;
  Pcommand = command;
  Pcontrol = control;
  Pchannel = channel;
}

byte Pot::defineSySex(byte sysex_number)
{
if (sysex_number == 1){  //---------------------------CutOff
    Potec1[7] = 0x00;
    Potec1[9] = 0x09;
    Potec1[11] = 0x34;
  }

  if (sysex_number == 2){
    Potec1[7] = 0x00;
    Potec1[9] = 0x0a;
    Potec1[11] = 0x34;
  }

  if (sysex_number == 3){
    Potec1[7] = 0x00;
    Potec1[9] = 0x0b;
    Potec1[11] = 0x34;
  }

  if (sysex_number == 4){
    Potec1[7] = 0x00;
    Potec1[9] = 0x0c;
    Potec1[11] = 0x34;
  }

  if (sysex_number == 5){
    Potec1[7] = 0x00;
    Potec1[9] = 0x0d;
    Potec1[11] = 0x34;
  }

  if (sysex_number == 6){
    Potec1[7] = 0x00;
    Potec1[9] = 0x0e;
    Potec1[11] = 0x34;
  }

  if (sysex_number == 7){
    Potec1[7] = 0x00;
    Potec1[9] = 0x0f;
    Potec1[11] = 0x34;
  }

  if (sysex_number == 8){    // trk 16
    Potec1[7] = 0x00;
    Potec1[9] = 0x18;
    Potec1[11] = 0x34;
  }  

  if (sysex_number == 9){  //--------------------------Resonance
    Potec1[7] = 0x00;
    Potec1[9] = 0x09;
    Potec1[11] = 0x35;
  }

  if (sysex_number == 10){
    Potec1[7] = 0x00;
    Potec1[9] = 0x0a;
    Potec1[11] = 0x35;
  }

  if (sysex_number == 11){
    Potec1[7] = 0x00;
    Potec1[9] = 0x0b;
    Potec1[11] = 0x35;
  }

  if (sysex_number == 12){
    Potec1[7] = 0x00;
    Potec1[9] = 0x0c;
    Potec1[11] = 0x35;
  }

  if (sysex_number == 13){
    Potec1[7] = 0x00;
    Potec1[9] = 0x0d;
    Potec1[11] = 0x35;
  }

  if (sysex_number == 14){
    Potec1[7] = 0x00;
    Potec1[9] = 0x0e;
    Potec1[11] = 0x35;
  }

  if (sysex_number == 15){
    Potec1[7] = 0x00;
    Potec1[9] = 0x0f;
    Potec1[11] = 0x35;
  }

  if (sysex_number == 16){  // trk 16
    Potec1[7] = 0x00;
    Potec1[9] = 0x18;
    Potec1[11] = 0x35;
  }  

  if (sysex_number == 17){  //------------------------EG Intensity
    Potec1[7] = 0x00;
    Potec1[9] = 0x09;
    Potec1[11] = 0x36;
  }

  if (sysex_number == 18){
    Potec1[7] = 0x00;
    Potec1[9] = 0x0a;
    Potec1[11] = 0x36;
  }

  if (sysex_number == 19){
    Potec1[7] = 0x00;
    Potec1[9] = 0x0b;
    Potec1[11] = 0x36;
  }

  if (sysex_number == 20){
    Potec1[7] = 0x00;
    Potec1[9] = 0x0c;
    Potec1[11] = 0x36;
  }

  if (sysex_number == 21){
    Potec1[7] = 0x00;
    Potec1[9] = 0x0d;
    Potec1[11] = 0x36;
  }

  if (sysex_number == 22){
    Potec1[7] = 0x00;
    Potec1[9] = 0x0e;
    Potec1[11] = 0x36;
  }

  if (sysex_number == 23){
    Potec1[7] = 0x00;
    Potec1[9] = 0x0f;
    Potec1[11] = 0x36;
  }

  if (sysex_number == 24){  // trk 16
    Potec1[7] = 0x00;
    Potec1[9] = 0x18;
    Potec1[11] = 0x36;
  }  

  if (sysex_number == 33){ //-------------------------Attack
    Potec1[7] = 0x00;
    Potec1[9] = 0x09;
    Potec1[11] = 0x38;
  }

  if (sysex_number == 34){
    Potec1[7] = 0x00;
    Potec1[9] = 0x0a;
    Potec1[11] = 0x38;
  }

  if (sysex_number == 35){
    Potec1[7] = 0x00;
    Potec1[9] = 0x0b;
    Potec1[11] = 0x38;
  }

  if (sysex_number == 36){
    Potec1[7] = 0x00;
    Potec1[9] = 0x0c;
    Potec1[11] = 0x38;
  }

  if (sysex_number == 37){
    Potec1[7] = 0x00;
    Potec1[9] = 0x0d;
    Potec1[11] = 0x38;
  }

  if (sysex_number == 38){
    Potec1[7] = 0x00;
    Potec1[9] = 0x0e;
    Potec1[11] = 0x38;
  }

  if (sysex_number == 39){
    Potec1[7] = 0x00;
    Potec1[9] = 0x0f;
    Potec1[11] = 0x38;
  }

  if (sysex_number == 40){  // trk 16
    Potec1[7] = 0x00;
    Potec1[9] = 0x18;
    Potec1[11] = 0x38;
  }  

  if (sysex_number == 41){ //------------------------Decay
    Potec1[7] = 0x00;
    Potec1[9] = 0x09;
    Potec1[11] = 0x39;
  }

  if (sysex_number == 42){
    Potec1[7] = 0x00;
    Potec1[9] = 0x0a;
    Potec1[11] = 0x39;
  }

  if (sysex_number == 43){
    Potec1[7] = 0x00;
    Potec1[9] = 0x0b;
    Potec1[11] = 0x39;
  }

  if (sysex_number == 44){
    Potec1[7] = 0x00;
    Potec1[9] = 0x0c;
    Potec1[11] = 0x39;
  }

  if (sysex_number == 45){
    Potec1[7] = 0x00;
    Potec1[9] = 0x0d;
    Potec1[11] = 0x39;
  }

  if (sysex_number == 46){
    Potec1[7] = 0x00;
    Potec1[9] = 0x0e;
    Potec1[11] = 0x39;
  }

  if (sysex_number == 47){
    Potec1[7] = 0x00;
    Potec1[9] = 0x0f;
    Potec1[11] = 0x39;
  }

  if (sysex_number == 48){  // trk 16
    Potec1[7] = 0x00;
    Potec1[9] = 0x18;
    Potec1[11] = 0x39;
  }  

  if (sysex_number == 57){  //---------------------------------Release
    Potec1[7] = 0x00;
    Potec1[9] = 0x09;
    Potec1[11] = 0x3b;
  }

  if (sysex_number == 58){
    Potec1[7] = 0x00;
    Potec1[9] = 0x0a;
    Potec1[11] = 0x3b;
  }

  if (sysex_number == 59){
    Potec1[7] = 0x00;
    Potec1[9] = 0x0b;
    Potec1[11] = 0x3b;
  }

  if (sysex_number == 60){
    Potec1[7] = 0x00;
    Potec1[9] = 0x0c;
    Potec1[11] = 0x3b;
  }

  if (sysex_number == 61){
    Potec1[7] = 0x00;
    Potec1[9] = 0x0d;
    Potec1[11] = 0x3b;
  }

  if (sysex_number == 62){
    Potec1[7] = 0x00;
    Potec1[9] = 0x0e;
    Potec1[11] = 0x3b;
  }

  if (sysex_number == 63){
    Potec1[7] = 0x00;
    Potec1[9] = 0x0f;
    Potec1[11] = 0x3b;
  }

  if (sysex_number == 64){  // trk 16
    Potec1[7] = 0x00;
    Potec1[9] = 0x18;
    Potec1[11] = 0x3b;
  }  

  if (sysex_number == 65){
    Potec1[7] = 0x00;
    Potec1[9] = 0x1f;
    Potec1[11] = 0x05;   
  }  
    if (sysex_number == 66){  //pitch offset OSC 1 en program
    Potec1[7] = 0x03;
    Potec1[9] = 0x06;
    Potec1[11] = 0x00;
  }  

  if (sysex_number == 67){   //pitch offset OSC 2 en program
    Potec1[7] = 0x03;
    Potec1[9] = 0x16;
    Potec1[11] = 0x00;
  }  
  
 return 1;
}

/*****
Lee el valor del potenciómetro, y si hubo un cambio actualizo en _retvalue
Esta funcion mapea el ADC a 7bits (0-127)
******/
byte Pot::getValue()
{
  _value = analogRead(_pin);
  int tmp = (_oldValue - _value);
  int _retvalue;
  if (tmp >= 8 || tmp <= -8) // Estos 2 valores es el filtro de ruido o sensibilidad, a mas, mas filtro. These 2 values is the noise filter or sensitivity, more, more filter.
  {
    _oldValue = _value;

      _retvalue = map(_value, 0, 1020, 0, 127); // Mapeo, Mapping    

      return _retvalue;
  }
  return 255;
}

/*****
Lee el valor del potenciómetro, y si hubo un cambio actualizo en _retvalue
Esta funcion mapea el ADC a 7 o 14 bits. Por ahora con _bits en 14 está pensada
para usarse con PitchBend
******/
int Pot::getValueBits(byte _bits)
{
  _value = analogRead(_pin);
  int tmp = (_oldValue - _value);
//  int _retvalue;
  if (tmp >= 8 || tmp <= -8) // Estos 2 valores es el filtro de ruido o sensibilidad, a mas, mas filtro. These 2 values is the noise filter or sensitivity, more, more filter.
  {
    _oldValue = _value;

    if(_bits == 7)
      _retvalue = map(_value, 0, 1023, 0, 127);
     
    if(_bits == 14)
      _retvalue = map(_value, 2, 1023, 0, 16383);
    
    if(_bits == 100)
      _retvalue = map(_value, 0, 1023, -8192, 8191);
      
      return _retvalue;
  }
  return 255;
}

void Pot::newValue(byte command, byte value, byte channel)
{
  Pcommand = command;
  Pcontrol = value;
  Pchannel = channel;
}

bool Pot::potec(bool parte, byte modificar)
{
  if (parte == false)
  {
    // FILTRADO DE VALOR ANALOGICO POR UMBRAL.  ANALOG VALUE FILTERING BY THRESHOLD
    _value = analogRead(_pin); // modificar pin de lectura
    tmp = (_oldValue - _value);
    if (tmp >= 8 || tmp <= -8) // Estos 2 valores es el filtro de ruido o sensibilidad, a mas, mas filtro. These 2 values is the noise filter or sensitivity, more, more filter.
    {
      _oldValue = _value >> 3;
      _oldValue = _oldValue << 3;
//      _value >> 3;
      Potec[modificar] = _value >> 3; // Reemplazo por el nuevo valor del potenciómetro      
      return 1;
    }
    return 0;
  }
}  
  /*
  if (parte == true)
  {
    _value = analogRead(_pin); // modificar pin de lectura
    tmp = (_oldValue - _value);
    if (tmp >= 8 || tmp <= -8) // Estos 2 valores es el filtro de ruido o sensibilidad, a mas, mas filtro. These 2 values is the noise filter or sensitivity, more, more filter.
    {
      _oldValue = _value >> 3;
      _oldValue = _oldValue << 3;
//      _value >> 3;
      Potec[modificar] = _value;
      unsigned int valorPotenciometro = _value;
      Potec[modificar] = valorPotenciometro >> 7;            // Parte alta del mensaje (valor/128)
      Potec[modificar + 1] = valorPotenciometro & 0b1111111; // Reemplazo por el nuevo valor del potenciómetro
      return 1;
    }
    return 0;
  }
}
*/
bool Pot::potec2(bool parte, byte modificar, int min, int max)
{

    // FILTRADO DE VALOR ANALOGICO POR UMBRAL.  ANALOG VALUE FILTERING BY THRESHOLD
    
    _value = analogRead(_pin); // modificar pin de lectura
    tmp = (_oldValue - _value);
    if (tmp >= 8 || tmp <= -8) // Estos 2 valores es el filtro de ruido o sensibilidad, a mas, mas filtro. These 2 values is the noise filter or sensitivity, more, more filter.
    {
      _oldValue = _value;

      const long rangedValue = map(_value, 0, 1023, min, max);

      const byte sysExValueData[5] = { 
        (rangedValue >> 28) & 0x0f, // First 4 bits
        (rangedValue >> 21) & 0x7f, // Next 7 bits
        (rangedValue >> 14) & 0x7f, // etc..
        (rangedValue >> 7)  & 0x7f, // until
          rangedValue        & 0x7f // the last 7 bits
        };


        Potec1[12] = sysExValueData[0];
        Potec1[13] = sysExValueData[1];
        Potec1[14] = sysExValueData[2];
        Potec1[15] = sysExValueData[3];
        Potec1[16] = sysExValueData[4];
      
      return 1;
    }

    return 0;

}
bool Pot::potec3(bool parte, byte modificar, int min, int max)

{
    // FILTRADO DE VALOR ANALOGICO POR UMBRAL.  ANALOG VALUE FILTERING BY THRESHOLD
    _value = analogRead(_pin); // modificar pin de lectura
    int _retvalue;
    int tmp = (_oldValue - _value);
    if (tmp >= 8 || tmp <= -8) // Estos 2 valores es el filtro de ruido o sensibilidad, a mas, mas filtro. These 2 values is the noise filter or sensitivity, more, more filter.
    {
     _oldValue = _value;

     _retvalue = map(_value, 0, 1023, min, max);   
     Potec[modificar] = _retvalue ; // Reemplazo por el nuevo valor del potenciómetro 
      return 1;
     }
  return _retvalue;
 
  
} 
 
bool Pot::potec4(bool parte, byte modificar)
{
  if (parte == false)
  {
    // FILTRADO DE VALOR ANALOGICO POR UMBRAL.  ANALOG VALUE FILTERING BY THRESHOLD
    _value = analogRead(_pin); // modificar pin de lectura
    tmp = (_oldValue - _value);
    if (tmp >= 8 || tmp <= -8) // Estos 2 valores es el filtro de ruido o sensibilidad, a mas, mas filtro. These 2 values is the noise filter or sensitivity, more, more filter.
    {
      _oldValue = _value >> 3;
      _oldValue = _oldValue << 3;
//      _value >> 3;
      Potec[modificar] = _value >> 9; // Reemplazo por el nuevo valor del potenciómetro      
      return 1;
    }
    return 0;
  }  
}
bool Pot::potec5(bool parte, byte modificar)
{
  if (parte == false)
  {
    // FILTRADO DE VALOR ANALOGICO POR UMBRAL.  ANALOG VALUE FILTERING BY THRESHOLD
    _value = analogRead(_pin); // modificar pin de lectura
    tmp = (_oldValue - _value);
    if (tmp >= 8 || tmp <= -8) // Estos 2 valores es el filtro de ruido o sensibilidad, a mas, mas filtro. These 2 values is the noise filter or sensitivity, more, more filter.
    {
      _oldValue = _value >> 3;
      _oldValue = _oldValue << 3;
//      _value >> 3;
      Potec[modificar] = _value >> 3; // Reemplazo por el nuevo valor del potenciómetro      
      return 1;
    }
    return 0;
  }  
}  
/*****
Verifico si hubo cambio en el potenciómetro asignado al pitchbend
devuelvo 0 falso, 1 hubo cambio y está el valor en la variable _retvalue
******/

bool Pot::pitchbend(int valr)
{
  _value = valr;
  if (_value != _oldbend)
  {
    _oldbend = _value;
    return 1;
  }
  return 0;
}

/*****
Devuelvo la variable _retvalue que se actualiza con 
******/
int Pot::pitchvalue()
{
  return _retvalue; 
} 
bool Pot::potec6(bool parte, byte modificar)
{
  if (parte == false)
  {
    // FILTRADO DE VALOR ANALOGICO POR UMBRAL.  ANALOG VALUE FILTERING BY THRESHOLD
    _value = analogRead(_pin); // modificar pin de lectura
    tmp = (_oldValue - _value);
    if (tmp >= 8 || tmp <= -8) // Estos 2 valores es el filtro de ruido o sensibilidad, a mas, mas filtro. These 2 values is the noise filter or sensitivity, more, more filter.
    {
      _oldValue = _value >> 3;
      _oldValue = _oldValue << 3;
//      _value >> 3;
      Potec[modificar] = _value >> 3; // Reemplazo por el nuevo valor del potenciómetro      
      return 1;
    }
    return 0;
  }  
}  