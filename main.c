#include <stc15.h>

#define DT P3_2
#define CLK P3_0
#define SW P3_1
#define buzzer P3_3

#define C_DisplayDefaultDigits_U8            0xffu
#define util_Hex2Ascii(Hex)  (((Hex)>0x09) ? ((Hex) + 0x37): ((Hex) + 0x30))
#define abs(x)        (((x) < 0) ? -(x) : (x))
#define util_GetMod32(dividend,divisor) (int)(dividend - (divisor * (int)(dividend/divisor)))
#define C_MaxDigitsToDisplay_U8              10u
  
int turn = 0;
unsigned char bulletSpeed = 15;

int shipPosX = 0;
int EnemyPosX = 10000;
signed char EnemyMovX = 0;
signed char shipMovX = 0;
unsigned char shipSize = 200;
unsigned char hit = 0;
unsigned char fired = 0;
unsigned char neg = 0;

int xBulletMov, yBulletMov;
int xBulletPos, yBulletPos;
unsigned char  a;
unsigned char d = 0;
unsigned char menu = 0;
unsigned char hold = 0;
int pos = 0;

unsigned char __code degrees[91]={90 ,89 ,88 ,87 ,86 ,85 ,84 ,83 ,82 ,81 ,80 ,79 ,78 ,77 ,76 ,75 ,74 ,73 ,72 ,71 ,70 ,69 ,68 ,67 ,66 ,65 ,64 ,63 ,62 ,61 ,60 ,59 ,58 ,57 ,56 ,55 ,54 ,53 ,52 ,51 ,50 ,49 ,48 ,47 ,46 ,45 ,44 ,43 ,42 ,41 ,40 ,39 ,38 ,37 ,36 ,35 ,34 ,33 ,32 ,31 ,30 ,29 ,28 ,27 ,26 ,25 ,24 ,23 ,22 ,21 ,20 ,19 ,18 ,17 ,16 ,15 ,14 ,13 ,12 ,11 ,10 ,9 ,8 ,7 ,6 ,5 ,4 ,3 ,2 ,1 ,0};
unsigned char __code sinD[91] =  {0 ,4,8 ,13,17,22,26,31,35,39,44 ,48 ,53 ,57 ,61 ,65 ,70 ,74 ,78 ,83 ,87 ,91 ,95 ,99 ,103 ,107,111 ,115 ,119 ,123 ,127 ,131 ,135 ,138 ,142 ,146 ,149 ,153 ,156 ,160 ,163 ,167 ,170 ,173 ,177 ,180 ,183 ,186 ,189 ,192 ,195 ,198 ,200 ,203 ,206 ,208 ,211 ,213 ,216 ,218 ,220 ,223 ,225 ,227 ,229 ,231 ,232 ,234 ,236 ,238 ,239 ,241 ,242 ,243 ,245 ,246 ,247 ,248 ,249 ,250 ,251 ,251 ,252 ,253 ,253 ,254 ,254 ,254 ,254 ,254,255};

 
//int cosD[7] = { 0, 13, 25, 35, 43, 48, 50}; 

void delay(unsigned char time)
{
	unsigned char h,a;
	for(h = 0; h < time; h++)
	{
		for(a = 0; a < 120; a++);
	}
}

unsigned char OledLineNum,OledCursorPos;

unsigned char __code const OledFontTable[][5]=
{
        {0x00, 0x00, 0x00, 0x00, 0x00},   // space
        {0x00, 0x00, 0x2f, 0x00, 0x00},   // !
        {0x00, 0x07, 0x00, 0x07, 0x00},   // "
        {0x14, 0x7f, 0x14, 0x7f, 0x14},   // #
        {0x24, 0x2a, 0x7f, 0x2a, 0x12},   // $
        {0x23, 0x13, 0x08, 0x64, 0x62},   // %
        {0x36, 0x49, 0x55, 0x22, 0x50},   // &
        {0x00, 0x05, 0x03, 0x00, 0x00},   // '
        {0x00, 0x1c, 0x22, 0x41, 0x00},   // (
        {0x00, 0x41, 0x22, 0x1c, 0x00},   // )
        {0x14, 0x08, 0x3E, 0x08, 0x14},   // *
        {0x08, 0x08, 0x3E, 0x08, 0x08},   // +
        {0x00, 0x00, 0xA0, 0x60, 0x00},   // ,
        {0x08, 0x08, 0x08, 0x08, 0x08},   // -
        {0x00, 0x60, 0x60, 0x00, 0x00},   // .
        {0x20, 0x10, 0x08, 0x04, 0x02},   // /

        {0x3E, 0x51, 0x49, 0x45, 0x3E},   // 0
        {0x00, 0x42, 0x7F, 0x40, 0x00},   // 1
        {0x42, 0x61, 0x51, 0x49, 0x46},   // 2
        {0x21, 0x41, 0x45, 0x4B, 0x31},   // 3
        {0x18, 0x14, 0x12, 0x7F, 0x10},   // 4
        {0x27, 0x45, 0x45, 0x45, 0x39},   // 5
        {0x3C, 0x4A, 0x49, 0x49, 0x30},   // 6
        {0x01, 0x71, 0x09, 0x05, 0x03},   // 7
        {0x36, 0x49, 0x49, 0x49, 0x36},   // 8
        {0x06, 0x49, 0x49, 0x29, 0x1E},   // 9

        {0x00, 0x36, 0x36, 0x00, 0x00},   // :
        {0x00, 0x56, 0x36, 0x00, 0x00},   // ;
        {0x08, 0x14, 0x22, 0x41, 0x00},   // <
        {0x14, 0x14, 0x14, 0x14, 0x14},   // =
        {0x00, 0x41, 0x22, 0x14, 0x08},   // >
        {0x02, 0x01, 0x51, 0x09, 0x06},   // ?
        {0x32, 0x49, 0x59, 0x51, 0x3E},   // @

        {0x7C, 0x12, 0x11, 0x12, 0x7C},   // A
        {0x7F, 0x49, 0x49, 0x49, 0x36},   // B
        {0x3E, 0x41, 0x41, 0x41, 0x22},   // C
        {0x7F, 0x41, 0x41, 0x22, 0x1C},   // D
        {0x7F, 0x49, 0x49, 0x49, 0x41},   // E
        {0x7F, 0x09, 0x09, 0x09, 0x01},   // F
        {0x3E, 0x41, 0x49, 0x49, 0x7A},   // G
        {0x7F, 0x08, 0x08, 0x08, 0x7F},   // H
        {0x00, 0x41, 0x7F, 0x41, 0x00},   // I
        {0x20, 0x40, 0x41, 0x3F, 0x01},   // J
        {0x7F, 0x08, 0x14, 0x22, 0x41},   // K
        {0x7F, 0x40, 0x40, 0x40, 0x40},   // L
        {0x7F, 0x02, 0x0C, 0x02, 0x7F},   // M
        {0x7F, 0x04, 0x08, 0x10, 0x7F},   // N
        {0x3E, 0x41, 0x41, 0x41, 0x3E},   // O
        {0x7F, 0x09, 0x09, 0x09, 0x06},   // P
        {0x3E, 0x41, 0x51, 0x21, 0x5E},   // Q
        {0x7F, 0x09, 0x19, 0x29, 0x46},   // R
        {0x46, 0x49, 0x49, 0x49, 0x31},   // S
        {0x01, 0x01, 0x7F, 0x01, 0x01},   // T
        {0x3F, 0x40, 0x40, 0x40, 0x3F},   // U
        {0x1F, 0x20, 0x40, 0x20, 0x1F},   // V
        {0x3F, 0x40, 0x38, 0x40, 0x3F},   // W
        {0x63, 0x14, 0x08, 0x14, 0x63},   // X
        {0x07, 0x08, 0x70, 0x08, 0x07},   // Y
};


void oledSendByte(unsigned char ch)
{
    signed char i;		 

    P5_5 = 0;
    for ( i = 7; i>=0; i--)
    {
        P5_4 = (ch >> (i)) & 0x1; 
        P5_5 = 1; 
        P5_5 = 0;  
    }
    P5_4 = 1;
    P5_5 = 1;
    while(P5_4==1);
    P5_5 = 0;
}

void oledSendStart(unsigned char address)
{
    P5_4 = 0;
    P5_4 = 1;
    P5_5 = 1;
    P5_4 = 0;
    P5_5 = 0;
    oledSendByte(address);
}

void oledSendCommand(unsigned char cmd)
{
    oledSendStart(0x3C<<1);
    oledSendByte(0x00);
    oledSendByte(cmd);
    P5_4 = 0;
    P5_5 = 1;
    P5_4 = 0;	
    P5_4 = 1;
}

void OLED_SetCursor(unsigned char lineNumber,unsigned char cursorPosition)
{
	OledLineNum=lineNumber;   /* Save the specified line number */
	OledCursorPos=cursorPosition; /* Save the specified cursor position */

	oledSendCommand(0x21);
	oledSendCommand(cursorPosition);
	oledSendCommand(127);

	oledSendCommand(0x22);
	oledSendCommand(lineNumber);
	oledSendCommand(7);

	oledSendStart(0x3C<<1);
	oledSendByte(0x40);
}

void enter()
{
    /*Increment the current line number.
      In case it exceeds the limit, rool it back to first line */
    OledLineNum++;
    OledLineNum = OledLineNum&0x07;
    OLED_SetCursor(OledLineNum,0); /* Finally move it to next line */
}


void OLED_DisplayChar(unsigned char ch)
{
    unsigned char dat,i=0;

    if(((OledCursorPos+5)>=128))
    {
        /* If the cursor has reached to end of line on page1
        OR NewLine command is issued Then Move the cursor to next line */
        enter();
    }
	ch = ch-0x20; // As the lookup table starts from Space(0x20)

	while(1)
	{
		dat= OledFontTable[ch][i]; /* Get the data to be displayed for LookUptable*/


		oledSendByte(dat); /* Display the data and keep track of cursor */
		OledCursorPos++;

		i++;

		if(i==5) /* Exit the loop if End of char is encountered */
		{
			oledSendByte(0x00); /* Display the data and keep track of cursor */
			OledCursorPos++;
			break;
		}
	}
}


void OLED_DisplayNumber(unsigned int v_number_u32, unsigned char v_numOfDigitsToDisplay_u8)
{
    unsigned char i=0,a[10];

    if(v_number_u32==0)
    {
        /* If the number is zero then update the array with the same for displaying */
        for(i=0;((i<v_numOfDigitsToDisplay_u8) && (i<C_MaxDigitsToDisplay_U8));i++)
            OLED_DisplayChar('0');
    }
    else
    {
        for(i=0;i<v_numOfDigitsToDisplay_u8;i++)
        {
            /* Continue extracting the digits from right side
               till the Specified v_numOfDigitsToDisplay_u8 */
            if(v_number_u32!=0)
            {
                /* Extract the digits from the number till it becomes zero.
                First get the remainder and divide the number by TypeOfNum(10-Dec, 16-Hex) each time.

                example for Decimal number:
                If v_number_u32 = 123 then extracted remainder will be 3 and number will be 12.
                The process continues till it becomes zero or max digits reached*/
                a[i]=util_GetMod32(v_number_u32,10);
                v_number_u32=v_number_u32/10;
            }
            else if( (v_numOfDigitsToDisplay_u8 == C_DisplayDefaultDigits_U8) ||
                    (v_numOfDigitsToDisplay_u8 > C_MaxDigitsToDisplay_U8))
            {
                /* Stop the iteration if the Max number of digits are reached or
                 the user expects exact(Default) digits in the number to be displayed */
                break;
            }
            else
            {
                /* In case user expects more digits to be displayed than the actual digits in number,
                  then update the remaining digits with zero.
               Ex: v_num_u32 is 123 and user wants five digits then 00123 has to be displayed */
                a[i]=0;
            }
        }

        while(i!=0)
        {
            /* Finally get the ascii values of the digits and display*/
            OLED_DisplayChar(util_Hex2Ascii(a[i-1]));
            i--;
        }
    }
}


void OLED_DisplayString(unsigned char *ptr)
{
    while((*ptr) != 0)
        OLED_DisplayChar(*ptr++);
}

void OLED_Clear()
{
    int i;

    oledSendCommand(0x21);
    oledSendCommand(0);
    oledSendCommand(127);

    oledSendCommand(0x22);
    oledSendCommand(0);
    oledSendCommand(7);


    oledSendStart(0x3C<<1);
    oledSendByte(0x40);

    for (i=0; i<1024; i++)      // Write Zeros to clear the display
    {
        oledSendByte(0);
    }

    oledSendCommand(0x21);
    oledSendCommand(0);
    oledSendCommand(127);

    oledSendCommand(0x22);
    oledSendCommand(0);
    oledSendCommand(7);	  

    oledSendStart(0x3C<<1);
    oledSendByte(0x40);
}


void OLED_Init(void)
{ 
    oledSendCommand(0xAE);
    oledSendCommand(0xD5);
    oledSendCommand(0x80);
    oledSendCommand(0xA8);
    oledSendCommand(0x3F);
    oledSendCommand(0xD3);
    oledSendCommand(0x0);
    oledSendCommand(0x40);
    oledSendCommand(0x8D);
    oledSendCommand(0x14);
    oledSendCommand(0x20);
    oledSendCommand(0x00);
    oledSendCommand(0xA1);
    oledSendCommand(0xC8);
    oledSendCommand(0xDA);
    oledSendCommand(0x12);
    oledSendCommand(0x81);
    oledSendCommand(0xCF);
    oledSendCommand(0xD9);
    oledSendCommand(0xF1);
    oledSendCommand(0xD8);
    oledSendCommand(0x40);
    oledSendCommand(0xA4);
    oledSendCommand(0xA6);
    oledSendCommand(0xAF);

    OLED_Clear();  /* Clear the complete LCD during init */
}

void returnBegin()
{
     /* In case it exceeds the limit, rool it back to first line */
    OLED_SetCursor(OledLineNum,0); /* Finally move it to next line */
}

/**************************************************************************
 * ************************************************************************
 * ************                                           *****************
 * ************                                           *****************
 * ************                                           *****************
 * ************                                           *****************
 * ************             END OF LIBRARY                *****************
 * ************                                           *****************
 * ************                                           *****************
 * ************                                           *****************
 * ************                                           *****************
 * ************************************************************************
 * ************************************************************************
 * ************************************************************************
 * ************************************************************************
 * ************************************************************************
 * ************************************************************************
 * ************************************************************************
 * ************************************************************************
 * ************************************************************************
 * ************************************************************************
 * ************************************************************************
 * ************************************************************************
 * ************************************************************************
 * ************************************************************************ 
 * ************************************************************************/



void exint4() __interrupt 16
{
	if(menu == 0)
		a = 1;
	delay(3);
	if (!CLK)
	{
		if (DT == 0)
		{
			if(turn == 30000)
			{
				turn = -30000;
				return;
			}
			turn++;
		}
		
		if(DT == 1)
		{
			if(turn == -30000)
			{
				turn = 30000;
				return;
			}
			turn--;	
		}
	}
}
 
void intTimer0() __interrupt 1
{
	//OLED_DisplayString("FIRED");
	TR0 = 0;
	TL0 = 0x00;
	TH0 = 0x10;
	TR0 = 1;
	d++;
	if(d >= 0x08)
	{
		neg++;
		//~ //OLED_DisplayString("FIRED");
		if(neg == 2)
		{	
			EnemyPosX = EnemyPosX + EnemyMovX;
			if(EnemyPosX >= 32000 || EnemyPosX <= 0)
			{
				EnemyMovX = 0-EnemyMovX;
			}
			shipPosX = shipPosX + shipMovX;
			if(shipPosX >= 32000 || shipPosX <= 0)
			{
				shipMovX = 0-shipMovX;
			}
			neg = 0;
		}
		d = 0;
		if(fired == 1)
		{
			
			if(xBulletPos >= 32765 || yBulletPos >= 32765)
			{
				hit = 3;
				fired = 0;
				return;
			}
			yBulletMov = yBulletMov - 4;
			xBulletPos = xBulletPos + xBulletMov;
			yBulletPos = yBulletPos + yBulletMov;
			//OLED_DisplayString("FIRED");
			if(yBulletPos <= 0)
			{
				//OLED_DisplayString("HITTTTT!!!!!!!!!!!!!!!!!!");
				//enter();
				if(abs((xBulletPos - EnemyPosX)) < shipSize)
				{
					hit = 2;
				}
				else
				{
					hit = 1;
				}
				fired = 0;
				pos = xBulletPos - EnemyPosX;
				//yBulletMov = 0;
				//return;
			}
		}
		else
		{
			a = 0;
		}
	}	
}

void displayTurn(int max,int min)
{
	if(turn > max)
		turn = min;
	if(turn < min)
		turn = max;
	if(turn < 0)
	{
		OLED_DisplayChar('-');
		OLED_DisplayNumber(0-turn, 2);
	}
	else
		OLED_DisplayNumber(turn, 3);
	returnBegin();
}

void fire(signed char XDeg)
{
	signed char direction = (XDeg/abs(XDeg));
	unsigned i;
	xBulletPos = 0;
	yBulletPos = 0;
	/*** z Plane ***/
	if(XDeg == 0)
	{
		yBulletMov = 255;
		xBulletMov = 0;
	}
	else if(XDeg == 90)
	{
		yBulletMov = 0;
		xBulletMov = 255;
	}
	else
	{
		for(i = 0; degrees[i] != abs(XDeg); i++);
		xBulletMov = shipMovX+(sinD[90-i] * direction);
		yBulletMov = sinD[i];
	}
	fired = 1;
}

void writeNeg(int num, unsigned char width)
{
	if(num < 0)
	{
		OLED_DisplayChar('-');
		OLED_DisplayNumber(0-num, width);
	}
	else
		OLED_DisplayNumber(num, width+1);
}

void buzz(unsigned char tone)
{
	buzzer = 1;
	delay(tone*1);
	buzzer = 0;
	delay(tone*1);
}

void main()
{
	int count = 0;
	buzzer = 0;
	P3M1 = 0x00;
	P3M0 = 0xFF;
	INT_CLKO = 0x40;
	EA = 1;
	TL0 = 0x00;
	TH0 = 0x10;
	TMOD = 0x01;
	TR0 = 1;
	ET0 = 1;
	T2H = 0x00;
	T2L = 0x00;
	//IE2 = 0x04;
	//AUXR = 0x20;
	OLED_Init();
	OLED_SetCursor(0,0);
	OLED_DisplayString("WELCOME TO BATTLESHIP");
	enter();
	OLED_DisplayString("PRESS ENTER TO CONTINUE");
	while(SW == 1);
	while(SW == 0);
	while(1)
	{
		delay(255);
		OLED_SetCursor(0,0);
		if(menu == 0)
		{
			OLED_DisplayString("     WIND: ");
			writeNeg(-0, 1);
			OLED_DisplayString("                             ");
			OLED_DisplayString("E: ");
			writeNeg(EnemyPosX, 5);
			OLED_DisplayString("  P: ");
			writeNeg(shipPosX, 5);
			OLED_DisplayString("                      ");
			OLED_DisplayString("ES: ");
			writeNeg(EnemyMovX, 1);
			OLED_DisplayString("     PS: ");
			writeNeg(shipMovX, 1);
			OLED_DisplayString("                    ");
			OLED_DisplayString("     BX: ");
			writeNeg(xBulletPos, 5);
			OLED_DisplayString(" BY: ");
			writeNeg(yBulletPos, 5);
		}
		if(menu == 1)
		{
			OLED_DisplayString("HOW MANY DEGREES?   ");
			enter();
			displayTurn(90,-90);
			enter();
			OLED_DisplayString("                                                                                                                        ");
			
		}
		if(menu == 2)
		{
			OLED_DisplayString("SHIP SPEED?         ");
			enter();
			displayTurn(10,-10);
			enter();
			OLED_DisplayString("                                                                                                                        ");
		}
		if(menu == 3)
		{
			OLED_DisplayString("ORDERS?             ");
			enter();
			OLED_DisplayString("1. PAUSE");
			enter();
			OLED_DisplayString("                                                                                                                        ");
		}
		if(menu == 4)
		{
			OLED_DisplayString("PAUSED. PRESS TO RESUME");
			enter();
			OLED_DisplayString("                                                                                                                        ");
		}
		if(menu == 5)
		{
			OLED_DisplayString("YOU HAVE MISSED!");
			xBulletPos = 0;
			yBulletPos = 0;
			enter();
			writeNeg(pos, 6);
			enter();
			OLED_DisplayString("                                                                                                                        ");
		}
		if(menu == 6)
		{
			OLED_DisplayString("YOU HAVE HIT! NEXT LEVEL!");
			enter();
			OLED_DisplayString("                                                                                                                        ");
		}
		if(menu == 7)
		{
			OLED_DisplayString("YOU HAVE SHOT OUT OF BOUNDS");
			enter();
			OLED_DisplayString("                                                                                                                        ");
		}
		if(a == 1)
		{
			menu = 1;
			a = 0;
		}
		if(menu == 0)
		{
			if(SW == 0)
			{
				count++;
			}
			else
			{
				if(count > 10)
				{
					menu = 3;
					count = 0;
				}
				else if(count > 0)
				{
					menu = 2;
					count = 0;
				}
			}
		}
		if(menu == 1)
		{
			if(SW == 0)
			{
				count++;
			}
			else if(count > 0)
			{
				menu = 0;
				count = 0;
				fire(turn);
			}
		}
		if(menu == 2)
		{
			if(SW == 0)
			{
				count++;
			}
			else if(count > 0)
			{
				shipMovX = turn;
				menu = 0;
				count = 0;
			}
		}
		if(menu == 3)
		{
			if(SW == 0)
			{
				count++;
			}
			else if(count > 0)
			{
				EA = 0;
				TR0 = 0;
				AUXR = 0x00;
				count = 0;
				menu = 4;
				count = 0;
			}
		}
		if(menu == 4)
		{
			if(SW == 0)
			{
				count++;
			}
			else if(count > 0)
			{
				EA = 1;
				TR0 = 1;
				AUXR = 0x20;
				count = 0;
				menu = 0;
				count = 0;
			}
		}
		if(menu == 5)
		{
			if(SW == 0)
			{
				count++;
			}
			else if(count > 0)
			{
				menu = 0;
				count = 0;
			}
		}
		if(menu == 6)
		{
			if(SW == 0)
			{
				count++;
			}
			else if(count > 0)
			{
				shipPosX = 0;
				shipMovX = 0;
				EnemyMovX = EnemyMovX + 3;
				EnemyPosX = 0;
				menu = 0;
				count = 0;
			}
		}
		if(menu == 7)
		{
			if(SW == 0)
			{
				count++;
			}
			else if(count > 0)
			{
				menu = 0;
				count = 0;
			}
		}
		if(hit == 1)
		{
			menu = 5;
			hit = 0;
			//fired = 0;
		}
		if(hit == 2)
		{
			menu = 6;
			hit = 0;
			//fired = 0;
		}
		if(hit == 3)
		{
			menu = 7;
			hit = 0;
			//fired = 0;
		}
	}
}
