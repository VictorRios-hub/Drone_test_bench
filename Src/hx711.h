#ifndef HX711_H_
#define HX711_H_

void init_pins();

float trl_grams(int value);

void HX711_set_offset(int n);

int HX711_read_mean(int n);

unsigned int HX711_read();

float HX711_get_value_g(int n);

#endif /* HX711_H_ */
