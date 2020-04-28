#include "mbed.h"

#include "fsl_port.h"

#include "fsl_gpio.h"

#define UINT14_MAX        16383

// FXOS8700CQ I2C address

#define FXOS8700CQ_SLAVE_ADDR0 (0x1E<<1) // with pins SA0=0, SA1=0

#define FXOS8700CQ_SLAVE_ADDR1 (0x1D<<1) // with pins SA0=1, SA1=0

#define FXOS8700CQ_SLAVE_ADDR2 (0x1C<<1) // with pins SA0=0, SA1=1

#define FXOS8700CQ_SLAVE_ADDR3 (0x1F<<1) // with pins SA0=1, SA1=1

// FXOS8700CQ internal register addresses

#define FXOS8700Q_STATUS 0x00

#define FXOS8700Q_OUT_X_MSB 0x01

#define FXOS8700Q_OUT_Y_MSB 0x03

#define FXOS8700Q_OUT_Z_MSB 0x05

#define FXOS8700Q_M_OUT_X_MSB 0x33

#define FXOS8700Q_M_OUT_Y_MSB 0x35

#define FXOS8700Q_M_OUT_Z_MSB 0x37

#define FXOS8700Q_WHOAMI 0x0D

#define FXOS8700Q_XYZ_DATA_CFG 0x0E

#define FXOS8700Q_CTRL_REG1 0x2A

#define FXOS8700Q_M_CTRL_REG1 0x5B

#define FXOS8700Q_M_CTRL_REG2 0x5C

#define FXOS8700Q_WHOAMI_VAL 0xC7


I2C i2c( PTD9,PTD8);

Serial pc(USBTX, USBRX);

DigitalOut led1(LED1);
//DigitalOut led2(LED2);
//DigitalOut led3(LED3);

InterruptIn sw2(SW2);

EventQueue queue;

int m_addr = FXOS8700CQ_SLAVE_ADDR1;

float t[100][4];


void FXOS8700CQ_readRegs(int addr, uint8_t * data, int len);

void FXOS8700CQ_writeRegs(uint8_t * data, int len);

void logger(int m, int n);

/*void test_irq() {

   //queue.call(logger, 1, 100);
   printf("test \r\n");
   //wait(1);
   //led1 = !led1;///////////////

}*/

void sw2_irq() {

   queue.call(logger, 1, 100);
   //printf("inq \r\n");
   //led1 = 1;//////////////////
   //led2=0;
   //led3=1;//////////////
   //queue.call(test_irq);
   //wait(1);
   //led1 = !led1;///////////////

}

int main() {

   pc.baud(115200);

   //EventQueue queue;

   led1 = 1;
   //wait(1);///////////////
   //led1 = !led1;
   //wait(1);
   //led1 = !led1;///////////////

   queue.call(&logger, 0, 1);
   //printf("beforeq \r\n");
   //queue.call(printf, "called immediately\r\n");
   //queue.call(&sw2_irq);
   //printf("afterq \r\n");

   sw2.rise(&sw2_irq);

   queue.dispatch();

   //while(1);

}

void logger(int m, int n) {

   //pc.baud(115200);
   //led1 = 1;//////////////////
   //led2 = 1;
   //led3 = 0;///////////////////
   //wait(5);
   //led1 = 1;///////////////

   uint8_t who_am_i, data[2], res[6];

   int16_t acc16;

   //float t[3];


   // Enable the FXOS8700Q


   FXOS8700CQ_readRegs( FXOS8700Q_CTRL_REG1, &data[1], 1);

   data[1] |= 0x01;

   data[0] = FXOS8700Q_CTRL_REG1;

   FXOS8700CQ_writeRegs(data, 2);


   // Get the slave address

   FXOS8700CQ_readRegs(FXOS8700Q_WHOAMI, &who_am_i, 1);


   //pc.printf("Here is %x\r\n", who_am_i);

   for (int i=m; i<n; i++) {


      FXOS8700CQ_readRegs(FXOS8700Q_OUT_X_MSB, res, 6);


      acc16 = (res[0] << 6) | (res[1] >> 2);

      if (acc16 > UINT14_MAX/2)

         acc16 -= UINT14_MAX;

      t[i][0] = ((float)acc16) / 4096.0f;


      acc16 = (res[2] << 6) | (res[3] >> 2);

      if (acc16 > UINT14_MAX/2)

         acc16 -= UINT14_MAX;

      t[i][1] = ((float)acc16) / 4096.0f;


      acc16 = (res[4] << 6) | (res[5] >> 2);

      if (acc16 > UINT14_MAX/2)

         acc16 -= UINT14_MAX;

      t[i][2] = ((float)acc16) / 4096.0f;

      if(t[i][2]<0 || t[i][2]*t[i][2]<0.5)
         t[i][3] = 1;
      else
         t[i][3] = 0;
      


      //printf("FXOS8700Q ACC: X=%1.4f(%x%x) Y=%1.4f(%x%x) Z=%1.4f(%x%x)\r\n",\

         //   t[0], res[0], res[1],\

           

      led1 = 0;

      wait(0.05);

      led1 = 1;

      wait(0.05);

   }

   if(n==100) {
      for (int i = 0; i < 100; i++){
         for (int j = 0; j < 4; j++){

            pc.printf("%1.3f\r\n", t[i][j]);

            wait(0.1);

         }
      }
   }
}


void FXOS8700CQ_readRegs(int addr, uint8_t * data, int len) {

   char c = addr;

   i2c.write(m_addr, &c, 1, true);

   i2c.read(m_addr, (char *)data, len);

}


void FXOS8700CQ_writeRegs(uint8_t * data, int len) {

   i2c.write(m_addr, (char *)data, len);

}