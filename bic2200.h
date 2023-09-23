//#################################################################################################
// Library to Control a BIC-2200-XX-CAN with a Arduino and a MCP2525
// Uses the Arduino CAN Libary by Sandeep Mistry
// Copyright (c) Dominic Eichinger. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
//#################################################################################################

#ifndef BIC2200_H
#define BIC2200_H

#include <Arduino.h>
#include <CAN.h>

// CAN Registers of BIC-2200:
// Source: https://www.meanwell.com/upload/pdf/bic-2200-e.pdf

#define CMD_OPERATION               0x0000
#define CMD_VOUT_SET                0x0020
#define CMD_IOUT_SET                0x0030
#define CMD_FAULT_STATUS            0x0040
#define CMD_READ_VIN                0x0050
#define CMD_READ_VOUT               0x0060
#define CMD_READ_IOUT               0x0061
#define CMD_READ_TEMPERATURE_1      0x0062
#define CMD_MFR_ID_B0B5             0x0080
#define CMD_MFR_ID_B6B11            0x0081
#define CMD_MFR_MODEL_B0B5          0x0082
#define CMD_MFR_MODEL_B6B11         0x0083
#define CMD_MFR_REVISION_B0B5       0x0084
#define CMD_MFR_LOCATION_B0B2       0x0085
#define CMD_MFR_DATE_B0B5           0x0086
#define CMD_MFR_SERIAL_B0B5         0x0087
#define CMD_MFR_SERIAL_B6B11        0x0088
#define CMD_SCALING_FACTOR          0x00C0
#define CMD_SYSTEM_STATUS           0x00C1
#define CMD_SYSTEM_CONFIG           0x00C2
#define CMD_DIRECTION_CTRL          0x0100
#define CMD_REVERSE_VOUT_SET        0x0120
#define CMD_REVERSE_IOUT_SET        0x0130
#define CMD_BIDIRECTIONAL_CONFIG    0x0140

#define MSG_ID_CAN_SEND_00          0x000C0300    // Add CAN Device Address to this Adress 
#define MSG_ID_CAN_RECEIVE_00       0x000C0200    // Add CAN Device Address to this Adress 
#define MSG_ID_BROADCAST            0x000C0300FF

#define CAN_BAUDRATE 250E3
#define CAN_CLK_FREQUENCY 8E6
#define CAN_TIMEOUT     500


class BIC2200 {

public:
    int begin(int CS_Pin, byte CAN_Adress);

    float readTemperature();
    float readInputVoltage();
    float readOutputVoltage();
    float readOutputCurrent();

    void setSystemConfig(int config);
    int getSystemConfig();
    
    void setBidirecitonalConfig(int config);
    int getBidirecitonalConfig();

    void setOutputVoltage(int voltage);
    int getOutputVoltage();

    void setOutputCurrent(int current);
    int getOutputCurrent();

    void setReverseOutputVoltage(int voltage);
    int getReverseOutputVoltage();

    void setReverseOutputCurrent(int current);
    int getReverseOutputCurrent();

    void setOperation(bool operation);   // false = OFF; true = ON
    int getOperation();

    void setDirection(bool direction);   // false = AC->DC; true = DC-> AC
    int getDirection();

    int getSystemStatus();
    int getScalingFactors();

private:
	int _CS;
    unsigned long _CAN_ID_RECEIVE;
    unsigned long _CAN_ID_SEND;

    int _getRegisterValue(int reg, byte * data);
    void _setRegisterValue(int reg, byte * data, int len);

};

#endif