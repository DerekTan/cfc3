//W77E58Í·ÎÄ¼þ;ÕûÀí£ºÄôÐ¡ÃÍ¡£¸Ã×ÊÁÏÀ´×Ô¡°51µ¥Æ¬»úÊÀ½ç¡±http://go.163.com/~dz2000,»¶Ó­·ÃÎÊ¡£
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
sfr PCON  = 0x87;//PCON.7(SMOD)²¦ÌØÂÊ¼Ó±¶£¬PCON.1(PD)µôµç·½Ê½£¬PCON.0(IDL)¶³½á·½Ê½
				//PCON.6(SMOD0)Ö¡´í¼ì²âÔÊÐí£¬PCON.3(GF1)PCON.2(GF0)
sfr TCON  = 0x88;//¶¨Ê±¿ØÖÆ¼Ä´æÆ÷
sfr TMOD  = 0x89;//"gate,c/t,m1,m0"x2¶¨Ê±Æ÷·½Ê½GATE=1Ê±Ö»ÓÐintx=1Ê±²Å¿ÉÒÔ¿ª·Å¶¨Ê±Æ÷x£»
	              //c/t =1Ê±¼ÆÊéÊý·½Ê½£¬=0Ê±¶¨Ê±·½Ê½¡£m1m0=00Ê±13Î»¼ÆÊý£¬=01Ê±16Î»=10Ê±×Ô×°Èë8Î»£¬
	              //11Ê±¶¨Ê±Æ÷0·ÖÁ½¸ö£¬¶¨Ê±Æ÷1Í£Ö¹¡£
sfr TL0   = 0x8A;
sfr TL1   = 0x8B;
sfr TH0   = 0x8C;
sfr TH1   = 0x8D;
sfr IE    = 0xA8;
sfr IP    = 0xB8;//ÖÐ¶ÏÓÅÏÈ¼¶
sfr SCON  = 0x98;//´®¿Ú¿ØÖÆÓë×´Ì¬
sfr SBUF  = 0x99;

/*--W77E58 Extensions---------------------------------------------*/
sfr T2CON = 0xC8;
sfr T2MOD = 0xc9;//HC5,HC4,HC3,HC2,T2CR,-,T2OE,DCEN
	            //hc5-hc2 =1Ê±ÍâÖÐ¶Ï5-2±êÖ¾Ó²¼þ×Ô¶¯Çå³ý¡£
	            //t2cr=1 ²¶»ñÍê³ÉÊ±×Ô¶¯¸´Î»
	            //t2oe=1 ¶¨Ê±Æ÷2Êä³öÔÊÐí
	            //dcen=1 ¼õ¼ÆÊýÔÊÐí£¬½áºÏÍâ²¿ÊäÈët2exÊ¹ÓÃ£¬16Î»×Ô×°ÈëÄ£Ê½
sfr RCAP2L= 0xCA;//ÖØ×°µÄÔ¤ÖÃÊý£¬²¶»ñµÄÊä³ö
sfr RCAP2H= 0xCB;
sfr TL2   = 0xCC;
sfr TH2   = 0xCD;

sfr DPL1  = 0x84;
sfr DPH1  = 0x85;
sfr DPS   = 0x86;

sfr CKCON = 0x8e;//wd1,wd0,t2m,t1m,t0m,md2,md1,md0
				//wd1wd0:¿´ÃÅ¹·¼ÆÊý00=2^17,01=2^20,10=2^23,11=2^26
				//t2m,t1m,t0mµÈÓÚ1Ê±Ê±ÖÓ4·ÖÆµ
				//md2,md1,md0 MOVXÖ´ÐÐ»úÆ÷ÖÜÆÚ000=2£¬001=3¡£¡£¡£111=9
sfr EXIF  = 0x91;//ie5,ie4,ie3,ie2,XT/RG,RGMD,RGSL,-
				//ie5,ie3=1ÍâÖÐ¶ÏÏÂÌø±äÖÐ¶Ï±êÖ¾
				//ie4,ie3=2ÍâÖÐ¶ÏÉÏÌø±äÖÐ¶Ï±êÖ¾
sfr P4    = 0xa5;//µÍËÄÎ»ÓÐÐ§
sfr SADDR = 0xa9;//´Ó»ú´®¿Ú0µØÖ·
sfr SADDR1= 0xaa;//´Ó»ú´®¿Ú1µØÖ·
sfr SADEN = 0xb9;//´®¿Ú0µØÖ·ÆÁ±Î£¬µÈÓÚ0Ê±ËùÓÐµØÖ·¶¼»áÒýÆðÖÐ¶Ï
sfr SADEN1= 0xba;//´®¿Ú0µØÖ·ÆÁ±Î£¬µÈÓÚ0Ê±ËùÓÐµØÖ·¶¼»áÒýÆðÖÐ¶Ï
sfr SCON1 = 0xc0;
sfr SBUF1 = 0xc1;
sfr ROMMAP= 0xc2;//ROMMAP.7ÎªµÈ´ýÐÅºÅÊ¹ÄÜ£¬ÓÃmovxÖ¸ÁîÊ±£¬wait½ÅÎªp4.0
sfr PMR   = 0xc4;//CD1,CD0,SWB,-,XTOFF,ALE-OFF,-DME0
                //cd0cd1=0Ê±ÖÓ²»±ä£¬=1-1/4£¬=2-1/64£¬=3=1/1024
                //swb=1Ç¿ÖÆ4·ÖÆµ£¬ÍâÖÐ¶Ï»ò´®¿ÚÖÐ¶Ï»½ÐÑ
                //aleoff =1Ê±aleÐÅºÅÖÕÖ¹£¬Íâ²¿ÄÚ´æ·ÃÎÊÊ±×Ô¶¯»½ÐÑ
                //dme0 =1Ê±ÄÚ²¿1kramÊ¹ÄÜ
sfr STATUS= 0xc5;//-,HIP,LIP,XTUP,SPTA1,SPRA1,SPTA0,SPRA0
                //hip=1ÕýÔÚ´¦Àí¸ßÓÅÏÈ¼¶ÖÐ¶Ï
                //lip=1ÕýÔÚ´¦ÀíµÍÓÅÏÈ¼¶ÖÐ¶Ï
                //spta1´®¿Ú0ÕýÔÚ·¢ËÍÊý¾Ý
                //spra1´®¿Ú0ÕýÔÚ½ÓÊÕÊý¾Ý
                //spta0´®¿Ú0ÕýÔÚ·¢ËÍÊý¾Ý
                //spra0´®¿Ú0ÕýÔÚ½ÓÊÕÊý¾Ý
sfr TA    = 0xc7;//±£»¤Î»
sfr WDCON = 0xd8;//SMOD_1,POR,-,-,WDIF,WTRF,EWT,RWT
	              //smod1 ¼Ó±¶´®¿Ú1µÄ²¦ÌØÂÊ,proµçÔ´¸´Î»±êÖ¾
	              //por µçÔ´¸´Î»±êÖ¾
	        //wdif¿´ÃÅ¹·¶¨Ê±ÖÐ¶Ï±êÖ¾£¬ÈíÇå
	        //wtrf¿´ÃÅ¹·¶¨Ê±Æ÷¸´Î»±êÖ¾£¬¿´ÃÅ¹·ÒýÆð¸´Î»ºó£¬¸ÃÎ»ÖÃ1
	        //ewt ÔÊÐí¿´ÃÅ¹·¶¨Ê±Æ÷×Ô¶¯¸´Î»
	        //rwt¸´Î»¿´ÃÅ¹·¶¨Ê±Æ÷£¬Èç¹û¿´ÃÅ¹·Òç³ö»¹Ã»ÓÐ±»¸´Î»¼ÆÊýÆ÷£¬½«»áÒýÆðÖÐ¶Ï£¬ÔÙ¹ý512ÖÜÆÚ½«¸´Î»
sfr EIE   = 0xe8;//-£¬-£¬-£¬EWDI,EX5,EX4,EX3,EX2
				//ewdi=1£¬ÔÊÐí¿´ÃÅ¹·ÖÐ¶Ï£
				//ex5-ex2£»Íâ²¿ÖÐ¶ÏÔÊÐí
sfr EIP   = 0xf8;//-£¬-£¬-£¬pwdi£¬px5,px4,px3,px2
				//pwdi=1 ¿´ÃÅ¹·ÖÐ¶ÏÓÅÏÈ
				//px5-px2=1 Íâ²¿ÖÐ¶ÏÓÅÏÈ

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
sbit TF1   = 0x8F;//¶¨Ê±Æ÷1Òç³ö±êÖ¾£¬×Ô¶¯ÇåÁã
sbit TR1   = 0x8E;//timer1ÔËÐÐ¿ØÖÆÎ»
sbit TF0   = 0x8D;//¶¨Ê±Æ÷0Òç³ö±êÖ¾£¬×Ô¶¯ÇåÁã
sbit TR0   = 0x8C;//timer0ÔËÐÐ¿ØÖÆÎ»
sbit IE1   = 0x8B;//ÍâÖÐ¶Ï1Ìø±äÖÐ¶ÏÇëÇó±êÖ¾£¬×Ô¶¯ÇåÁã
sbit IT1   = 0x8A;//ÖÐ¶Ï1Ìø±ä¼ì²âÊ¹ÄÜ
sbit IE0   = 0x89;//ÍâÖÐ¶Ï0Ìø±äÖÐ¶ÏÇëÇó±êÖ¾£¬×Ô¶¯ÇåÁã
sbit IT0   = 0x88;//ÖÐ¶Ï0Ìø±ä¼ì²âÊ¹ÄÜ

/*  IE		*/
sbit EA    = 0xAF;//
sbit ES    = 0xAC;
sbit ET1   = 0xAB;
sbit EX1   = 0xAA;
sbit ET0   = 0xA9;
sbit EX0   = 0xA8;

/*  IP		*/
sbit PS    = 0xBC;//´®¿Ú0ÖÐ¶ÏÓÅÏÈÉè¶¨
sbit PT1   = 0xBB;//¶¨Ê±Æ÷1ÖÐ¶ÏÓÅÏÈ
sbit PX1   = 0xBA;//ÍâÖÐ¶Ï1
sbit PT0   = 0xB9;//¶¨Ê±Æ÷0ÖÐ¶ÏÓÅÏÈ
sbit PX0   = 0xB8;//ÍâÖÐ¶Ï0

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
sbit SM0   = 0x9F;//SM0/FE  ´®¿Ú0·½Ê½0Ñ¡Ôñ»òÖ¡´í±êÖ¾£¬ÈË¹¤ÇåÁã
sbit SM1   = 0x9E;//´®¿Ú1¹¤×÷·½Ê½Ñ¡Ôñ0-4/12tclkÍ¬²½£¬1-10Î»¿ÉÉèbpsÒì²½£¬2-64/32tclkÒì²½£¬3-11Î»¿É±äbpsÒì²½
sbit SM2   = 0x9D;//ÔÊÐí·½Ê½2ºÍ3µÄ¶à»úÍ¨Ñ¶¿ØÖÆÎ»¡£·½Ê½2¡¢3ÖÐ=1Ê±½ÓÊÕµ½µÄµÚ9Î»Îª0Ê±²»Æô¶¯½ÓÊÕÖÐ¶Ïri±êÖ¾
	   			//·½Ê½1ÖÐ£¬=1Ê±Ö»ÓÐÊÕµ½ÓÐÐ§Í£Ö¹Ê±²ÅÆô¶¯ri£¬·½Ê½0ÖÐ=1½«²¦ÌØÂÊÌá¸ß3±¶¡£
sbit REN   = 0x9C;//ÔÊÐí½ÓÊÕ±êÖ¾
sbit TB8   = 0x9B;//·½Ê½23ÖÐÒª·¢ËÍµÄµÚ9Î»¡£
sbit RB8   = 0x9A;//·½Ê½23ÖÐ½ÓÊÕµ½µÄµÚ9Î»¡£
sbit TI    = 0x99;//·¢ËÍÖÐ¶Ï±êÖ¾
sbit RI    = 0x98;//½ÓÊÕÖÐ¶Ï±êÖ¾

/*--W77E58 Extensions--------------------------------------------*/
/* PSW		*/
sbit F1    = 0xd1;

/*  IE		*/
sbit ET2   = 0xAD;//¶¨Ê±Æ÷2ÔÊÐí
sbit ES1   = 0xae;//´®¿Ú1ÓÅÏÈ

/*  IP		*/
sbit PT2   = 0xBD;//¶¨Ê±Æ÷2ÓÅÏÈ
sbit PS1   = 0xbe;//´®¿Ú1ÓÅÏÈ

/*  P1		*/
sbit T2    = 0x90;//¼ÆÊýÆ÷2ÊäÈë¶Ë¿Ú
sbit T2EX  = 0x91;//¼ÆÊýÆ÷²¶»ñ´¥·¢
sbit RXD1  = 0x92;//´®¿Ú1Èë
sbit TXD1  = 0x93;//´®¿Ú2³ö
sbit INT2  = 0x94;//ÍâÖÐ¶Ï2£¬ÉÏÌø±ä´¥·¢
sbit INT3  = 0x95;//ÍâÖÐ¶Ï3£¬ÏÂÌø±ä´¥·¢
sbit INT4  = 0x96;//ÍâÖÐ¶Ï4£¬ÉÏÌø±ä´¥·¢
sbit INT5  = 0x97;//ÍâÖÐ¶Ï5£¬ÏÂÌø±ä´¥·¢

             
/*  T2CON	*/
sbit TF2   = 0xCF;//¶¨Ê±Æ÷2Òç³ö±êÖ¾£¬ÈíÇå³ý
sbit EXF2  = 0xCE;//¶¨Ê±Æ÷2Íâ²¿±êÖ¾£¬µ±exen2=1ÇÒt2exÒý½Å¸ºÌø±äÒýÆð²¶»ñ»òÖØÔØÊ±ÖÃÎ»£¬ÈíÇå¡£
sbit RCLK  = 0xCD;//½ÓÊÕÊ±ÖÓ±êÖ¾£¬=1Ê±´®¿Ú0ÓÃ¶¨Ê±Æ÷2Òç³ö×öÊ±ÖÓ¡£
sbit TCLK  = 0xCC;//·¢ËÍÊ±ÖÓ±êÖ¾£¬=1Ê±´®¿Ú0ÓÃ¶¨Ê±Æ÷2Òç³ö×öÊ±ÖÓ¡£
sbit EXEN2 = 0xCB;//¶¨Ê±Æ÷2Íâ²¿ÔÊÐí±êÖ¾£¬=1Ê±ÈôÎ´ÓÃ×÷²¦ÌØÂÊ·¢ÉúÆ÷£¬t2ex½ÅµÄ¸ºÌø±äÒýÆð²¶»ñ
sbit TR2   = 0xCA;//¶¨Ê±Æ÷2ÔËÐÐ¿ØÖÆÎ»
sbit C_T2  = 0xC9;//¼ÆÊý/¶¨Ê±Ñ¡Ôñ£¬=1Ê±¼ÆÊý
sbit CP_RL= 0xC8;//²¶»ñ/ÖØÔØ±êÖ¾£¬=1Ê±,Íâ²¿ÔÊÐíÊ±£¬vt2ex¸ºÌø±ä·¢Éú²¶»ñ
                 //=0Ê± Òç³ö»òÍâ²¿ÔÊÐíÊ±£¬vt2ex¸ºÌø±ä·¢ÉúÖØÔØ

/*  SCON1	*/  //×¢ÊÍ²Î¿¼SCON
sbit SM0_1 = 0xC7;
sbit SM1_1 = 0xC6;
sbit SM2_1 = 0xC5;
sbit REN_1 = 0xC4;
sbit TB8_1 = 0xC3;
sbit RB8_1 = 0xC2;
sbit TI_1  = 0xC1;
sbit RI_1  = 0xC0;

/* WDCON	*/
sbit SMOD_1= 0xDF; //¼Ó±¶´®¿Ú1µÄ²¦ÌØÂÊ,proµçÔ´¸´Î»±êÖ¾
sbit POR   = 0xDE; //µçÔ´¸´Î»±êÖ¾
sbit WDIF  = 0XDB; //¿´ÃÅ¹·¶¨Ê±ÖÐ¶Ï±êÖ¾£¬ÈíÇå
sbit WTRF  = 0xDA; //¿´ÃÅ¹·¶¨Ê±Æ÷¸´Î»±êÖ¾£¬¿´ÃÅ¹·ÒýÆð¸´Î»ºó£¬¸ÃÎ»ÖÃ1
sbit EWT   = 0xD9; // ÔÊÐí¿´ÃÅ¹·¶¨Ê±Æ÷×Ô¶¯¸´Î»
sbit RWT   = 0xD8; //¸´Î»¿´ÃÅ¹·¶¨Ê±Æ÷£¬Èç¹û¿´ÃÅ¹·Òç³ö»¹Ã»ÓÐ±»¸´Î»¼ÆÊýÆ÷£¬½«»áÒýÆðÖÐ¶Ï£¬ÔÙ¹ý512ÖÜÆÚ½«¸´Î»


/* EIE		*/
sbit EWDI  = 0xec; //ÔÊÐí¿´ÃÅ¹·ÖÐ¶Ï
sbit EX5   = 0xeb; //ex5-ex2£»Íâ²¿ÖÐ¶ÏÔÊÐí
sbit EX4   = 0xea; 
sbit EX3   = 0xe9; 
sbit EX2   = 0xe8; 

/* EIP		*/
sbit PWDI  = 0xfc; //pwdi=1 ¿´ÃÅ¹·ÖÐ¶ÏÓÅÏÈ
sbit PX5   = 0xfb; 	//px5-px2=1 Íâ²¿ÖÐ¶ÏÓÅÏÈ
sbit PX4   = 0xfa; 
sbit PX3   = 0xf9; 
sbit PX2   = 0xf8; 
