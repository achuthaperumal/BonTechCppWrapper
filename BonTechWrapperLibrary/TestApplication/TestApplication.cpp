// TestApplication.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include "nivision.h"
#include "afxwin.h"
#include "ImageCAPDllEx.h"
#include "CalibrationDllEx.h"
using namespace std;

/*int readRawFile(unsigned short* pSrc, int width, int height)
{
    ifstream file;
    file.open("test.raw", ios::in | ios::binary);
    if (file.is_open() == true)
    {
        file.read(reinterpret_cast<char*>(pSrc), width * height * 2);
        file.close();
        retUrn 1;
    }
    else return 0;
}*/

int main()
{
    /*int m_HistogramSize = 1 << 16;
    std::cout << "Hello World!\n";
    std::cout << sizeof(short int)<<std::endl;

    short int x= 65535;
    short int y= 32768;
    int z = x ^ y;
    std::cout << z<<std::endl;*/

    Image* myImage;
    int handle, y, nErrorCode;
    unsigned int nHostIP, nSensorIP;
    
    nHostIP = 84125888;                                             //192.168.3.5
    nSensorIP = 4026771648;                                         //192.168.3.240
    
    unsigned short* pImage = new unsigned short[3072 * 3072];
    /*unsigned short* pcalImage = new unsigned short[3072 * 3072];*/
    
    LPCTSTR  ConfigDir = TEXT("D:\\BSDx64\\192.168.3.240\\Config");
    LPCTSTR  calRefPath = TEXT("D:\\BSDx64\\192.168.3.240\\Reference");
    /*CRect rectMargin(0, 0, 3072, 3072);*/
        
    imaqSetWindowThreadPolicy(IMAQ_SEPARATE_THREAD);
    imaqGetWindowHandle(&handle);
    imaqSetWindowSize(handle, 1280, 720);
    imaqSetWindowZoomToFit(handle, 1);

    myImage = imaqCreateImage(IMAQ_IMAGE_U16, 3);   

    nErrorCode = ImageCapConnectSensor(nHostIP, nSensorIP, ConfigDir);
    std::cout << hex << "Connect Status: "<<nErrorCode<<std::endl;

    nErrorCode = ImageCapModeChange(nSensorIP, IMAGECAP_SOFTTRIGGER_MODE);
    std::cout << hex << "Acquisition Mode Status: " << nErrorCode << std::endl;

    nErrorCode = ImageCapImageAcquistion(nSensorIP, pImage, SOFTTRIGGER_INSTANT_MODE,calRefPath);
    std::cout << hex << "Acquisition Status: " << nErrorCode << std::endl;

    /*nErrorCode = ImageCalibration(pImage, calRefPath, 3072, 3072, IMAGECAP_TRIGGER_MODE, rectMargin, FALSE);
    std::cout << dec <<"Calibration Status: " << nErrorCode << std::endl;*/

    imaqArrayToImage(myImage,pImage,3072,3072);

    /*for (y = 0; y < LVHeight; ++y)
    {
        memcpy((unsigned short*)myImageInfo.imageStart + myImageInfo.pixelsPerLine * y, pImage + LVWidth * y, LVWidth*2);
    }*/

    imaqDisplayImage(myImage, handle, 1);
    cin >> y;

    imaqDispose(myImage);
    delete[] pImage;
    nErrorCode = ImageCapDisConnectSensor(nSensorIP);
}
