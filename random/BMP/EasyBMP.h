/***** [NOTE]
	Parad0x13: I edited the distribution of EasyBMP by Paul Macklin so that it existed
	in a single headerfile and single binary. This is simply to make things easier for me as I do not
	plan on updating the EasyBMP library past this version (1.06) and wanted a single file instead.
	I have not changed any code during the simple copy/paste proceedure of making it all one file.
	Then I minified the code using the following minifier: https://github.com/Scylardor/cminify
*****/

/*************************************************
*                                                *
*  EasyBMP Cross-Platform Windows Bitmap Library *
*                                                *
*  Author: Paul Macklin                          *
*   email: macklin01@users.sourceforge.net       *
* support: http://easybmp.sourceforge.net        *
*                                                *
*          file: EasyBMP.h                       *
*    date added: 01-31-2005                      *
* date modified: 12-01-2006                      *
*       version: 1.06                            *
*                                                *
*   License: BSD (revised/modified)              *
* Copyright: 2005-6 by the EasyBMP Project       *
*                                                *
* description: Main include file                 *
*                                                *
*************************************************/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_DEPRECATE
#endif
#include<iostream>
#include<cmath>
#include<cctype>
#include<cstring>
#ifndef EasyBMP
#define EasyBMP
#ifdef __BCPLUSPLUS__
#include<stdio.h>
#else
#include<cstdio>
#endif
#ifdef __GNUC__
#endif
#ifdef __INTEL_COMPILER
#endif
#ifndef _DefaultXPelsPerMeter_
#define _DefaultXPelsPerMeter_
#define DefaultXPelsPerMeter 3780
#endif
#ifndef _DefaultYPelsPerMeter_
#define _DefaultYPelsPerMeter_
#define DefaultYPelsPerMeter 3780
#endif
#ifndef _EasyBMP_Custom_Math_Functions_
#define _EasyBMP_Custom_Math_Functions_
inline double Square(double number){return number*number;}inline int IntSquare(int number){return number*number;}
#endif
int IntPow(int base,int exponent);
#ifndef _EasyBMP_Defined_WINGDI
#define _EasyBMP_Defined_WINGDI
typedef unsigned char ebmpBYTE;typedef unsigned short ebmpWORD;typedef unsigned int ebmpDWORD;
#endif
#ifndef _EasyBMP_DataStructures_h_
#define _EasyBMP_DataStructures_h_
inline bool IsBigEndian(){short word=0x0001;if((*(char*)&word)!=0x01){return true;}return false;}inline ebmpWORD FlipWORD(ebmpWORD in){return((in>>8)|(in<<8));}inline ebmpDWORD FlipDWORD(ebmpDWORD in){return(((in&0xFF000000)>>24)|((in&0x000000FF)<<24)|((in&0x00FF0000)>>8)|((in&0x0000FF00)<<8));}typedef struct RGBApixel{ebmpBYTE Blue;ebmpBYTE Green;ebmpBYTE Red;ebmpBYTE Alpha;}RGBApixel;class BMFH{public:ebmpWORD bfType;ebmpDWORD bfSize;ebmpWORD bfReserved1;ebmpWORD bfReserved2;ebmpDWORD bfOffBits;BMFH();void display(void);void SwitchEndianess(void);};class BMIH{public:ebmpDWORD biSize;ebmpDWORD biWidth;ebmpDWORD biHeight;ebmpWORD biPlanes;ebmpWORD biBitCount;ebmpDWORD biCompression;ebmpDWORD biSizeImage;ebmpDWORD biXPelsPerMeter;ebmpDWORD biYPelsPerMeter;ebmpDWORD biClrUsed;ebmpDWORD biClrImportant;BMIH();void display(void);void SwitchEndianess(void);};
#endif
#ifndef _EasyBMP_BMP_h_
#define _EasyBMP_BMP_h_
bool SafeFread(char*buffer,int size,int number,FILE*fp);bool EasyBMPcheckDataSize(void);class BMP{private:int BitDepth;int Width;int Height;RGBApixel**Pixels;RGBApixel*Colors;int XPelsPerMeter;int YPelsPerMeter;ebmpBYTE*MetaData1;int SizeOfMetaData1;ebmpBYTE*MetaData2;int SizeOfMetaData2;bool Read32bitRow(ebmpBYTE*Buffer,int BufferSize,int Row);bool Read24bitRow(ebmpBYTE*Buffer,int BufferSize,int Row);bool Read8bitRow(ebmpBYTE*Buffer,int BufferSize,int Row);bool Read4bitRow(ebmpBYTE*Buffer,int BufferSize,int Row);bool Read1bitRow(ebmpBYTE*Buffer,int BufferSize,int Row);bool Write32bitRow(ebmpBYTE*Buffer,int BufferSize,int Row);bool Write24bitRow(ebmpBYTE*Buffer,int BufferSize,int Row);bool Write8bitRow(ebmpBYTE*Buffer,int BufferSize,int Row);bool Write4bitRow(ebmpBYTE*Buffer,int BufferSize,int Row);bool Write1bitRow(ebmpBYTE*Buffer,int BufferSize,int Row);ebmpBYTE FindClosestColor(RGBApixel&input);public:int TellBitDepth(void);int TellWidth(void);int TellHeight(void);int TellNumberOfColors(void);void SetDPI(int HorizontalDPI,int VerticalDPI);int TellVerticalDPI(void);int TellHorizontalDPI(void);BMP();BMP(BMP&Input);~BMP();RGBApixel*operator()(int i,int j);RGBApixel GetPixel(int i,int j)const;bool SetPixel(int i,int j,RGBApixel NewPixel);bool CreateStandardColorTable(void);bool SetSize(int NewWidth,int NewHeight);bool SetBitDepth(int NewDepth);bool WriteToFile(const char*FileName);bool ReadFromFile(const char*FileName);RGBApixel GetColor(int ColorNumber);bool SetColor(int ColorNumber,RGBApixel NewColor);};
#endif
#ifndef _EasyBMP_VariousBMPutilities_h_
#define _EasyBMP_VariousBMPutilities_h_
BMFH GetBMFH(const char*szFileNameIn);BMIH GetBMIH(const char*szFileNameIn);void DisplayBitmapInfo(const char*szFileNameIn);int GetBitmapColorDepth(const char*szFileNameIn);void PixelToPixelCopy(BMP&From,int FromX,int FromY,BMP&To,int ToX,int ToY);void PixelToPixelCopyTransparent(BMP&From,int FromX,int FromY,BMP&To,int ToX,int ToY,RGBApixel&Transparent);void RangedPixelToPixelCopy(BMP&From,int FromL,int FromR,int FromB,int FromT,BMP&To,int ToX,int ToY);void RangedPixelToPixelCopyTransparent(BMP&From,int FromL,int FromR,int FromB,int FromT,BMP&To,int ToX,int ToY,RGBApixel&Transparent);bool CreateGrayscaleColorTable(BMP&InputImage);bool Rescale(BMP&InputImage,char mode,int NewDimension);
#endif
#ifndef _EasyBMP_Version_
#define _EasyBMP_Version_ 1.06
#define _EasyBMP_Version_Integer_ 106
#define _EasyBMP_Version_String_ "1.06"
#endif
#ifndef _EasyBMPwarnings_
#define _EasyBMPwarnings_
#endif
void SetEasyBMPwarningsOff(void);void SetEasyBMPwarningsOn(void);bool GetEasyBMPwarningState(void);
#endif
