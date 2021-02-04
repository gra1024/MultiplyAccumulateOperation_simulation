#ifndef config_h
#define config_h
#include <iostream>
#include <stdio.h>
#include <string>
#include <iomanip>

#define nmax 64
#define p_max 256
#define l_max 64

using namespace std;

struct mip{
    short Al = 0,Ao = 1,MA1i = 0,MA1l = 0,MA1o= 1,MA2l = 0,ADeo = 1;
    short MD1i = 0,MD1l = 0,MD1o = 1,MD2i = 0,MD2l = 0,MPY = 0,Mo = 1,Beo = 1;
    short Byo = 1,AI1 = 0,AI2 = 0,Lpl = 0,Lpc = 0;
    short r_w = 0,MMD1o = 1,MMD2o = 1,Memo = 1;                             /* memory control */
    short alu = 0;
    short ncld = 0,ovf = 0,zf = 0,mf = 0,stp = 0;                       /* branch control */
    short emit_field = 0;                                          /* emit field */
};

struct data_asip{
    short mem[nmax] = {
        1,2,3,4,5,6,7,8,
        10,20,30,40,50,60,70,80
    };
    short a_reg = 0,ma1_reg = 0,ma2_reg = 0,md1_reg = 0,md2_reg = 0;
    short A = 0,B = 0,C = 0;
    short ov = 0, z = 0, m = 0, jump=0, aluc = 0, emfd = 0;
    short pre_ov = 0,pre_z = 0,pre_m = 0;
    short mode,m_out = 0;
    short loop_reg = 0;
    mip mip;
    short one_step = 0,relative_pc = 0,cont_term = 0;
    string errorM;
    int error = 0;
};



#endif /* config_h */
