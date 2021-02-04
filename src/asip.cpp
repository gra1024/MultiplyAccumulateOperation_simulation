#include "asip.h"
#include <stdlib.h>

using namespace std;

asip::asip(){
    p_C=&C; p_ov=&ov; p_z=&z;  p_m=&m;
    p_mip=&s[0];
    p_mip0=&s[0];

    a_reg = 0;
    ma1_reg = 0;
    ma2_reg = 0;
    md1_reg = 0;
    md2_reg = 0;
    A = 0;
    B = 0;
    C = 0;
    ov = 0;
    z = 0;
    m = 0;
    jump=0;
    m_out = 0;
    loop_reg = 0;
    relative_pc=0;
}

data_asip asip::culc(int display)
{
    //memory write
    if((p_mip->r_w)==0){
        if(((p_mip->Memo)+(p_mip->MMD1o)) < 1 || ((p_mip->Memo)+(p_mip->MMD2o)) < 1 ){
            d.errorM = "error:memory data bus conflict";
            d.error = 1;
            return d;
        }
        if(((p_mip->Memo)==1)&&((p_mip->MMD1o)==0)){
            if(ma1_reg < nmax){
                /* check memory address renge */
                mem[ma1_reg]=md1_reg;
            }
            else {
                d.errorM = "error:out of memory range";
                d.error = 1;
                return d;
            }
        }
        if(((p_mip->Memo)==1)&&((p_mip->MMD2o)==0)){
            if(ma2_reg < nmax){
                /* check memory address renge */
                mem[ma2_reg]=md2_reg;
            }
            else {
                d.errorM = "error:out of memory range";
                d.error = 1;
                return d;
            }
        }
    }


    /* memory read   */
    if((p_mip->r_w)==1){
        if( ((p_mip->Memo)+(p_mip->MMD1o)) < 1 || ((p_mip->Memo)+(p_mip->MMD1o)) < 1 ){
            d.errorM = "error:memory data bus conflict";
            d.error = 1;
            return d;
        }
        if(((p_mip->Memo)==0)&&((p_mip->MD1i)==0)&&((p_mip->MD1l)==1)){
            if(ma1_reg < nmax){
                /* check memory address range   */
                md1_reg=mem[ma1_reg];
            }
            else {
                d.errorM = "error:out of memory range";
                d.error = 1;
                return d;
            }
        }
        if(((p_mip->Memo)==0)&&((p_mip->MD1i)==0)&&((p_mip->MD2l)==1)){
            if(ma2_reg < nmax){
                /* check memory address range   */
                md2_reg=mem[ma2_reg];
            }
            else {
                d.errorM = "error:out of memory range";
                d.error = 1;
                return d;
            }
        }
    }
    /* memory address control  */
    if((p_mip->MA1l == 1) && (p_mip->MA1i == 0) && (p_mip->ADeo == 0)) ma1_reg=p_mip->emit_field;
    if( (p_mip->MA2l==1)&&(p_mip->ADeo==0)) ma2_reg=(p_mip->emit_field);

    /* value of emit-field is set to ma_reg */
    if( ((p_mip->MA1l)==1)&&((p_mip->MA1i)==1) )ma1_reg=C;

    /* output of ALU is set to ma_reg */
    if( (p_mip->AI1)==1) ma1_reg++;
    if( (p_mip->AI2)==1) ma2_reg++;

    /* A-bus control */
    if( ( (p_mip->Ao)+(p_mip->MA1o)+(p_mip->Byo) ) < 2 ) {
        d.errorM = "error:A-bus conflict";
        d.error = 1;
        return d;
    }
    if(p_mip->Ao == 0) A=a_reg;
    if(p_mip->MA1o == 0) A=ma1_reg;
    if(p_mip->Byo == 0) A=C;

    //B-bus control
    if( ((p_mip->MD1o)+(p_mip->Mo)+(p_mip->Beo)) < 2 ){
        d.errorM = "error:B-bus conflict";
        d.error = 1;
        return d;
    }
    if(p_mip->MD1o == 0) B=md1_reg;
    if(p_mip->Mo == 0) B=m_out;
    if(p_mip->Beo == 0) B=p_mip->emit_field;

    //latch control of registers
    if(p_mip->Al == 1) a_reg=C;
    //if((p_mip->MA1l == 1) && (p_mip->MA1i == 1)) ma1_reg=C;
    if((p_mip->MD1l == 1) && (p_mip->MD1i == 2)) md1_reg=C;

    //if((p_mip->MA1l == 1) && (p_mip->MA1i == 0) && (p_mip->ADeo == 0)) ma1_reg=p_mip->emit;
    if((p_mip->MD1l == 1) && (p_mip->MD1i == 1) && (p_mip->ADeo == 0)) md1_reg=p_mip->emit_field;
    if((p_mip->MD2l == 1) && (p_mip->MD2i == 1) && (p_mip->ADeo == 0)) md2_reg=p_mip->emit_field;
    if((p_mip->Lpl == 1) && (p_mip->Beo == 0)) loop_reg = p_mip->emit_field;

    //check branch condition
    if( ((ov==1)&&((p_mip->ovf)==1)) || ((z==1)&&((p_mip->zf)==1)) || ((m==1)&&((p_mip->mf)==1)) ) {
        jump=1;
    }
    else jump=0;
    pre_ov=ov; pre_z=z; pre_m=m;

    //ALU  and Multiplication
    aluc=(p_mip->alu);
    falu(aluc,A,B,p_C,p_ov,p_z,p_m);
    mode=(p_mip->MPY);
    m_out=multi(md1_reg,md2_reg,mode);

    //loop register control
    if(p_mip->Lpc == 1) loop_reg=loop_reg-1;

    //control one step execution
    //instruction
    if(display == 1){
        cout << relative_pc << " th instruction" << endl;

        cout << "Al   =" << setw(1) << p_mip->Al << " ";
        cout << "Ao   =" << setw(1) << p_mip->Ao << " ";
        cout << "MA1i =" << setw(1) << p_mip->MA1i << " ";
        cout << "MA1o =" << setw(1) << p_mip->MA1o << " ";
        cout << "MA2l =" << setw(1) << p_mip->MA2l << " ";
        cout << "ADeo =" << setw(1) << p_mip->ADeo  << " " << endl;

        cout << "MD1i =" << setw(1) << p_mip->MD1i << " ";
        cout << "MD1l =" << setw(1) << p_mip->MD1l << " ";
        cout << "MD1o =" << setw(1) << p_mip->MD1o << " ";
        cout << "MD2i =" << setw(1) << p_mip->MD2i << " ";
        cout << "MD2l =" << setw(1) << p_mip->MD2l << " ";
        cout << "MPY  =" << setw(1) << p_mip->MPY << " ";
        cout << "Mo   =" << setw(1) << p_mip->Mo << " ";
        cout << "Beo  =" << setw(1) << p_mip->Beo  << " " << endl;

        cout << "Byo  =" << setw(1) << p_mip->Byo << " ";
        cout << "AI1  =" << setw(1) << p_mip->AI1 << " ";
        cout << "AI2  =" << setw(1) << p_mip->AI2 << " ";
        cout << "Lpl  =" << setw(1) << p_mip->Lpl << " ";
        cout << "Lpc  =" << setw(1) << p_mip->Lpc  << " " << endl;

        cout << "r_w  =" << setw(1) << p_mip->r_w << " ";
        cout << "MMD1o=" << setw(1) << p_mip->MMD1o << " ";
        cout << "MMD2o=" << setw(1) << p_mip->MMD2o << " ";
        cout << "Memo =" << setw(1) << p_mip->Memo  << " " << endl;

        cout << "alu  =" << setw(1) << p_mip->alu  << " "<< endl;

        cout << "ncld =" << setw(1) << p_mip->ncld << " ";
        cout << "ovf  =" << setw(1) << p_mip->ovf << " ";
        cout << "zf   =" << setw(1) << p_mip->zf << " ";
        cout << "mf   =" << setw(1) << p_mip->mf << " ";
        cout << "stp  =" << setw(1) << p_mip->stp << " " << endl;

        cout << "emit =" << setw(7) << p_mip->emit_field << " " << endl;


        //registers and memory contents
        cout << "intermediate results of registers and memory contents:" << endl;

        cout << "a_reg   =" << setw(6) << a_reg << " ";
        cout << "ma1_reg =" << setw(6) << ma1_reg << " ";
        cout << "ma2_reg =" << setw(6) << ma2_reg << " " << endl;

        cout << "md1_reg =" << setw(6) << md1_reg << " ";
        cout << "md2_reg =" << setw(6) << md2_reg << " ";
        cout << "loop_reg=" << setw(6) << loop_reg << " " << endl;

        cout << "multipliction result=" << setw(6) << m_out <<"     ";
        cout << "ov=" << setw(1) << pre_ov << "  ";
        cout << "z=" << setw(1) << pre_z << "  ";
        cout << "m=" << setw(1) << pre_m << "  "<< endl;

        cout << "memory contents:" << endl;

        for (int i = 0; i < nmax; i++){
            cout << setw(6) << mem[i] << " ";
            if(i%8 == 7) cout << endl;
        }
    }

    //check the range of pipe line control register
    if((p_mip->Lpc == 1) && (loop_reg >= l_max)){
        d.errorM = "error:out of range of loop register";
        d.error = 1;
        return d;
    }


    //branch control
    emfd = p_mip->emit_field;
    if((emfd >= p_max) && ((jump == 1) || (p_mip->ncld==1))){
        d.errorM = "error:program stalled";
        d.error = 1;
        return d;
    }

    // substitute d
    for (int i = 0; i < nmax; i++){
        d.mem[i] = mem[i];
    }
    d.a_reg = a_reg;
    d.ma1_reg = ma1_reg;
    d.ma2_reg = ma2_reg;
    d.md1_reg = md1_reg;
    d.md2_reg = md2_reg;
    d.A = A;
    d.B = B;
    d.C = C;
    d.ov = ov;
    d.z = z;
    d.m = m;
    d.jump = jump;
    d.aluc = aluc;
    d.emfd = emfd;
    d.pre_ov = pre_ov;
    d.pre_z = pre_z;
    d.pre_m = pre_m;
    d.mode = mode;
    d.m_out = m_out;
    d.loop_reg = loop_reg;
    d.mip = s[relative_pc];
    d.relative_pc = relative_pc;
    d.cont_term = cont_term;

    //set relative_pc
    if(jump==1 || p_mip->ncld==1){
        p_mip = p_mip0 + emfd;
        relative_pc = emfd;
    }
    else{
        if( !((p_mip->Lpc==1) && (loop_reg!=0))){
            p_mip = p_mip + 1;
            relative_pc=relative_pc + 1;
        }
    }


    return d;
}

void asip::falu(short alu,short A,short B,short *p_C,short *p_ov,short *p_z,short *p_m)

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
/* ALU (16 bits-width operation)                                 */
/*                                                               */
/* A and B are input variables                                   */
/* C is an operation result                                      */
/* ov,z and m are status flags of operation result               */
/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
{
   short temp0;

   /*  ALU operation   */
   switch (alu) {
       case 0: /* no operation  */
           break;

       case 1: /*  A+B  */
           temp0=A+B;
           *p_ov=0; *p_m=0; *p_z=0;  /* initialize status flags */
           /* check if the result is in the appropriate range  */
           if( (A >= 0) && ( B >= 0)) {
                  if(temp0 < 0 ) {
                      *p_ov=1;
                      printf(" overflow occurred ");
                }
              }
           if( (A < 0) && ( B < 0)) {
                  if(temp0 > 0 ) {
                      *p_ov=1;
                      printf(" overflow occurred ");
                }
              }
           if(temp0 < 0) { *p_m=1; }
           if(temp0 == 0) { *p_z=1; }
           *p_C=temp0;
           break;

       case 2: /*  A-B  */
           temp0 = A - B;
           *p_ov=0; *p_m=0; *p_z=0;  /* initialize status flags */
           /* check if the result is in the appropriate range  */
           if( (A > 0) && (B < 0)) {
                  if(temp0 < 0 ) {
                      *p_ov=1;
                      printf(" overflow occurred ");
                }
              }
           if( (A < 0) && ( B > 0)) {
                  if(temp0 > 0 ) {
                      *p_ov=1;
                      printf(" overflow occured ");
                }
              }
           if(temp0 < 0) { *p_m=1; }
           if(temp0 == 0) { *p_z=1; }
           *p_C=temp0;
           break;

       case 3: /*  B-A  */
           temp0 = B - A ;
           *p_ov=0; *p_m=0; *p_z=0;  /* initialize status flags */
           /* check if the result is in the appropriate range  */
           if( (A < 0) && (B > 0)) {
                  if(temp0 < 0 ) {
                      *p_ov=1;
                      printf(" overflow occurred ");
                }
              }
           if( (A > 0) && ( B < 0)) {
                  if(temp0 > 0 ) {
                      *p_ov=1;
                      printf(" overflow occurred ");
                }
              }
           if(temp0 < 0) { *p_m=1; }
           if(temp0 == 0) { *p_z=1; }
           *p_C=temp0;
           break;

       case 4: /*  A or B  */
           temp0 = A | B ;
           *p_ov=0; *p_m=0; *p_z=0;  /* initialize status flags */

           if(temp0 < 0) { *p_m=1; }
           if(temp0 == 0) { *p_z=1; }
           *p_C=temp0;
           break;

       case 5: /*  A and B  */
           temp0 = A & B;
           *p_ov=0; *p_m=0; *p_z=0;  /* initialize status flags */

           if(temp0 < 0) { *p_m=1; }
           if(temp0 == 0) { *p_z=1; }
           *p_C=temp0;
           break;

       case 6: /*  A exclusive-or B  */
           temp0 = A ^ B;
           *p_ov=0; *p_m=0; *p_z=0;  /* initialize status flags */

           if(temp0 < 0) { *p_m=1; }
           if(temp0 == 0) { *p_z=1; }
           *p_C=temp0;
           break;


       case 7: /*  1bit right arithmetic shift A  */
           *p_ov=0; *p_m=0; *p_z=0;  /* initialize status flags */
               if( (A & 0x0001)==0x0001 ) { *p_ov=1;
                            printf(" shift out occurred ");
               }  /* ver 4.4 add */
            /* set ovf flag ; if 1bit right arithmetic shift A then carry LSB */

           temp0 = A >> 1;
           if(temp0 < 0) { *p_m=1; }
           if(temp0 == 0) { *p_z=1; }
           *p_C=temp0;
           break;

       case 8: /*  1bit left shift A  */
           *p_ov=0; *p_m=0; *p_z=0;  /* initialize status flags */
               if( (A & 0x8000)==0x8000 ) { *p_ov=1;
                            printf(" carry occurred ");
               }  /* ver 4.4 add */
             /* set ovf flag ; if 1bit left shift A then carry MSB */
               temp0 = A << 1;

           if(temp0 < 0) { *p_m=1; }
           if(temp0 == 0) { *p_z=1; }
           *p_C=temp0;
           break;
  }


}
short asip::multi(short m1,short m2,short mode)
/*------------------------------------------------------------------*/
/* multiplication for 2's complement numbers.                */
/* a decimal point is located between MSB and the second significant*/
/* bit and this routine accepts also negative numbers.            */
/* m1,m2--inputs                               */
/* mode=0---neglect 4 least significant bits                */
/*      1---use all 16 bits                         */
/*------------------------------------------------------------------*/
{
    short t_m1,t_m2,t_out_16=1;
    int t_out;

    t_m1=m1;    t_m2=m2;
    if(mode ==0){  /* neglect 4 least significant bits */
        /* convert positive numbers */
        t_m1=t_m1>>4;    t_m2=t_m2>>4;
        if((t_m1 < 0)&&(t_m1!=0xfffff800)){ t_m1= (t_m1^0xffffffff)+1;}
        if(t_m1==0xfffff800){ t_m1=0x07ff;}
        if((t_m2 < 0)&&(t_m2!=0xfffff800)){ t_m2= (t_m2^0xffffffff)+1;}
        if(t_m2==0xfffff800){ t_m2=0x07ff;}
        t_out=(int)t_m1*t_m2;

        /* check if overflow occurred */
        if( ( (m1&0xfffffff0)==0xffff8000 )&&( (m2&0xfffffff0)==0xffff8000 ) ){
            printf("overflow occured:result is approximated\n");
            t_out=0x3fffff;
            }
        /* convert 16 bits width */
        t_out_16=t_out>>7;
        if( ( (m1>0)&&(m2<0) ) || ( (m1<0)&&(m2>0) ) ){
            t_out_16=-t_out_16;
            }   /* convert a negative number */
        }

    if(mode ==1){  /* use all 16 bits */
        /* convert positive numbers */
        if((t_m1 < 0)&&(t_m1!=0xffff8000)){ t_m1=( t_m1^0xffffffff)+1;}
        if(t_m1==0xffff8000){ t_m1=0x7fff;}
        if((t_m2 < 0)&&(t_m2!=0xffff8000)){ t_m2=( t_m2^0xffffffff)+1;}
        if(t_m2==0xffff8000){ t_m2=0x7fff;}
        t_out=(int)t_m1*t_m2;

        /* check if overflow occurred */
        if( (m1==0xffff8000)&&(m2==0xffff8000) ){
            printf("overflow occured:result is approximated\n");
            t_out=0x3fffffff;
            }
        /* convert 16 bits width */
        t_out_16=t_out>>15;
        if( ( (m1>0)&&(m2<0) ) || ( (m1<0)&&(m2>0) ) ){
            t_out_16=-t_out_16;
            }   /* convert a negative number */
        }
    if(mode == 2){
        t_out_16 = m1 * m2;
    }
    return(t_out_16);

}
