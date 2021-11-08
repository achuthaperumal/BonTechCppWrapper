#include "pch.h"
#include<fstream>
#include "BonTechLibrary.h"
#include "ImageCAPDllEx.h"
using namespace std;


static int nErrorCode;
char* pSrc = new char[3052 * 2500 * 2];
unsigned short* refImg = NULL;


int __stdcall Connect(unsigned int nHostIP, unsigned int nSensorIP, LPTSTR ConfigDir)
{
    nErrorCode = ImageCapConnectSensor(nHostIP, nSensorIP, ConfigDir);
    return nErrorCode;
}

int __stdcall Disconnect(unsigned int nSensorIP)
{
    nErrorCode = ImageCapDisConnectSensor(nSensorIP);
    return nErrorCode;
}

int __stdcall GetImageSize(unsigned int nSensorIP, unsigned short* nWidth, unsigned short* nHeight)
{
    unsigned short height, width;
    nErrorCode = ImageCapGetFrameSize(nSensorIP, &width, &height);
    *nWidth = width;
    *nHeight = height;
    return nErrorCode;
}

int __stdcall SetAcquisitionMode(unsigned int nSensorIP, int nMode)
{
    nErrorCode = ImageCapModeChange(nSensorIP, nMode);
    return nErrorCode;
}

int __stdcall SetCaptureMode(unsigned int nSensorIP, int nMode)
{
    nErrorCode = ImageCapCommandSend(nSensorIP, nMode);
    return nErrorCode;
}

int __stdcall StartContinuousAcquisition(unsigned int nSensorIP, unsigned short *pImage, unsigned int nMode, LPCTSTR lpszRefPath)
{
    
    nErrorCode = ImageCapStartCapture(nSensorIP, &refImg, nMode, lpszRefPath);
    memcpy(pImage, refImg, 3072 * 3072 * 2);
    return nErrorCode;

}

int __stdcall StopContinuousAcquisition(unsigned int nSensorIP)
{
    nErrorCode = ImageCapStopCapture(nSensorIP);
    return nErrorCode;
}

int __stdcall CaptureSingleImage(unsigned int nSensorIP, unsigned short* pImage,unsigned int nMode, LPCTSTR calRefPath)
{
    nErrorCode = ImageCapImageAcquistion(nSensorIP, pImage, nMode,calRefPath);
    //memcpy(pImage, testImg, 3072 * 3072 * 2);
    return nErrorCode;
}

int __stdcall CancelSingleImageCapture(unsigned int nSensorIP)
{
    nErrorCode = ImageCapAcquisitionCancel(nSensorIP);
    return nErrorCode;
}

int readRawFile(char* pSrc, int width, int height)
{
    ifstream file;
    file.open("test.raw", ios::in | ios::binary);
    if (file.is_open() == true)
    {
        file.read(pSrc, width * height * 2);
        file.close();
        return 1;
    }
    else return 0;
}


int __stdcall CopyImageToLV(char *buff) 
{
    
    int y, LVHeight, LVWidth;
    LVHeight = 3052;
    LVWidth = 2500;
    memset(buff, 200, LVWidth * LVHeight * 2);
    return 1;
}

int __stdcall Copy_C_Image_To_LabVIEW_Image(char* LVImagePtr, int LVLineWidth, int LVWidth, int LVHeight)
{
    ImageInfo testImageInfo;
    Image* testImage;
    int y;
    PixelValue pixelValue;

    char* pSrc = new char[LVHeight * LVWidth * 2];
    readRawFile(pSrc, LVWidth, LVHeight);

    // Create the image as IMAQ_IMAGE_U8
    testImage = imaqCreateImage(IMAQ_IMAGE_U16, 3);

    // Set the image size
    imaqSetImageSize(testImage, LVWidth, LVHeight);

    // Fill the image with 128's
    pixelValue.grayscale = 32000;
    imaqFillImage(testImage, pixelValue, NULL);

    // Get the image info -- this should be that same as the height and width 
    // the image was created from
    imaqGetImageInfo(testImage, &testImageInfo);

    // Copy, line by line, the C image into the LV image.
    for (y = 0; y < LVHeight*2; ++y) {
        memcpy(LVImagePtr + y * LVLineWidth, (unsigned short*)testImageInfo.imageStart+testImageInfo.pixelsPerLine*y, LVWidth*2);
    }

    // Dispose of the C image
    imaqDispose(testImage);
    delete[] pSrc;
    return 0;
}


LPTSTR __stdcall GetSerialNumber(unsigned int nSensorIP, LPTSTR lpszSerialNum)
{
    nErrorCode = ImageCapGetSerialNumber(nSensorIP, lpszSerialNum);
    return lpszSerialNum;
}