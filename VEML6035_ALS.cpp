/*
  This is a library for the VEML6035 Ambient Light Sensor. (https://www.vishay.com/docs/84889/veml6035.pdf)
  It is built on the existing VEML6030 library from Sparkfun:
  https://github.com/sparkfun/SparkFun_Ambient_Light_Sensor_Arduino_Library
  
  By: Nikolas Zingraf
  Date: January 2026
  License: This code is public domain.

 */

#include "VEML6035_ALS.h"

VEML6035::VEML6035(uint8_t address){  _address = address; } //Constructor for I2C

bool VEML6035::begin( TwoWire &wirePort )
{
  
  _i2cPort = &wirePort;

  // Device is powered down by default. 
  powerOn(); 

  _i2cPort->beginTransmission(_address);
  uint8_t _ret = _i2cPort->endTransmission();
  if( !_ret )
    return true; 
  else 
    return false; 

}

// REG0x00, bit[12]
// Sets the sensitivity adjustment of the Ambient Light Sensor.
// The values are either high sensitivity (1 x) [0] or low sensitivity (1/8 x) [1].
void VEML6035::setSENS(bool sensVal){

  uint16_t bits; 

  bits = sensVal;

  _writeRegister(SETTING_REG, SENS_MASK, bits, SENS_POS); 

}

// REG0x00, bit[12]
// Sets the sensitivity adjustment of the Ambient Light Sensor.
// The values are either high sensitivity (1 x) [0] or low sensitivity (1/8 x) [1].
bool VEML6035::readSENS(){

  uint16_t regVal = _readRegister(SETTING_REG); // Get register
  regVal &= (~SENS_MASK); // Invert the SENS mask to _keep_ the SENS bit
  regVal = (regVal >> SENS_POS); // Move value to front of the line. 
   
  if (regVal == 0)
    return 0;
  else if (regVal == 1)
    return 1;
  else   
    return UNKNOWN_ERROR; 

}

// REG0x00, bit[11]
// This function sets the DG value of the Ambient Light Sensor.
// It is either normal [0] or decreased [1].
void VEML6035::setDG(bool dgVal){

  uint16_t bits; 

  bits = dgVal;
  
  _writeRegister(SETTING_REG, DG_MASK, bits, DG_POS); 

}

// REG0x00, bit[11]
// This function sets the DG value of the Ambient Light Sensor.
// It is either normal [0] or decreased [1].
bool VEML6035::readDG(){
 
  uint16_t regVal = _readRegister(SETTING_REG); // Get register
  regVal &= (~DG_MASK); // Invert the DG mask to _keep_ the DG bit
  regVal = (regVal >> DG_POS); // Move value to front of the line. 
   
  if (regVal == 0)
    return 0;
  else if (regVal == 1)
    return 1;
  else   
    return UNKNOWN_ERROR; 
  
}


// REG0x00, bits [10]
// This function sets the gain for the Ambient Light Sensor. Possible values
// are normal or double sensitivity.
void VEML6035::setGain(bool gainVal){

  uint16_t bits; 

  bits = gainVal;
  
  _writeRegister(SETTING_REG, GAIN_MASK, bits, GAIN_POS); 

}

// REG0x00, bits [12:11]
// This function reads the gain for the Ambient Light Sensor. Possible values
// are 1/8, 1/4, 1, and 2. The highest setting should only be used if the
// sensors is behind dark glass, where as the lowest setting should be used in
// dark rooms. The datasheet suggests always leaving it at around 1/4 or 1/8.
bool VEML6035::readGain(){
 
  uint16_t regVal = _readRegister(SETTING_REG); // Get register
  regVal &= (~GAIN_MASK); // Invert the gain mask to _keep_ the gain
  regVal = (regVal >> GAIN_POS); // Move values to front of the line. 
   
  if (regVal == 0)
    return 0;
  else if (regVal == 1)
    return 1;
  else   
    return UNKNOWN_ERROR; 
  
}

// REG0x00, bits[9:6]
// This function sets the integration time (the saturation time of light on the
// sensor) of the ambient light sensor. Higher integration time leads to better
// resolution but slower sensor refresh times. 
void VEML6035::setIntegTime(uint16_t time){ 
 
  uint16_t bits;

  if (time == 100) // Default setting.
    bits = 0; 
  else if (time == 200)
    bits = 1; 
  else if (time == 400)
    bits = 2; 
  else if (time == 800)
    bits = 3; 
  else if (time == 50)
    bits = 8; 
  else if (time == 25)
    bits = 12; 
  else
    return;

  _writeRegister(SETTING_REG, INTEG_MASK, bits, INTEG_POS);  
  

}

// REG0x00, bits[9:6]
// This function reads the integration time (the saturation time of light on the
// sensor) of the ambient light sensor. Higher integration time leads to better
// resolution but slower sensor refresh times. 
uint16_t VEML6035::readIntegTime(){

  uint16_t regVal = _readRegister(SETTING_REG); 
  regVal &= (~INTEG_MASK); 
  regVal = (regVal >> INTEG_POS); 

  if (regVal == 0)
    return 100;
  else if (regVal == 1)
    return 200;
  else if (regVal == 2)
    return 400;
  else if (regVal == 3)
    return 800;
  else if (regVal == 8)
    return 50;
  else if (regVal == 12)
    return 25;
  else   
    return UNKNOWN_ERROR; 

}

// REG0x00, bits[5:4]
// This function sets the persistence protect number. 
void VEML6035::setProtect(uint8_t protVal){

  uint16_t bits; 

  if (protVal == 1)
    bits = 0; 
  else if (protVal == 2)
    bits = 1;
  else if (protVal == 4)
    bits = 2;
  else if (protVal == 8)
    bits = 3;
  else
    return;

  _writeRegister(SETTING_REG, PERS_PROT_MASK, bits, PERS_PROT_POS); 

}

// REG0x00, bits[5:4]
// This function reads the persistence protect number. 
uint8_t VEML6035::readProtect(){

  uint16_t regVal = _readRegister(SETTING_REG); 
  regVal &= (~PERS_PROT_MASK); 
  regVal = (regVal >> PERS_PROT_POS); 

  if (regVal == 0)
    return 1;
  else if (regVal == 1)
    return 2;
  else if (regVal == 2)
    return 4;
  else if (regVal == 3)
    return 8;
  else
    return UNKNOWN_ERROR;

}

// REG0x00, bit[3]
  // This  function selects for which channel the interrupts triggers.
  // 0 = ALS CH Int, 1 = WL CH Int
  void VEML6035::setIntChannel(bool channelVal){

    uint16_t bits; 

    bits = channelVal;
    
    _writeRegister(SETTING_REG, INT_CHANNEL_MASK, bits, INT_CHANNEL_POS); 

  }

  // REG0x00, bit[2]
  // This function enables the white channel.
  void VEML6035::enableWhite(){

    _writeRegister(SETTING_REG, WHITE_CH_EN_MASK, ENABLE, WHITE_CH_EN_POS); 

  }

  // REG0x00, bit[2]
  // This function disables the white channel.
  void VEML6035::disableWhite(){

    _writeRegister(SETTING_REG, WHITE_CH_EN_MASK, DISABLE, WHITE_CH_EN_POS); 

  }

// REG0x00, bit[1]
// This function enables the Ambient Light Sensor's interrupt. 
void VEML6035::enableInt(){

  _writeRegister(SETTING_REG, INT_EN_MASK, ENABLE, INT_EN_POS); 

}

// REG0x00, bit[1]
// This function disables the Ambient Light Sensor's interrupt. 
void VEML6035::disableInt(){

  _writeRegister(SETTING_REG, INT_EN_MASK, DISABLE, INT_EN_POS); 

}

// REG0x00, bit[1]
// This function checks if the interrupt is enabled or disabled. 
uint8_t VEML6035::readIntSetting(){

  uint16_t regVal = _readRegister(SETTING_REG); 
  regVal &= (~INT_EN_MASK); 
  regVal = (regVal >> INT_EN_POS); 
  return regVal;

}

// REG0x00, bit[0]
// This function powers down the Ambient Light Sensor. The light sensor will
// hold onto the last light reading which can be acessed while the sensor is 
// shut down. 0.5 micro Amps are consumed while shutdown. 
void VEML6035::shutDown(){

  _writeRegister(SETTING_REG, SD_MASK, SHUTDOWN , NO_SHIFT);

}

// REG0x00, bit[0]
// This function powers up the Ambient Light Sensor. The last value that was
// read during shut down will be overwritten on the sensor's subsequent read.
// After power up, a small 4ms delay is applied to give time for the internal
// osciallator and signal processor to power up.   
void VEML6035::powerOn(){

  _writeRegister(SETTING_REG, SD_MASK, POWER, NO_SHIFT);
  delay(4);

}

// REG0x03, bit[0]
// This function enables the current power save mode value and puts the Ambient
// Light Sensor into power save mode. 
void VEML6035::enablePowSave(){
    
  _writeRegister(POWER_SAVE_REG, POW_SAVE_EN_MASK, ENABLE, NO_SHIFT);  

}

// REG0x03, bit[0]
// This function disables the current power save mode value and pulls the Ambient
// Light Sensor out of power save mode. 
void VEML6035::disablePowSave(){

  _writeRegister(POWER_SAVE_REG, POW_SAVE_EN_MASK, DISABLE, NO_SHIFT);  

}

// REG0x03, bit[0]
// This function checks to see if power save mode is enabled or disabled. 
uint8_t VEML6035::readPowSavEnabled(){

  uint16_t regVal = _readRegister(POWER_SAVE_REG); 
  regVal &= (~POW_SAVE_EN_MASK); 
  return regVal;

}

// REG0x03, bit[2:1]
// This function sets the power save mode value. It takes a value of 1-4. Each
// incrementally higher value descreases the sampling rate of the sensor and so
// increases power saving. The datasheet suggests enabling these modes when
// continually sampling the sensor. 
// PSM_WAIT: 0:0 = 0.4s, 0:1 = 0.8s, 1:0 = 1.6s, 1:1 = 3.2s
void VEML6035::setPowSavMode(uint16_t modeVal){

  uint16_t bits; 

  if (modeVal == 1)
    bits = 0;
  else if (modeVal == 2)
    bits = 1;
  else if (modeVal == 3)
    bits = 2;
  else if (modeVal == 4)
    bits = 3;
  else 
    return; 

  _writeRegister(POWER_SAVE_REG, POW_SAVE_MASK, bits, PSM_POS);  

}

// REG0x03, bit[2:1]
// This function reads the power save mode value. The function above takes a value of 1-4. Each
// incrementally higher value descreases the sampling rate of the sensor and so
// increases power saving. The datasheet suggests enabling these modes when
// continually sampling the sensor. 
uint8_t VEML6035::readPowSavMode(){

  uint16_t regVal = _readRegister(POWER_SAVE_REG); 
  regVal &= (~POW_SAVE_MASK); 
  regVal = (regVal >> PSM_POS); 
  
  if (regVal == 0)
    return 1; 
  else if (regVal == 1)
    return 2;
  else if (regVal == 2)
    return 3;
  else if (regVal == 3)
    return 4;
  else 
    return UNKNOWN_ERROR;

}

// REG0x06, bits[15:14]
// This function reads the interrupt register to see if an interrupt has been
// triggered. There are two possible interrupts: a lower limit and upper limit 
// threshold, both set by the user.  
uint8_t VEML6035::readInterrupt(){

  uint16_t regVal = _readRegister(INTERRUPT_REG); 
  regVal &= INT_MASK; 
  regVal = (regVal >> INT_POS); 

  if (regVal == 0)
    return NO_INT;
  else if (regVal == 1)
    return INT_HIGH;
  else if (regVal == 2)
    return INT_LOW;
  else
    return UNKNOWN_ERROR;

}

// REG0x02, bits[15:0]
// This function sets the lower limit for the Ambient Light Sensor's interrupt. 
// It takes a lux value as its paramater.
void VEML6035::setIntLowThresh(uint32_t luxVal){

  if (luxVal < 0 || luxVal > 120000)
    return;
  
  uint16_t luxBits = _calculateBits(luxVal); 
  _writeRegister(L_THRESH_REG, THRESH_MASK, luxBits, NO_SHIFT);

}

// REG0x02, bits[15:0]
// This function sets the lower limit for the Ambient Light Sensor's interrupt. 
// It takes the raw value as its paramater.
void VEML6035::setIntLowThreshRAW(uint16_t rawVal){

  uint32_t luxVal = _calculateLux(rawVal);

  if (luxVal < 0 || luxVal > 120000)
    return;
  
  _writeRegister(L_THRESH_REG, THRESH_MASK, rawVal, NO_SHIFT);

}

// REG0x02, bits[15:0]
// This function reads the lower limit for the Ambient Light Sensor's interrupt. 
uint32_t VEML6035::readLowThresh(){

  uint16_t threshVal = _readRegister(L_THRESH_REG);
  uint32_t threshLux = _calculateLux(threshVal); 
  return threshLux; 

}

// REG0x02, bits[15:0]
// This function reads the lower limit (RAW) for the Ambient Light Sensor's interrupt. 
uint16_t VEML6035::readLowThreshRAW(){

  uint16_t threshVal = _readRegister(L_THRESH_REG);
  return threshVal; 

}

// REG0x01, bits[15:0]
// This function sets the upper limit for the Ambient Light Sensor's interrupt. 
// It takes a lux value as its paramater.
void VEML6035::setIntHighThresh(uint32_t luxVal){

  if (luxVal < 0 || luxVal > 120000)
    return;

  uint16_t luxBits = _calculateBits(luxVal); 
  _writeRegister(H_THRESH_REG, THRESH_MASK, luxBits, NO_SHIFT);

}

// REG0x01, bits[15:0]
// This function sets the upper limit for the Ambient Light Sensor's interrupt. 
// It takes the raw value as its paramater.
void VEML6035::setIntHighThreshRAW(uint16_t rawVal){

  uint32_t luxVal = _calculateLux(rawVal);

  if (luxVal < 0 || luxVal > 120000)
    return;

  _writeRegister(H_THRESH_REG, THRESH_MASK, rawVal, NO_SHIFT);

}

// REG0x01, bits[15:0]
// This function reads the upper limit for the Ambient Light Sensor's interrupt. 
uint32_t VEML6035::readHighThresh(){

  uint16_t threshVal = _readRegister(H_THRESH_REG);
  uint32_t threshLux = _calculateLux(threshVal); 
  return threshLux; 

}

// REG0x01, bits[15:0]
// This function reads the upper limit (RAW) for the Ambient Light Sensor's interrupt. 
uint16_t VEML6035::readHighThreshRAW(){

  uint16_t threshVal = _readRegister(H_THRESH_REG);
  return threshVal; 

}

// REG[0x04], bits[15:0]
// This function gets the sensor's ambient light's lux value. The lux value is
// determined based on current gain and integration time settings. If the lux
// value exceeds 1000 then a compensation formula is applied to it. 
uint32_t VEML6035::readLight(){

  uint16_t lightBits =  _readRegister(AMBIENT_LIGHT_DATA_REG); 
  uint32_t luxVal = _calculateLux(lightBits); 

  return luxVal;

}

// REG[0x04], bits[15:0]
// This function gets the sensor's ambient light's RAW lux value.
uint16_t VEML6035::readLightRAW(){

  uint16_t lightBits =  _readRegister(AMBIENT_LIGHT_DATA_REG); 

  return lightBits;

}

// REG[0x05], bits[15:0]
// This function gets the sensor's ambient light's lux value. The lux value is
// determined based on current gain and integration time settings. If the lux
// value exceeds 1000 then a compensation formula is applied to it. 
uint32_t VEML6035::readWhiteLight(){

  uint16_t lightBits = _readRegister(WHITE_LIGHT_DATA_REG); 
  uint32_t luxVal = _calculateLux(lightBits); 

  return luxVal;

}

// REG[0x05], bits[15:0]
// This function gets the sensor's white ambient light's RAW value.
uint16_t VEML6035::readWhiteLightRAW(){

  uint16_t lightBits = _readRegister(WHITE_LIGHT_DATA_REG); 
  
  return lightBits;

}

// The lux value of the Ambient Light sensor depends on both the gain and the
// integration time settings. This function determines which conversion value
// to use by using the bit representation of the gain as an index to look up
// the conversion value in the correct integration time array. It then converts 
// the value and returns it.  
uint32_t VEML6035::_calculateLux(uint16_t _lightBits){

  float _luxConv; 
  uint8_t _convPos;  

  bool _gain = readGain(); 
  bool _dg = readDG();
  bool _sense = readSENS();
  uint16_t _integTime = readIntegTime();

  // Check SENS and GAIN settings
  if (_gain == 1) {
    if (_sense == 0) {
      // GAIN = 1, SENS = 0
      _convPos = 0;
    } else if (_sense == 1) {
      // GAIN = 1, SENS = 1
      _convPos = 2;
    } else {
      return UNKNOWN_ERROR;
    }
  } else if (_gain == 0) {
    if (_sense == 0) {
      // GAIN = 0, SENS = 0
      _convPos = 1;
    } else if (_sense == 1) {
      // GAIN = 0, SENS = 1
      _convPos = 3;
    } else {
      return UNKNOWN_ERROR;
    }
  } else {
    return UNKNOWN_ERROR;
  }

  // Check DG and iteration time settings
  if (_dg == 1) {
    // double
    if(_integTime == 800)
      _luxConv = eightHItDG[_convPos]; 
    else if(_integTime == 400)
      _luxConv = fourHItDG[_convPos];
    else if(_integTime == 200)
      _luxConv = twoHItDG[_convPos];
    else if(_integTime == 100)
      _luxConv = oneHItDG[_convPos];
    else if(_integTime == 50)
      _luxConv = fiftyItDG[_convPos];
    else if(_integTime == 25)
      _luxConv = twentyFiveItDG[_convPos];
    else
      return UNKNOWN_ERROR; 
  } else if (_dg == 0) {
    // normal
    if(_integTime == 800)
      _luxConv = eightHItnoDG[_convPos]; 
    else if(_integTime == 400)
      _luxConv = fourHItnoDG[_convPos];
    else if(_integTime == 200)
      _luxConv = twoHItnoDG[_convPos];
    else if(_integTime == 100)
      _luxConv = oneHItnoDG[_convPos];
    else if(_integTime == 50)
      _luxConv = fiftyItnoDG[_convPos];
    else if(_integTime == 25)
      _luxConv = twentyFiveItnoDG[_convPos];
    else
      return UNKNOWN_ERROR; 
  } else {
    return UNKNOWN_ERROR;
  }

  // Multiply the value from the 16 bit register to the conversion value and return
  // it. 
  uint32_t _calculatedLux = (_luxConv * _lightBits);
  return _calculatedLux;

}


// This function does the opposite calculation then the function above. The interrupt
// threshold values given by the user are dependent on the gain and
// intergration time settings. As a result the lux value needs to be
// calculated with the current settings and this function accomplishes
// that.  
uint16_t VEML6035::_calculateBits(uint32_t _luxVal){

  float _luxConv; 
  uint8_t _convPos;  

  bool _gain = readGain();
  bool _dg = readDG();
  bool _sense = readSENS();
  float _integTime = readIntegTime();

  // Check SENS and GAIN settings
  if (_gain == 1) {
    if (_sense == 0) {
      // GAIN = 1, SENS = 0
      _convPos = 0;
    } else if (_sense == 1) {
      // GAIN = 1, SENS = 1
      _convPos = 2;
    } else {
      return UNKNOWN_ERROR;
    }
  } else if (_gain == 0) {
    if (_sense == 0) {
      // GAIN = 0, SENS = 0
      _convPos = 1;
    } else if (_sense == 1) {
      // GAIN = 0, SENS = 1
      _convPos = 3;
    } else {
      return UNKNOWN_ERROR;
    }
  } else {
    return UNKNOWN_ERROR;
  }

  // Check DG and iteration time settings
  if (_dg == 1) {
    // double
    if(_integTime == 800)
      _luxConv = eightHItDG[_convPos]; 
    else if(_integTime == 400)
      _luxConv = fourHItDG[_convPos];
    else if(_integTime == 200)
      _luxConv = twoHItDG[_convPos];
    else if(_integTime == 100)
      _luxConv = oneHItDG[_convPos];
    else if(_integTime == 50)
      _luxConv = fiftyItDG[_convPos];
    else if(_integTime == 25)
      _luxConv = twentyFiveItDG[_convPos];
    else
      return UNKNOWN_ERROR; 
  } else if (_dg == 0) {
    // normal
    if(_integTime == 800)
      _luxConv = eightHItnoDG[_convPos]; 
    else if(_integTime == 400)
      _luxConv = fourHItnoDG[_convPos];
    else if(_integTime == 200)
      _luxConv = twoHItnoDG[_convPos];
    else if(_integTime == 100)
      _luxConv = oneHItnoDG[_convPos];
    else if(_integTime == 50)
      _luxConv = fiftyItnoDG[_convPos];
    else if(_integTime == 25)
      _luxConv = twentyFiveItnoDG[_convPos];
    else
      return UNKNOWN_ERROR; 
  } else {
    return UNKNOWN_ERROR;
  }

  // Divide the value of lux bythe conversion value and return
  // it. 
  uint16_t _calculatedBits = (_luxVal/_luxConv);
  return _calculatedBits;

}

// This function writes to a 16 bit register. Paramaters include the register's address, a mask 
// for bits that are ignored, the bits to write, and the bits' starting
// position.
void VEML6035::_writeRegister(uint8_t _wReg, uint16_t _mask,\
                                            uint16_t _bits, uint8_t _startPosition)
{
  
  uint16_t _i2cWrite; 

  _i2cWrite = _readRegister(_wReg); // Get the current value of the register
  _i2cWrite &= _mask; // Mask the position we want to write to.
  _i2cWrite |= (_bits << _startPosition);  // Place the given bits to the variable
  _i2cPort->beginTransmission(_address); // Start communication.
  _i2cPort->write(_wReg); // at register....
  _i2cPort->write(_i2cWrite); // Write LSB to register...
  _i2cPort->write(_i2cWrite >> 8); // Write MSB to register...
  _i2cPort->endTransmission(); // End communcation.

}

// This function reads a 16 bit register. It takes the register's
// address as its' parameter.
uint16_t VEML6035::_readRegister(uint8_t _reg)
{

  uint16_t _regValue; 

  _i2cPort->beginTransmission(_address); 
  _i2cPort->write(_reg); // Moves pointer to register.
  _i2cPort->endTransmission(false); // 'False' here sends a restart message so that bus is not released
  _i2cPort->requestFrom(_address, static_cast<uint8_t>(2)); // Two reads for 16 bit registers
  _regValue = _i2cPort->read(); // LSB
  _regValue |= uint16_t(_i2cPort->read()) << 8; //MSB
  return(_regValue);

}

