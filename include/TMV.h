///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// The Template Matrix/Vector Library for C++ was created by Mike Jarvis     //
// Copyright (C) 1998 - 2009                                                 //
//                                                                           //
// The project is hosted at http://sourceforge.net/projects/tmv-cpp/         //
// where you can find the current version and current documention.           //
//                                                                           //
// For concerns or problems with the software, Mike may be contacted at      //
// mike_jarvis@users.sourceforge.net                                         //
//                                                                           //
// This program is free software; you can redistribute it and/or             //
// modify it under the terms of the GNU General Public License               //
// as published by the Free Software Foundation; either version 2            //
// of the License, or (at your option) any later version.                    //
//                                                                           //
// This program is distributed in the hope that it will be useful,           //
// but WITHOUT ANY WARRANTY; without even the implied warranty of            //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             //
// GNU General Public License for more details.                              //
//                                                                           //
// You should have received a copy of the GNU General Public License         //
// along with this program in the file LICENSE.                              //
//                                                                           //
// If not, write to:                                                         //
// The Free Software Foundation, Inc.                                        //
// 51 Franklin Street, Fifth Floor,                                          //
// Boston, MA  02110-1301, USA.                                              //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////


#ifndef TMV_H
#define TMV_H

#include "TMV_Vec.h"

#include "TMV_Mat.h"

#include "TMV_Diag.h"

#include "TMV_Tri.h"

// Put the arithmetic header files last, after all the Mult, Add, etc.
// functions have been declared.
#include "tmv/TMV_Norm.h"
#include "tmv/TMV_DivMD.h"
#include "tmv/TMV_ProdXM.h"
#include "tmv/TMV_SumMM.h"
#include "tmv/TMV_SumMX.h"
#include "tmv/TMV_ProdMV.h"
#include "tmv/TMV_OProdVV.h"
#include "tmv/TMV_ProdMM.h"
#include "tmv/TMV_QuotXM.h"
#include "tmv/TMV_QuotVM.h"
#include "tmv/TMV_QuotMM.h"

#endif
