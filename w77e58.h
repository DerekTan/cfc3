//W77E58头文件;整理：聂小猛。该资料来自“51单片机世界”http://go.163.com/~dz2000,欢迎访问。
/*--BYTE Registers-------------------------------------------------*/
sfr P0    = 0x80;
sfr P1    = 0x90;
sfr P2    = 0xA0;
sfr P3    = 0xB0;
#define p0 P0
#define p1 P1
#define p2 P2
#define p3 P3


sfr PSW   = 0xD0;
sfr ACC   = 0xE0;
sfr B     = 0xF0;
sfr SP    = 0x81;
sfr DPL   = 0x82;
sfr DPH   = 0x83;
sfr PCON  = 0x87;//PCON.7(SMOD)拨特率加倍，PCON.1(PD)掉电方式，PCON.0(IDL)冻结方式
				//PCON.6(SMOD0)帧错检测允许，PCON.3(GF1)PCON.2(GF0)
sfr TCON  = 0x88;//定时控制寄存器
sfr TMOD  = 0x89;//Timer Mode Control :
/*"gate,c/t,m1,m0"x2定时器方式GATE=1时只有intx=1时才可以开放定时器x；
c/t = 1时计书数方式，
    = 0时定时方式。
m1m0= 00时13位计数，
    = 01时16位=10时自装入8位，
    = 11时定时器0分两个，定时器1停止。
*/
sfr TL0   = 0x8A;
sfr TL1   = 0x8B;
sfr TH0   = 0x8C;
sfr TH1   = 0x8D;
sfr IE    = 0xA8;
sfr IP    = 0xB8;//中断优先级
sfr SCON  = 0x98;//串口控制与状态
sfr SBUF  = 0x99;

/*--W77E58 Extensions---------------------------------------------*/
sfr T2CON = 0xC8;
sfr T2MOD = 0xc9;//HC5,HC4,HC3,HC2,T2CR,-,T2OE,DCEN
	            //hc5-hc2 =1时外中断5-2标志硬件自动清除。
	            //t2cr=1 捕获完成时自动复位
	            //t2oe=1 定时器2输出允许
	            //dcen=1 减计数允许，结合外部输入t2ex使用，16位自装入模式
sfr RCAP2L= 0xCA;//重装的预置数，捕获的输出
sfr RCAP2H= 0xCB;
sfr TL2   = 0xCC;
sfr TH2   = 0xCD;

sfr DPL1  = 0x84;
sfr DPH1  = 0x85;
sfr DPS   = 0x86;

sfr CKCON = 0x8e;
/* wd1|wd0|t2m|t1m|t0m|md2|md1|md0  */
/* wd1wd0:   watchdog timer mode select bits
 * 00=2^17,
 * 01=2^20,
 * 10=2^23,
 * 11=2^26
 * t2m,t1m,t0m: timer (x) clock set
 * 等于1:时钟4分频
 * 等于0:时钟12分频
//md2,md1,md0 MOVX执行机器周期000=2，001=3。。。111=9
*/
sfr EXIF  = 0x91;//ie5,ie4,ie3,ie2,XT/RG,RGMD,RGSL,-
				//ie5,ie3=1外中断下跳变中断标志
				//ie4,ie3=2外中断上跳变中断标志
sfr P4    = 0xa5;//低四位有效
sfr SADDR = 0xa9;//从机串口0地址
sfr SADDR1= 0xaa;//从机串口1地址
sfr SADEN = 0xb9;//串口0地址屏蔽，等于0时所有地址都会引起中断
sfr SADEN1= 0xba;//串口0地址屏蔽，等于0时所有地址都会引起中断
sfr SCON1 = 0xc0;//serial port control 1
sfr SBUF1 = 0xc1;
sfr ROMMAP= 0xc2;//ROMMAP.7为等待信号使能，用movx指令时，wait脚为p4.0
sfr PMR   = 0xc4;//Power Manager Register. CD1,CD0,SWB,-,XTOFF,ALE-OFF,-DME0
                //cd0cd1=0时钟不变，=1-1/4，=2-1/64，=3=1/1024
                //swb=1强制4分频，外中断或串口中断唤醒
                //aleoff =1时ale信号终止，外部内存访问时自动唤醒
                //dme0 =1时内部1kram使能
sfr STATUS= 0xc5;//-,HIP,LIP,XTUP,SPTA1,SPRA1,SPTA0,SPRA0
                //hip=1正在处理高优先级中断
                //lip=1正在处理低优先级中断
                //spta1串口0正在发送数据
                //spra1串口0正在接收数据
                //spta0串口0正在发送数据
                //spra0串口0正在接收数据
sfr TA    = 0xc7;//保护位
sfr WDCON = 0xd8;//SMOD_1,POR,-,-,WDIF,WTRF,EWT,RWT
	              //smod1 加倍串口1的拨特率,pro电源复位标志
	              //por 电源复位标志
	        //wdif看门狗定时中断标志，软清
	        //wtrf看门狗定时器复位标志，看门狗引起复位后，该位置1
	        //ewt 允许看门狗定时器自动复位
	        //rwt复位看门狗定时器，如果看门狗溢出还没有被复位计数器，将会引起中断，再过512周期将复位
sfr EIE   = 0xe8;//-，-，-，EWDI,EX5,EX4,EX3,EX2
				//ewdi=1，允许看门狗中断�
				//ex5-ex2；外部中断允许
sfr EIP   = 0xf8;//-，-，-，pwdi，px5,px4,px3,px2
				//pwdi=1 看门狗中断优先
				//px5-px2=1 外部中断优先

/*--BIT Registers-------------------------------------------------*/
/*  PSW		*/
sbit CY    = 0xD7;
sbit AC    = 0xD6;
sbit F0    = 0xD5;
sbit RS1   = 0xD4;
sbit RS0   = 0xD3;
sbit OV    = 0xD2;
sbit P     = 0xD0;

/*  TCON	*/
sbit TF1   = 0x8F;//定时器1溢出标志，自动清零
sbit TR1   = 0x8E;//timer1运行控制位
sbit TF0   = 0x8D;//定时器0溢出标志，自动清零
sbit TR0   = 0x8C;//timer0运行控制位
sbit IE1   = 0x8B;//外中断1跳变中断请求标志，自动清零
sbit IT1   = 0x8A;//中断1跳变检测使能
sbit IE0   = 0x89;//外中断0跳变中断请求标志，自动清零
sbit IT0   = 0x88;//中断0跳变检测使能

/*  IE		*/
sbit EA    = 0xAF;//
sbit ES    = 0xAC;
sbit ET1   = 0xAB;
sbit EX1   = 0xAA;
sbit ET0   = 0xA9;
sbit EX0   = 0xA8;

/*  IP		*/
sbit PS    = 0xBC;//串口0中断优先设定
sbit PT1   = 0xBB;//定时器1中断优先
sbit PX1   = 0xBA;//外中断1
sbit PT0   = 0xB9;//定时器0中断优先
sbit PX0   = 0xB8;//外中断0

/*  P3		*/
sbit RD   = 0xB7;
sbit WR   = 0xB6;
sbit T1   = 0xB5;
sbit T0   = 0xB4;
sbit INT1 = 0xB3;
sbit INT0 = 0xB2;
sbit TXD  = 0xB1;
sbit RXD  = 0xB0;

/*  SCON	*/
sbit SM0   = 0x9F;//SM0/FE  串口0方式0选择或帧错标志，人工清零
sbit SM1   = 0x9E;//串口1工作方式选择0-4/12tclk同步，1-10位可设bps异步，2-64/32tclk异步，3-11位可变bps异步
sbit SM2   = 0x9D;//允许方式2和3的多机通讯控制位。方式2、3中=1时接收到的第9位为0时不启动接收中断ri标志
	   			//方式1中，=1时只有收到有效停止时才启动ri，方式0中=1将拨特率提高3倍。
sbit REN   = 0x9C;//允许接收标志
sbit TB8   = 0x9B;//方式23中要发送的第9位。
sbit RB8   = 0x9A;//方式23中接收到的第9位。
sbit TI    = 0x99;//发送中断标志
sbit RI    = 0x98;//接收中断标志

/*--W77E58 Extensions--------------------------------------------*/
/* PSW		*/
sbit F1    = 0xd1;

/*  IE		*/
sbit ET2   = 0xAD;//定时器2允许
sbit ES1   = 0xae;//串口1优先

/*  IP		*/
sbit PT2   = 0xBD;//定时器2优先
sbit PS1   = 0xbe;//串口1优先

/*  P1		*/
sbit T2    = 0x90;//计数器2输入端口
sbit T2EX  = 0x91;//计数器捕获触发
sbit RXD1  = 0x92;//串口1入
sbit TXD1  = 0x93;//串口2出
sbit INT2  = 0x94;//外中断2，上跳变触发
sbit INT3  = 0x95;//外中断3，下跳变触发
sbit INT4  = 0x96;//外中断4，上跳变触发
sbit INT5  = 0x97;//外中断5，下跳变触发

             
/*  T2CON	*/
sbit TF2   = 0xCF;//定时器2溢出标志，软清除
sbit EXF2  = 0xCE;//定时器2外部标志，当exen2=1且t2ex引脚负跳变引起捕获或重载时置位，软清。
sbit RCLK  = 0xCD;//接收时钟标志，=1时串口0用定时器2溢出做时钟。
sbit TCLK  = 0xCC;//发送时钟标志，=1时串口0用定时器2溢出做时钟。
sbit EXEN2 = 0xCB;//定时器2外部允许标志，=1时若未用作拨特率发生器，t2ex脚的负跳变引起捕获
sbit TR2   = 0xCA;//定时器2运行控制位
sbit C_T2  = 0xC9;//计数/定时选择，=1时计数
sbit CP_RL= 0xC8;//捕获/重载标志，=1时,外部允许时，vt2ex负跳变发生捕获
                 //=0时 溢出或外部允许时，vt2ex负跳变发生重载

/*  SCON1	*/  //注释参考SCON
sbit SM0_1 = 0xC7;
sbit SM1_1 = 0xC6;
sbit SM2_1 = 0xC5;
sbit REN_1 = 0xC4;
sbit TB8_1 = 0xC3;
sbit RB8_1 = 0xC2;
sbit TI_1  = 0xC1;
sbit RI_1  = 0xC0;

/* WDCON	*/
sbit SMOD_1= 0xDF; //加倍串口1的拨特率,pro电源复位标志
sbit POR   = 0xDE; //电源复位标志
sbit WDIF  = 0XDB; //看门狗定时中断标志，软清
sbit WTRF  = 0xDA; //看门狗定时器复位标志，看门狗引起复位后，该位置1
sbit EWT   = 0xD9; // 允许看门狗定时器自动复位
sbit RWT   = 0xD8; //复位看门狗定时器，如果看门狗溢出还没有被复位计数器，将会引起中断，再过512周期将复位


/* EIE		*/
sbit EWDI  = 0xec; //允许看门狗中断
sbit EX5   = 0xeb; //ex5-ex2；外部中断允许
sbit EX4   = 0xea; 
sbit EX3   = 0xe9; 
sbit EX2   = 0xe8; 

/* EIP		*/
sbit PWDI  = 0xfc; //pwdi=1 看门狗中断优先
sbit PX5   = 0xfb; 	//px5-px2=1 外部中断优先
sbit PX4   = 0xfa; 
sbit PX3   = 0xf9; 
sbit PX2   = 0xf8; 
