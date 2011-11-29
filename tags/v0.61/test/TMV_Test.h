#ifndef TMV_TEST_H
#define TMV_TEST_H

#include <iostream>

//#define LONGDOUBLE

#define EPS (10*tmv::Epsilon<T>())

#define XDEBUG
#include "TMV_Base.h"

extern bool showtests;
extern bool showacc;
extern bool showdiv;
extern bool donorm2;
extern bool showstartdone;
extern bool aliasok;
extern bool symoprod;
extern bool dontthrow;
extern std::string lastsuccess;

void PreAssert(std::string s);
void DoAssert(bool x, std::string s);

#define Assert(x,s) \
do {  \
  PreAssert(s);  \
  DoAssert(x,s); \
} while (false);

extern bool XXDEBUG1;
extern bool XXDEBUG2;
extern bool XXDEBUG3;
extern bool XXDEBUG4;
extern bool XXDEBUG5;
extern bool XXDEBUG6;
extern bool XXDEBUG7;
extern bool XXDEBUG8;
      
#endif