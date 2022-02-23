#include <iobb.h>
#include "hx711.h"

// PINS du HX711:
// VDD = VCC = 3.3V
// GND -> GND

// DATAOUT -> P9 11
#define CON_DATA 9
#define PIN_DATA 11

// CLK -> P9 13 
#define CON_CLK 9
#define PIN_CLK 13



// OFFSET et PENTE conversion grammes
int  OFFSET =  0;
int  PENTE = 160;

// Initialisation des pins pour la connexion au capteur
void init_pins(){
    
    iolib_init();

    //Le pin data
    iolib_setdir(CON_DATA, PIN_DATA, DigitalIn);

    // Le pin clock
    iolib_setdir(CON_CLK, PIN_CLK, DigitalOut);
    pin_high(CON_CLK, PIN_CLK);
}

float trl_grams(int value)
{
    return (value-OFFSET)/PENTE;
}

void HX711_set_offset(int n)
{
   OFFSET = HX711_read_mean(n);
}

int HX711_read_mean(int n){
    int sum = 0;
    char i = 0;
    for(i = 0; i < n; i++)
    {
        sum = sum + HX711_read();
    }
    return sum / n;
}

unsigned int HX711_read(){
    unsigned int data = 0;
    unsigned int newbit;
    unsigned char i;

    pin_low(CON_CLK, PIN_CLK);
    data = 0;
    while(is_high(CON_DATA, PIN_DATA));
    for (i = 0; i < 24; i++){
        pin_high(CON_CLK, PIN_CLK);
        
        newbit = is_high(CON_DATA, PIN_DATA);

        pin_low(CON_CLK, PIN_CLK);

        data = (data << 1) | newbit;

    }
    pin_high(CON_CLK, PIN_CLK);
    // La donnée est envoyée en complément à deux
    int value;
    data = data << 8;
    int * p = &data;
    value = *p;
    value = value / 256;

    return(value);
}

//Renvoie la mesure en gramme
float HX711_get_value_g(int n)
{
  return (HX711_read_mean(n)-OFFSET)/PENTE;
}

