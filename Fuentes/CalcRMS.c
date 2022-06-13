#include "xparameters.h"
#include "xil_io.h"
#include "FilAVG.h"
#include <stdio.h>
#include <stdint.h>
#include "sleep.h"
 
// clk     => S_AXI_ACLK,
//             a_rst   => slv_reg0(0),
//             s_rst   => slv_reg0(1),
//             ena     => slv_reg3(0),
//             sigin   => slv_reg1(15 downto 0),
//             sigout  => salFil(15 downto 0)

typedef union {
    struct {
        uint8_t b0:1;
        uint8_t b1:1;
        uint32_t baux:30;
    };
    struct {
        uint32_t bitsData:32;
    };
} Bits_t;

typedef union {
    struct {
        uint16_t signal:16;
        uint16_t sigaux:16;
    };
    struct {
        uint32_t signalData:32;
    };
} Signal_t;

//tabla datos de señal senoidal rectificada
uint16_t seno16[] = {0x0000,0x33a8,0x5f73,0x7cb6,0x86fd,0x7cb6,0x5f73,0x33a8,0x0000,0x33a8,0x5f73,0x7cb6,0x86fd,0x7cb6,0x5f73,0x33a8};

//valor de entrada para múltiplicar la señal
uint8_t valor = 1;

Bits_t reset_data = {.bitsData=0};
Bits_t enable_data = {.bitsData=0};
Signal_t input_data = {.signalData=0};
Signal_t output_data = {.signalData=0};

int main (void) {

	uint8_t i = 0;
	uint8_t count = 20;
	float mul = 1;
	float tension = 220.0;

    xil_printf("-- Inicio del programa.... --\r\n");
    xil_printf("-- Para voltaje 220V presione A \r\n-- Para voltaje 110V presione otro \r\n");
    valor = inbyte();

    if(valor=='a'){
    	mul=1;
    }
    else {mul=0.5;
    }

    xil_printf("Valor ingresado: %d V\r\n", (uint16_t)(mul*tension));

    FILAVG_mWriteReg(XPAR_FILAVG_0_S_AXI_BASEADDR, FILAVG_S_AXI_SLV_REG0_OFFSET, reset_data.bitsData);
    FILAVG_mWriteReg(XPAR_FILAVG_0_S_AXI_BASEADDR, FILAVG_S_AXI_SLV_REG3_OFFSET, enable_data.bitsData);
    usleep(5000);

    while(1){
        //setear enable
        enable_data.b0 = 1;
        FILAVG_mWriteReg(XPAR_FILAVG_0_S_AXI_BASEADDR, FILAVG_S_AXI_SLV_REG3_OFFSET, enable_data.bitsData);

        //enviar un dato
        input_data.signal = (uint16_t)(mul*seno16[i]);
        FILAVG_mWriteReg(XPAR_FILAVG_0_S_AXI_BASEADDR, FILAVG_S_AXI_SLV_REG1_OFFSET, input_data.signalData);

        //sacar dato
        uint32_t res = FILAVG_mReadReg(XPAR_FILAVG_0_S_AXI_BASEADDR, FILAVG_S_AXI_SLV_REG2_OFFSET);
        xil_printf("Valor RMS: %d V\r\n", res/100);

        //borrar enable
        enable_data.b0 = 0;
        FILAVG_mWriteReg(XPAR_FILAVG_0_S_AXI_BASEADDR, FILAVG_S_AXI_SLV_REG3_OFFSET, enable_data.bitsData);

        i = (i + 1) % 16;

        count--;
        if(count==0){
        	count = 20;
        	reset_data.b0 = 1;
        	FILAVG_mWriteReg(XPAR_FILAVG_0_S_AXI_BASEADDR, FILAVG_S_AXI_SLV_REG0_OFFSET, reset_data.bitsData);
        	usleep(5000);
        	reset_data.b0 = 0;
        	FILAVG_mWriteReg(XPAR_FILAVG_0_S_AXI_BASEADDR, FILAVG_S_AXI_SLV_REG0_OFFSET, reset_data.bitsData);
        	xil_printf("Reset...\r\n");
        }

        sleep(1);

    }
}
