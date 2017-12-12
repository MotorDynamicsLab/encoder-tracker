#include <SPI.h>

#define NSS_PIN 22 
#define READ_ENA_PIN 49
#define NULL 0

static const int spi_encoder_offset = 4;
enum SpiEncoders
{
  _X1 = 0b0001,
  _X2 = 0b0010,
  _Y  = 0b0100,
  _Z  = 0b1000,
};

void setup()
{
  pinMode(READ_ENA_PIN, INPUT_PULLUP);
  pinMode(NSS_PIN, OUTPUT);
  digitalWrite(NSS_PIN, HIGH);

  Serial.begin(9600);
  
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(SPI_CLOCK_DIV4);
  SPI.begin();
  //SPI.beginTransaction(SPISettings(20000000, MSBFIRST, SPI_MODE0));
}


void loop()
{
  const int readPeriodMs = 500;
  
  if (LOW == digitalRead(READ_ENA_PIN))
  {
    long EncoderData = ReadEncoder(_X1);
    Serial.print("EncoderData: ");
    Serial.println(EncoderData);
  }
  
  delay(readPeriodMs);
}


void ResetEncoder(SpiEncoders encoders)
{
  const byte reset_cmd = 0x00;
  
  digitalWrite(NSS_PIN,LOW);
  SPI.transfer(reset_cmd | (encoders << spi_encoder_offset) );
  digitalWrite(NSS_PIN,HIGH);
}


long ReadEncoder(SpiEncoders encoder)
{
  const byte read_cmd  = 0x0F;
  int readSize = 0;
  long encoderVal = 0;;
  
  digitalWrite(NSS_PIN,LOW);
  SPI.transfer(read_cmd | (encoder << spi_encoder_offset) );
  delayMicroseconds(8);

  for (int i = 0; i < readSize; ++i)
  {
    encoderVal = SPI.transfer(1);
    encoderVal << 8;
  }
  
  digitalWrite(NSS_PIN,HIGH);
}

