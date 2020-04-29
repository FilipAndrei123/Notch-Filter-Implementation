/*
 * funcs.h
 *
 * Created: 25.12.2019 18:34:27
 *  Author: Asus
 */ 


#ifndef FUNCS_H_
#define FUNCS_H_

void getTf(double* a,double* b,double wo,double wc,double Te,double u[3],double y[3]);
double lsim(char input,double u[3],double y[3],double* a,double* b);
double HzToRads(double val);
double RadsToHz(double val);
int readADC (char ch);

#endif /* FUNCS_H_ */