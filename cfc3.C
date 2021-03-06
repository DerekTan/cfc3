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

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#undef CHIP_905
#define LED_MSG_LEN     5
#define DISP_ID_NUM     6
#define SHOW_TIME       60          //unit: 0.1s
#define ALIVE_TIME      300          //uint: 0.1s
#define RECEIVED_ARRAY_LEN  64
#define ALIVE_ARRAY_LEN     100

#define LED_TICK()                   \
    do {                                    \
        LED_CLK_PIN = 1;                 \
        LED_CLK_PIN = ~LED_CLK_PIN;   \
    }                                   \
while (0)

#define LED_OPEN()              \
    do {                        \
        LED_STR_PIN = 0;     \
    }   while (0)

#define LED_CLOSE()              \
    do {                        \
        LED_STR_PIN = 1;     \
    }   while (0)

#define TIMED_ACESS()   do {TA = 0xaa; TA = 0x55; } while (0)
#define ENABLE_WD_RST() do { TIMED_ACESS(); EWT = 1; } while (0)
#define WD_RST()        do { TIMED_ACESS(); RWT = 1; } while (0)


typedef struct {
    uint id;
    uchar idStr[LED_MSG_LEN];
    uchar isAlive;        // from alive array or not?
    uchar dtime;
} tDispArrayItem;

typedef struct {
    uint id;
    //uchar idStr[LED_MSG_LEN];
    uchar inDisplay;
    uint atime;
} tAliveArrayItem;

xdata tDispArrayItem DispArray[DISP_ID_NUM];
xdata tAliveArrayItem AliveArray[ALIVE_ARRAY_LEN];

uchar aliveIdNum=0;
uchar undispAliveIdNum=0;
uchar pAliveNext=0;

xdata uint ReceivedArray[RECEIVED_ARRAY_LEN];//接收id组
uchar receivedIdNum=0;//接收指针

/******************************/
//sbit LED_DATA_PIN=P1^0;
//sbit LED_CLK_PIN=P1^1;
//sbit LED_STR_PIN=P1^3;
sbit LED_CLK_PIN = P0^3;
sbit LED_DATA_PIN = P0^4;
sbit LED_STR_PIN = P0^5;

sbit DIP1 = P1^4;
sbit DIP2 = P1^5;
sbit DIP3 = P1^6;
sbit DIP4 = P1^7;

sbit TEST_PIN = P0^0;
sbit SPEAKER_PIN = P2^1;

#define SPEAKER_ON     1
#define SPEAKER_OFF    0

#if 0
sbit FMQ=P3^2;
sbit LED=P3^6;
sbit LED402=P3^7;
sbit RE=P3^5;//=0
//sbit aaaaa=P0^0;
sbit DR=P3^3;
#endif
sbit DE=P3^4;//=1tx,=0rx

#ifdef CHIP_905
sbit TRX_CE=P0^2;           //?????????
#endif
#if 0
sbit MISO=P0^4;
sbit MOSI=P0^5;
sbit SCK=P0^6;
sbit CSN=P0^7;
#endif

sbit www=P2^0;//0708

uchar glbLedRefreshFlag = TRUE;
uchar glbAlarmNum = 0;
/******************************/
#if 0
idata uint idgh_id[32];
#endif
uchar idgh_zz=0,addr;
#if 0
uchar rec_in_time,start95time;
#endif
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

void uart1SendChar(uchar ch);
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
void ledShowChar(uchar ch){
    char i;
    for(i=7;i>=0;i--){
        LED_DATA_PIN=((ch>>i)&0x01);//先输出7
        LED_TICK();
        //LED_CLK_PIN=0;LED_CLK_PIN=0;LED_CLK_PIN=0;
        //LED_CLK_PIN=1;LED_CLK_PIN=1;LED_CLK_PIN=1;
    }
}
/******************************/

xdata uchar dispBuf[18];    //显示数组0最低位

/******************************/

void id2str(uchar idx, uint id, uchar isalive){
    uchar i;
    uint id_temp;

    id_temp = id & 0x1fff;
    if(id & 0x8000) id_temp |= 0x2000;      //MSB

    for(i=0; i<LED_MSG_LEN; i++){      //singleIdStr[0]: LSB
        DispArray[idx].idStr[i] = ledTable[id_temp % 10];
        id_temp /= 10;
        if (id_temp == 0) break;
    }
    /* remove the head 0s */
    while (++i < LED_MSG_LEN)
        DispArray[idx].idStr[i] = 0;

    if (id & 0x2000) DispArray[idx].idStr[1] |= 0x80;    //电池电量
    if (id & 0x4000) DispArray[idx].idStr[2] |= 0x80;    //按键报警
    if (isalive == FALSE) DispArray[idx].idStr[4] |= 0x80;    //新来的！
}

void ledShowAll(){//显示数组中的数
    uchar i, j;

    glbLedRefreshFlag = FALSE;

    LED_OPEN();
    for (i=0; i<DISP_ID_NUM; i++) {
        if (DispArray[i].dtime != 0) {
            for (j=0; j<LED_MSG_LEN; j++)
                ledShowChar(DispArray[i].idStr[j]);
        }
        else {
            for (j=0; j<LED_MSG_LEN; j++)
                ledShowChar(0);
        }
    }
    LED_CLOSE();
}

void ledFormat(uchar seg){//全部显示同一个数字
    uchar i, j;
    LED_OPEN();

    for (i=0; i<DISP_ID_NUM; i++) {
        for (j=0; j<LED_MSG_LEN; j++)
            ledShowChar(seg);
    }
    LED_CLOSE();
}

void initDispArray(void) {
    uchar i;
    //DispArray[0].dtime = 0;

    for(i=0; i<DISP_ID_NUM; i++){      
        DispArray[i].dtime = 0;
    }
    glbLedRefreshFlag = TRUE;
}

void addDisp2Alive(uint id) {
    AliveArray[aliveIdNum].id = id;
    AliveArray[aliveIdNum].atime = ALIVE_TIME;
    AliveArray[aliveIdNum].inDisplay = FALSE;
    aliveIdNum++;
    undispAliveIdNum++;
}

void return2Alive(uint id) {    //如果aliveArray中能找到，则刷新，如果已经从alive队列中删除，则直接扔掉
    uchar i;
    for (i=0; i<aliveIdNum; i++) {
        if (AliveArray[i].id == id) {
            AliveArray[i].inDisplay = FALSE;
            undispAliveIdNum++;
            return;
        }
    }
}

void refreshLedMsgTimer(void) {
    uchar i, k;
    for (i=0; i<DISP_ID_NUM; i++) {
        if (DispArray[i].dtime > 0) {
            if (--DispArray[i].dtime == 0) {
                glbLedRefreshFlag = TRUE;
                if (DispArray[i].isAlive == FALSE) {
                    addDisp2Alive(DispArray[i].id);
                }
                else {  //in Alive Array
                    return2Alive(DispArray[i].id);
                }
            }
        }
    }

    k = 0;
    for (i=0; i<aliveIdNum; i++) {
        if (--AliveArray[i].atime == 0) {
            k++;
            if (AliveArray[i].inDisplay == FALSE)
                undispAliveIdNum--;
        }
        else if (k)
            AliveArray[i-k] = AliveArray[i];
    }
    aliveIdNum -= k;
}

void addId2DispArray(uchar idx, uint id, uchar isalive) {
    DispArray[idx].id = id;
    id2str(idx, id, isalive);
    DispArray[idx].dtime = SHOW_TIME;
    DispArray[idx].isAlive = isalive;
    glbLedRefreshFlag = TRUE;
    if (isalive == FALSE) glbAlarmNum++;
}

uchar findIdFromDispArray(uint id) {
    uchar i;
    for (i=0; i<DISP_ID_NUM; i++) {
        if (DispArray[i].dtime>0 && DispArray[i].id == id) {
            //DispArray[i].dtime = SHOW_TIME;
            return TRUE;
        }
    }
    return FALSE;
}

uchar findIdFromAliveArray(uint id) {
    uchar i;
    for (i=0; i<aliveIdNum; i++) {
        if (AliveArray[i].id == id) {
            AliveArray[i].atime = ALIVE_TIME;
            return TRUE;
        }
    }
    return FALSE;
}
/******************************/
uchar cnt_01s = 0;//接收时间
uchar cnt_alarm = 0;    //控制蜂鸣器, clk 0.02s; cnt_alarm 0-40 ON, 40-50 OFF;
uchar glbTimer = 0;
uchar glbRefreshTimerFlag = 0;
/****************
 * f=110592/13/(65536-xxxx)
 ****************/

void time0() interrupt 1 {
    /* TF0 automatically cleared by hardware */
    TH0=0xBD; TL0=0x8A; //0.02s, actual 
#if 0
	rec_in_time+=1;//接收延时
#endif
	if(++cnt_01s > 5){//0.1s
		cnt_01s=0;
#ifdef CHIP_905
		start95time+=1;
#endif
        //initDispArray();
        refreshLedMsgTimer();
	}
    if (glbAlarmNum !=0 ) {
        cnt_alarm++;
        if (cnt_alarm == 1)
            SPEAKER_PIN = SPEAKER_ON;
        else if (cnt_alarm == 15)
            SPEAKER_PIN = SPEAKER_OFF;
        else if (cnt_alarm == 25) {
            cnt_alarm = 0;
            glbAlarmNum--;
        }
    }
}
/******************************/

/******************************/

/*******************crc8(x^8+x^5+x^4+1)**********/
uchar crc;
uchar sint0_data[2];//接收id暂存

#if 1
void crc8(){
    uchar index;
    uchar b;
    crc=0xff;
    for(index=0;index<2;index++){
        crc ^=sint0_data[index];
        for(b=0;b<8;b++){
            if(crc & 0x80) crc=(crc<<1)^0x31;
            else crc=(crc<<1);
        }
    }
}
#endif
/******************************/
//接收到ID时rec_id_suc=1;接收id暂存ReceivedArray[2];0高1低
uchar uartInt0Status=0,axor,aa_reg;
bit rec_id_suc=0;//接收id成功标志=1
uint uartCurrRxId;
void sint0() interrupt 4{//接收发送中断
    RI_1=0;
    if(RI){
        RI=0;
        switch(uartInt0Status){
            case 0:if(SBUF==0xaa) uartInt0Status=1;
                       break;
            case 1:if(SBUF==0xaa) {uartInt0Status=4;aa_reg=0;}
                       else uartInt0Status=0;
                       break;
            case 2:if(SBUF==0xaa) uartInt0Status=4;
                       else uartInt0Status=0;
                       break;
            case 3:if(SBUF==0xaa) uartInt0Status=4;
                       else uartInt0Status=0;
                       break;
            case 4: if(SBUF==0xaa) {aa_reg+=1;break;}
                        else {
                            sint0_data[0]=SBUF&0x1f;
                            axor=sint0_data[0];
                            uartCurrRxId=SBUF;//axor;
                            uartCurrRxId<<=8;
                            uartInt0Status=5;
                            break;
                        }
            case 5: sint0_data[1]=SBUF;
                    axor^=SBUF;
                    uartCurrRxId+=SBUF;
                    uartInt0Status=6;
                    break;
            case 6: axor^=SBUF;
                    crc8();
                    if(crc==SBUF) {
                        uartInt0Status=7;
                    }
                    else uartInt0Status=0;
                    break;
            case 7: if(axor==SBUF){
                        rec_id_suc=1;//接收放uartCurrRxId
                        //LED402=!LED402;
                    }
                    uartInt0Status=0;
                    break;
            default:uartInt0Status=0;
                    break;
        }
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


/*****************************************************************************
 * 名称：findIdFromReceivedArray(uint16 *ptr_id,uint16 size,uint16 id)
 * 功能：找和id相同的号
 * 入口参数：size=长度，id=用户号
 * 出口参数：返回size无相同，否则为相同id号位置
 ****************************************************************************/
uchar findIdFromReceivedArray(uint *ptr_id,uint size,uint id){
    uchar i;
    for(i=0;i<size;i++){
        if(*(ptr_id+i)==id) break;
    }
    return i;
}
/*****************************************************************************/
xdata uint rectx_id[16];//接收id组
uchar rectx_id_zz=0;//接收指针
/*****************************************************************************
 * 名称：handleUartRxId()
 * 功能：处理接收到的ID
 * 入口参数：无
 * 出口参数：无
 ****************************************************************************/
void handleUartRxId(){
    if(rec_id_suc){//有接收排队
        rec_id_suc=0;
        if (receivedIdNum >= RECEIVED_ARRAY_LEN) return;
        if (findIdFromAliveArray(uartCurrRxId) == TRUE) return;
        if (findIdFromDispArray(uartCurrRxId) == TRUE) return;  //must after finding from AliveArray
        if(findIdFromReceivedArray(ReceivedArray, receivedIdNum, uartCurrRxId)==receivedIdNum){//无相同
            ReceivedArray[receivedIdNum]=uartCurrRxId;        //添加到队列后面
            receivedIdNum++;
            //receivedIdNum&=0x3f;//最多64个
        }
#if 0
        if(findIdFromReceivedArray(rectx_id,rectx_id_zz,uartCurrRxId)==rectx_id_zz){//无相同
            rectx_id[rectx_id_zz]=uartCurrRxId;
            if(rectx_id_zz<15) rectx_id_zz++;
        }
#endif
    }
}

#ifdef CHIP_905
/******************************/
void rec_id95_cl(){
    if(rec_id95_suc){//有接收排队
        rec_id95_suc=0;
        if(findIdFromReceivedArray(ReceivedArray,receivedIdNum,rec95id)==receivedIdNum){//无相同
            ReceivedArray[receivedIdNum]=rec95id;
            receivedIdNum++;
            receivedIdNum&=0x3f;
        }
        if(findIdFromReceivedArray(rectx_id,rectx_id_zz,rec95id)==rectx_id_zz){//无相同
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
void idPopAndDisp(void){
    uchar i=0, j=0, k=0;
    //add received IDs to display array
    if (receivedIdNum != 0) {
        for (j=0; j<DISP_ID_NUM; j++) {
            if (DispArray[j].dtime != 0) { 
                continue;
            }
            addId2DispArray(j, ReceivedArray[i], FALSE);
            i++;
            if (i == receivedIdNum) break;
        }
        for(k=i; k<receivedIdNum; k++)    //pop ids displayed
            ReceivedArray[k-i] = ReceivedArray[k];
        receivedIdNum -= i;
    }
    //add alive IDs which are not being displayed to display array
    while(j<DISP_ID_NUM && undispAliveIdNum>0) {
        if (DispArray[j].dtime != 0) {
            j++;
            continue;
        }
        if (AliveArray[pAliveNext].inDisplay != TRUE) {
            addId2DispArray(j, AliveArray[pAliveNext].id, TRUE);
            AliveArray[pAliveNext].inDisplay = TRUE;
            j++;
            undispAliveIdNum--;
        }
        pAliveNext = (pAliveNext+1) % aliveIdNum;
    }
}
/******************************/
/******************************/
/******************************/
uchar pUartTx=0;//发指针
xdata uchar uartTxBuf[64];//15为长度
uchar pUartRx=0;//收指针
xdata uchar uartRxBuf[64];

void uart1SendChar(uchar ch) {
    SBUF1=ch;
    while(!TI_1);
    TI_1 = 0;
}
void sint1() interrupt 7{
#if 0
    if(TI){
        TI=0;
        if(uartTxBuf[63]!=0){
            ACC=uartTxBuf[pUartTx];TB8=P;
            SBUF1=uartTxBuf[pUartTx];
            pUartTx=++pUartTx&0x3f;
            uartTxBuf[63]-=1;
        }
        else{
            REN=1;DE=0;//开接收
        }
    }
    if(RI){
        RI=0;
        uartRxBuf[pUartRx]=SBUF1;//ACC=SBUF1;
        //if(TB8==P)
        pUartRx=++pUartRx&0x3f;
    }
#endif
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

#if 0
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
                    REN=0;DE=1;SBUF1=0xee;pUartTx=1;
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
                    REN=0;DE=1;SBUF1=0xee;uartHandlerStatus=0;
                }
                break;
        default:
                uartHandlerStatus=0;
                break;
    }
}
#endif

/******************************/
//T2发生器：Fosc/(32*(65536-(RCAP2H,RCAP2L)))
//T1发生器：TH1=TL1= 256-(F*1000000*(SMOD+1))/(32*12*B)

/******************************/
main(){
    SPEAKER_PIN = SPEAKER_OFF;
    ENABLE_WD_RST();
    PMR |= 0x01;            //enable the on-chip 1KB MOVX SRAM

#ifdef CHIP_905
    TRX_CE = 0;
#endif

#if 0
    SCK=0;
    CSN=1;
#endif

    CKCON |= 0x40;          //set WD time-out 2^17 clocks
    TMOD=0x21;              //set T1: mode 2, 8bits auto-reload from THx
                            //set T0: 18bits, no prescale   //spec error, should be 16bits

    /* set timer 0 */
    TH0=0x4c;TL0=0x00;      //50ms
    TR0=1;                  //turn T0 on
    ET0=1;                  //enable T0 interrupt
    PT0=1;                  //set T0 interrupt higher priority

    /* UART1 SETTINGS */
#if 0
	TH1=0xfd;TL1=0xfd;//9.6k
#else
    TH1 = 0xf4;
    TL1 = 0xf4;             //2.4k
#endif

    SCON1=0x70;             //Uart1 mode: 1
                            //set Serial Recption enable
    //SCON1 = 0xd0;           //Uart1 mode: 3
    
    TR1 = 1;                //set timer 1 on
    ES1=1;                  //Enable Serial Port 1 interrupt

    /* UART0 SETTINGS */
    RCAP2H=0xff;            //MSB of timer2 auto-reload
    RCAP2L=0xdc;	        //LSB of timer2 auto-reload
    //RCAP2L=0xb8;	        //LSB of timer2 auto-reload
                            //ffdc 9.6k;ffb8 4.8k;fee0h-1.2k
    T2CON=0x34;             //serial port 0, tx: T2, RX: T2
                            //baud rate generator mode, 16bit counter with auto-reload
    SCON=0x70;              //Serial port 0 control : mode 1-----mode 3???

    ES=1;                   //Enable Serial Port 0 interrupt
    EA=1;                   //global interrupt enable

    //FMQ=0;DE=0;RE=0;        //P3^2 P3^4 P3^5

    addr=P1;addr=addr>>4;

#ifdef CHIP_905
    write_con();
#endif
    ledFormat(0x40);

    initDispArray();

    while(1) {
#ifdef CHIP_905
        TRX_CE=1;
#endif

#if 0
        uart1SendChar('h');
        uart1SendChar('e');
        uart1SendChar('l');
        uart1SendChar('l');
        uart1SendChar('o');
#endif
        WD_RST(); 
        addr = P1>>4;
#ifdef CHIP_905
        rx95();
#endif
        WD_RST(); 
        handleUartRxId();
#ifdef CHIP_905
        rec_id95_cl();
#endif
        WD_RST(); 
        idPopAndDisp();
        WD_RST(); 


        if (glbLedRefreshFlag == TRUE)
            ledShowAll();
            
        www=!www;//0708
        INT0 = !INT0;       //touch MAX813L
#if 0
        uartHandler();
#endif
#ifdef CHIP_905
        if(start95time>250){//10秒收不到
            start95time=0;LED402=!LED402;//070724
            write_con();
        }
#endif
    }
}
