//W77E58ͷ�ļ�;������С�͡����������ԡ�51��Ƭ�����硱http://go.163.com/~dz2000,��ӭ���ʡ�
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
sfr PCON  = 0x87;//PCON.7(SMOD)�����ʼӱ���PCON.1(PD)���緽ʽ��PCON.0(IDL)���᷽ʽ
				//PCON.6(SMOD0)֡��������PCON.3(GF1)PCON.2(GF0)
sfr TCON  = 0x88;//��ʱ���ƼĴ���
sfr TMOD  = 0x89;//"gate,c/t,m1,m0"x2��ʱ����ʽGATE=1ʱֻ��intx=1ʱ�ſ��Կ��Ŷ�ʱ��x��
	              //c/t =1ʱ��������ʽ��=0ʱ��ʱ��ʽ��m1m0=00ʱ13λ������=01ʱ16λ=10ʱ��װ��8λ��
	              //11ʱ��ʱ��0����������ʱ��1ֹͣ��
sfr TL0   = 0x8A;
sfr TL1   = 0x8B;
sfr TH0   = 0x8C;
sfr TH1   = 0x8D;
sfr IE    = 0xA8;
sfr IP    = 0xB8;//�ж����ȼ�
sfr SCON  = 0x98;//���ڿ�����״̬
sfr SBUF  = 0x99;

/*--W77E58 Extensions---------------------------------------------*/
sfr T2CON = 0xC8;
sfr T2MOD = 0xc9;//HC5,HC4,HC3,HC2,T2CR,-,T2OE,DCEN
	            //hc5-hc2 =1ʱ���ж�5-2��־Ӳ���Զ������
	            //t2cr=1 �������ʱ�Զ���λ
	            //t2oe=1 ��ʱ��2�������
	            //dcen=1 ��������������ⲿ����t2exʹ�ã�16λ��װ��ģʽ
sfr RCAP2L= 0xCA;//��װ��Ԥ��������������
sfr RCAP2H= 0xCB;
sfr TL2   = 0xCC;
sfr TH2   = 0xCD;

sfr DPL1  = 0x84;
sfr DPH1  = 0x85;
sfr DPS   = 0x86;

sfr CKCON = 0x8e;//wd1,wd0,t2m,t1m,t0m,md2,md1,md0
				//wd1wd0:���Ź�����00=2^17,01=2^20,10=2^23,11=2^26
				//t2m,t1m,t0m����1ʱʱ��4��Ƶ
				//md2,md1,md0 MOVXִ�л�������000=2��001=3������111=9
sfr EXIF  = 0x91;//ie5,ie4,ie3,ie2,XT/RG,RGMD,RGSL,-
				//ie5,ie3=1���ж��������жϱ�־
				//ie4,ie3=2���ж��������жϱ�־
sfr P4    = 0xa5;//����λ��Ч
sfr SADDR = 0xa9;//�ӻ�����0��ַ
sfr SADDR1= 0xaa;//�ӻ�����1��ַ
sfr SADEN = 0xb9;//����0��ַ���Σ�����0ʱ���е�ַ���������ж�
sfr SADEN1= 0xba;//����0��ַ���Σ�����0ʱ���е�ַ���������ж�
sfr SCON1 = 0xc0;
sfr SBUF1 = 0xc1;
sfr ROMMAP= 0xc2;//ROMMAP.7Ϊ�ȴ��ź�ʹ�ܣ���movxָ��ʱ��wait��Ϊp4.0
sfr PMR   = 0xc4;//CD1,CD0,SWB,-,XTOFF,ALE-OFF,-DME0
                //cd0cd1=0ʱ�Ӳ��䣬=1-1/4��=2-1/64��=3=1/1024
                //swb=1ǿ��4��Ƶ�����жϻ򴮿��жϻ���
                //aleoff =1ʱale�ź���ֹ���ⲿ�ڴ����ʱ�Զ�����
                //dme0 =1ʱ�ڲ�1kramʹ��
sfr STATUS= 0xc5;//-,HIP,LIP,XTUP,SPTA1,SPRA1,SPTA0,SPRA0
                //hip=1���ڴ�������ȼ��ж�
                //lip=1���ڴ�������ȼ��ж�
                //spta1����0���ڷ�������
                //spra1����0���ڽ�������
                //spta0����0���ڷ�������
                //spra0����0���ڽ�������
sfr TA    = 0xc7;//����λ
sfr WDCON = 0xd8;//SMOD_1,POR,-,-,WDIF,WTRF,EWT,RWT
	              //smod1 �ӱ�����1�Ĳ�����,pro��Դ��λ��־
	              //por ��Դ��λ��־
	        //wdif���Ź���ʱ�жϱ�־������
	        //wtrf���Ź���ʱ����λ��־�����Ź�����λ�󣬸�λ��1
	        //ewt �����Ź���ʱ���Զ���λ
	        //rwt��λ���Ź���ʱ����������Ź������û�б���λ�����������������жϣ��ٹ�512���ڽ���λ
sfr EIE   = 0xe8;//-��-��-��EWDI,EX5,EX4,EX3,EX2
				//ewdi=1�������Ź��жϣ
				//ex5-ex2���ⲿ�ж�����
sfr EIP   = 0xf8;//-��-��-��pwdi��px5,px4,px3,px2
				//pwdi=1 ���Ź��ж�����
				//px5-px2=1 �ⲿ�ж�����

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
sbit TF1   = 0x8F;//��ʱ��1�����־���Զ�����
sbit TR1   = 0x8E;//timer1���п���λ
sbit TF0   = 0x8D;//��ʱ��0�����־���Զ�����
sbit TR0   = 0x8C;//timer0���п���λ
sbit IE1   = 0x8B;//���ж�1�����ж������־���Զ�����
sbit IT1   = 0x8A;//�ж�1������ʹ��
sbit IE0   = 0x89;//���ж�0�����ж������־���Զ�����
sbit IT0   = 0x88;//�ж�0������ʹ��

/*  IE		*/
sbit EA    = 0xAF;//
sbit ES    = 0xAC;
sbit ET1   = 0xAB;
sbit EX1   = 0xAA;
sbit ET0   = 0xA9;
sbit EX0   = 0xA8;

/*  IP		*/
sbit PS    = 0xBC;//����0�ж������趨
sbit PT1   = 0xBB;//��ʱ��1�ж�����
sbit PX1   = 0xBA;//���ж�1
sbit PT0   = 0xB9;//��ʱ��0�ж�����
sbit PX0   = 0xB8;//���ж�0

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
sbit SM0   = 0x9F;//SM0/FE  ����0��ʽ0ѡ���֡���־���˹�����
sbit SM1   = 0x9E;//����1������ʽѡ��0-4/12tclkͬ����1-10λ����bps�첽��2-64/32tclk�첽��3-11λ�ɱ�bps�첽
sbit SM2   = 0x9D;//����ʽ2��3�Ķ��ͨѶ����λ����ʽ2��3��=1ʱ���յ��ĵ�9λΪ0ʱ�����������ж�ri��־
	   			//��ʽ1�У�=1ʱֻ���յ���Чֹͣʱ������ri����ʽ0��=1�����������3����
sbit REN   = 0x9C;//������ձ�־
sbit TB8   = 0x9B;//��ʽ23��Ҫ���͵ĵ�9λ��
sbit RB8   = 0x9A;//��ʽ23�н��յ��ĵ�9λ��
sbit TI    = 0x99;//�����жϱ�־
sbit RI    = 0x98;//�����жϱ�־

/*--W77E58 Extensions--------------------------------------------*/
/* PSW		*/
sbit F1    = 0xd1;

/*  IE		*/
sbit ET2   = 0xAD;//��ʱ��2����
sbit ES1   = 0xae;//����1����

/*  IP		*/
sbit PT2   = 0xBD;//��ʱ��2����
sbit PS1   = 0xbe;//����1����

/*  P1		*/
sbit T2    = 0x90;//������2����˿�
sbit T2EX  = 0x91;//���������񴥷�
sbit RXD1  = 0x92;//����1��
sbit TXD1  = 0x93;//����2��
sbit INT2  = 0x94;//���ж�2�������䴥��
sbit INT3  = 0x95;//���ж�3�������䴥��
sbit INT4  = 0x96;//���ж�4�������䴥��
sbit INT5  = 0x97;//���ж�5�������䴥��

             
/*  T2CON	*/
sbit TF2   = 0xCF;//��ʱ��2�����־�������
sbit EXF2  = 0xCE;//��ʱ��2�ⲿ��־����exen2=1��t2ex���Ÿ��������𲶻������ʱ��λ�����塣
sbit RCLK  = 0xCD;//����ʱ�ӱ�־��=1ʱ����0�ö�ʱ��2�����ʱ�ӡ�
sbit TCLK  = 0xCC;//����ʱ�ӱ�־��=1ʱ����0�ö�ʱ��2�����ʱ�ӡ�
sbit EXEN2 = 0xCB;//��ʱ��2�ⲿ�����־��=1ʱ��δ���������ʷ�������t2ex�ŵĸ��������𲶻�
sbit TR2   = 0xCA;//��ʱ��2���п���λ
sbit C_T2  = 0xC9;//����/��ʱѡ��=1ʱ����
sbit CP_RL= 0xC8;//����/���ر�־��=1ʱ,�ⲿ����ʱ��vt2ex�����䷢������
                 //=0ʱ ������ⲿ����ʱ��vt2ex�����䷢������

/*  SCON1	*/  //ע�Ͳο�SCON
sbit SM0_1 = 0xC7;
sbit SM1_1 = 0xC6;
sbit SM2_1 = 0xC5;
sbit REN_1 = 0xC4;
sbit TB8_1 = 0xC3;
sbit RB8_1 = 0xC2;
sbit TI_1  = 0xC1;
sbit RI_1  = 0xC0;

/* WDCON	*/
sbit SMOD_1= 0xDF; //�ӱ�����1�Ĳ�����,pro��Դ��λ��־
sbit POR   = 0xDE; //��Դ��λ��־
sbit WDIF  = 0XDB; //���Ź���ʱ�жϱ�־������
sbit WTRF  = 0xDA; //���Ź���ʱ����λ��־�����Ź�����λ�󣬸�λ��1
sbit EWT   = 0xD9; // �����Ź���ʱ���Զ���λ
sbit RWT   = 0xD8; //��λ���Ź���ʱ����������Ź������û�б���λ�����������������жϣ��ٹ�512���ڽ���λ


/* EIE		*/
sbit EWDI  = 0xec; //�����Ź��ж�
sbit EX5   = 0xeb; //ex5-ex2���ⲿ�ж�����
sbit EX4   = 0xea; 
sbit EX3   = 0xe9; 
sbit EX2   = 0xe8; 

/* EIP		*/
sbit PWDI  = 0xfc; //pwdi=1 ���Ź��ж�����
sbit PX5   = 0xfb; 	//px5-px2=1 �ⲿ�ж�����
sbit PX4   = 0xfa; 
sbit PX3   = 0xf9; 
sbit PX2   = 0xf8; 
