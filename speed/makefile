#EIGEN= -I/home/mjarvis/eigen-2.0.0
#EIGEN= -I/Users/Mike/Astro/eigen-2.0.12
#EIGEN= -I../eigen
#EIGEN= -I/sw/include/eigen2
EIGEN= -I/Users/Mike/Astro/eigen-3.0.1

#INCLUDE= -I../include $(EIGEN)

#CC= icpc
#CFLAGS= $(INCLUDE) -g3 -O -w -DTMV_NO_LIB -DTMV_OPT=3 -openmp -wd279
#CFLAGS= $(INCLUDE) -g3 -fast -w -DTMV_NO_LIB -DTMV_OPT=3 -openmp -wd279
#CFLAGS= $(INCLUDE) -g3 -w -O3 -DTMV_NO_LIB -DTMV_OPT=3 -wd279
#CFLAGS= $(INCLUDE) -openmp -DNOBLAS -wd279
#LIB= -L../lib -ltmv -L/usr/local/intel/mkl/10.0.1.014/lib/32 -lmkl_ia32 -lguide -lpthread

#INCLUDE= -I../../tags/v0.63/include $(EIGEN) -DTMV_V063
INCLUDE= -I../include $(EIGEN) 
#INCLUDE= -I../include $(EIGEN) -I/sw64/include
CC= g++-4
#CFLAGS= $(INCLUDE) -DNDEBUG -g3 -O2 -DTMV_NO_LIB -DTMV_OPT=3
CFLAGS= $(INCLUDE) -DNDEBUG -O2 -DTMV_OPT=3 -fno-var-tracking-assignments
#CFLAGS= $(INCLUDE) -DNDEBUG -g3 -O2 -DTMV_OPT=3 -fopenmp
#CFLAGS= $(INCLUDE) -g3 -O -DTMV_NO_LIB -DTMV_OPT=3 -fopenmp -DTMV_MEM_TEST
#CFLAGS= $(INCLUDE) -ggdb -DNDEBUG -O2 -DTMV_OPT=3 -fopenmp
#CFLAGS= $(INCLUDE) -g3 -O

TMVLINK := $(shell cat ../share/tmv-link)
LIBS = $(TMVLINK) 

#LIBS= 
#LIBS= -L../../tags/v0.63/lib -ltmv_symband -ltmv -llapack -lblas -lgfortran -lpthread -fopenmp
#LIBS= -L../lib -ltmv /usr/lib/liblapack.dylib /usr/lib/libblas.dylib -lgfortran -lpthread -fopenmp
#LIBS= -L../lib -ltmv -L/usr/lib -llapack -lblas -lgfortran -lpthread -fopenmp
#LIBS= -L/usr/lib -llapack -lblas -lgfortran -lpthread
#LIBS= -L/sw64/lib -llapack -lcblas -latlas -lpthread -fopenmp
#LIBS= -L/usr/lib -L/usr/local/lib -llapack -lgoto2 -lpthread -fopenmp
#LIBS= -fopenmp
#LIBS= -L../lib -ltmv -lpthread -fopenmp
#LIBS= -L../lib -ltmv_3 -llapack -lblas -lgfortran
#LIBS= -L../lib -ltmv_mem -llapack -lblas -lgfortran -lpthread
#LIBS= -L../lib -llapack -lblas -lgfortran -lpthread

all : tmvspeed5_always_make 
#all : tmvspeed1 tmvspeed2 tmvspeed3 tmvspeed4 tmvspeed5

tmvspeed1_always_make :
	$(CC) $(CFLAGS) TMV_Speed_1.cpp -o tmvspeed1 $(LIBS)

tmvspeed1x_always_make :
	$(CC) $(CFLAGS) TMV_Speed_1x.cpp -o tmvspeed1x $(LIBS)

tmvspeed2_always_make :
	$(CC) $(CFLAGS) TMV_Speed_2.cpp -o tmvspeed2 $(LIBS)

tmvspeed2x_always_make :
	$(CC) $(CFLAGS) TMV_Speed_2x.cpp -o tmvspeed2x $(LIBS)

tmvspeed3_always_make :
	$(CC) $(CFLAGS) TMV_Speed_3.cpp -o tmvspeed3 $(LIBS)

tmvspeed4_always_make :
	$(CC) $(CFLAGS) TMV_Speed_4.cpp -o tmvspeed4 $(LIBS)

tmvspeed5_always_make :
	$(CC) $(CFLAGS) TMV_Speed_5.cpp -o tmvspeed5 $(LIBS)

tmvspeed1 : TMV_Speed_1.cpp
	$(CC) $(CFLAGS) TMV_Speed_1.cpp -o tmvspeed1 $(LIBS)

tmvspeed1x : TMV_Speed_1x.cpp
	$(CC) $(CFLAGS) TMV_Speed_1x.cpp -o tmvspeed1x $(LIBS)

tmvspeed2 : TMV_Speed_2.cpp
	$(CC) $(CFLAGS) TMV_Speed_2.cpp -o tmvspeed2 $(LIBS)

tmvspeed2x : TMV_Speed_2x.cpp
	$(CC) $(CFLAGS) TMV_Speed_2x.cpp -o tmvspeed2x $(LIBS)

tmvspeed3 : TMV_Speed_3.cpp
	$(CC) $(CFLAGS) TMV_Speed_3.cpp -o tmvspeed3 $(LIBS)

tmvspeed4 : TMV_Speed_4.cpp
	$(CC) $(CFLAGS) TMV_Speed_4.cpp -o tmvspeed4 $(LIBS)

tmvspeed5 : TMV_Speed_5.cpp
	$(CC) $(CFLAGS) TMV_Speed_5.cpp -o tmvspeed5 $(LIBS)

