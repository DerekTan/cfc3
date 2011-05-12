/*
   人员上位机与RFID专用检测仪通讯协议

   下行命令
   1．	索取当前信息
   信息头(FF FF)+检测站号(01-0F)+命令码（01）+校验字节
   2．索取上一帧信息
   信息头(FF FF)+检测站号(01-0F)+命令码（07）+校验字节

   上行命令
   1．	无信息
   信息头(EE EE)+检测站号(01-0F)+命令码（00）+校验字节
   2．	有信息
   信息头(EE EE)+检测站号(01-0F)+命令码（04）+长度+ID1+ID2+...校验字节
   长度<=32，16个用户，ID号低位在前。

*/
//判aror和aa
//电池位=1表示没有电402=d13,905=d13
//接收4位

//080409改在周源山
#include <w77e58.h>

#define uint unsigned int
#define uchar unsigned char

#undef CHIP_905
#define LED_ID_STR_LEN     5
#define LED_MSG_NUM     6
#define SHOW_TIME       10          //unit: s

#define EXTERN_LED_STR      P1_0
#define EXTERN_LED_CLK      P1_1
#define EXTERN_LED_DATA     P1_2

#define EXTERN_LED_TICK()                   \
    do {                                    \
        EXTERN_LED_CLK = 1;                 \
        EXTERN_LED_CLK = ~EXTERN_LED_CLK;   \
    }                                   \
while (0)

#define ledOpen()              \
    do {                        \
        EXTERN_LED_STR = 1;     \
    }   while (0)

#define ledClose()              \
    do {                        \
        EXTERN_LED_STR = 0;     \
    }   while (0)

#define TIMED_ACESS()   do {TA = 0xaa; TA = 0x55; } while (0)
#define ENABLE_WD_RST() do { TIMED_ACESS(); EWT = 1; } while (0)
#define WD_RST()        do { TIMED_ACESS(); RWT = 1; } while (0)


typedef struct {
    uchar idStr[LED_ID_STR_LEN];
    uchar timer;
} LED_MSG;

LED_MSG ledMsgArray[LED_MSG_NUM];
/******************************/
//sbit LED_DATA_PIN=P1^0;
//sbit LED_CLK_PIN=P1^1;
//sbit LED_STR_PIN=P1^3;
sbit LED_CLK_PIN = P0^3;
sbit LED_DATA_PIN = P0^4;
sbit LED_STR_PIN = P0^5;

sbit FMQ=P3^2;
sbit LED=P3^6;
sbit LED402=P3^7;
sbit DE=P3^4;//=1tx,=0rx
sbit RE=P3^5;//=0
//sbit aaaaa=P0^0;
sbit DR=P3^3;
#ifdef CHIP_905
sbit TRX_CE=P0^2;           //?????????
#endif
sbit MISO=P0^4;
sbit MOSI=P0^5;
sbit SCK=P0^6;
sbit CSN=P0^7;

sbit www=P2^0;//0708


/******************************/
#if 0
idata uint idgh_id[32];
#endif
uchar idgh_zz=0,addr;
uchar rec_in_time,start95time;
uchar trx485=0;
xdata uchar yrx=0;//=0有接收
/**************************************************/
#define RF_C0 0x0       //422.4+RF_C_/10,0x73=433.9M,0x77=434.3
#define RF_C1 0x00      //(00)5=1重发，4=1接收小电流，32发射功率
                        //00=-10;01=-2;10=+6;11=+10
#define RF_C2 0x11      //(44)地址宽度
#define RF_C3 0x04      //(20)接收长度///////////////////
#define RF_C4 0x04      //(20)发送长度///////////////////
#define RF_C5 0x79      //(e7)接收地址1
#define RF_C6 0x79      //(e7)接收地址2
#define RF_C7 0x79      //(e7)接收地址3
#define RF_C8 0x79      //(e7)接收地址4
#define RF_C9 0xc0      //(e7)7=1十六位，6=1有校验，
                        //543=000-4M，=011-16M，
                        //2=1有时钟出，10=00-4m,01-2m,10-1m,11-500k
/**************************************************/
#define WC 0x00 //写configuration
#define RC 0x10 //读configuration
#define WTP 0x20 //写TX_PAYLOAD
#define RTP 0x21 //读TX_PAYLOAD
#define WTA 0x22 //写地址
#define RTA 0x23 //读地址
#define RRP 0x24 //读接收数据
/**************************************************/
xdata uchar data0[10];

#ifdef CHIP_905
void out905(uchar ch){//将aa输出到905
    char i;
    for(i=7;i>=0;i--){
        MOSI=((ch>>i)&0x01);//先输出7
        SCK=1;SCK=1;SCK=1;SCK=1;SCK=1;SCK=1;SCK=1;SCK=1;//2条不可靠
        SCK=0;SCK=0;SCK=0;SCK=0;SCK=0;SCK=0;SCK=0;SCK=0;
    }
}
/**************************************************/
uchar in905(){//从905输入
    uchar i,aa=0;
    MISO=1;
    for(i=0;i<=7;i++){
        aa=aa<<1|MISO;
        SCK=1;SCK=1;SCK=1;SCK=1;SCK=1;SCK=1;SCK=1;SCK=1;
        SCK=0;SCK=0;SCK=0;SCK=0;SCK=0;SCK=0;SCK=0;SCK=0;
    }
    return(aa);
}
/**************************************************/
void write_con(){//将RF_C0-9写到905
    uchar temp;
    TRX_CE=0;TRX_CE=0;TRX_CE=0;TRX_CE=0;TRX_CE=0;TRX_CE=0;TRX_CE=0;TRX_CE=0;TRX_CE=0;TRX_CE=0;TRX_CE=0;TRX_CE=0;TRX_CE=0;TRX_CE=0;TRX_CE=0;TRX_CE=0;TRX_CE=0;TRX_CE=0;TRX_CE=0;TRX_CE=0;TRX_CE=0;TRX_CE=0;TRX_CE=0;TRX_CE=0;TRX_CE=0;TRX_CE=0;TRX_CE=0;TRX_CE=0;TRX_CE=0;TRX_CE=0;TRX_CE=0;TRX_CE=0;
    CSN=0;CSN=0;CSN=0;CSN=0;CSN=0;CSN=0;CSN=0;CSN=0;CSN=0;CSN=0;CSN=0;CSN=0;CSN=0;CSN=0;CSN=0;CSN=0;CSN=0;CSN=0;CSN=0;CSN=0;CSN=0;CSN=0;CSN=0;CSN=0;
    out905(WC);
    out905(RF_C0);
    out905(RF_C1);
    out905(RF_C2);
    out905(RF_C3);
    out905(RF_C4);
    out905(RF_C5);
    out905(RF_C6);
    out905(RF_C7);
    out905(RF_C8);
    temp=RF_C9;
    temp|=0x18;//011-16m,
    //temp|=0x20;//100=20M
    temp|=0x07;//有时钟输出
    out905(temp);
    CSN=1;
    TRX_CE=1;
}

/**************************************************/
void read_con(){//读
    CSN=0;CSN=0;CSN=0;CSN=0;CSN=0;CSN=0;
    out905(RC);
    data0[0]=in905();
    data0[1]=in905();
    data0[2]=in905();
    data0[3]=in905();
    data0[4]=in905();
    data0[5]=in905();
    data0[6]=in905();
    data0[7]=in905();
    data0[8]=in905();
    data0[9]=in905();
    CSN=1;
}
/**************************************************/
void read_rx_payload(){//读
    CSN=0;CSN=0;CSN=0;CSN=0;CSN=0;CSN=0;
    out905(RRP);
    data0[0]=in905();
    data0[1]=in905();
    data0[2]=in905();
    data0[3]=in905();
    CSN=1;
}
#endif

/******************************/
uchar code ledTable[22]={ 0x3f,0x06,0x5b,0x4f,//0,1,2,3
    0x66,0x6d,0x7d,0x07,//4,5,6,7
    0x7f,0x6f,   //8,9
    0x77, 0x7c,  //A, b
    0x58, 0x5e,  //c, d
    //0x39, 0x5e,  //C, d
    0x79, 0x71,  //E, F
    0x3d,0x76,   //G,H
    0x01,0x40,0x08,0x00};//上,中,下,黑
/******************************/
void ledShowChar(uchar aa){
    uchar my_flag1;
    char i;
    for(i=7;i>=0;i--){
        my_flag1=aa;//ledTable[aa];
        LED_DATA_PIN=((my_flag1>>i)&0x01);//先输出7
        LED_CLK_PIN=0;LED_CLK_PIN=0;LED_CLK_PIN=0;
        LED_CLK_PIN=1;LED_CLK_PIN=1;LED_CLK_PIN=1;
    }
}
/******************************/

uchar dis_time;             //显示时间
xdata uchar dispBuf[18];    //显示数组0最低位

void ledShowAll(){//显示数组中的数
    LED_STR_PIN=0;
    ledShowChar(dispBuf[17]);ledShowChar(dispBuf[16]);
    ledShowChar(dispBuf[15]);ledShowChar(dispBuf[14]);
    ledShowChar(dispBuf[13]);ledShowChar(dispBuf[12]);
    ledShowChar(dispBuf[11]);ledShowChar(dispBuf[10]);
    ledShowChar(dispBuf[9]);ledShowChar(dispBuf[8]);
    ledShowChar(dispBuf[7]);ledShowChar(dispBuf[6]);
    ledShowChar(dispBuf[5]);ledShowChar(dispBuf[4]);
    ledShowChar(dispBuf[3]);ledShowChar(dispBuf[2]);
    ledShowChar(dispBuf[1]);ledShowChar(dispBuf[0]);
    LED_STR_PIN=1;
}

void refreshLedMsgTimer(void) {
    uchar i;
    for (i=0; i<LED_MSG_NUM; i++) {
        if (ledMsgArray[i].timer>0)
            ledMsgArray[i].timer--;
    }
}
/******************************/
uchar abc,abc1;//接收时间
/******************************/
void time0() interrupt 1 using 2{
    TH0=0xf1;TL0=0x9f;//4ms---9a
    //TH0=0x4c;TL0=0x00;//50ms
    rec_in_time++;//接收延时
    abc++;
    if(abc>12){//50ms
        abc=0;abc1++;
        dis_time++;//1000ms显示
        start95time+=1;
    }
}
/******************************/

/******************************/
uchar singleIdStr[5];//int-bcd
void id2str(uint aa){//输入temp0输出4最高位
    uchar i;
    uint temp0,temp1;
    temp0=aa;
    for(i=0;i!=5;i++){      //singleIdStr[0]: LSB
        temp1=temp0;
        temp0=temp0/10;
        singleIdStr[i]=temp1-temp0*10;
    }
}

/******************************/

/*******************crc8(x^8+x^5+x^4+1)**********/
uchar crc;
uchar sint1_data[2];//接收id暂存

void crc8(){
    uchar index;
    uchar b;
    crc=0xff;
    for(index=0;index<2;index++){
        crc ^=sint1_data[index];
        for(b=0;b<8;b++){
            if(crc & 0x80) crc=(crc<<1)^0x31;
            else crc=(crc<<1);
        }
    }
}
/******************************/
//接收到ID时rec_id_suc=1;接收id暂存receivedIdArray[2];0高1低
uchar uartInt1Status=0,axor,aa_reg;
bit rec_id_suc=0;//接收id成功标志=1
uint uartCurrRxId;
void sint1() interrupt 7 using 1{
    RI_1=0;
    switch(uartInt1Status){
        case 0:if(SBUF1==0xaa) uartInt1Status=1;
                   break;
        case 1:if(SBUF1==0xaa) {uartInt1Status=4;aa_reg=0;}
                   else uartInt1Status=0;
                   break;
        case 2:if(SBUF1==0xaa) uartInt1Status=4;
                   else uartInt1Status=0;
                   break;
        case 3:if(SBUF1==0xaa) uartInt1Status=4;
                   else uartInt1Status=0;
                   break;
        case 4: if(SBUF1==0xaa) {aa_reg+=1;break;}
                    else {
                        sint1_data[0]=SBUF1&0x1f;
                        axor=sint1_data[0];
                        uartCurrRxId=SBUF1;//axor;
                        uartCurrRxId<<=8;
                        uartInt1Status=5;
                        break;
                    }
        case 5: sint1_data[1]=SBUF1;
                axor^=SBUF1;
                uartCurrRxId+=SBUF1;
                uartInt1Status=6;
                break;
        case 6: axor^=SBUF1;
                crc8();
                if(crc==SBUF1) {
                    uartInt1Status=7;
                }
                else uartInt1Status=0;
                break;
        case 7: if(axor==SBUF1){
                    rec_id_suc=1;//接收放uartCurrRxId
                    LED402=!LED402;
                }
                uartInt1Status=0;
                break;
        default:uartInt1Status=0;
                break;
    }
}

#ifdef CHIP_905
/******************************/
bit rec_id95_suc=0;
uint rec95id;
void rx95(){//
    uchar crc,index,b;
    if(DR==0){
        EA=0;//070724
        read_rx_payload();
        rec95id=data0[0];
        rec95id<<=8;
        rec95id+=data0[1];
        rec95id&=0x3fff;//0x1fff=080114
        data0[0]&=0x1f;
        crc=0x97;
        for(index=0;index<2;index++){;//0高位，1低位
            crc ^=data0[index];
            for(b=0;b<8;b++){
                if(crc & 0x80) crc=(crc<<1)^0x31;
                else crc=(crc<<1);
            }
        }
        b=data0[0]^data0[1]^data0[2];
        if((crc==data0[2])&&(b==data0[3])){//校验成功
            LED=!LED;start95time=0;
            rec_id95_suc=1;
        }
        EA=1;//070724
        while(DR==0);
    }
}
#endif

/*****************************************************************************/
xdata uint receivedIdArray[64];//接收id组
uchar receivedIdPos=0;//接收指针

/*****************************************************************************
 * 名称：find_id(uint16 *ptr_id,uint16 size,uint16 id)
 * 功能：找和id相同的号
 * 入口参数：size=长度，id=用户号
 * 出口参数：返回size无相同，否则为相同id号位置
 ****************************************************************************/
uchar find_id(uint *ptr_id,uint size,uint id){
    uchar i,ret=0;
    for(i=0;i<size;i++){
        ret=i;
        if(*(ptr_id+i)==id) break;
        ret=size;
    }
    return ret;
}
/*****************************************************************************/
xdata uint rectx_id[16];//接收id组
uchar rectx_id_zz=0;//接收指针
/*****************************************************************************
 * 名称：rec_id402_cl()
 * 功能：找和id相同的号
 * 入口参数：无
 * 出口参数：无
 ****************************************************************************/
void rec_id402_cl(){
    if(rec_id_suc){//有接收排队
        rec_id_suc=0;
        if(find_id(receivedIdArray,receivedIdPos,uartCurrRxId)==receivedIdPos){//无相同
            receivedIdArray[receivedIdPos]=uartCurrRxId;        //添加到队列后面
            receivedIdPos++;
            receivedIdPos&=0x3f;//最多64个
        }
        if(find_id(rectx_id,rectx_id_zz,uartCurrRxId)==rectx_id_zz){//无相同
            rectx_id[rectx_id_zz]=uartCurrRxId;
            if(rectx_id_zz<15) rectx_id_zz++;
        }
    }
}

#ifdef CHIP_905
/******************************/
void rec_id95_cl(){
    if(rec_id95_suc){//有接收排队
        rec_id95_suc=0;
        if(find_id(receivedIdArray,receivedIdPos,rec95id)==receivedIdPos){//无相同
            receivedIdArray[receivedIdPos]=rec95id;
            receivedIdPos++;
            receivedIdPos&=0x3f;
        }
        if(find_id(rectx_id,rectx_id_zz,rec95id)==rectx_id_zz){//无相同
            rectx_id[rectx_id_zz]=rec95id;
            if(rectx_id_zz<15) rectx_id_zz++;
            //rectx_id_zz&=0x0f;
        }
    }
}
#endif

/******************************/
#define DISTIME 20
uchar idDispStatus=0;
uint cgh=0;//要查工号的id号
void idPopAndDisp(){//将receivedIdArray内容显示
    uchar i;
    uint aaaa;
    switch(idDispStatus){     //default: 0
        case 0:	if(receivedIdPos!=0){//有接收
                    aaaa=receivedIdArray[0]&0x1fff;
                    if((receivedIdArray[0]&0x8000)) aaaa+=0x2000;//20110126

                    id2str(aaaa);
                    if (singleIdStr[4] <= 1)
                        singleIdStr[3] += 10*singleIdStr[4];
                    dispBuf[17]=ledTable[singleIdStr[3]];   //MSB
                    dispBuf[16]=ledTable[singleIdStr[2]];
                    dispBuf[15]=ledTable[singleIdStr[1]];
                    dispBuf[14]=ledTable[singleIdStr[0]];
                    if(trx485==0) dispBuf[14]|=0x80;//亮小数点
                    if(dispBuf[17]==0x3f){//前面0不显示
                        dispBuf[17]=0;
                        if(dispBuf[16]==0x3f){
                            dispBuf[16]=0;
                            if(dispBuf[15]==0x3f){
                                dispBuf[15]=0;
                            }
                        }
                    }
                    if((receivedIdArray[0]&0x4000)==0x4000) dispBuf[16]|=0x80;
                    if((receivedIdArray[0]&0x2000)==0x2000) dispBuf[15]|=0x80;
                    dispBuf[8]=dispBuf[17];
                    dispBuf[7]=dispBuf[16];
                    dispBuf[6]=dispBuf[15];
                    dispBuf[5]=dispBuf[14];
                    dispBuf[13]=0x00;
                    dispBuf[4]=0x00;
                    dispBuf[12]=0x00;
                    dispBuf[3]=0x00;
                    dispBuf[11]=0x00;
                    dispBuf[2]=0x00;
                    dispBuf[10]=0x00;
                    dispBuf[1]=0x00;
                    dispBuf[9]=0x00;
                    dispBuf[0]=0x00;

                    if((yrx&0x01)==1){dispBuf[0]=0x80;}
                    ledShowAll();
                    dis_time=0;
                    if(receivedIdPos>50) dis_time=18;
                    else if(receivedIdPos>40) dis_time=16;
                    else if(receivedIdPos>30) dis_time=14;
                    else if(receivedIdPos>20) dis_time=12;
                    else if(receivedIdPos>10) dis_time=10;
                    else if(receivedIdPos>5) dis_time=5;
                    idDispStatus=1;
                    /* 删掉第一个已经显示的id */
                    for(i=0;i<receivedIdPos;i++){//前移一格
                        receivedIdArray[i]=receivedIdArray[i+1];
                    }
                    receivedIdPos=--receivedIdPos&0x3f;
                }else{
                    dispBuf[0]=0;dispBuf[1]=0;
                    dispBuf[2]=0;dispBuf[3]=0;
                    dispBuf[4]=0;dispBuf[5]=0;
                    dispBuf[6]=0;dispBuf[7]=0;
                    dispBuf[8]=0;dispBuf[9]=0;
                    dispBuf[10]=0;dispBuf[11]=0;
                    dispBuf[12]=0;dispBuf[13]=0;
                    dispBuf[14]=0;dispBuf[15]=0;
                    dispBuf[16]=0;dispBuf[17]=0;
                    if((yrx&0x01)==1){dispBuf[0]=0x80;}
                    ledShowAll();
                }
                break;
        case 1: if(dis_time>20){//显示0.5-1s
                    idDispStatus=0;
                    dispBuf[0]=0;dispBuf[1]=0;
                    dispBuf[2]=0;dispBuf[3]=0;
                    dispBuf[4]=0;dispBuf[5]=0;
                    dispBuf[6]=0;dispBuf[7]=0;
                    dispBuf[8]=0;dispBuf[9]=0;
                    dispBuf[10]=0;dispBuf[11]=0;
                    dispBuf[12]=0;dispBuf[13]=0;
                    dispBuf[14]=0;dispBuf[15]=0;
                    dispBuf[16]=0;dispBuf[17]=0;
                    if((yrx&0x01)==1){dispBuf[0]+=0x80;dispBuf[9]+=0x80;}
                    ledShowAll();
                }
                break;

        default:
                idDispStatus=0;
                break;
    }
}
/******************************/
/******************************/
/******************************/
uchar pUartTx=0;//发指针
xdata uchar uartTxBuf[64];//15为长度
uchar pUartRx=0;//收指针
xdata uchar uartRxBuf[64];

void sint0() interrupt 4 using 1{//接收发送中断
    if(TI){
        TI=0;
        if(uartTxBuf[63]!=0){
            ACC=uartTxBuf[pUartTx];TB8=P;
            SBUF=uartTxBuf[pUartTx];
            pUartTx=++pUartTx&0x3f;
            uartTxBuf[63]-=1;
        }
        else{
            REN=1;DE=0;//开接收
        }
    }
    if(RI){
        RI=0;
        uartRxBuf[pUartRx]=SBUF;//ACC=SBUF;
        //if(TB8==P)
        pUartRx=++pUartRx&0x3f;
    }
}
/******************************/
uchar posUartRxHandler=0;
uchar uartHandlerStatus=0;
uchar ccgh0,ccgh1;//暂存接收工号

/*****************************************************************************
 * 名称：uartHandler()
 * 功能：按字节处理UART收到的数据，已处理过的不再重复处理
 * 入口参数：无
 * 出口参数：无
 ****************************************************************************/

void uartHandler(){//接收数据处理并发送
    uchar i,j;
    switch(uartHandlerStatus){       //default: 0
        case 0:	if(posUartRxHandler!=pUartRx){//判第一个ff
                    if(uartRxBuf[posUartRxHandler]==0xff) 
                        uartHandlerStatus+=1;
                    else 
                        uartHandlerStatus=0;
                    posUartRxHandler=++posUartRxHandler&0x3f;
                }
                break;
        case 1:	if(posUartRxHandler!=pUartRx){//判第二个ff
                    if(uartRxBuf[posUartRxHandler]==0xff) 
                        uartHandlerStatus+=1;
                    else
                        uartHandlerStatus=0;
                    posUartRxHandler=++posUartRxHandler&0x3f;
                }
                break;
        case 2:	if(posUartRxHandler!=pUartRx){//判地址
                    if(uartRxBuf[posUartRxHandler]!=0xff)
                    {
                        if(uartRxBuf[posUartRxHandler]==addr)
                        {
                            uartHandlerStatus+=1;
                            abc1=0;
                            yrx+=1;
                        }//051107
                        else 
                            uartHandlerStatus=0;
                    }
                    posUartRxHandler=++posUartRxHandler&0x3f;
                }
                break;
        case 3:	if(posUartRxHandler!=pUartRx){//判命令
                    if(uartRxBuf[posUartRxHandler]==0x01)
                        uartHandlerStatus+=1;//当前
                    else if(uartRxBuf[posUartRxHandler]==0x07)
                        uartHandlerStatus=7;//上一帧
                    else 
                        uartHandlerStatus=0;
                    posUartRxHandler=++posUartRxHandler&0x3f;
                }
                break;
        case 4:	if(posUartRxHandler!=pUartRx){//判命令1校验
                   if(uartRxBuf[posUartRxHandler]==(addr^0x01)){//comm1ok
                        rec_in_time=0;
                        uartHandlerStatus=5;
                        trx485=!trx485;
                        DE=1;
                    }
                    else 
                        uartHandlerStatus=0;
                    posUartRxHandler=++posUartRxHandler&0x3f;
                }
                break;
        case 5:	if(rec_in_time>=5){//接收后延时发送
                    if(rectx_id_zz!=0){//有号
                        //uartTxBuf[0]=0xee;//发有信息
                        uartTxBuf[1]=0xee;
                        uartTxBuf[2]=addr;
                        uartTxBuf[3]=0x04;
                        j=rectx_id_zz<<1;
                        uartTxBuf[4]=j;
                        uartTxBuf[5+j]=uartTxBuf[2]^uartTxBuf[3]^uartTxBuf[4];
                        for(i=0;i<rectx_id_zz;i++){
                            uartTxBuf[5+(2*i)]=rectx_id[i];
                            uartTxBuf[5+j]^=uartTxBuf[5+(2*i)];
                            uartTxBuf[6+(2*i)]=rectx_id[i]>>8;
                            uartTxBuf[5+j]^=uartTxBuf[6+(2*i)];
                        }
                        uartTxBuf[62]=5+j;
                        uartTxBuf[63]=5+j;
                        uartHandlerStatus=0;
                        rectx_id_zz=0;
                    }else{//无号
                        //uartTxBuf[0]=0xee;//发无信息
                        uartTxBuf[1]=0xee;
                        uartTxBuf[2]=addr;
                        uartTxBuf[3]=0x00;
                        uartTxBuf[4]=addr;
                        uartTxBuf[62]=0x04;
                        uartTxBuf[63]=0x04;
                        uartHandlerStatus=0;
                    }
                    REN=0;DE=1;SBUF=0xee;pUartTx=1;
                }
                break;
        case 6:	if(((pUartRx-posUartRxHandler)&0x3f)>=6){//接收时间
                    rec_in_time=0;
                    uartHandlerStatus=18;
                    trx485=!trx485;
                }
                break;
        case 7:	if(posUartRxHandler!=pUartRx){//判命令7校验
                    if(uartRxBuf[posUartRxHandler]==(addr^0x07)){//comm7ok
                        rec_in_time=0;
                        uartHandlerStatus=8;
                    }
                    else uartHandlerStatus=0;
                    posUartRxHandler=++posUartRxHandler&0x3f;
                }
                break;
        case 8:	if(rec_in_time>=10){//换发送组发送上一帧
                    pUartTx=1;
                    uartTxBuf[63]=uartTxBuf[62];
                    REN=0;DE=1;SBUF=0xee;uartHandlerStatus=0;
                }
                break;
        default:
                uartHandlerStatus=0;
                break;
    }
}
/******************************/
//T2发生器：Fosc/(32*(65536-(RCAP2H,RCAP2L)))
//T1发生器：TH1=TL1= 256-(F*1000000*(SMOD+1))/(32*12*B)

/******************************/
main(){
    ENABLE_WD_RST();
    PMR |= 0x01;            //enable the on-chip 1KB MOVX SRAM

#ifdef CHIP_905
    TRX_CE = 0;
#endif

    SCK=0;
    CSN=1;

    CKCON |= 0x40;          //set WD time-out 2^17 clocks
    TMOD=0x21;              //set timer1: 8bits auto-reload from THx
                            //set timer0: 18bits, no prescale
    TH0=0x4c;TL0=0x00;TR0=1;ET0=1;PT0=1;//50ms
    TH1=0xfd;TL1=0xfd;TR1=1;//9.6k

    SCON1=0x50;             //Uart1 mode: 1
                            //set Serial Recption enable
    ES1=1;                  //Enable Serial Port 1 interrupt

    RCAP2H=0xff;            //MSB of timer2 auto-reload
    RCAP2L=0xb8;	        //LSB of timer2 auto-reload
                            //ffdc 9.6k;ffb8 4.8k;fee0h-1.2k
    T2CON=0x34;             //Serial port 0, Receive clock flag: timer 2 overflow used
                            //Serial port 0, Transmit clock flag: timer 2 overflow used

    SCON=0xd0;              //Serial port control : mode 3
                            //enable multiprocessor communication
    ES=1;                   //Enable Serial Port 0 interrupt
    EA=1;                   //global interrupt enable

    FMQ=0;DE=0;RE=0;        //P3^2 P3^4 P3^5

#if 0
    for(addr=0;addr<32;addr++){//清对应表
        idgh_id[addr]=0xffff;
    }
#endif
    addr=P1;addr=addr>>4;

#ifdef CHIP_905
    write_con();
#endif
    dispBuf[0]=0x40;
    dispBuf[1]=0x40;
    dispBuf[2]=0x40;
    dispBuf[3]=0x40;
    dispBuf[4]=0x40;
    dispBuf[5]=0x40;
    dispBuf[6]=0x40;
    dispBuf[7]=0x40;
    dispBuf[8]=0x40;
    dispBuf[9]=0x40;
    dispBuf[10]=0x40;
    dispBuf[11]=0x40;
    dispBuf[12]=0x40;
    dispBuf[13]=0x40;
    dispBuf[14]=0x40;
    dispBuf[15]=0x40;
    dispBuf[16]=0x40;
    dispBuf[17]=0x40;
    ledShowAll();



    while(1){
#ifdef CHIP_905
        TRX_CE=1;
#endif
        WD_RST(); 
        addr = P1>>4;
#ifdef CHIP_905
        rx95();
#endif
        WD_RST(); 
        rec_id402_cl();
#ifdef CHIP_905
        rec_id95_cl();
#endif
        WD_RST(); 
        idPopAndDisp();
        WD_RST(); 
        www=!www;//0708
        uartHandler();
#ifdef CHIP_905
        if(start95time>250){//10秒收不到
            start95time=0;LED402=!LED402;//070724
            write_con();
        }
#endif
    }
}
