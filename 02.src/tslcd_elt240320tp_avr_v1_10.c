#include "app_config.h"
//  _____________________
// | _______________	 |
// ||<-- origin in software
// ||	---------	|[]	 |
// ||		/		|[]	 |
// || 	-------->	|[]	 |
// ||_______________|[]	 |
// |<-- LCD's origin (0x0000)
//
//define TS_ORN_PORTRAIT when your LCD is installed in vertical
//  ________________
// |<-- LCD's origin (0x0000) = origin in software
// ||--------------||
// ||			   ||
// ||	------     ||
// ||		/	   ||
// ||	  /		   ||
// ||	/		   ||
// ||	------>	   ||
// ||______________||
// | [] [] [] [] []	|
// |________________|
//

/*
#ifdef TS_ORN_PORTRAIT
#define TS_SIZE_X					240
#define TS_SIZE_Y					320
#define TS_VAL_ENTRY_MOD			0x0030
#define TS_INS_GRAM_ADX				TS_INS_GRAM_HOR_AD
#define TS_INS_GRAM_ADY				TS_INS_GRAM_VER_AD
#define TS_INS_START_ADX   			TS_INS_HOR_START_AD
#define TS_INS_END_ADX   			TS_INS_HOR_END_AD
#define TS_INS_START_ADY   			TS_INS_VER_START_AD
#define TS_INS_END_ADY   			TS_INS_VER_END_AD
#else
#define TS_SIZE_X					320
#define TS_SIZE_Y					240
#define TS_VAL_ENTRY_MOD			0x0028
#define TS_INS_GRAM_ADX				TS_INS_GRAM_VER_AD
#define TS_INS_GRAM_ADY				TS_INS_GRAM_HOR_AD
#define TS_INS_START_ADX   			TS_INS_VER_START_AD
#define TS_INS_END_ADX   			TS_INS_VER_END_AD
#define TS_INS_START_ADY   			TS_INS_HOR_START_AD
#define TS_INS_END_ADY   			TS_INS_HOR_END_AD
#endif
*/

//extern void delay_1ms(unsigned int n);

void TSLCDCharDisp(char charactor,ts_pos_t sx,ts_pos_t sy,ts_mode_t mode); //low level function to print a character on LCD

#include "font_courier.h"

#define FONT_BIT_WIDTH 8
#define FONT_BYTE_HIGHT 2
#define FONT_SIZE FONT_BIT_WIDTH*FONT_BYTE_HIGHT

#define FONT_WIDTH FONT_BIT_WIDTH
#define FONT_HEIGHT FONT_BYTE_HIGHT*8

unsigned char char_buf[FONT_BIT_WIDTH][FONT_BYTE_HIGHT];

void buf_store(unsigned char charactor);

unsigned short font_color;
unsigned short back_color;
ts_pos_t offsetx,offsety;
ts_pos_t ts_margin_xl = 0;
ts_pos_t ts_margin_xr = TS_SIZE_X - 1;
ts_pos_t ts_margin_yu = 0;
ts_pos_t ts_margin_yl = TS_SIZE_Y - 1;

#define TSLCDGetMarginXl()			ts_margin_xl
#define TSLCDGetMarginXr()			ts_margin_xr
#define TSLCDGetMarginYu()			ts_margin_yu
#define TSLCDGetMarginYl()			ts_margin_yl

void TSLCDSetFontColor(unsigned short color) //set text's color
{
	font_color = color;
}

void TSLCDSetBackColor(unsigned short color) //set back color for TS_MODE_FULL
{
	back_color = color;
}

void TSLCDOutDat(unsigned short dat) //write data to LCD
{
	//TS_XDAT = dat >> 8; 
	//TS_XDAT = dat;
	Setb(LCD_RS_PRTS,LCD_RS_PIN);

	Setb(LCD_RD_PRTS,LCD_RD_PIN);
	Clrb(LCD_WR_PRTC,LCD_WR_PIN);

	LCD_DB_DPRT = 0xFF;
	LCD_DB_PORT = dat >> 8;

	Clrb(LCD_CS_PRTC,LCD_CS_PIN);
//	Clrb(LCD_CS_PRTC,LCD_CS_PIN);
//	Clrb(LCD_CS_PRTC,LCD_CS_PIN);
//	Clrb(LCD_CS_PRTC,LCD_CS_PIN);
//	Clrb(LCD_CS_PRTC,LCD_CS_PIN);
	Setb(LCD_CS_PRTS,LCD_CS_PIN);

	LCD_DB_PORT = dat;

	Clrb(LCD_CS_PRTC,LCD_CS_PIN);
//	Clrb(LCD_CS_PRTC,LCD_CS_PIN);
//	Clrb(LCD_CS_PRTC,LCD_CS_PIN);
//	Clrb(LCD_CS_PRTC,LCD_CS_PIN);
//	Clrb(LCD_CS_PRTC,LCD_CS_PIN);
	Setb(LCD_CS_PRTS,LCD_CS_PIN);

	Setb(LCD_WR_PRTS,LCD_WR_PIN);

	LCD_DB_DPRT = 0;
}

void TSLCDOutDat2(unsigned char dath,unsigned char datl) //write data to LCD
{
	//TS_XDAT = dath; 
	//TS_XDAT = datl;
	Setb(LCD_RS_PRTS,LCD_RS_PIN);

	Setb(LCD_RD_PRTS,LCD_RD_PIN);
	Clrb(LCD_WR_PRTC,LCD_WR_PIN);

	LCD_DB_DPRT = 0xFF;
	LCD_DB_PORT = dath;

	Clrb(LCD_CS_PRTC,LCD_CS_PIN);
//	Clrb(LCD_CS_PRTC,LCD_CS_PIN);
//	Clrb(LCD_CS_PRTC,LCD_CS_PIN);
//	Clrb(LCD_CS_PRTC,LCD_CS_PIN);
//	Clrb(LCD_CS_PRTC,LCD_CS_PIN);
	Setb(LCD_CS_PRTS,LCD_CS_PIN);

	LCD_DB_PORT = datl;

	Clrb(LCD_CS_PRTC,LCD_CS_PIN);
//	Clrb(LCD_CS_PRTC,LCD_CS_PIN);
//	Clrb(LCD_CS_PRTC,LCD_CS_PIN);
//	Clrb(LCD_CS_PRTC,LCD_CS_PIN);
//	Clrb(LCD_CS_PRTC,LCD_CS_PIN);
	Setb(LCD_CS_PRTS,LCD_CS_PIN);

	Setb(LCD_WR_PRTS,LCD_WR_PIN);

	LCD_DB_DPRT = 0;
}

void TSLCDOutIns(unsigned short ins) //write instruction to LCD
{
	//TS_XINS = ins >> 8; 
	//TS_XINS = ins;
	Clrb(LCD_RS_PRTC,LCD_RS_PIN);

	Setb(LCD_RD_PRTS,LCD_RD_PIN);
	Clrb(LCD_WR_PRTC,LCD_WR_PIN);

	LCD_DB_DPRT = 0xFF;
	LCD_DB_PORT = ins >> 8;

	Clrb(LCD_CS_PRTC,LCD_CS_PIN);
//	Clrb(LCD_CS_PRTC,LCD_CS_PIN);
//	Clrb(LCD_CS_PRTC,LCD_CS_PIN);
//	Clrb(LCD_CS_PRTC,LCD_CS_PIN);
//	Clrb(LCD_CS_PRTC,LCD_CS_PIN);
	Setb(LCD_CS_PRTS,LCD_CS_PIN);

	LCD_DB_PORT = ins;

	Clrb(LCD_CS_PRTC,LCD_CS_PIN);
//	Clrb(LCD_CS_PRTC,LCD_CS_PIN);
//	Clrb(LCD_CS_PRTC,LCD_CS_PIN);
//	Clrb(LCD_CS_PRTC,LCD_CS_PIN);
//	Clrb(LCD_CS_PRTC,LCD_CS_PIN);
	Setb(LCD_CS_PRTS,LCD_CS_PIN);

	Setb(LCD_WR_PRTS,LCD_WR_PIN);

	LCD_DB_DPRT = 0;
}

unsigned short TSLCDInDat(void) //read data from LCD
{
//	unsigned short dat = TS_XDAT << 8;
//	dat |= TS_XDAT;
	unsigned short dat = 0;

	LCD_DB_DPRT = 0;

	Setb(LCD_RS_PRTS,LCD_RS_PIN);

	Setb(LCD_WR_PRTS,LCD_WR_PIN);
	Clrb(LCD_RD_PRTC,LCD_RD_PIN);

	Clrb(LCD_CS_PRTC,LCD_CS_PIN);
	Clrb(LCD_CS_PRTC,LCD_CS_PIN);
	dat = LCD_DB_PINP;
	Setb(LCD_CS_PRTS,LCD_CS_PIN);

	dat <<= 8;

	Clrb(LCD_CS_PRTC,LCD_CS_PIN);
	Clrb(LCD_CS_PRTC,LCD_CS_PIN);
	dat |= LCD_DB_PINP;
	Setb(LCD_CS_PRTS,LCD_CS_PIN);

	Setb(LCD_RD_PRTS,LCD_RD_PIN);

	return (dat);
}

unsigned short TSLCDInIns(void) //read data from LCD
{
//	unsigned short ins = TS_XINS << 8;
//	ins |= TS_XINS;
	unsigned short ins = 0;

	LCD_DB_DPRT = 0;

	Clrb(LCD_RS_PRTC,LCD_RS_PIN);

	Setb(LCD_WR_PRTS,LCD_WR_PIN);
	Clrb(LCD_RD_PRTC,LCD_RD_PIN);

	Clrb(LCD_CS_PRTC,LCD_CS_PIN);
	Clrb(LCD_CS_PRTC,LCD_CS_PIN);
	ins = LCD_DB_PINP;
	Setb(LCD_CS_PRTS,LCD_CS_PIN);

	ins <<= 8;

	Clrb(LCD_CS_PRTC,LCD_CS_PIN);
	Clrb(LCD_CS_PRTC,LCD_CS_PIN);
	ins |= LCD_DB_PINP;
	Setb(LCD_CS_PRTS,LCD_CS_PIN);

	Setb(LCD_RD_PRTS,LCD_RD_PIN);

	return (ins);
}

void TSLCDRst(void) //pulse reset signal to LCD
{
	Orb(LCD_RST_DPRT,LCD_RST_PIN);
	Clrb(LCD_RST_PRTC,LCD_RST_PIN);
	delay_1ms(50);
	Setb(LCD_RST_PRTS,LCD_RST_PIN);
}

void TSLCDInit(void) //initial LCD
{
	unsigned short driver_code;
	delay_1ms(100);

	TSLCDOutIns(TS_INS_START_OSC);
	driver_code = TSLCDInDat();
	if (driver_code == 0x9320) //ILI9320
	{
		TSLCDOutIns(0x00E5);
		TSLCDOutDat(0x8000); 					//set the internal vcore voltage
		TSLCDOutIns(TS_INS_START_OSC);
		TSLCDOutDat(0x0001); 					//start oscillator
		delay_1ms(50);

		TSLCDOutIns(TS_INS_DRIV_OUT_CTRL);
		TSLCDOutDat(0x0100); 					//set SS, SM
		TSLCDOutIns(TS_INS_DRIV_WAV_CTRL);
		TSLCDOutDat(0x0700); 					//set 1 line inversion
	
		TSLCDOutIns(TS_INS_ENTRY_MOD);
		TSLCDOutDat(TS_VAL_ENTRY_MOD);			//set GRAM write direction, BGR=0

		TSLCDOutIns(TS_INS_RESIZE_CTRL);
		TSLCDOutDat(0x0000); 					//no resizing

		TSLCDOutIns(TS_INS_DISP_CTRL2);
		TSLCDOutDat(0x0202); 					//front & back porch periods = 2
		TSLCDOutIns(TS_INS_DISP_CTRL3);
		TSLCDOutDat(0x0000); 					
		TSLCDOutIns(TS_INS_DISP_CTRL4);
		TSLCDOutDat(0x0000); 					
		TSLCDOutIns(TS_INS_RGB_DISP_IF_CTRL1);
		TSLCDOutDat(0x0000); 					//select system interface				
		TSLCDOutIns(TS_INS_FRM_MARKER_POS);
		TSLCDOutDat(0x0000); 					
		TSLCDOutIns(TS_INS_RGB_DISP_IF_CTRL2);
		TSLCDOutDat(0x0000);					
	
		TSLCDOutIns(TS_INS_POW_CTRL1);
		TSLCDOutDat(0x0000);
		TSLCDOutIns(TS_INS_POW_CTRL2);
		TSLCDOutDat(0x0000); 					
		TSLCDOutIns(TS_INS_POW_CTRL3);
		TSLCDOutDat(0x0000);
		TSLCDOutIns(TS_INS_POW_CTRL4);
		TSLCDOutDat(0x0000); 					
		delay_1ms(200);

		TSLCDOutIns(TS_INS_POW_CTRL1);
		TSLCDOutDat(0x17B0);
		TSLCDOutIns(TS_INS_POW_CTRL2);
		TSLCDOutDat(0x0137); 					
		delay_1ms(50);

		TSLCDOutIns(TS_INS_POW_CTRL3);
		TSLCDOutDat(0x013C);
		delay_1ms(50);

		TSLCDOutIns(TS_INS_POW_CTRL4);
		TSLCDOutDat(0x1400);
		TSLCDOutIns(TS_INS_POW_CTRL7);
		TSLCDOutDat(0x0007);
		delay_1ms(50);

		TSLCDOutIns(TS_INS_GRAM_HOR_AD);
		TSLCDOutDat(0x0000);
		TSLCDOutIns(TS_INS_GRAM_VER_AD);
		TSLCDOutDat(0x0000);

		TSLCDOutIns(TS_INS_GAMMA_CTRL1);
		TSLCDOutDat(0x0007);
		TSLCDOutIns(TS_INS_GAMMA_CTRL2);
		TSLCDOutDat(0x0504);
		TSLCDOutIns(TS_INS_GAMMA_CTRL3);
		TSLCDOutDat(0x0703);
		TSLCDOutIns(TS_INS_GAMMA_CTRL4);
		TSLCDOutDat(0x0002);
		TSLCDOutIns(TS_INS_GAMMA_CTRL5);
		TSLCDOutDat(0x0707);
		TSLCDOutIns(TS_INS_GAMMA_CTRL6);
		TSLCDOutDat(0x0406);
		TSLCDOutIns(TS_INS_GAMMA_CTRL7);
		TSLCDOutDat(0x0006);
		TSLCDOutIns(TS_INS_GAMMA_CTRL8);
		TSLCDOutDat(0x0404);
		TSLCDOutIns(TS_INS_GAMMA_CTRL9);
		TSLCDOutDat(0x0700);
		TSLCDOutIns(TS_INS_GAMMA_CTRL10);
		TSLCDOutDat(0x0A08);

		TSLCDOutIns(TS_INS_HOR_START_AD);
		TSLCDOutDat(0x0000);
		TSLCDOutIns(TS_INS_HOR_END_AD);
		TSLCDOutDat(0x00EF);
		TSLCDOutIns(TS_INS_VER_START_AD);
		TSLCDOutDat(0x0000);
		TSLCDOutIns(TS_INS_VER_END_AD);
		TSLCDOutDat(0x013F);
		TSLCDOutIns(TS_INS_GATE_SCAN_CTRL1);
		TSLCDOutDat(0x2700);
		TSLCDOutIns(TS_INS_GATE_SCAN_CTRL2);
		TSLCDOutDat(0x0001);
		TSLCDOutIns(TS_INS_GATE_SCAN_CTRL3);
		TSLCDOutDat(0x0000);

		TSLCDOutIns(TS_INS_PART_IMG1_DISP_POS);
		TSLCDOutDat(0x0000);
		TSLCDOutIns(TS_INS_PART_IMG1_START_AD);
		TSLCDOutDat(0x0000);
		TSLCDOutIns(TS_INS_PART_IMG1_END_AD);
		TSLCDOutDat(0x0000);
		TSLCDOutIns(TS_INS_PART_IMG2_DISP_POS);
		TSLCDOutDat(0x0000);
		TSLCDOutIns(TS_INS_PART_IMG2_START_AD);
		TSLCDOutDat(0x0000);
		TSLCDOutIns(TS_INS_PART_IMG2_END_AD);
		TSLCDOutDat(0x0000);

		TSLCDOutIns(TS_INS_PANEL_IF_CTRL1);
		TSLCDOutDat(0x0010);
		TSLCDOutIns(TS_INS_PANEL_IF_CTRL2);
		TSLCDOutDat(0x0000);
		TSLCDOutIns(TS_INS_PANEL_IF_CTRL3);
		TSLCDOutDat(0x0003);
		TSLCDOutIns(TS_INS_PANEL_IF_CTRL4);
		TSLCDOutDat(0x0110);
		TSLCDOutIns(TS_INS_PANEL_IF_CTRL5);
		TSLCDOutDat(0x0000);
		TSLCDOutIns(TS_INS_PANEL_IF_CTRL6);
		TSLCDOutDat(0x0000);

		TSLCDOutIns(TS_INS_DISP_CTRL1);
		TSLCDOutDat(0x0173);
	}
	if (driver_code == 0x9325) //ILI9325
	{
		TSLCDOutIns(0x00E3);
		TSLCDOutDat(0x3008); 					//set the internal timing
		TSLCDOutIns(0x00E7);
		TSLCDOutDat(0x0012); 					//set the internal timing
		TSLCDOutIns(0x00EF);
		TSLCDOutDat(0x1231); 					//set the internal timing
		TSLCDOutIns(TS_INS_START_OSC);
		TSLCDOutDat(0x0001); 					//start oscillator
		delay_1ms(50);

		TSLCDOutIns(TS_INS_DRIV_OUT_CTRL);
		TSLCDOutDat(0x0100); 					//set SS, SM
		TSLCDOutIns(TS_INS_DRIV_WAV_CTRL);
		TSLCDOutDat(0x0700); 					//set 1 line inversion
	
		TSLCDOutIns(TS_INS_ENTRY_MOD);
		TSLCDOutDat(TS_VAL_ENTRY_MOD);			//set GRAM write direction, BGR=0

		TSLCDOutIns(TS_INS_RESIZE_CTRL);
		TSLCDOutDat(0x0000); 					//no resizing

		TSLCDOutIns(TS_INS_DISP_CTRL2);
		TSLCDOutDat(0x0202); 					//front & back porch periods = 2
		TSLCDOutIns(TS_INS_DISP_CTRL3);
		TSLCDOutDat(0x0000); 					
		TSLCDOutIns(TS_INS_DISP_CTRL4);
		TSLCDOutDat(0x0000); 					
		TSLCDOutIns(TS_INS_RGB_DISP_IF_CTRL1);
		TSLCDOutDat(0x0000); 					//select system interface				
		TSLCDOutIns(TS_INS_FRM_MARKER_POS);
		TSLCDOutDat(0x0000); 					
		TSLCDOutIns(TS_INS_RGB_DISP_IF_CTRL2);
		TSLCDOutDat(0x0000);					
	
		TSLCDOutIns(TS_INS_POW_CTRL1);
		TSLCDOutDat(0x0000);
		TSLCDOutIns(TS_INS_POW_CTRL2);
		TSLCDOutDat(0x0007); 					
		TSLCDOutIns(TS_INS_POW_CTRL3);
		TSLCDOutDat(0x0000);
		TSLCDOutIns(TS_INS_POW_CTRL4);
		TSLCDOutDat(0x0000); 					
		delay_1ms(200);

		TSLCDOutIns(TS_INS_POW_CTRL1);
		TSLCDOutDat(0x1690);
		TSLCDOutIns(TS_INS_POW_CTRL2);
		TSLCDOutDat(0x0227); //TSLCDOutDat(0x0137); 					
		delay_1ms(50);

		TSLCDOutIns(TS_INS_POW_CTRL3);
		TSLCDOutDat(0x001A); //TSLCDOutDat(0x013C);
		delay_1ms(50);

		TSLCDOutIns(TS_INS_POW_CTRL4);
		TSLCDOutDat(0x1800); //TSLCDOutDat(0x1400);
		TSLCDOutIns(TS_INS_POW_CTRL7);
		TSLCDOutDat(0x002A); //TSLCDOutDat(0x0007);
		delay_1ms(50);

		TSLCDOutIns(TS_INS_GRAM_HOR_AD);
		TSLCDOutDat(0x0000);
		TSLCDOutIns(TS_INS_GRAM_VER_AD);
		TSLCDOutDat(0x0000);

		TSLCDOutIns(TS_INS_GAMMA_CTRL1);
		TSLCDOutDat(0x0007);
		TSLCDOutIns(TS_INS_GAMMA_CTRL2);
		TSLCDOutDat(0x0605);
		TSLCDOutIns(TS_INS_GAMMA_CTRL3);
		TSLCDOutDat(0x0106);
		TSLCDOutIns(TS_INS_GAMMA_CTRL4);
		TSLCDOutDat(0x0206);
		TSLCDOutIns(TS_INS_GAMMA_CTRL5);
		TSLCDOutDat(0x0808);
		TSLCDOutIns(TS_INS_GAMMA_CTRL6);
		TSLCDOutDat(0x0007);
		TSLCDOutIns(TS_INS_GAMMA_CTRL7);
		TSLCDOutDat(0x0201);
		TSLCDOutIns(TS_INS_GAMMA_CTRL8);
		TSLCDOutDat(0x0007);
		TSLCDOutIns(TS_INS_GAMMA_CTRL9);
		TSLCDOutDat(0x0602);
		TSLCDOutIns(TS_INS_GAMMA_CTRL10);
		TSLCDOutDat(0x0808);

		TSLCDOutIns(TS_INS_HOR_START_AD);
		TSLCDOutDat(0x0000);
		TSLCDOutIns(TS_INS_HOR_END_AD);
		TSLCDOutDat(0x00EF);
		TSLCDOutIns(TS_INS_VER_START_AD);
		TSLCDOutDat(0x0000);
		TSLCDOutIns(TS_INS_VER_END_AD);
		TSLCDOutDat(0x013F);
		TSLCDOutIns(TS_INS_GATE_SCAN_CTRL1);
		TSLCDOutDat(0xA700);
		TSLCDOutIns(TS_INS_GATE_SCAN_CTRL2);
		TSLCDOutDat(0x0001);
		TSLCDOutIns(TS_INS_GATE_SCAN_CTRL3);
		TSLCDOutDat(0x0000);

		TSLCDOutIns(TS_INS_PART_IMG1_DISP_POS);
		TSLCDOutDat(0x0000);
		TSLCDOutIns(TS_INS_PART_IMG1_START_AD);
		TSLCDOutDat(0x0000);
		TSLCDOutIns(TS_INS_PART_IMG1_END_AD);
		TSLCDOutDat(0x0000);
		TSLCDOutIns(TS_INS_PART_IMG2_DISP_POS);
		TSLCDOutDat(0x0000);
		TSLCDOutIns(TS_INS_PART_IMG2_START_AD);
		TSLCDOutDat(0x0000);
		TSLCDOutIns(TS_INS_PART_IMG2_END_AD);
		TSLCDOutDat(0x0000);

		TSLCDOutIns(TS_INS_PANEL_IF_CTRL1);
		TSLCDOutDat(0x0010);
		TSLCDOutIns(TS_INS_PANEL_IF_CTRL2);
		TSLCDOutDat(0x0000);
		TSLCDOutIns(TS_INS_PANEL_IF_CTRL3);
		TSLCDOutDat(0x0003);
		TSLCDOutIns(TS_INS_PANEL_IF_CTRL4);
		TSLCDOutDat(0x0110);
		TSLCDOutIns(TS_INS_PANEL_IF_CTRL5);
		TSLCDOutDat(0x0000);
		TSLCDOutIns(TS_INS_PANEL_IF_CTRL6);
		TSLCDOutDat(0x0000);

		TSLCDOutIns(TS_INS_DISP_CTRL1);
		TSLCDOutDat(0x0133);
	}
}

void TSLCDShowPic2(ts_pos_t sx,ts_pos_t ex,ts_pos_t sy,ts_pos_t ey,const unsigned short *pic,ts_mode_t mode)
//show picture from code memory with specific size
{
	unsigned long k = 0;
	unsigned short color;
	unsigned int x,y;
	unsigned int i,j;
	if (sx < ts_margin_xl)
		sx = ts_margin_xl;
	if (ex > ts_margin_xr)
		ex = ts_margin_xr;
	if (sy < ts_margin_yu)
		sy = ts_margin_yu;
	if (ey > ts_margin_yl)			 
		ey = ts_margin_yl;

	TSLCDOutIns(TS_INS_START_ADX);
	TSLCDOutDat(sx);
	TSLCDOutIns(TS_INS_END_ADX);
	TSLCDOutDat(ex);
	TSLCDOutIns(TS_INS_GRAM_ADX);
	TSLCDOutDat(sx);
	x = ex - sx + 1;

#ifndef TS_ORN_PORTRAIT
	sy = TS_SIZE_Y - 1 - sy; 	// mirror start y address
	ey = TS_SIZE_Y - 1 - ey; 	// mirror end y address
	TSLCDOutIns(TS_INS_START_ADY);
	TSLCDOutDat(ey);
	TSLCDOutIns(TS_INS_END_ADY);
	TSLCDOutDat(sy);
	TSLCDOutIns(TS_INS_GRAM_ADY);
	TSLCDOutDat(sy);//fix from bug of v1_00
	y = sy - ey + 1;
#else
	TSLCDOutIns(TS_INS_START_ADY);
	TSLCDOutDat(sy);
	TSLCDOutIns(TS_INS_END_ADY);
	TSLCDOutDat(ey);
	TSLCDOutIns(TS_INS_GRAM_ADY);
	TSLCDOutDat(sy);
	y = ey - sy + 1;
#endif

	TSLCDOutIns(TS_INS_RW_GRAM);

	if (mode == TS_MODE_FULL)
	{
		for (j=0; j<y; j++)
			for (i=0; i<x; i++)
			{
				TSLCDOutDat(pgm_read_word(&pic[k]));
				k++;
			}
	}
	else
	if (mode == TS_MODE_NORMAL)
	{
		for (j=0; j<y; j++)
			for (i=0; i<x; i++)
			{
				if (pgm_read_word(&pic[k]) == TS_COL_WHITE)
				{
					color = TSLCDInDat(); 		// ignore invalid data
					color = TSLCDInDat();
					TSLCDOutDat(color);
				}
				else
				{
					TSLCDOutDat(pgm_read_word(&pic[k]));
				}
				k++;
			}
	}
}

void TSLCDFillRect(ts_pos_t sx,ts_pos_t ex,ts_pos_t sy,ts_pos_t ey,unsigned short color,ts_mode_t mode) //draw a rectangular
{
	unsigned int x,y;
	unsigned int i,j;
	if (sx < ts_margin_xl)
		sx = ts_margin_xl;
	if (ex > ts_margin_xr)
		ex = ts_margin_xr;
	if (sy < ts_margin_yu)
		sy = ts_margin_yu;
	if (ey > ts_margin_yl)			 
		ey = ts_margin_yl;

	TSLCDOutIns(TS_INS_START_ADX);
	TSLCDOutDat(sx);
	TSLCDOutIns(TS_INS_END_ADX);
	TSLCDOutDat(ex);
	TSLCDOutIns(TS_INS_GRAM_ADX);
	TSLCDOutDat(sx);
	x = ex - sx + 1;

#ifndef TS_ORN_PORTRAIT
	sy = TS_SIZE_Y - 1 - sy; 	// mirror start y address
	ey = TS_SIZE_Y - 1 - ey; 	// mirror end y address
	TSLCDOutIns(TS_INS_START_ADY);
	TSLCDOutDat(ey);
	TSLCDOutIns(TS_INS_END_ADY);
	TSLCDOutDat(sy);
	TSLCDOutIns(TS_INS_GRAM_ADY);
	TSLCDOutDat(sy);//fix from bug of v1_00
	y = sy - ey + 1;
#else
	TSLCDOutIns(TS_INS_START_ADY);
	TSLCDOutDat(sy);
	TSLCDOutIns(TS_INS_END_ADY);
	TSLCDOutDat(ey);
	TSLCDOutIns(TS_INS_GRAM_ADY);
	TSLCDOutDat(sy);
	y = ey - sy + 1;
#endif

	TSLCDOutIns(TS_INS_RW_GRAM);

	if ((mode == TS_MODE_NORMAL) || (mode == TS_MODE_FULL))
	{
		for (j=0; j<y; j++)
			for (i=0; i<x; i++)
			{
				TSLCDOutDat(color);
			}
	}
	else
	if (mode == TS_MODE_INVERSE)
	{
		for (j=0; j<y; j++)
			for (i=0; i<x; i++)
			{
				color = TSLCDInDat(); 		// ignore invalid data
				color = TSLCDInDat();
				TSLCDOutDat(~color);
			}
	}
}

void TSLCDFillCirc(ts_pos_t cx,ts_pos_t cy,ts_pos_t rad,unsigned short color, ts_mode_t mode) //draw a circle
{
#ifndef TS_ORN_PORTRAIT
	int sy_buf,ey_buf;
#endif
	int sx,sy,ex,ey;
	int i,j;
	unsigned short color_buf;
	unsigned short rad2 = rad*rad;
	sx = cx - rad;
	ex = cx + rad;
	sy = cy - rad;
	ey = cy + rad;

	if (sx < ts_margin_xl)
		sx = ts_margin_xl;
	if (ex > ts_margin_xr)
		ex = ts_margin_xr;
	if (sy < ts_margin_yu)
		sy = ts_margin_yu;
	if (ey > ts_margin_yl)			 
		ey = ts_margin_yl;

	TSLCDOutIns(TS_INS_START_ADX);
	TSLCDOutDat(sx);
	TSLCDOutIns(TS_INS_END_ADX);
	TSLCDOutDat(ex);
	TSLCDOutIns(TS_INS_GRAM_ADX);
	TSLCDOutDat(sx);
//	x = ex - sx + 1;

#ifndef TS_ORN_PORTRAIT
	sy_buf = TS_SIZE_Y - 1 - sy; 	// mirror start y address
	ey_buf = TS_SIZE_Y - 1 - ey; 	// mirror end y address
	TSLCDOutIns(TS_INS_START_ADY);
	TSLCDOutDat(ey_buf);
	TSLCDOutIns(TS_INS_END_ADY);
	TSLCDOutDat(sy_buf);
	TSLCDOutIns(TS_INS_GRAM_ADY);
	TSLCDOutDat(sy_buf);//fix from bug of v1_00
//	y = sy_buf - ey_buf + 1;
#else
	TSLCDOutIns(TS_INS_START_ADY);
	TSLCDOutDat(sy);
	TSLCDOutIns(TS_INS_END_ADY);
	TSLCDOutDat(ey);
	TSLCDOutIns(TS_INS_GRAM_ADY);
	TSLCDOutDat(sy);
//	y = ey - sy + 1;
#endif

	TSLCDOutIns(TS_INS_RW_GRAM);

	if (mode == TS_MODE_NORMAL)
	{
		for (j=sy-cy; j<=ey-cy; j++)
			for (i=sx-cx; i<=ex-cx; i++)
			{
				if ((i)*(i) + (j)*(j) < rad2)
				{
					TSLCDOutDat(color);
				}
				else
				{
					color_buf = TSLCDInDat(); 		// ignore invalid data
					color_buf = TSLCDInDat();
					TSLCDOutDat(color_buf);
				}
			}
	}
	else
	if (mode == TS_MODE_INVERSE)
	{
		for (j=sy-cy; j<=ey-cy; j++)
			for (i=sx-cx; i<=ex-cx; i++)
			{
				if ((i)*(i) + (j)*(j) < rad2)
				{
					color_buf = TSLCDInDat(); 		// ignore invalid data
					color_buf = TSLCDInDat();
					TSLCDOutDat(~color_buf);
				}
				else
				{
					color_buf = TSLCDInDat(); 		// ignore invalid data
					color_buf = TSLCDInDat();
					TSLCDOutDat(color_buf);
				}
			}
	}
	else
	if (mode == TS_MODE_FULL)
	{
		for (j=sy-cy; j<=ey-cy; j++)
			for (i=sx-cx; i<=ex-cx; i++)
			{
				if ((i)*(i) + (j)*(j) < rad2)
				{
					TSLCDOutDat(color);
				}
				else
				{
					TSLCDOutDat(back_color);
				}
			}
	}
}

void TSLCDSetMargins(ts_pos_t xl,ts_pos_t xr,ts_pos_t yu,ts_pos_t yl) //set margins for FillRect,FillCirc
{
	ts_margin_xl = xl;
	ts_margin_xr = xr;
	ts_margin_yu = yu;
	ts_margin_yl = yl;
}

void TSLCDSetMarginsDefault(void) //Reset margins to default value
{
	ts_margin_xl = 0;
	ts_margin_xr = TS_SIZE_X - 1;
	ts_margin_yu = 0;
	ts_margin_yl = TS_SIZE_Y - 1;
}

void buf_store(unsigned char charactor)
{
	unsigned char i,j;
	int char_p = charactor*FONT_SIZE;

	for (i=0; i<FONT_BIT_WIDTH; i++)
		for (j=0; j<FONT_BYTE_HIGHT; j++)
		{
			char_buf[i][j] = pgm_read_byte(&font[char_p]);
			char_p++;
		}
}

unsigned char buf_read(unsigned char column,unsigned char row)
{
	unsigned char read_pixel;

	if (row < 8)
	{
		read_pixel = (char_buf[column][0] >> (7-row)) & 0x01;
	}
	else
	{
		row = row - 8;
		read_pixel = (char_buf[column][1] >> (7-row)) & 0x01;
	}
	return (read_pixel);
}

void TSLCDCharDisp(char charactor,ts_pos_t sx,ts_pos_t sy,ts_mode_t mode) //low level function to print a character on LCD
{
	unsigned int x,y;
	unsigned char i,j;
	ts_pos_t ex,ey;
	unsigned short c;

	ex = sx + FONT_WIDTH - 1;
	ey = sy + FONT_HEIGHT - 1;

	buf_store(charactor - 0x20);

	TSLCDOutIns(TS_INS_START_ADX);
	TSLCDOutDat(sx);
	TSLCDOutIns(TS_INS_END_ADX);
	TSLCDOutDat(ex);
	TSLCDOutIns(TS_INS_GRAM_ADX);
	TSLCDOutDat(sx);
	x = ex - sx + 1;

#ifndef TS_ORN_PORTRAIT
	sy = TS_SIZE_Y - 1 - sy; 	// mirror start y address
	ey = TS_SIZE_Y - 1 - ey; 	// mirror end y address
	TSLCDOutIns(TS_INS_START_ADY);
	TSLCDOutDat(ey);
	TSLCDOutIns(TS_INS_END_ADY);
	TSLCDOutDat(sy);
	TSLCDOutIns(TS_INS_GRAM_ADY);
	TSLCDOutDat(sy);//fix from bug of v1_00
	y = sy - ey + 1;
#else
	TSLCDOutIns(TS_INS_START_ADY);
	TSLCDOutDat(sy);
	TSLCDOutIns(TS_INS_END_ADY);
	TSLCDOutDat(ey);
	TSLCDOutIns(TS_INS_GRAM_ADY);
	TSLCDOutDat(sy);
	y = ey - sy + 1;
#endif

	TSLCDOutIns(TS_INS_RW_GRAM);

	if (mode == TS_MODE_NORMAL)
	{
		for (j=0; j<y; j++)
			for (i=0; i<x; i++)
			{
				if (buf_read(i,j))
				{
					TSLCDOutDat(font_color);
				}
				else
				{
					c = TSLCDInDat(); 		// ignore invalid data
					c = TSLCDInDat();
					TSLCDOutDat(c);
				}
			}
	}
	else
	if (mode == TS_MODE_INVERSE)
	{
		for (j=0; j<y; j++)
			for (i=0; i<x; i++)
			{
				c = TSLCDInDat(); 			// ignore invalid data
				c = TSLCDInDat();
				if (buf_read(i,j))
				{
					TSLCDOutDat(~c);
				}
				else
				{
					TSLCDOutDat(c);
				}
			}
	}
	else
	if (mode == TS_MODE_FULL)
	{
		for (j=0; j<y; j++)
			for (i=0; i<x; i++)
			{
				if (buf_read(i,j))
				{
					TSLCDOutDat(font_color);
				}
				else
				{
					TSLCDOutDat(back_color);
				}
			}
	}
}

void TSLCDSetOffset(ts_pos_t x,ts_pos_t y) //set LCD offset for character display
{
	offsetx = x;
	offsety = y;
}

void TSLCDPrintStr(unsigned char line,unsigned char column,char *str,ts_mode_t mode) //print string on LCD
{
	int i = 0;
	ts_pos_t posx,posy;
	posx = offsetx + column*FONT_WIDTH;
	posy = offsety + line*FONT_HEIGHT;

	while(str[i])
	{
		TSLCDCharDisp(str[i],posx,posy,mode);
		posx += FONT_WIDTH;
		i++;
	}
}

void TSLCDPrintTxt(unsigned char line,unsigned char column,const char *txt,ts_mode_t mode) //print text from code memory
{
	int i = 0;
	ts_pos_t posx,posy;
	posx = offsetx + column*FONT_WIDTH;
	posy = offsety + line*FONT_HEIGHT;

	while(pgm_read_byte(&txt[i]))
	{
		TSLCDCharDisp(pgm_read_byte(&txt[i]),posx,posy,mode);
		posx += FONT_WIDTH;
		i++;
	}
}

void TSLCDPrintCh(unsigned char line,unsigned char column,char c,ts_mode_t mode) //print a character on LCD
{
	ts_pos_t posx,posy;
	posx = offsetx + column*FONT_WIDTH;
	posy = offsety + line*FONT_HEIGHT;

	TSLCDCharDisp(c,posx,posy,mode);
}

