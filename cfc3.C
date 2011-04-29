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
/******************************/
sbit SO40=P1^0;
sbit SCK40=P1^1;
sbit STR=P1^3;
sbit FMQ=P3^2;	
sbit LED=P3^6;	
sbit LED402=P3^7;
sbit DE=P3^4;//=1tx,=0rx
sbit RE=P3^5;//=0
sbit aaaaa=P0^0;
sbit DR=P3^3;
sbit TRX_CE=P0^2;
sbit MISO=P0^4;
sbit MOSI=P0^5;
sbit SCK=P0^6;
sbit CSN=P0^7;	

sbit www=P2^0;//0708
/******************************/
idata uint idgh_id[32],idgh_gh[32];
uchar idgh_zz=0,add;
uchar rec_in_time,start95time;
uchar trx485=0;
xdata uchar yrx=0;//=0有接收
/**************************************************/
#define RF_C0 0x0 //422.4+RF_C_/10,0x73=433.9M,0x77=434.3
#define RF_C1 0x00 //(00)5=1重发，4=1接收小电流，32发射功率00=-10;01=-2;10=+6;11=+10
#define RF_C2 0x11 //(44)地址宽度
#define RF_C3 0x04 //(20)接收长度///////////////////
#define RF_C4 0x04 //(20)发送长度///////////////////
#define RF_C5 0x79 //(e7)接收地址1
#define RF_C6 0x79 //(e7)接收地址2
#define RF_C7 0x79 //(e7)接收地址3
#define RF_C8 0x79 //(e7)接收地址4
#define RF_C9 0xc0 //(e7)7=1十六位，6=1有校验，543=000-4M，=011-16M，2=1有时钟出，10=00-4m,01-2m,10-1m,11-500k
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
void out905(uchar aa){//将aa输出到905
	uchar my_flag1;
	char i;
	for(i=7;i>=0;i--){
		my_flag1=aa;
		MOSI=((my_flag1>>i)&0x01);//先输出7
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
	TRX_CE=0;TRX_CE=0;TRX_CE=0;TRX_CE=0;TRX_CE=0;TRX_CE=0;TRX_CE=0;TRX_CE=0;
	TRX_CE=0;TRX_CE=0;TRX_CE=0;TRX_CE=0;TRX_CE=0;TRX_CE=0;TRX_CE=0;TRX_CE=0;
	TRX_CE=0;TRX_CE=0;TRX_CE=0;TRX_CE=0;TRX_CE=0;TRX_CE=0;TRX_CE=0;TRX_CE=0;
	TRX_CE=0;TRX_CE=0;TRX_CE=0;TRX_CE=0;TRX_CE=0;TRX_CE=0;TRX_CE=0;TRX_CE=0;
	CSN=0;CSN=0;CSN=0;CSN=0;CSN=0;CSN=0;CSN=0;CSN=0;CSN=0;CSN=0;CSN=0;CSN=0;
	CSN=0;CSN=0;CSN=0;CSN=0;CSN=0;CSN=0;CSN=0;CSN=0;CSN=0;CSN=0;CSN=0;CSN=0;
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

/******************************/
uchar code table_led[22]={ 0x3f,0x06,0x5b,0x4f,//0,1,2,3
			   0x66,0x6d,0x7d,0x07,//4,5,6,7
			   0x7f,0x6f,   //8,9
               0x77, 0x7c,  //A, b
               0x58, 0x5e,  //c, d
               //0x39, 0x5e,  //C, d
               0x79, 0x71,  //E, F
               0x3d,0x76,   //G,H
			   0x01,0x40,0x08,0x00};//上,中,下,黑
/******************************/
void out94(uchar aa){
	uchar my_flag1;
	char i;
	for(i=7;i>=0;i--){
		my_flag1=aa;//table_led[aa];
		SO40=((my_flag1>>i)&0x01);//先输出7
		SCK40=0;SCK40=0;SCK40=0;
		SCK40=1;SCK40=1;SCK40=1;
	}
}
/******************************/

uchar dis_time;//显示时间
xdata uchar dis_buf[18];//显示数组0最低位

void out4094(){//显示数组中的数
		STR=0;
		out94(dis_buf[17]);out94(dis_buf[16]);
		out94(dis_buf[15]);out94(dis_buf[14]);
		out94(dis_buf[13]);out94(dis_buf[12]);
		out94(dis_buf[11]);out94(dis_buf[10]);
		out94(dis_buf[9]);out94(dis_buf[8]);
		out94(dis_buf[7]);out94(dis_buf[6]);
		out94(dis_buf[5]);out94(dis_buf[4]);
		out94(dis_buf[3]);out94(dis_buf[2]);
		out94(dis_buf[1]);out94(dis_buf[0]);
		STR=1;
}
/******************************/
uchar abc,abc1;//接收时间
/******************************/
void time0() interrupt 1 using 2{
	TH0=0xf1;TL0=0x9f;//4ms---9a
	//TH0=0x4c;TL0=0x00;//50ms
	rec_in_time+=1;//接收延时
	abc++;
	if(abc>12){//50ms
		abc=0;abc1++;
		dis_time+=1;//1000ms显示
		start95time+=1;
	}
}
/******************************/

/******************************/
uchar int_bcd[5];//int-bcd 
void zint_bcd(uint aa){//输入temp0输出4最高位
	uchar i;
	uint temp0,temp1;
	temp0=aa;
	for(i=0;i!=5;i++){
		temp1=temp0;
		temp0=temp0/10;
                int_bcd[i]=temp1-temp0*10;
	}
}
/******************************/
uchar fint_bcd[5];//int-bcd 
void fzint_bcd(uint aa){//输入temp0输出4最高位
	uchar i;
	uint temp0,temp1;
	temp0=aa;
	for(i=0;i!=5;i++){
		temp1=temp0;
		temp0=temp0/10;
                fint_bcd[i]=temp1-temp0*10;
	}
}
/******************************/
	
/******************************/

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
//接收到ID时rec_id_suc=1;接收id暂存rec_id[2];0高1低
uchar jcsint1=0,axor,aa_reg;
bit rec_id_suc=0;//接收id成功标志=1  
uint rec402id;
void sint1() interrupt 7 using 1{
	RI_1=0;
	switch(jcsint1){
		case 0:if(SBUF1==0xaa) jcsint1=1;
			break;
		case 1:if(SBUF1==0xaa) {jcsint1=4;aa_reg=0;}
			else jcsint1=0;
			break;
		case 2:if(SBUF1==0xaa) jcsint1=4;
			else jcsint1=0;
			break;
		case 3:if(SBUF1==0xaa) jcsint1=4;
			else jcsint1=0;
			break;
		case 4: if(SBUF1==0xaa) {aa_reg+=1;break;}
			else {
				sint1_data[0]=SBUF1&0x1f;
				axor=sint1_data[0];
				rec402id=SBUF1;//axor;
				rec402id<<=8;
				jcsint1=5;
				break;
			}
		case 5: sint1_data[1]=SBUF1;
			axor^=SBUF1;
			rec402id+=SBUF1;
			jcsint1=6;
			break;
		case 6: axor^=SBUF1;
			crc8();
			if(crc==SBUF1) {
				//rec_id_suc=1;
        //LED402=!LED402;
				jcsint1=7;
			}
			else jcsint1=0;
			break;
		case 7: if(axor==SBUF1){
				rec_id_suc=1;//接收放rec402id
				LED402=!LED402;
			}
			jcsint1=0;
			break;
		default:jcsint1=0;
			break;
	}
}
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
/*****************************************************************************/
xdata uint rec_id[64];//接收id组
uchar rec_id_zz=0;//接收指针

/*****************************************************************************
* 名称：find_id(uint16 *ptr_id,uint16 size,uint16 id)
* 功能：找和id相同的号
* 入口参数：size=长度，id=用户号
* 出口参数：返回size无相同，否则为相同id号位置
****************************************************************************/
uchar find_id(uint *ptr_id,uint size,uint id){
	uchar b,bb=0;
	for(b=0;b<size;b++){
		bb=b;
		if(*(ptr_id+b)==id) break;
		bb=size;
	}
	return bb;
}
/*****************************************************************************/
xdata uint rectx_id[16];//接收id组
uchar rectx_id_zz=0;//接收指针
/******************************/
void rec_id402_cl(){
	if(rec_id_suc){//有接收排队
		rec_id_suc=0;
		if(find_id(rec_id,rec_id_zz,rec402id)==rec_id_zz){//无相同
			rec_id[rec_id_zz]=rec402id;
			rec_id_zz++;
			rec_id_zz&=0x3f;
		}
		if(find_id(rectx_id,rectx_id_zz,rec402id)==rectx_id_zz){//无相同
			rectx_id[rectx_id_zz]=rec402id;
			if(rectx_id_zz<15) rectx_id_zz++;
			//rectx_id_zz&=0x0f;
		}
	}
}
/******************************/
void rec_id95_cl(){
	if(rec_id95_suc){//有接收排队
		rec_id95_suc=0;
		if(find_id(rec_id,rec_id_zz,rec95id)==rec_id_zz){//无相同
			rec_id[rec_id_zz]=rec95id;
			rec_id_zz++;
			rec_id_zz&=0x3f;
		}
		if(find_id(rectx_id,rectx_id_zz,rec95id)==rectx_id_zz){//无相同
			rectx_id[rectx_id_zz]=rec95id;
			if(rectx_id_zz<15) rectx_id_zz++;
			//rectx_id_zz&=0x0f;
		}
	}
}
/******************************/
#define DISTIME 20
uchar jcidxs=0;
uint cgh=0;//要查工号的id号
void idxs(){//将rec_id内容显示
	uchar ii;
	uint aaaa;
	switch(jcidxs){
		case 0:	if(rec_id_zz!=0){//有接收
							aaaa=rec_id[0]&0x1fff;
							if((rec_id[0]&0x8000)) aaaa+=0x2000;//20110126
			
								zint_bcd(aaaa);
                                if (int_bcd[4] <= 1)
                                    int_bcd[3] += 10*int_bcd[4];
								dis_buf[17]=table_led[int_bcd[3]];	
								dis_buf[16]=table_led[int_bcd[2]];	
								dis_buf[15]=table_led[int_bcd[1]];	
								dis_buf[14]=table_led[int_bcd[0]];
								if(trx485==0) dis_buf[14]|=0x80;//亮小数点
								if(dis_buf[17]==0x3f){//前面0不显示
									dis_buf[17]=0;
									if(dis_buf[16]==0x3f){
										dis_buf[16]=0;
										if(dis_buf[15]==0x3f){
											dis_buf[15]=0;
										}
									}
								}	
								if((rec_id[0]&0x4000)==0x4000) dis_buf[16]|=0x80;
								if((rec_id[0]&0x2000)==0x2000) dis_buf[15]|=0x80;
								dis_buf[8]=dis_buf[17];
								dis_buf[7]=dis_buf[16];
								dis_buf[6]=dis_buf[15];
								dis_buf[5]=dis_buf[14];
								dis_buf[13]=0x00;
								dis_buf[4]=0x00;
								dis_buf[12]=0x00;
								dis_buf[3]=0x00;
								dis_buf[11]=0x00;
								dis_buf[2]=0x00;
								dis_buf[10]=0x00;
								dis_buf[1]=0x00;
								dis_buf[9]=0x00;
								dis_buf[0]=0x00;

							if((yrx&0x01)==1){dis_buf[0]=0x80;}
							out4094();
							dis_time=0;
							if(rec_id_zz>50) dis_time=18;
							else if(rec_id_zz>40) dis_time=16;
							else if(rec_id_zz>30) dis_time=14;
							else if(rec_id_zz>20) dis_time=12;
							else if(rec_id_zz>10) dis_time=10;
							else if(rec_id_zz>5) dis_time=5;
							jcidxs=1;
							for(ii=0;ii<rec_id_zz;ii++){//前移一格
								rec_id[ii]=rec_id[ii+1];
							}
							rec_id_zz=--rec_id_zz&0x3f;
						}else{
							dis_buf[0]=0;dis_buf[1]=0;
							dis_buf[2]=0;dis_buf[3]=0;
							dis_buf[4]=0;dis_buf[5]=0;
							dis_buf[6]=0;dis_buf[7]=0;
							dis_buf[8]=0;dis_buf[9]=0;
							dis_buf[10]=0;dis_buf[11]=0;
							dis_buf[12]=0;dis_buf[13]=0;
							dis_buf[14]=0;dis_buf[15]=0;
							dis_buf[16]=0;dis_buf[17]=0;
							if((yrx&0x01)==1){dis_buf[0]=0x80;}
							out4094();
						}
						break;
		case 1: if(dis_time>20){//显示0.5-1s
					jcidxs=0;
					dis_buf[0]=0;dis_buf[1]=0;
					dis_buf[2]=0;dis_buf[3]=0;
					dis_buf[4]=0;dis_buf[5]=0;
					dis_buf[6]=0;dis_buf[7]=0;
					dis_buf[8]=0;dis_buf[9]=0;
					dis_buf[10]=0;dis_buf[11]=0;
					dis_buf[12]=0;dis_buf[13]=0;
					dis_buf[14]=0;dis_buf[15]=0;
					dis_buf[16]=0;dis_buf[17]=0;
					if((yrx&0x01)==1){dis_buf[0]+=0x80;dis_buf[9]+=0x80;}
					out4094();
			}
			break;
		
		default:jcidxs=0;break;
	}
}
/******************************/
/******************************/
/******************************/
uchar t_out=0;//发指针
xdata uchar t_buf[64];//15为长度
uchar r_in=0;//收指针
xdata uchar r_buf[64];

void sint0() interrupt 4 using 1{//接收发送中断
	if(TI){
		TI=0;
		if(t_buf[63]!=0){
			ACC=t_buf[t_out];TB8=P;
			SBUF=t_buf[t_out];
			t_out=++t_out&0x3f;
			t_buf[63]-=1;
		}
		else{
		REN=1;DE=0;//开接收
		}
	}
	if(RI){
		RI=0;
		r_buf[r_in]=SBUF;//ACC=SBUF;
		//if(TB8==P) 
		r_in=++r_in&0x3f;
	}
}
/******************************/
uchar rec_in_cl=0;
uchar jcrec_in=0;
uchar ccgh0,ccgh1;//暂存接收工号

void rec_data_cl(){//接收数据处理并发送
	uchar i,j;
	switch(jcrec_in){
		case 0:	if(rec_in_cl!=r_in){//判第一个ff
				if(r_buf[rec_in_cl]==0xff) jcrec_in+=1;	
				else jcrec_in=0;
				rec_in_cl=++rec_in_cl&0x3f;
			}
			break;
		case 1:	if(rec_in_cl!=r_in){//判第二个ff
				if(r_buf[rec_in_cl]==0xff) jcrec_in+=1;	
				else jcrec_in=0;
				rec_in_cl=++rec_in_cl&0x3f;
			}
			break;
		case 2:	if(rec_in_cl!=r_in){//判地址
				if(r_buf[rec_in_cl]!=0xff){
					if(r_buf[rec_in_cl]==add) {jcrec_in+=1;abc1=0;yrx+=1;}//051107
					else jcrec_in=0;
				}
				rec_in_cl=++rec_in_cl&0x3f;
			}
			break;
		case 3:	if(rec_in_cl!=r_in){//判命令
				if(r_buf[rec_in_cl]==0x01) jcrec_in+=1;//当前
				else if(r_buf[rec_in_cl]==0x07) jcrec_in=7;//上一帧
				else jcrec_in=0;
				rec_in_cl=++rec_in_cl&0x3f;
			}
			break;
		case 4:	if(rec_in_cl!=r_in){//判命令1校验
				if(r_buf[rec_in_cl]==(add^0x01)){//comm1ok
					rec_in_time=0;
					jcrec_in=5;
					trx485=!trx485;DE=1;
				}	
				else jcrec_in=0;
				rec_in_cl=++rec_in_cl&0x3f;
			}
			break;
		case 5:	if(rec_in_time>=5){//接收后延时发送
				if(rectx_id_zz!=0){//有号
					//t_buf[0]=0xee;//发有信息
					t_buf[1]=0xee;
					t_buf[2]=add;
					t_buf[3]=0x04;
					j=rectx_id_zz<<1;
					t_buf[4]=j;
					t_buf[5+j]=t_buf[2]^t_buf[3]^t_buf[4];
					for(i=0;i<rectx_id_zz;i++){
						t_buf[5+(2*i)]=rectx_id[i];
						t_buf[5+j]^=t_buf[5+(2*i)];
						t_buf[6+(2*i)]=rectx_id[i]>>8;
						t_buf[5+j]^=t_buf[6+(2*i)];
					}
					t_buf[62]=5+j;
					t_buf[63]=5+j;
					jcrec_in=0;		
					rectx_id_zz=0;
				}else{//无号
					//t_buf[0]=0xee;//发无信息
					t_buf[1]=0xee;
					t_buf[2]=add;
					t_buf[3]=0x00;
					t_buf[4]=add;
					t_buf[62]=0x04;
					t_buf[63]=0x04;
					jcrec_in=0;
				}
				REN=0;DE=1;SBUF=0xee;t_out=1;
			}
			break;
		case 6:	if(((r_in-rec_in_cl)&0x3f)>=6){//接收时间
				rec_in_time=0;
				jcrec_in=18;
				trx485=!trx485;
			}
			break;
		case 7:	if(rec_in_cl!=r_in){//判命令7校验
				if(r_buf[rec_in_cl]==(add^0x07)){//comm7ok
					rec_in_time=0;
					jcrec_in=8;
				}	
				else jcrec_in=0;
				rec_in_cl=++rec_in_cl&0x3f;
			}
			break;
		case 8:	if(rec_in_time>=10){//换发送组发送上一帧
			t_out=1;
			t_buf[63]=t_buf[62];
			REN=0;DE=1;SBUF=0xee;jcrec_in=0;
			}
			break;
		default:jcrec_in=0;break;
	}
}
/******************************/
//T2发生器：Fosc/(32*(65536-(RCAP2H,RCAP2L))) 
//T1发生器：TH1=TL1= 256-(F*1000000*(SMOD+1))/(32*12*B)

/******************************/
main(){
	TA=0xaa;TA=0x55;EWT=1;PMR|=0x01;
	TRX_CE=0;SCK=0;CSN=1;
	CKCON=CKCON|0x40;	
	TMOD=0x21;
	TH0=0x4c;TL0=0x00;TR0=1;ET0=1;PT0=1;//50ms
	TH1=0xfd;TL1=0xfd;TR1=1;//9.6k
	SCON1=0x50;
	ES1=1;
	RCAP2H=0xff;
	RCAP2L=0xb8;	//ffdc 9.6k;ffb8 4.8k;fee0h-1.2k
	T2CON=0x34;
	SCON=0xd0;//50;
	ES=1;
	EA=1;
	FMQ=0;DE=0;RE=0;
	for(add=0;add<32;add++){//清对应表
		idgh_id[add]=0xffff;
	}
	add=P1;add=add>>4;
	write_con();
	dis_buf[0]=0x40;
	dis_buf[1]=0x40;
	dis_buf[2]=0x40;
	dis_buf[3]=0x40;
	dis_buf[4]=0x40;
	dis_buf[5]=0x40;
	dis_buf[6]=0x40;
	dis_buf[7]=0x40;
	dis_buf[8]=0x40;
	dis_buf[9]=0x40;
	dis_buf[10]=0x40;
	dis_buf[11]=0x40;
	dis_buf[12]=0x40;
	dis_buf[13]=0x40;
	dis_buf[14]=0x40;
	dis_buf[15]=0x40;
	dis_buf[16]=0x40;
	dis_buf[17]=0x40;
	out4094();
	
	
	
	while(1){
		TRX_CE=1;
		TA=0xaa;TA=0x55;RWT=1;
		add=P1>>4;
		rx95();
		TA=0xaa;TA=0x55;RWT=1;
		rec_id402_cl();
		rec_id95_cl();
		TA=0xaa;TA=0x55;RWT=1;
		idxs();
		TA=0xaa;TA=0x55;RWT=1;
		www=!www;//0708
		rec_data_cl();
		if(start95time>250){//10秒收不到
			start95time=0;LED402=!LED402;//070724
			write_con();
		}
		
	}
}

