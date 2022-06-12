#include "xparameters.h"
#include "xil_io.h"
#include "sum_ip.h"
#inlcude <stdio.h>
 

// clk     => S_AXI_ACLK,
//             a_rst   => slv_reg0(0),
//             s_rst   => slv_reg0(1),
//             ena     => slv_reg3(0),
//             sigin   => slv_reg1(15 downto 0),
//             sigout  => salFil(15 downto 0)
typedef union Bits_t{
    struct {
        uint8_t b0:1;
        uint8_t b1:1;
        uint32_t baux:30
    }
    struct {
        uint32_t bitsData:32
    }
};

typedef union Signal_t{
    struct {
        uint16_t signal:16;
        uint16_t sigaux:16
    }
    struct {
        uint32_t signalData:32
    }
};

//tabla datos de señal senoidal
uint16_t seno16 [16] = {0x0000,0x33a8,0x5f73,0x7cb6,0x86fd,0x7cb6,0x5f73,0x33a8,0x0000,0x33a8,0x5f73,0x7cb6,0x86fd,0x7cb6,0x5f73,0x33a8}

//valor de entrada para múltiplicar la señal
uint8_t valor = 1;

int main (void) {

    //variables
    Bits_t reset_data;
    reset_data.bitsData = 0;

    Bits_t enable_data;
    enable_data.bitsData = 0;

    Signal_t input_data;
    input_data.signalData = 0;

    Signal_t output_data;
    output_data.signalData = 0;

    xil_printf("-- Inicio del programa para validar el uso de IP cores propios --\r\n");
    valor = inbyte();
    xil_printf("Valor ingresado: %d\r\n", valor);

    while(1){
        //setear enable
        enable_data.b0 = 1;
        SUM_IP_mWriteReg(XPAR_SUM_IP_S_AXI_BASEADDR, SUM_IP_S_AXI_SLV_REG0_OFFSET, enable_data.bitsData);
        //enviar un dato
        //aumentar el indice
        //borrar enable
    }
    
    SUM_IP_mWriteReg(XPAR_SUM_IP_S_AXI_BASEADDR, SUM_IP_S_AXI_SLV_REG0_OFFSET, opA);
    SUM_IP_mWriteReg(XPAR_SUM_IP_S_AXI_BASEADDR, SUM_IP_S_AXI_SLV_REG1_OFFSET, opB);
    res = SUM_IP_mReadReg(XPAR_SUM_IP_S_AXI_BASEADDR, SUM_IP_S_AXI_SLV_REG2_OFFSET);

    xil_printf("Cuenta: %d + %d = %d\r\n", opA, opB, res);

}