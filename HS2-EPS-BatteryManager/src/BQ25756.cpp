// Outer class for BQ25756 should be included here
#include "BQ25756.h"

class BQ25756{
public: 
    void getStatus(){
        Serial.println("BQ25756 is initialized")
    }

    /**
     *  @brief Reset register
     * 
     *  Reset all the registers to the default value 
     *  by writing REG_RST to 1.
     *  REG_RST goes back to 0 automatically after writing to 1.
     */
    void resetRegister()
    {
        uint8_t regRstVal = read8bitRegister(POW_PATH_REV_CONT);
        uint8_t writeVal = regRstVal | 0x80;
        writeRegister(POW_PATH_REV_CONT, writeVal);
        
    }

    class ADCControl{
        public:
            // Enable ADC Control
            // ADC should be enabled before reading ADC value
            void enableADC()
            {
                uint8_t currVal = read8bitRegister(ADC_CONT);
                uint8_t newVal = currVal & 0x80;
                writeRegister(ADC_CONT, newVal);
            }

            // Return
            //      bool: True if ADC control is enabled
            bool ADCEnabled()
            {
                return (read8bitRegister(ADC_CONT) >> 7) == 1;
            }
            
            // Get ADC conversion rate
            // Return
            //      bool: True if Continuous conversion, False if One-shot conversion
            bool getADCRate()
            {
                return (read8bitRegister(ADC_CONT) >> 6) == 0;
            }

            // TODO: Figure out ADC sample speed code

            // Enable IAC ADC Control
            void enableIAC_ADC()
            {
                uint8_t currVal = read8bitRegister(ADC_CHANNEL_CONT);
                uint8_t newVal = currVal & 0x3F;
                writeRegister(ADC_CHANNEL_CONT, newVal);
            }

            // Enable IBAT ADC Control
            void enableIBAT_ADC()
            {
                uint8_t currVal = read8bitRegister(ADC_CHANNEL_CONT);
                uint8_t newVal = currVal & 0xBF;
                writeRegister(ADC_CHANNEL_CONT, newVal);
            }

            // Enable VAC ADC Control
            void enableVAC_ADC()
            {
                uint8_t currVal = read8bitRegister(ADC_CHANNEL_CONT);
                uint8_t newVal = currVal & 0xDF;
                writeRegister(ADC_CHANNEL_CONT, newVal);
            }

            // Enable VBAT ADC Control
            void enableVBAT_ADC()
            {
                uint8_t currVal = read8bitRegister(ADC_CHANNEL_CONT);
                uint8_t newVal = currVal & 0xEF;
                writeRegister(ADC_CHANNEL_CONT, newVal);
            }

            // Enable TS ADC Control
            void enableTS_ADC()
            {
                uint8_t currVal = read8bitRegister(ADC_CHANNEL_CONT);
                uint8_t newVal = currVal & 0xFB;
                writeRegister(ADC_CHANNEL_CONT, newVal);
            }

            // Enable VFB ADC Control
            void enableVFB_ADC()
            {
                uint8_t currVal = read8bitRegister(ADC_CHANNEL_CONT);
                uint8_t newVal = currVal & 0xFD;
                writeRegister(ADC_CHANNEL_CONT, newVal);
            }

            // Enable All ADC Channel Control
            // This enable ADC Control for IAC, IBAT, VAC, VBAT, TS, VFB
            void enableAllADCControl()
            {
                uint8_t currVal = read8bitRegister(ADC_CHANNEL_CONT);
                uint8_t newVal = 0x00;
                writeRegister(ADC_CHANNEL_CONT, newVal);
            }





    }
    
}