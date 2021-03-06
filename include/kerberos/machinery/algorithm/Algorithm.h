//
//  Class: Algorithm
//  Description: An algorithm to detect motion using a sequence of three images.
//  Created:     17/07/2014
//  Author:      Cédric Verstraeten
//  Mail:        hello@cedric.ws
//  Website:     www.kerberos.io
//
//  The copyright to the computer program(s) herein
//  is the property of kerberos.io, Belgium.
//  The program(s) may be used and/or copied .
//
/////////////////////////////////////////////////////

#include "Factory.h"
#include "capture/Image.h"

#ifndef __Algorithm_H_INCLUDED__   // if Algorithm.h hasn't been included yet...
#define __Algorithm_H_INCLUDED__   // #define this so the compiler knows it has been included

namespace kerberos
{
    class Algorithm
    {
        protected:
            const char * name;

        public:
            virtual ~Algorithm(){};
            virtual void setup(const StringMap & settings);
            virtual void initialize(ImageVector & images) = 0;
            virtual Image evaluate(ImageVector & images, JSON & data) = 0;
    };
    
    template<const char * Alias, typename Class>
    class AlgorithmCreator: public Algorithm
    {
        protected:
            AlgorithmCreator(){name = ID;}
            
        public:
            static Algorithm * create(){return new Class();}
            static const char * ID;
    };
}
#endif