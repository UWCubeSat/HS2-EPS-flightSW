// ADC control
// Enable ADC control
// Read ADC values : IAC
//                   IBAT
//                   VAC
//                   VBAT
//                   TS
//                   VFB



void enableADC(); 

float readIAC_ADC();

float readIBAT_ADC();

float readVAC_ADC(); 

float readVBAT_ADC();

float readTS_ADC(float REGN);

float readVFB_ADC();