
#include <iostream>
#include "TMV.h"

int main() try 
{
    // Several ways to create and initialize matrices:

    // Create with uninitialized values
    tmv::Matrix<double,tmv::NoDivider> m1(4,3); 
    // In debugging mode (the default), these are all initialized to 888
    // to help you more easily notice when you have failed to correctly
    // initialize a matrix.
    std::cout<<"m1 = \n"<<m1<<std::endl;
    //! m1 = 
    //! 4  3
    //! (  888  888  888  )
    //! (  888  888  888  )
    //! (  888  888  888  )
    //! (  888  888  888  )
    //! 

    // Initialize with direct element access:
    for(size_t i=0;i<m1.nrows();i++) 
        for(size_t j=0;j<m1.ncols();j++) 
            m1(i,j) = 2.*i-3.*j+10.; 
    std::cout<<"m1 = \n"<<m1<<std::endl;
    //! m1 = 
    //! 4  3
    //! (  10  7  4  )
    //! (  12  9  6  )
    //! (  14  11  8  )
    //! (  16  13  10  )
    //! 

    // Create with all 2's.
    tmv::Matrix<double,tmv::NoDivider> m2(4,3,2.); 
    std::cout<<"m2 = \n"<<m2<<std::endl;
    //! m2 = 
    //! 4  3
    //! (  2  2  2  )
    //! (  2  2  2  )
    //! (  2  2  2  )
    //! (  2  2  2  )
    //! 

    // Create from given elements in a C array;
    double mm[12] = {1,2,3,4, 5,6,7,8, 9,10,11,12};
    tmv::Matrix<double,tmv::NoDivider> m3(4,3,mm); // Default order is ColMajor
    std::cout<<"m3 (ColMajor) = \n"<<m3<<std::endl;
    //! m3 (ColMajor) = 
    //! 4  3
    //! (  1  5  9  )
    //! (  2  6  10  )
    //! (  3  7  11  )
    //! (  4  8  12  )
    //! 

    tmv::Matrix<double,tmv::RowMajor | tmv::NoDivider> m4(4,3,mm);
    std::cout<<"m4 (RowMajor) = \n"<<m4<<std::endl;
    //! m4 (RowMajor) = 
    //! 4  3
    //! (  1  2  3  )
    //! (  4  5  6  )
    //! (  7  8  9  )
    //! (  10  11  12  )
    //! 

    // Initialize with << notation.
    // Note that this is more intuitive with RowMajor Storage
    m3 <<
        2, -5,  1, -3, 
       -4,  8, -7,  1,
       -1,  2,  4,  0;
    std::cout<<"m3 (ColMajor) => \n"<<m3<<std::endl;
    //! m3 (ColMajor) => 
    //! 4  3
    //! (  2  -4  -1  )
    //! (  -5  8  2  )
    //! (  1  -7  4  )
    //! (  -3  1  0  )
    //! 

    m4 <<
        2, -4,  1,
       -5,  8,  2,
        1, -7,  4,
       -3,  1,  0;
    std::cout<<"m4 (RowMajor) => \n"<<m4<<std::endl;
    //! m4 (RowMajor) => 
    //! 4  3
    //! (  2  -4  1  )
    //! (  -5  8  2  )
    //! (  1  -7  4  )
    //! (  -3  1  0  )
    //! 

    // Create from STL vector of vectors
    std::vector<std::vector<double> > mm2(3,std::vector<double>(3));
    for(size_t i=0;i<3;i++) 
        for(size_t j=0;j<3;j++) 
            mm2[i][j] = 2.*i+j-3.*i*j;
    tmv::Matrix<double,tmv::NoDivider> m5(mm2); 
    std::cout<<"m5 = \n"<<m5<<std::endl;
    //! m5 = 
    //! 3  3
    //! (  0  1  2  )
    //! (  2  0  -2  )
    //! (  4  -1  -6  )
    //! 


    // Norms, etc. 

    std::cout<<"Norm1(m1) = "<<Norm1(m1)<<std::endl;
    //! Norm1(m1) = 52
#if 0
    // TODO Turn this back on once svn is written.
    std::cout<<"Norm2(m1) = "<<Norm2(m1)<<std::endl;
    //! Norm2(m1) = Warning:
    //! Calling Matrix::Norm2 without previously calling DivideUsing(SV)
    //! 36.452
    // This is potentially inefficient, especially for large matrices.
    // To avoid the warning, you can use:
    std::cout<<"Norm2(m1) = "<<m1.doNorm2()<<std::endl;
    //! Norm2(m1) = 36.452
#endif
    std::cout<<"NormInf(m1) = "<<NormInf(m1)<<std::endl;
    //! NormInf(m1) = 39
    std::cout<<"NormF(m1) = "<<NormF(m1);
    std::cout<<" = "<<Norm(m1)<<std::endl;
    //! NormF(m1) = 36.4966 = 36.4966
    std::cout<<"MaxAbsElement(m1) = "<<MaxAbsElement(m1)<<std::endl;
    //! MaxAbsElement(m1) = 16
    std::cout<<"Trace(m5) = "<<Trace(m5)<<std::endl;
    //! Trace(m5) = -6
    std::cout<<"Det(m5) = "<<Det(m5)<<std::endl;
    //! Det(m5) = 0


    // Views:

    std::cout<<"m1.row(1) = "<<m1.row(1)<<std::endl;
    //! m1.row(1) = 3 ( 12  9  6 )
    std::cout<<"m1.col(2) = "<<m1.col(2)<<std::endl;
    //! m1.col(2) = 4 ( 4  6  8  10 )
    std::cout<<"m1.diag() = "<<m1.diag()<<std::endl;
    //! m1.diag() = 3 ( 10  9  8 )
    std::cout<<"m1.diag(1) = "<<m1.diag(1)<<std::endl;
    //! m1.diag(1) = 2 ( 7  6 )
    std::cout<<"m1.diag(-1) = "<<m1.diag(-1)<<std::endl;
    //! m1.diag(-1) = 3 ( 12  11  10 )
    std::cout<<"m1.subMatrix(2,4,0,2) = \n"<<m1.subMatrix(2,4,0,2)<<std::endl;
    //! m1.subMatrix(2,4,0,2) = 
    //! 2  2
    //! (  14  11  )
    //! (  16  13  )
    //! 
    std::cout<<"m1.subMatrix(0,4,1,3,2,1) = \n"<<
        m1.subMatrix(0,4,1,3,2,1)<<std::endl;
    //! m1.subMatrix(0,4,1,3,2,1) = 
    //! 2  2
    //! (  7  4  )
    //! (  11  8  )
    //! 
    std::cout<<"m1.transpose() = \n"<<m1.transpose()<<std::endl;
    //! m1.transpose() = 
    //! 3  4
    //! (  10  12  14  16  )
    //! (  7  9  11  13  )
    //! (  4  6  8  10  )
    //! 
    std::cout<<"m1.colRange(1,3) = \n"<<m1.colRange(1,3)<<std::endl;
    //! m1.colRange(1,3) = 
    //! 4  2
    //! (  7  4  )
    //! (  9  6  )
    //! (  11  8  )
    //! (  13  10  )
    //! 
    std::cout<<"m1.rowPair(3,0) = \n"<<m1.rowPair(3,0)<<std::endl;
    //! m1.rowPair(3,0) = 
    //! 2  3
    //! (  16  13  10  )
    //! (  10  7  4  )
    //! 
    // Can use the views within expressions
    m1.rowRange(0,3) += m5.transpose();
    std::cout<<"m1.rowRange(0,3) += m5.transpose() => m1 = \n"<<m1<<std::endl;
    //! m1.rowRange(0,3) += m5.transpose() => m1 = 
    //! 4  3
    //! (  10  9  8  )
    //! (  13  9  5  )
    //! (  16  9  2  )
    //! (  16  13  10  )
    //! 
    m1.row(0) *= 2.;
    std::cout<<"m1.row(0) *= 2 => m1 = \n"<<m1<<std::endl;
    //! m1.row(0) *= 2 => m1 = 
    //! 4  3
    //! (  20  18  16  )
    //! (  13  9  5  )
    //! (  16  9  2  )
    //! (  16  13  10  )
    //! 


    // Fortran Indexing:

    tmv::Matrix<double,tmv::ColMajor|tmv::NoDivider,tmv::FortranStyle> fm1 = m1;
    std::cout<<"fm1 = m1 = \n"<<fm1<<std::endl;
    //! fm1 = m1 = 
    //! 4  3
    //! (  20  18  16  )
    //! (  13  9  5  )
    //! (  16  9  2  )
    //! (  16  13  10  )
    //! 
    std::cout<<"fm1(1,1) = "<<fm1(1,1)<<std::endl;
    //! fm1(1,1) = 20
    std::cout<<"fm1(4,3) = "<<fm1(4,3)<<std::endl;
    //! fm1(4,3) = 10
    std::cout<<"fm1.row(1) = "<<fm1.row(1)<<std::endl;
    //! fm1.row(1) = 3 ( 20  18  16 )
    std::cout<<"fm1.col(3) = "<<fm1.col(3)<<std::endl;
    //! fm1.col(3) = 4 ( 16  5  2  10 )
    std::cout<<"fm1.subMatrix(2,3,1,2) = \n"<<fm1.subMatrix(2,3,1,2)<<std::endl;
    //! fm1.subMatrix(2,3,1,2) = 
    //! 2  2
    //! (  13  9  )
    //! (  16  9  )
    //! 
    std::cout<<"fm1.subMatrix(1,3,2,3,2,1) = \n"<<
        fm1.subMatrix(1,3,2,3,2,1)<<std::endl;
    //! fm1.subMatrix(1,3,2,3,2,1) = 
    //! 2  2
    //! (  18  16  )
    //! (  9  2  )
    //! 
    std::cout<<"fm1.colRange(1,2) = \n"<<fm1.colRange(1,2)<<std::endl;
    //! fm1.colRange(1,2) = 
    //! 4  2
    //! (  20  18  )
    //! (  13  9  )
    //! (  16  9  )
    //! (  16  13  )
    //! 
    std::cout<<"fm1.rowPair(4,1) = \n"<<fm1.rowPair(4,1)<<std::endl;
    //! fm1.rowPair(4,1) = 
    //! 2  3
    //! (  16  13  10  )
    //! (  20  18  16  )
    //! 


    // Matrix arithmetic:

    tmv::Matrix<double,tmv::NoDivider> m1pm3 = m1 + m3;  
    std::cout<<"m1 + m3 = \n"<<m1pm3<<std::endl;
    //! m1 + m3 = 
    //! 4  3
    //! (  22  14  15  )
    //! (  8  17  7  )
    //! (  17  2  6  )
    //! (  13  14  10  )
    //! 
    // Works correctly even if matrices are stored in different order:
    tmv::Matrix<double,tmv::NoDivider> m3pm4 = m3 + m4; 
    std::cout<<"m3 + m4 = \n"<<m3pm4<<std::endl;
    //! m3 + m4 = 
    //! 4  3
    //! (  4  -8  0  )
    //! (  -10  16  4  )
    //! (  2  -14  8  )
    //! (  -6  2  0  )
    //! 

    m1 *= 2.;  
    std::cout<<"m1 *= 2 = \n"<<m1<<std::endl;
    //! m1 *= 2 = 
    //! 4  3
    //! (  40  36  32  )
    //! (  26  18  10  )
    //! (  32  18  4  )
    //! (  32  26  20  )
    //! 

    m1 += m4; 
    std::cout<<"m1 += m4 = \n"<<m1<<std::endl;
    //! m1 += m4 = 
    //! 4  3
    //! (  42  32  33  )
    //! (  21  26  12  )
    //! (  33  11  8  )
    //! (  29  27  20  )
    //! 

    // Vector outer product
    tmv::Vector<double> v1 = m4.col(0);
    tmv::Vector<double> v2 = m4.row(1);
    tmv::Matrix<double,tmv::NoDivider> v1v2 = v1^v2;
    std::cout<<"v1 = "<<v1<<std::endl;
    //! v1 = 4 ( 2  -5  1  -3 )
    std::cout<<"v2 = "<<v2<<std::endl;
    //! v2 = 3 ( -5  8  2 )
    std::cout<<"v1^v2 = \n"<<v1v2<<std::endl;
    //! v1^v2 = 
    //! 4  3
    //! (  -10  16  4  )
    //! (  25  -40  -10  )
    //! (  -5  8  2  )
    //! (  15  -24  -6  )
    //! 
    std::cout<<"ColVectorViewOf(v1)*RowVectorViewOf(v2) = \n"<<
        ColVectorViewOf(v1)*RowVectorViewOf(v2)<<std::endl;
    //! ColVectorViewOf(v1)*RowVectorViewOf(v2) = 
    //! 4  3
    //! (  -10  16  4  )
    //! (  25  -40  -10  )
    //! (  -5  8  2  )
    //! (  15  -24  -6  )
    //! 

    // Matrix * Vector product
    std::cout<<"m1 * v2 = "<<m1*v2<<std::endl;
    //! m1 * v2 = 4 ( 112  127  -61  111 )
    std::cout<<"v1 * m1 = "<<v1*m1<<std::endl;
    //! v1 * m1 = 3 ( -75  -136  -46 )

    // Matrix * Matrix product
    tmv::Matrix<double,tmv::NoDivider> m1m5 = m1 * m5; 
    std::cout<<"m1 * m5 = \n"<<m1m5<<std::endl;
    //! m1 * m5 = 
    //! 4  3
    //! (  196  9  -178  )
    //! (  100  9  -82  )
    //! (  54  25  -4  )
    //! (  134  9  -116  )
    //! 
    std::cout<<"m1.row(0) * m5.col(2) = "<<m1.row(0)*m5.col(2)<<std::endl;
    //! m1.row(0) * m5.col(2) = -178
    std::cout<<"(m1 * m5)(0,2) = "<<m1m5(0,2)<<std::endl;
    //! (m1 * m5)(0,2) = -178

    // Can handle aliases:
    std::cout<<"m1 + 3*m1-m2 = \n"<<m1+3.*m1-m2<<std::endl;
    //! m1 + 3*m1-m2 = 
    //! 4  3
    //! (  166  126  130  )
    //! (  82  102  46  )
    //! (  130  42  30  )
    //! (  114  106  78  )
    //! 
    m1 += 3.*m1-m2;
    std::cout<<"m1 += 3*m1-m2 = \n"<<m1<<std::endl;
    //! m1 += 3*m1-m2 = 
    //! 4  3
    //! (  166  126  130  )
    //! (  82  102  46  )
    //! (  130  42  30  )
    //! (  114  106  78  )
    //! 
    std::cout<<"m5 * m5 = \n"<<m5*m5<<std::endl;
    //! m5 * m5 = 
    //! 3  3
    //! (  10  -2  -14  )
    //! (  -8  4  16  )
    //! (  -26  10  46  )
    //! 
    m5 *= m5;
    std::cout<<"m5 *= m5 = \n"<<m5<<std::endl;
    //! m5 *= m5 = 
    //! 3  3
    //! (  10  -2  -14  )
    //! (  -8  4  16  )
    //! (  -26  10  46  )
    //! 

    // Scalars can be treated as a multiple of identity matrix
    m5 += 32.;
    std::cout<<"m5 += 32 = \n"<<m5<<std::endl;
    //! m5 += 32 = 
    //! 3  3
    //! (  42  -2  -14  )
    //! (  -8  36  16  )
    //! (  -26  10  78  )
    //! 


    // Complex matrices:

    tmv::Matrix<std::complex<double>,tmv::NoDivider > cm4 = m4 * std::complex<double>(1,2);
    std::cout<<"cm4 = m4 * (1+2i) = \n"<<cm4<<std::endl;
    //! cm4 = m4 * (1+2i) = 
    //! 4  3
    //! (  (2,4)  (-4,-8)  (1,2)  )
    //! (  (-5,-10)  (8,16)  (2,4)  )
    //! (  (1,2)  (-7,-14)  (4,8)  )
    //! (  (-3,-6)  (1,2)  (0,0)  )
    //! 
    std::cout<<"cm4.conjugate() = \n"<<cm4.conjugate()<<std::endl;
    //! cm4.conjugate() = 
    //! 4  3
    //! (  (2,-4)  (-4,8)  (1,-2)  )
    //! (  (-5,10)  (8,-16)  (2,-4)  )
    //! (  (1,-2)  (-7,14)  (4,-8)  )
    //! (  (-3,6)  (1,-2)  (0,-0)  )
    //! 
    std::cout<<"cm4.transpose() = \n"<<cm4.transpose()<<std::endl;
    //! cm4.transpose() = 
    //! 3  4
    //! (  (2,4)  (-5,-10)  (1,2)  (-3,-6)  )
    //! (  (-4,-8)  (8,16)  (-7,-14)  (1,2)  )
    //! (  (1,2)  (2,4)  (4,8)  (0,0)  )
    //! 
    std::cout<<"cm4.adjoint() = \n"<<cm4.adjoint()<<std::endl;
    //! cm4.adjoint() = 
    //! 3  4
    //! (  (2,-4)  (-5,10)  (1,-2)  (-3,6)  )
    //! (  (-4,8)  (8,-16)  (-7,14)  (1,-2)  )
    //! (  (1,-2)  (2,-4)  (4,-8)  (0,-0)  )
    //! 
    std::cout<<"cm4.realPart() = \n"<<cm4.realPart()<<std::endl;
    //! cm4.realPart() = 
    //! 4  3
    //! (  2  -4  1  )
    //! (  -5  8  2  )
    //! (  1  -7  4  )
    //! (  -3  1  0  )
    //! 
    std::cout<<"cm4.imagPart() = \n"<<cm4.imagPart()<<std::endl;
    //! cm4.imagPart() = 
    //! 4  3
    //! (  4  -8  2  )
    //! (  -10  16  4  )
    //! (  2  -14  8  )
    //! (  -6  2  0  )
    //! 
    std::cout<<"Norm(cm4) = "<<Norm(cm4)<<std::endl;
    //! Norm(cm4) = 30.8221
    std::cout<<"cm4*cm4.adjoint() = \n"<<cm4*cm4.adjoint()<<std::endl;
    //! cm4*cm4.adjoint() = 
    //! 4  4
    //! (  (105,0)  (-200,0)  (170,0)  (-50,0)  )
    //! (  (-200,0)  (465,0)  (-265,0)  (115,0)  )
    //! (  (170,0)  (-265,0)  (330,0)  (-50,0)  )
    //! (  (-50,0)  (115,0)  (-50,0)  (50,0)  )
    //! 

    // Can mix real and complex in any combination
    std::cout<<"cm4 - m4 = \n"<<cm4 - m4<<std::endl;
    //! cm4 - m4 = 
    //! 4  3
    //! (  (0,4)  (0,-8)  (0,2)  )
    //! (  (0,-10)  (0,16)  (0,4)  )
    //! (  (0,2)  (0,-14)  (0,8)  )
    //! (  (0,-6)  (0,2)  (0,0)  )
    //! 
    std::cout<<"cm4 * m4.transpose() * (1-2i) = \n"<<
        cm4 * m4.transpose() * std::complex<double>(1,-2)<<std::endl;
    //! cm4 * m4.transpose() * (1-2i) = 
    //! 4  4
    //! (  (105,0)  (-200,0)  (170,0)  (-50,0)  )
    //! (  (-200,0)  (465,0)  (-265,0)  (115,0)  )
    //! (  (170,0)  (-265,0)  (330,0)  (-50,0)  )
    //! (  (-50,0)  (115,0)  (-50,0)  (50,0)  )
    //! 
    
    return 0;
} 
catch (tmv::Error& e) 
{
    std::cerr<<e<<std::endl;
    return 1;
}
