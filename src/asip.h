#ifndef asip_h
#define asip_h

#include "config.h"

class asip{
public:
    explicit asip();
    data_asip culc(int display);
private:
    void falu(short alu,short A,short B,short *p_C,short *p_ov,short *p_z,short *p_m);
    short multi(short m1,short m2,short mode);

    short mem[nmax] = {
        1,2,3,4,5,6,7,8,
        10,20,30,40,50,60,70,80
    };
    short a_reg,ma1_reg,ma2_reg,md1_reg,md2_reg;
    short A,B,C;
    short ov, z, m, jump,aluc,emfd;
    short pre_ov,pre_z,pre_m;
    short mode,m_out;
    short loop_reg;

    struct mip *p_mip, *p_mip0, s[p_max] = {
        {
            0,1,0,1,1,0,0,
            0,0,1,0,0,2,1,0,
            1,0,0,1,0,
            1,1,1,0,
            0,
            0,0,0,0,0,
            8
        },//0 emit -> MA2l
        {
            0,1,0,0,1,0,1,
            0,1,1,0,1,2,0,1,
            0,1,1,0,1,
            1,1,1,0,
            1,
            0,0,0,0,0,
            0
        },//1 loop
        {
            0,1,0,0,1,0,1,
            0,0,1,0,0,2,0,1,
            0,0,0,0,0,
            1,1,1,1,
            1,
            0,0,0,0,0,
            0
        },//2 last add use alu
        {
            0,1,0,0,1,0,1,
            2,1,1,0,0,2,1,1,
            1,0,0,0,0,
            1,1,1,1,
            0,
            0,0,0,0,0,
            0
        },//3 alu -> MD1l
        {
            0,1,0,0,1,0,1,
            0,0,1,0,0,2,1,1,
            1,0,0,0,0,
            0,0,1,1,
            0,
            0,0,0,0,0,
            0
        },//4 MD1l -> mem(16)
        {
            0,1,0,0,1,0,1,
            0,0,1,0,0,2,1,1,
            1,0,0,0,0,
            0,0,1,1,
            0,
            0,0,0,0,1,
            0
        }//4 stp
    };

    short *p_C, *p_ov, *p_z, *p_m;
    short relative_pc, cont_term;

    data_asip d;

};

#endif /* asip_h */
