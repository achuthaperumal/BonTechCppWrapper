#include "pch.h"
#include "BonTechLibrary.h"
#include "ImageCAPDllEx.h"


int __stdcall Connect(unsigned int nHostIP, unsigned int nSensorIP, LPTSTR ConfigDir)
{
    int nErrorCode;
    nErrorCode = ImageCapConnectSensor(nHostIP, nSensorIP, ConfigDir);
    return nErrorCode;
}

int __stdcall Disconnect(unsigned int nSensorIP)
{
    int nErrorCode;
    nErrorCode = ImageCapDisConnectSensor(nSensorIP);
    return nErrorCode;
}

int __stdcall GetImageSize(unsigned int nSensorIP, unsigned short* nWidth, unsigned short* nHeight)
{
    int nErrorCode;
    unsigned short height, width;
    nErrorCode = ImageCapGetFrameSize(nSensorIP, &width, &height);
    *nWidth = width;
    *nHeight = height;
    return nErrorCode;
}

int __stdcall SetAcquisitionMode(unsigned int nSensorIP, int nMode)
{
    int nErrorCode;
    nErrorCode = ImageCapModeChange(nSensorIP, nMode);
    return nErrorCode;
}

int __stdcall SendCaptureCommand(unsigned int nSensorIP, int nMode)
{
    int nErrorCode;
    nErrorCode = ImageCapCommandSend(nSensorIP, nMode);
    return nErrorCode;
}

int __stdcall StartContinuousAcquisition(unsigned int nSensorIP, unsigned short *refImg, unsigned short *pImage, unsigned int nMode, LPCTSTR lpszRefPath)
{
    int nErrorCode;
    nErrorCode = ImageCapStartCapture(nSensorIP, &refImg, nMode, lpszRefPath);
    memcpy(pImage, refImg, 3072 * 3072 * 2);
    return nErrorCode;

}

int __stdcall StopContinuousAcquisition(unsigned int nSensorIP)
{
    int nErrorCode;
    nErrorCode = ImageCapStopCapture(nSensorIP);
    return nErrorCode;
}

int __stdcall CaptureSingleImage(unsigned int nSensorIP, unsigned short* pImage,unsigned int nMode, LPCTSTR calRefPath)
{
    int nErrorCode;
    unsigned short* testImg;
    testImg = new unsigned short[3072 * 3072];
    nErrorCode = ImageCapImageAcquistion(nSensorIP, testImg, nMode,calRefPath);
    memcpy(pImage, testImg, 3072 * 3072 * 2);
    delete[] testImg;
    return nErrorCode;
}

int __stdcall CancelSingleImageCapture(unsigned int nSensorIP)
{
    int nErrorCode;
    nErrorCode = ImageCapAcquisitionCancel(nSensorIP);
    return nErrorCode;
}


LPTSTR __stdcall GetSerialNumber(unsigned int nSensorIP, LPTSTR lpszSerialNum)
{
    int nErrorCode;
    nErrorCode = ImageCapGetSerialNumber(nSensorIP, lpszSerialNum);
    return lpszSerialNum;
}