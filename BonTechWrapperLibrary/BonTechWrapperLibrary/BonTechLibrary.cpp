#include "pch.h"
#include "BonTechLibrary.h"
#include "ImageCAPDllEx.h"
#include "CalibrationDllEx.h"


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

int __stdcall GetFrameSize(unsigned int nSensorIP, unsigned short* nWidth, unsigned short* nHeight)
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

int __stdcall StartContinuousAcquisition(unsigned int nSensorIP, unsigned short *refImg, unsigned short *pImage, unsigned int nBuffLength, unsigned int nMode, LPCTSTR lpszRefPath)
{
    int nErrorCode;
    nErrorCode = ImageCapStartCapture(nSensorIP, &refImg, nMode, lpszRefPath);
    memcpy(pImage, refImg, nBuffLength);
    return nErrorCode;

}

int __stdcall GenerateReference(unsigned short* pDarkImage, unsigned short* pBrightImage, int nAcqMode, LPCTSTR lpszSavePath, int nWidth, int nHeight)
{
    /*int nErrorCode;
    nErrorCode = ImageCapStartCapture(nSensorIP, &refImg, nMode, lpszRefPath);
    memcpy(pImage, refImg, nBuffLength);
    return nErrorCode;*/
    int nErrorCode;
    nErrorCode = GenerateReferenceFile(pDarkImage, &pBrightImage, 1, nAcqMode, lpszSavePath, nWidth, nHeight);
    return nErrorCode;

}

int __stdcall StopContinuousAcquisition(unsigned int nSensorIP)
{
    int nErrorCode;
    nErrorCode = ImageCapStopCapture(nSensorIP);
    return nErrorCode;
}

int __stdcall CaptureSingleImage(unsigned int nSensorIP, unsigned short* pImage,unsigned int nHeight, unsigned int nWidth, unsigned int nMode, LPCTSTR calRefPath)
{
    int nErrorCode;
    unsigned short* testImg;
    testImg = new unsigned short[nHeight * nWidth];
    nErrorCode = ImageCapImageAcquistion(nSensorIP, testImg, nMode,calRefPath);
    memcpy(pImage, testImg, nHeight * nWidth * 2);
    delete[] testImg;
    return nErrorCode;
}

int __stdcall CancelSingleImageCapture(unsigned int nSensorIP)
{
    int nErrorCode;
    nErrorCode = ImageCapAcquisitionCancel(nSensorIP);
    return nErrorCode;
}


int __stdcall GetSerialNumber(unsigned int nSensorIP, LPTSTR lpszSerialNum)
{
    int nErrorCode;
    nErrorCode = ImageCapGetSerialNumber(nSensorIP, lpszSerialNum);
    return nErrorCode;
}