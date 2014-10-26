/*******************************************************************************
 *  modules.cpp
 *  Teensy3
 *
 * Purpose:     Provides routines for turning on/off selected modules.
 *
 * NOTE:        !!! Since this is work in progress not all situations
 *              have not been explored. Certain modules might need
 *              extra configurations that have not been implemented 
 *              yet!!!
 *******************************************************************************/

#include "module.h"
#include "bitband.h"

#define SIM_SCGC5_LPTIMER_BIT   0x00
#define SIM_SCGC4_USBOTG_BIT    0x12
#define SIM_SCGC4_VREF_BIT      0x14
#define SIM_SCGC4_UART2_BIT     0x0C
#define SIM_SCGC4_UART1_BIT     0x0B
#define SIM_SCGC4_UART0_BIT     0x0A
#define SIM_SCGC5_TSI_BIT       0x05
#define SIM_SCGC6_RTC_BIT       0x1D
#define SIM_SCGC6_SPI0_BIT      0x0C
#define SIM_SCGC6_PDB_BIT       0x16
#define SIM_SCGC6_I2S_BIT       0x0F
#define SIM_SCGC4_I2C0_BIT      0x06
#define SIM_SCGC4_CMP_BIT       0x13
#define SIM_SCGC6_ADC0_BIT      0x1B

void adcEnable() {
    //if ((SIM_SCGC6 & SIM_SCGC6_ADC0) == 0x00) {
    if ((BITBAND_U32(SIM_SCGC6, SIM_SCGC6_ADC0_BIT)) == 0x00) {
        BITBAND_U32(SIM_SCGC6, SIM_SCGC6_ADC0_BIT) = 0x01;
        //SIM_SCGC6 |= SIM_SCGC6_ADC0;// enable ADC clock
        ADC0_SC1A |= ADC_SC1_ADCH(0x05);// enable ADC
    }
}

void adcDisable() {
    //if (SIM_SCGC6 & SIM_SCGC6_ADC0) {
    if ((BITBAND_U32(SIM_SCGC6, SIM_SCGC6_ADC0_BIT)) == 0x01) {
        ADC0_SC1A |= ADC_SC1_ADCH(0x1F);// disable ADC
        BITBAND_U32(SIM_SCGC6, SIM_SCGC6_ADC0_BIT) = 0x00;
        //SIM_SCGC6 &= ~SIM_SCGC6_ADC0;// disable ADC clock
    }
}

void cmp0Enable() {
    //if ((SIM_SCGC4 & SIM_SCGC4_CMP) == 0x00) {
    if ((BITBAND_U32(SIM_SCGC4, SIM_SCGC4_CMP_BIT)) == 0x00) {
        BITBAND_U32(SIM_SCGC4, SIM_SCGC4_CMP_BIT) = 0x01;
        //SIM_SCGC4 |= SIM_SCGC4_CMP;// enable CMP0 clock
        CMP0_CR1 |= CMP_CR1_EN_MASK;// enable CMP0
    }else if ((CMP0_CR1 & CMP_CR1_EN_MASK) == 0x00) CMP0_CR1 |= CMP_CR1_EN_MASK;
}

void cmp0Disable() {
    //if (SIM_SCGC4 & SIM_SCGC4_CMP) {
    if ((BITBAND_U32(SIM_SCGC4, SIM_SCGC4_CMP_BIT)) == 0x01) {
        CMP0_CR1 &= ~CMP_CR1_EN_MASK;// disable CMP
        BITBAND_U32(SIM_SCGC4, SIM_SCGC4_CMP_BIT) = 0x00;
        //SIM_SCGC4 &= ~SIM_SCGC4_CMP;// disable CMP0 clock
    }
}

void cmp1Enable() {
    //if ((SIM_SCGC4 & SIM_SCGC4_CMP) == 0x00) {
    if ((BITBAND_U32(SIM_SCGC4, SIM_SCGC4_CMP_BIT)) == 0x00) {
        BITBAND_U32(SIM_SCGC4, SIM_SCGC4_CMP_BIT) = 0x01;
        //SIM_SCGC4 |= SIM_SCGC4_CMP;// enable CMP1 clock
        CMP1_CR1 |= CMP_CR1_EN_MASK;// enable CMP1
    }else if ((CMP1_CR1 & CMP_CR1_EN_MASK) == 0x00) CMP1_CR1 |= CMP_CR1_EN_MASK;
}

void cmp1Disable() {
    //if (SIM_SCGC4 & SIM_SCGC4_CMP) {
    if ((BITBAND_U32(SIM_SCGC4, SIM_SCGC4_CMP_BIT)) == 0x01) {
        CMP1_CR1 &= ~CMP_CR1_EN_MASK;// disable CMP1
        BITBAND_U32(SIM_SCGC4, SIM_SCGC4_CMP_BIT) = 0x00;
        //SIM_SCGC4 &= ~SIM_SCGC4_CMP;// disable CMP1 clock
    }
}

void dmaEnable() {
    
}

void dmaDisable() {
    
}

void i2cEnable() {
    //if ((SIM_SCGC4 & SIM_SCGC4_I2C0) == 0x00) {
    if ((BITBAND_U32(SIM_SCGC4, SIM_SCGC4_I2C0_BIT)) == 0x00) {
        BITBAND_U32(SIM_SCGC4, SIM_SCGC4_I2C0_BIT) = 0x01;
        //SIM_SCGC4 |= SIM_SCGC4_I2C0;// enable I2C clock
        I2C0_C1 |= I2C_C1_IICEN;// enable I2C
    }
}

void i2cDisable() {
    //if (SIM_SCGC4 & SIM_SCGC4_I2C0) {
    if ((BITBAND_U32(SIM_SCGC4, SIM_SCGC4_I2C0_BIT)) == 0x01) {
        I2C0_C1 &= ~I2C_C1_IICEN;// disable I2C
        BITBAND_U32(SIM_SCGC4, SIM_SCGC4_I2C0_BIT) = 0x00;
        //SIM_SCGC4 &= ~SIM_SCGC4_I2C0;// disable I2C clock
    }
}

void i2sEnable() {
    //if ((SIM_SCGC6 & SIM_SCGC6_I2S) == 0x00) {
    if ((BITBAND_U32(SIM_SCGC6, SIM_SCGC6_I2S_BIT)) == 0x00) {
        BITBAND_U32(SIM_SCGC6, SIM_SCGC6_I2S_BIT) = 0x01;
        //SIM_SCGC6 |= SIM_SCGC6_I2S;// enable I2S clock
        I2S0_TCSR |= I2S_TCSR_TE_MASK;// enable I2S
    }
}

void i2sDisable() {
    //if (SIM_SCGC6 & SIM_SCGC6_I2S) {
    if ((BITBAND_U32(SIM_SCGC6, SIM_SCGC6_I2S_BIT)) == 0x01) {
        I2S0_TCSR &= ~I2S_TCSR_TE_MASK;// disable I2S
        BITBAND_U32(SIM_SCGC6, SIM_SCGC6_I2S_BIT) = 0x00;
        //SIM_SCGC6 &= ~SIM_SCGC6_I2S;// disable I2S clock
    }
}

void lptmEnable() {
    //if (!(SIM_SCGC5 & SIM_SCGC5_LPTIMER) || !(LPTMR0_CSR & LPTMR_CSR_TEN_MASK)) {
    if ((BITBAND_U32(SIM_SCGC5, SIM_SCGC5_LPTIMER_BIT)) == 0x00) {
        BITBAND_U32(SIM_SCGC5, SIM_SCGC5_LPTIMER_BIT) = 0x01;
        //SIM_SCGC5 |= SIM_SCGC5_LPTIMER;// enable LPTMR clock
        LPTMR0_CSR |= LPTMR_CSR_TEN_MASK;// enable LPTMR
    }
}

void lptmDisable() {
    //if (SIM_SCGC5 & SIM_SCGC5_LPTIMER) {
    if ((BITBAND_U32(SIM_SCGC5, SIM_SCGC5_LPTIMER_BIT)) == 0x01) {
        //LPTMR0_CSR |= LPTMR_CSR_TCF_MASK;
        LPTMR0_CSR &= LPTMR_CSR_TEN_MASK;// disable LPTMR
        BITBAND_U32(SIM_SCGC5, SIM_SCGC5_LPTIMER_BIT) = 0x00;
        //SIM_SCGC5 &= ~SIM_SCGC5_LPTIMER;// disable LPTIMER clock
    }
}
void pdbEnable() {
    //if ((SIM_SCGC6 & SIM_SCGC6_PDB) == 0x00) {
    if ((BITBAND_U32(SIM_SCGC6, SIM_SCGC6_PDB_BIT)) == 0x00) {
        BITBAND_U32(SIM_SCGC6, SIM_SCGC6_PDB_BIT) = 0x01;
        //SIM_SCGC6 |= SIM_SCGC6_PDB;// enable PDB clock
        PDB0_SC |= PDB_SC_PDBEN;// enable PDB
    }
}

void pdbDisable() {
    //if (SIM_SCGC6 & SIM_SCGC6_PDB) {
    if ((BITBAND_U32(SIM_SCGC6, SIM_SCGC6_PDB_BIT)) == 0x01) {
        PDB0_SC &= ~PDB_SC_PDBEN;// disable PDB
        BITBAND_U32(SIM_SCGC6, SIM_SCGC6_PDB_BIT) = 0x00;
        //SIM_SCGC6 &= ~SIM_SCGC6_PDB;// disable PDB clock
    }
}

void spiEnable() {
    //if ((SIM_SCGC6 & SIM_SCGC6_SPI0) == 0x00) {
    if ((BITBAND_U32(SIM_SCGC6, SIM_SCGC6_SPI0_BIT)) == 0x00) {
        BITBAND_U32(SIM_SCGC6, SIM_SCGC6_SPI0_BIT) = 0x01;
        //SIM_SCGC6 |= SIM_SCGC6_SPI0;// enable SPI clock
        SPI0_MCR |= SPI_MCR_MDIS;// enable SPI
    }
}

void spiDisable() {
    //if (SIM_SCGC6 & SIM_SCGC6_SPI0) {
    if ((BITBAND_U32(SIM_SCGC6, SIM_SCGC6_SPI0_BIT)) == 0x01) {
        SPI0_MCR &= ~SPI_MCR_MDIS;// disable SPI
        BITBAND_U32(SIM_SCGC6, SIM_SCGC6_SPI0_BIT) = 0x00;
        //SIM_SCGC6 &= ~SIM_SCGC6_SPI0;// disable SPI clock
    }
}

void rtcEnable() {
    //if ((SIM_SCGC6 & SIM_SCGC6_RTC) == 0x00) {
    if ((BITBAND_U32(SIM_SCGC6, SIM_SCGC6_RTC_BIT)) == 0x00) {
        //SIM_SCGC6 |= SIM_SCGC6_RTC;// enable RTC clock
        BITBAND_U32(SIM_SCGC6, SIM_SCGC6_RTC_BIT) = 0x01;
        RTC_CR |= RTC_CR_OSCE;// enable RTC
    }
}

void rtcDisable() {
    //if (SIM_SCGC6 & SIM_SCGC6_RTC) {
    if ((BITBAND_U32(SIM_SCGC6, SIM_SCGC6_RTC_BIT)) == 0x01) {
        RTC_CR &= ~RTC_CR_OSCE;// disable RTC
        BITBAND_U32(SIM_SCGC6, SIM_SCGC6_RTC_BIT) = 0x00;
        //SIM_SCGC6 &= ~SIM_SCGC6_RTC;// disable RTC clock
    }
}

void tsiEnable() {
    //if ((SIM_SCGC5 & SIM_SCGC5_TSI) == 0x00) {
    if ((BITBAND_U32(SIM_SCGC5, SIM_SCGC5_TSI_BIT)) == 0x00) {
        BITBAND_U32(SIM_SCGC5, SIM_SCGC5_TSI_BIT) = 0x01;
        //SIM_SCGC5 |= SIM_SCGC5_TSI;// enable TSI clock
        TSI0_GENCS |= TSI_GENCS_TSIEN;// enable TSI
    }
}

void tsiDisable() {
    //if (SIM_SCGC5 & SIM_SCGC5_TSI) {
    if ((BITBAND_U32(SIM_SCGC5, SIM_SCGC5_TSI_BIT)) == 0x01) {
       TSI0_GENCS &= ~TSI_GENCS_TSIEN;// disable TSI
       BITBAND_U32(SIM_SCGC5, SIM_SCGC5_TSI_BIT) = 0x00;
       //SIM_SCGC5 &= ~SIM_SCGC5_TSI;// disable TSI clock
    }
}

void uart1Enable() {
    //if ((SIM_SCGC4 & SIM_SCGC4_UART0) == 0x00) {
    if ((BITBAND_U32(SIM_SCGC4, SIM_SCGC4_UART0_BIT)) == 0x00) {
        BITBAND_U32(SIM_SCGC4, SIM_SCGC4_UART0_BIT) = 0x01;
        //SIM_SCGC4 |= SIM_SCGC4_UART0;// enable UART1 clock
        UART0_C2 |= UART_C2_TE;// enable TX
        UART0_C2 |= UART_C2_RE;// enable RX
    }
}

void uart1Disable() {
    //if (SIM_SCGC4 & SIM_SCGC4_UART0) {
    if ((BITBAND_U32(SIM_SCGC4, SIM_SCGC4_UART0_BIT)) == 0x01) {
        UART0_C2 &= ~UART_C2_TE;// disable UART1 TX
        UART0_C2 &= ~UART_C2_RE;// disable UART1 RX
        BITBAND_U32(SIM_SCGC4, SIM_SCGC4_UART0_BIT) = 0x00;
        //SIM_SCGC4 &= ~SIM_SCGC4_UART0;// disable UART1 clock
    }
}

void uart2Enable() {
    //if ((SIM_SCGC4 & SIM_SCGC4_UART1) == 0x00) {
    if ((BITBAND_U32(SIM_SCGC4, SIM_SCGC4_UART1_BIT)) == 0x00) {
        BITBAND_U32(SIM_SCGC4, SIM_SCGC4_UART1_BIT) = 0x01;
        //SIM_SCGC4 |= SIM_SCGC4_UART1;// enable UART2 clock
        UART1_C2 |= UART_C2_TE;// enable TX
        UART1_C2 |= UART_C2_RE;// enable RX
    }
}

void uart2Disable() {
    //if (SIM_SCGC4 & SIM_SCGC4_UART1) {
    if ((BITBAND_U32(SIM_SCGC4, SIM_SCGC4_UART1_BIT)) == 0x01) {
        UART1_C2 &= ~UART_C2_TE;// disable UART2 TX
        UART1_C2 &= ~UART_C2_RE;// disable  UART2 RX
        BITBAND_U32(SIM_SCGC4, SIM_SCGC4_UART1_BIT) = 0x00;
        //SIM_SCGC4 &= ~SIM_SCGC4_UART1;// disable UART2 clock
    }
}

void uart3Enable() {
    //if ((SIM_SCGC4 & SIM_SCGC4_UART2) == 0x00) {
    if ((BITBAND_U32(SIM_SCGC4, SIM_SCGC4_UART2_BIT)) == 0x00) {
        BITBAND_U32(SIM_SCGC4, SIM_SCGC4_UART2_BIT) = 0x01;
        //SIM_SCGC4 |= SIM_SCGC4_UART2;// enable UART3 clock
        UART2_C2 |= UART_C2_TE;// enable TX
        UART2_C2 |= UART_C2_RE;// enable RX
    }
}

void uart3Disable() {
    //if (SIM_SCGC4 & SIM_SCGC4_UART2) {
    if ((BITBAND_U32(SIM_SCGC4, SIM_SCGC4_UART2_BIT)) == 0x01) {
        UART2_C2 &= ~UART_C2_TE;// disable UART3 TX
        UART2_C2 &= ~UART_C2_RE;// disable  UART3 RX
        BITBAND_U32(SIM_SCGC4, SIM_SCGC4_UART2_BIT) = 0x00;
        //SIM_SCGC4 &= ~SIM_SCGC4_UART2;// disable UART3 clock
    }
}

void usbEnable() {
    //if ((SIM_SCGC4 & SIM_SCGC4_USBOTG) == 0x00) {
    if ((BITBAND_U32(SIM_SCGC4, SIM_SCGC4_USBOTG_BIT)) == 0x00) {
        BITBAND_U32(SIM_SCGC4, SIM_SCGC4_USBOTG_BIT) = 0x01;
        //SIM_SCGC4 |= SIM_SCGC4_USBOTG;// enable USB clock
        USB0_USBCTRL &= ~USB_USBCTRL_SUSP;// suspended State
        USB0_CTL |= USB_CTL_USBENSOFEN;// enable USB
        while ((USB0_CTL & USB_CTL_USBENSOFEN) != 0x01) ;
        //USB0_USBTRC0 |= 0x40; // undocumented bit
    }
}

void usbDisable() {
    //if (SIM_SCGC4 & SIM_SCGC4_USBOTG) {
    if ((BITBAND_U32(SIM_SCGC4, SIM_SCGC4_USBOTG_BIT)) == 0x01) {
        USB0_USBCTRL |= USB_USBCTRL_SUSP;// suspended State
        USB0_CTL &= ~USB_CTL_USBENSOFEN;// disable USB
        while ((USB0_CTL & USB_CTL_USBENSOFEN) != 0x00) ;
        BITBAND_U32(SIM_SCGC4, SIM_SCGC4_USBOTG_BIT) = 0x00;
        //SIM_SCGC4 &= ~SIM_SCGC4_USBOTG;// disable USB clock
    }
}

void vrefEnable() {
    //if ((SIM_SCGC4 & SIM_SCGC4_VREF) == 0x00) {
    if ((BITBAND_U32(SIM_SCGC4, SIM_SCGC4_VREF_BIT)) == 0x00) {
        BITBAND_U32(SIM_SCGC4, SIM_SCGC4_VREF_BIT) = 0x01;
        //SIM_SCGC4 |= SIM_SCGC4_VREF;// enable VREF clock
        VREF_SC |= VREF_SC_VREFEN_MASK;// enable VREF
    }
}

void vrefDisable() {
    //if (SIM_SCGC4 & SIM_SCGC4_VREF) {
    if ((BITBAND_U32(SIM_SCGC4, SIM_SCGC4_VREF_BIT)) == 0x01) {
        VREF_SC &= ~VREF_SC_VREFEN_MASK;// disable VREF
        BITBAND_U32(SIM_SCGC4, SIM_SCGC4_VREF_BIT) = 0x00;
        //SIM_SCGC4 &= ~SIM_SCGC4_VREF;// disable VREF clock
    }
}


