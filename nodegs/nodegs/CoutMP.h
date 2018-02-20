/*****************************************************************************\
*                                                                             *
* CoutMP.h -      Cout functions                                              *
*                                                                             *
*               Version 1.00                                                  *
*                                                                             *
*               Copyright (c) 2016-2016, Lord's OwChart. All rights reserved. *
*                                                                             *
*******************************************************************************/

#ifndef __COUTMP__H__
#define __COUTMP__H__
#pragma once

#include <iostream>  
#include <sstream>  
#include <omp.h>  
class CoutMP
{  
private:  
   std::ostringstream oss;  
   CoutMP(): oss(std::ostringstream::out)
   {  
   }  
public:  
    static CoutMP Print()
	{  
       return CoutMP();  
    }    
    template <class T>  
    CoutMP& operator<<(const T& input)
	{  
      oss << input;  
      return *this;  
    }   
    enum Action
	{  
       ENDL,  
        PRINT,  
    };  
   CoutMP& operator<<(const Action& input)
   {  
        switch(input)
		{  
           case ENDL:
			   {  
                oss << "\n";  
			   }  
           case PRINT:
			   {  
					std::cout << oss.str();  
					std::cout.flush();  
					oss.clear();     
			   }
			   break;  
        }  
        return *this;  
    }  
};  


#endif