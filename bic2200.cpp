//#################################################################################################
// Library to Control a BIC-2200-XX-CAN with a Arduino and a MCP2525
// Uses the Arduino CAN Libary by Sandeep Mistry
// Copyright (c) Dominic Eichinger. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
//#################################################################################################

#include "bic2200.h"

int BIC2200::begin(int CS_Pin, byte CAN_Adress) {
//#################################################################################################
//  Function:       begin
//  Access:         Public
//  Input:          CS_Pin (int) Chip Select Pin 
//                  CAN_Adress (byte) [0x00 - 0x07] CAN Bus Adress of BIC-2200
//  Output:         (int) 0 = CAN Initialisiation unsuccessfull; 1 = CAN Initialisiation successfull
//  Description:    Initialises the CAN Object and Calculates the CAN IDs
//#################################################################################################
    _CS = CS_Pin;
    _CAN_ID_SEND = MSG_ID_CAN_SEND_00 + CAN_Adress;
    _CAN_ID_RECEIVE = MSG_ID_CAN_RECEIVE_00 + CAN_Adress;

    CAN.setPins(_CS);
    CAN.setClockFrequency(CAN_CLK_FREQUENCY);
    return CAN.begin(CAN_BAUDRATE);

}

float BIC2200::readTemperature() {
//#################################################################################################
//  Function:       readTemperature
//  Access:         Public
//  Input:          -
//  Output:         (float) -1.0 = Temperature Readout Unsuccessfull; > 0.0 Temperature of BIC-2200
//  Description:    Read the Internal Temperature of BIC-2200
//#################################################################################################
    byte data[2];
    float temperature = -1.0;
    if (_getRegisterValue(CMD_READ_TEMPERATURE_1, data)) {
        temperature = ( ( data[1] << 8 ) + data[0]) * 0.1;
    }
    return temperature;
}

float BIC2200::readInputVoltage() {
//#################################################################################################
//  Function:       readInputVoltage
//  Access:         Public
//  Input:          -
//  Output:         (float) -1.0 = Voltage Readout Unsuccessfull; > 0.0 Input Voltage of BIC-2200
//  Description:    Read the Input Voltage of BIC-2200
//#################################################################################################
    byte data[2];
    float temperature = -1.0;
    if (_getRegisterValue(CMD_READ_VIN, data)) {
        temperature = ( ( data[1] << 8 ) + data[0]) * 0.1;
    }
    return temperature;
}

float BIC2200::readOutputVoltage() {
//#################################################################################################
//  Function:       readOutputVoltage
//  Access:         Public
//  Input:          -
//  Output:         (float) -1.0 = Voltage Readout Unsuccessfull; > 0.0 Output Voltage of BIC-2200
//  Description:    Read the Output Voltage of BIC-2200
//#################################################################################################
    byte data[2];
    float temperature = -1.0;
    if (_getRegisterValue(CMD_READ_VOUT, data)) {
        temperature = ( ( data[1] << 8 ) + data[0]) * 0.01;
    }
    return temperature;
}
float BIC2200::readOutputCurrent() {
//#################################################################################################
//  Function:       readOutputCurrent
//  Access:         Public
//  Input:          -
//  Output:         (float) -1.0 = Current Readout Unsuccessfull; > 0.0 Output Current of BIC-2200
//  Description:    Read the Output Current of BIC-2200
//#################################################################################################
    byte data[2];
    float temperature = -1.0;
    if (_getRegisterValue(CMD_READ_IOUT, data)) {
        temperature = ( ( data[1] << 8 ) + data[0]) * 0.01;
    }
    return temperature;
}

void BIC2200::setSystemConfig(int config) {
//#################################################################################################
//  Function:       setSystemConfig
//  Access:         Public
//  Input:          (int) config [0-5] 
//                       0 = 0x000 = CAN Controll disabled; Preset Power OFF
//                       1 = 0x001 = CAN Controll enabled; Preset Power OFF
//                       2 = 0x010 = CAN Controll disabled; Preset Power ON
//                       3 = 0x011 = CAN Controll enabled; Preset Power ON
//                       4 = 0x100 = CAN Controll disabled; Preset is previous set value
//                       5 = 0x101 = CAN Controll enabled; Preset is previous set value
//  Output:         -
//  Description:    Set the System Config Register of BIC-2200
//#################################################################################################
    byte data[2];
    data[0] = lowByte(config);
    data[1] = highByte(config);
    
    _setRegisterValue(CMD_SYSTEM_CONFIG, data, 2);
}

int BIC2200::getSystemConfig() {
//#################################################################################################
//  Function:       getSystemConfig
//  Access:         Public
//  Input:          -
//  Output:         (int) config [0-5] 
//                       0 = 0x000 = CAN Controll disabled; Preset Power OFF
//                       1 = 0x001 = CAN Controll enabled; Preset Power OFF
//                       2 = 0x010 = CAN Controll disabled; Preset Power ON
//                       3 = 0x011 = CAN Controll enabled; Preset Power ON
//                       4 = 0x100 = CAN Controll disabled; Preset is previous set value
//                       5 = 0x101 = CAN Controll enabled; Preset is previous set value
//  Description:    Get the System Config Register of BIC-2200
//#################################################################################################
    byte data[2];
    int config = -1;
    if ( _getRegisterValue(CMD_SYSTEM_CONFIG, data) ) {
        config = ( ( data[1] << 8 ) + data[0]);
    }
    return config;
}

void BIC2200::setBidirecitonalConfig(int config) {
//#################################################################################################
//  Function:       setBidirecitonalConfig
//  Access:         Public
//  Input:          (int) config [0-1] 
//                       0 = 0x000 = Bi-direction auto-detect mode. DIR_CTRL and C/D control (analogy) UN-controllable.
//                       1 = 0x001 = Bi-direction battery mode. DIR_CTRL and C/D control (analogy) controllable
//  Output:         -
//  Description:    Set the Bidirectional Config Register of BIC-2200
//#################################################################################################
    byte data[2];
    data[0] = lowByte(config);
    data[1] = highByte(config);
    
    _setRegisterValue(CMD_BIDIRECTIONAL_CONFIG, data, 2);
}

int BIC2200::getBidirecitonalConfig() {
//#################################################################################################
//  Function:       getBidirecitonalConfig
//  Access:         Public
//  Input:          -
//  Output:         (int) config [0-1] 
//                       0 = 0x000 = Bi-direction auto-detect mode. DIR_CTRL and C/D control (analogy) UN-controllable.
//                       1 = 0x001 = Bi-direction battery mode. DIR_CTRL and C/D control (analogy) controllable
//  Description:    Get the Bidirectional Config Register of BIC-2200
//#################################################################################################
    byte data[2];
    int config = -1;
    if ( _getRegisterValue(CMD_BIDIRECTIONAL_CONFIG, data) ) {
        config = ( ( data[1] << 8 ) + data[0]);
    }
    return config;
}

void BIC2200::setOutputVoltage(int voltage) {
//#################################################################################################
//  Function:       setOutputVoltage
//  Access:         Public
//  Input:          (int) voltage (Factor 0.01)
//  Output:         -
//  Description:    Sets the Output Voltage of the BIC-2200 (4800 = 48.0V)
//#################################################################################################
    byte data[2];
    data[0] = lowByte(voltage);
    data[1] = highByte(voltage);
    
    _setRegisterValue(CMD_VOUT_SET, data, 2);
}

int BIC2200::getOutputVoltage() {
//#################################################################################################
//  Function:       getOutputVoltage
//  Access:         Public
//  Input:          -
//  Output:         (int) voltage (Factor 0.01)
//  Description:    Gets the Output Voltage of the BIC-2200 (4800 = 48.0V)
//#################################################################################################
    byte data[2];
    int voltage = -1;
    if ( _getRegisterValue(CMD_VOUT_SET, data) ) {
        voltage = ( ( data[1] << 8 ) + data[0]);
    }
    return voltage;
}

void BIC2200::setOutputCurrent(int current) {
//#################################################################################################
//  Function:       setOutputCurrent
//  Access:         Public
//  Input:          (int) current (Factor 0.01)
//  Output:         -
//  Description:    Sets the Output Current of the BIC-2200 (1000 = 10.0A)
//#################################################################################################
    byte data[2];
    data[0] = lowByte(current);
    data[1] = highByte(current);
    
    _setRegisterValue(CMD_IOUT_SET, data, 2);
}

int BIC2200::getOutputCurrent() {
//#################################################################################################
//  Function:       getOutputCurrent
//  Access:         Public
//  Input:          -
//  Output:         (int) current (Factor 0.01)
//  Description:    Gets the Output Current of the BIC-2200 (1000 = 10.0A)
//#################################################################################################
    byte data[2];
    int current = -1;
    if ( _getRegisterValue(CMD_IOUT_SET, data) ) {
        current = ( ( data[1] << 8 ) + data[0]);
    }
    return current;
}

void BIC2200::setReverseOutputVoltage(int voltage) {
//#################################################################################################
//  Function:       setReverseOutputVoltage
//  Access:         Public
//  Input:          (int) voltage (Factor 0.01)
//  Output:         -
//  Description:    Sets the Reverse Output Voltage of the BIC-2200 (4800 = 48.0V)
//#################################################################################################
    byte data[2];
    data[0] = lowByte(voltage);
    data[1] = highByte(voltage);
    
    _setRegisterValue(CMD_REVERSE_VOUT_SET, data, 2);
}

int BIC2200::getReverseOutputVoltage() {
//#################################################################################################
//  Function:       getReverseOutputVoltage
//  Access:         Public
//  Input:          -
//  Output:         (int) voltage (Factor 0.01)
//  Description:    Gets the Reverse Output Voltage of the BIC-2200 (4800 = 48.0V)
//#################################################################################################
    byte data[2];
    int voltage = -1;
    if ( _getRegisterValue(CMD_REVERSE_VOUT_SET, data) ) {
        voltage = ( ( data[1] << 8 ) + data[0]);
    }
    return voltage;
}

void BIC2200::setReverseOutputCurrent(int current) {
//#################################################################################################
//  Function:       setReverseOutputCurrent
//  Access:         Public
//  Input:          (int) current (Factor 0.01)
//  Output:         -
//  Description:    Sets the Reverse Output Current of the BIC-2200 (1000 = 10.0A)
//#################################################################################################
    byte data[2];
    data[0] = lowByte(current);
    data[1] = highByte(current);
    
    _setRegisterValue(CMD_REVERSE_IOUT_SET, data, 2);
}

int BIC2200::getReverseOutputCurrent() {
//#################################################################################################
//  Function:       getReverseOutputCurrent
//  Access:         Public
//  Input:          -
//  Output:         (int) current (Factor 0.01)
//  Description:    Gets the Reverse Output Current of the BIC-2200 (1000 = 10.0A)
//#################################################################################################
    byte data[2];
    int current = -1;
    if ( _getRegisterValue(CMD_REVERSE_IOUT_SET, data) ) {
        current = ( ( data[1] << 8 ) + data[0]);
    }
    return current;
}

void BIC2200::setOperation(bool operation) {
//#################################################################################################
//  Function:       setOperation
//  Access:         Public
//  Input:          (bool) operation - false = OFF; true = ON
//  Output:         -
//  Description:    Sets the Device ON or OFF
//#################################################################################################
    byte data[1];
    if (operation) {
        data[0] = 0x01;
    } else {
        data[0] = 0x00;
    }
    
    _setRegisterValue(CMD_OPERATION, data, 1);
}

int BIC2200::getOperation() {
//#################################################################################################
//  Function:       getOperation
//  Access:         Public
//  Input:          -
//  Output:         (bool) operation - false = OFF; true = ON
//  Description:    Gets the State of the Device (ON/OFF)
//#################################################################################################
    byte data[1];
    int operation = -1;
    if (_getRegisterValue(CMD_OPERATION, data)) {
        operation = data[0];
    }
    return operation;
}

void BIC2200::setDirection(bool direction) {
//#################################################################################################
//  Function:       setDirection
//  Access:         Public
//  Input:          (bool) direction - false = AC->DC; true = DC->AC
//  Output:         -
//  Description:    Sets the Power Direction of the BIC-2200 
//#################################################################################################
    byte data[1];
    if (direction) {
        data[0] = 0x01;
    } else {
        data[0] = 0x00;
    }
    
    _setRegisterValue(CMD_DIRECTION_CTRL, data, 1);
}

int BIC2200::getDirection() {
//#################################################################################################
//  Function:       getDirection
//  Access:         Public
//  Input:          -
//  Output:         (bool) direction - false = AC->DC; true = DC->AC
//  Description:    Gets the Power Direction of the BIC-2200 
//#################################################################################################
    byte data[1];
    int direction = -1;
    if (_getRegisterValue(CMD_DIRECTION_CTRL, data)) {
        direction = data[0];
    }
    return direction;
}

int BIC2200::getSystemStatus() {
//#################################################################################################
//  Function:       getDirection
//  Access:         Public
//  Input:          -
//  Output:         (int) Details: https://www.meanwell.com/upload/pdf/bic-2200-e.pdf
//  Description:    Gets the System State of the BIC-2200
//#################################################################################################
    byte data[2];
    int status = -1;
    if ( _getRegisterValue(CMD_SYSTEM_STATUS, data) ) {
        status = ( ( data[1] << 8 ) + data[0]);
    }
    return status;
}

int BIC2200::getScalingFactors() {
//#################################################################################################
//  Function:       getScalingFactors
//  Access:         Public
//  Input:          -
//  Output:         (int) Details: https://www.meanwell.com/upload/pdf/bic-2200-e.pdf
//  Description:    Gets the Scaling Factors of the BIC-2200
//#################################################################################################
    byte data[2];
    int factors = -1;
    if ( _getRegisterValue(CMD_SCALING_FACTOR, data) ) {
        factors = ( ( data[1] << 8 ) + data[0]);
    }
    return factors;
}


int BIC2200::_getRegisterValue(int reg, byte * data){
//#################################################################################################
//  Function:       _getRegisterValue
//  Access:         Private
//  Input:          reg (int) Register to Read from BIC2200
//                  data (byte *) Pointer to a Byte Array where the data should be written to
//  Output:         (int) 0 = data Array not Valid; 1 = data Array is valid
//  Description:    Reads a Register from BIC-2200 
//#################################################################################################
    unsigned long startTime;
    unsigned int index = 0;
    int retVal = 0;
    byte data_received[8];
    bool blnDataValid = true;
    
    CAN.beginExtendedPacket(_CAN_ID_SEND);
    CAN.write(lowByte(reg));
    CAN.write(highByte(reg));
    CAN.endPacket();

    startTime = micros();

    while (((micros() - startTime ) < CAN_TIMEOUT ) && blnDataValid ) {
        int packetSize = CAN.parsePacket();
        if (packetSize || CAN.packetId() != -1) {
            while (CAN.available() && index < 8) {
                data_received[index] = CAN.read();
                ++index;
            }
            if (lowByte(reg) == data_received[0] && highByte(reg) == data_received[1]) {
                for (unsigned int i = 0; i < (index - 2); i++) {
                    data[i] = data_received[i + 2];
                }
            } else {
                blnDataValid = false;
            }
            if (blnDataValid) {
                retVal = 1; 
            }
        }
    }  

    return retVal;
}

void BIC2200::_setRegisterValue(int reg, byte * data, int len){
//#################################################################################################
//  Function:       _getRegisterValue
//  Access:         Private
//  Input:          reg (int) Register to Write to BIC2200
//                  data (byte *) Pointer to a Byte Array containing the Data to Write
//  Output:         (int) 0 = Register Value set unsuccessfull; 1 = Register Value sucessfully written
//  Description:    Writes a Register to BIC-2200 
//#################################################################################################

    CAN.beginExtendedPacket(_CAN_ID_SEND);
    CAN.write(lowByte(reg));
    CAN.write(highByte(reg));
    for (int i = 0; i < len; i++) {
        CAN.write(data[i]);
    }
    CAN.endPacket();
}