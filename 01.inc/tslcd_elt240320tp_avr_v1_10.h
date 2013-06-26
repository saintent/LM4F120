#ifndef _TSLCD_ELT240320TP_H_
#define _TSLCD_ELT240320TP_H_

#define TS_INS_START_OSC			0x00 //data read at this instruction should be 0x0789 --> use for test connection
#define TS_INS_DRIV_OUT_CTRL		0x01
#define TS_INS_DRIV_WAV_CTRL		0x02
#define TS_INS_ENTRY_MOD			0x03
#define TS_INS_RESIZE_CTRL			0x04
#define TS_INS_DISP_CTRL1			0x07
#define TS_INS_DISP_CTRL2			0x08
#define TS_INS_DISP_CTRL3			0x09
#define TS_INS_DISP_CTRL4			0x0A
#define TS_INS_RGB_DISP_IF_CTRL1	0x0C
#define TS_INS_FRM_MARKER_POS		0x0D
#define TS_INS_RGB_DISP_IF_CTRL2	0x0F
#define TS_INS_POW_CTRL1			0x10
#define TS_INS_POW_CTRL2			0x11
#define TS_INS_POW_CTRL3			0x12
#define TS_INS_POW_CTRL4			0x13
#define TS_INS_GRAM_HOR_AD			0x20
#define TS_INS_GRAM_VER_AD			0x21
#define TS_INS_RW_GRAM				0x22
#define TS_INS_POW_CTRL7			0x29
#define TS_INS_FRM_RATE_COL_CTRL	0x2B
#define TS_INS_GAMMA_CTRL1			0x30
#define TS_INS_GAMMA_CTRL2			0x31
#define TS_INS_GAMMA_CTRL3			0x32
#define TS_INS_GAMMA_CTRL4			0x35 
#define TS_INS_GAMMA_CTRL5			0x36
#define TS_INS_GAMMA_CTRL6			0x37
#define TS_INS_GAMMA_CTRL7			0x38
#define TS_INS_GAMMA_CTRL8			0x39
#define TS_INS_GAMMA_CTRL9			0x3C
#define TS_INS_GAMMA_CTRL10			0x3D
#define TS_INS_HOR_START_AD			0x50
#define TS_INS_HOR_END_AD			0x51
#define TS_INS_VER_START_AD			0x52
#define TS_INS_VER_END_AD			0x53
#define TS_INS_GATE_SCAN_CTRL1		0x60
#define TS_INS_GATE_SCAN_CTRL2		0x61
#define TS_INS_GATE_SCAN_CTRL3		0x6A
#define TS_INS_PART_IMG1_DISP_POS	0x80
#define TS_INS_PART_IMG1_START_AD	0x81
#define TS_INS_PART_IMG1_END_AD		0x82
#define TS_INS_PART_IMG2_DISP_POS	0x83
#define TS_INS_PART_IMG2_START_AD	0x84
#define TS_INS_PART_IMG2_END_AD		0x85
#define TS_INS_PANEL_IF_CTRL1		0x90
#define TS_INS_PANEL_IF_CTRL2		0x92
#define TS_INS_PANEL_IF_CTRL3		0x93
#define TS_INS_PANEL_IF_CTRL4		0x95
#define TS_INS_PANEL_IF_CTRL5		0x97
#define TS_INS_PANEL_IF_CTRL6		0x98

#define TS_COL_RED					0x001F
#define TS_COL_GREEN				0x07E0
#define TS_COL_BLUE					0xF800
#define TS_COL_YELLOW				0x07FF
#define TS_COL_PURPLE				0xF81F
#define TS_COL_BLACK				0x0000
#define TS_COL_WHITE				0xFFFF

#define ts_pos_t					int

typedef enum
{
	TS_MODE_NORMAL,
	TS_MODE_INVERSE,
	TS_MODE_FULL,
} ts_mode_t;

void TSLCDOutIns(unsigned short ins); //write instruction to LCD
void TSLCDOutDat(unsigned short dat); //write data to LCD
void TSLCDOutDat2(unsigned char dath,unsigned char datl); //write data to LCD
unsigned short TSLCDInIns(void); //read data from LCD
unsigned short TSLCDInDat(void); //read data from LCD

void TSLCDRst(void); //pulse reset signal to LCD
void TSLCDInit(void); //initial LCD
void TSLCDShowPic2(ts_pos_t sx,ts_pos_t ex,ts_pos_t sy,ts_pos_t ey,const unsigned short *pic,ts_mode_t mode);
//show picture from code memory with specific size
void TSLCDFillRect(ts_pos_t sx,ts_pos_t ex,ts_pos_t sy,ts_pos_t ey,unsigned short color,ts_mode_t mode); //draw a rectangular
void TSLCDFillCirc(ts_pos_t cx,ts_pos_t cy,ts_pos_t rad,unsigned short color, ts_mode_t mode); //draw a circle
void TSLCDSetMargins(ts_pos_t xl,ts_pos_t xr,ts_pos_t yu,ts_pos_t yl); //set margins for FillRect,FillCirc
void TSLCDSetMarginsDefault(void); //reset margins to default value

void TSLCDSetFontColor(unsigned short color); //set text's color
void TSLCDSetBackColor(unsigned short color); //set back color for TS_MODE_FULL
void TSLCDSetOffset(ts_pos_t x,ts_pos_t y); //set LCD offset for character display
void TSLCDPrintStr(unsigned char line,unsigned char column,char *str,ts_mode_t mode); //print string on LCD
void TSLCDPrintTxt(unsigned char line,unsigned char column,const char *txt,ts_mode_t mode); //print text from code memory
void TSLCDPrintCh(unsigned char line,unsigned char column,char c,ts_mode_t mode); //print a character on LCD

#endif

