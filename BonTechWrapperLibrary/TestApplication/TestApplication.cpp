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

    Image *calibratedImage, *uncalibratedImage;
    int handle1, handle2, y, nErrorCode;
    unsigned int nHostIP, nSensorIP;
    
    nHostIP = 84125888;                                             //192.168.3.5
    nSensorIP = 4026771648;                                         //192.168.3.240
    
    unsigned short* pImage = new unsigned short[3072 * 3072];
    
    LPCTSTR  ConfigDir = TEXT("C:\\BSD\\192.168.3.240\\Config");
    LPCTSTR  calRefPath = TEXT("C:\\BSD\\192.168.3.240\\Reference");
    /*CRect rectMargin(0, 0, 3072, 3072);*/   

    imaqSetWindowThreadPolicy(IMAQ_SEPARATE_THREAD);
    calibratedImage = imaqCreateImage(IMAQ_IMAGE_U16, 3);   
    uncalibratedImage = imaqCreateImage(IMAQ_IMAGE_U16, 3);

    //Connect to Sensor
    nErrorCode = ImageCapConnectSensor(nHostIP, nSensorIP, ConfigDir);
    std::cout << hex << "Connect Status: "<<nErrorCode<<std::endl;

    //Acquisition Mode Set
    nErrorCode = ImageCapModeChange(nSensorIP, IMAGECAP_SOFTTRIGGER_MODE);
    std::cout << hex << "Acquisition Mode Status: " << nErrorCode << std::endl;

    cout << "Press any key to Acquire Calibrated Image: ";
    cin >> y;

    //Acquire Calibrated Image
    imaqGetWindowHandle(&handle1);
    imaqSetWindowTitle(handle1, "Calibrated Image");
    imaqSetWindowSize(handle1, 1280, 720);
    imaqSetWindowZoomToFit(handle1, 1);

    nErrorCode = ImageCapImageAcquistion(nSensorIP, pImage, SOFTTRIGGER_INSTANT_MODE,calRefPath);
    std::cout << hex << "Acquisition Status: " << nErrorCode << std::endl;

    imaqArrayToImage(calibratedImage,pImage,3072,3072);
    imaqDisplayImage(calibratedImage, handle1, 1);

    //Acquire Uncalibrated Image
    cout << "Press any key to Acquire Uncalibrated Image: ";
    cin >> y;
    imaqGetWindowHandle(&handle2);
    imaqSetWindowTitle(handle2, "Uncalibrated Image");
    imaqSetWindowSize(handle2, 1280, 720);
    imaqSetWindowZoomToFit(handle2, 1);
    
    nErrorCode = ImageCapImageAcquistion(nSensorIP, pImage, SOFTTRIGGER_INSTANT_MODE, NULL);
    std::cout << hex << "Acquisition Status: " << nErrorCode << std::endl;

    imaqArrayToImage(uncalibratedImage, pImage, 3072, 3072);
    imaqDisplayImage(uncalibratedImage, handle2, 1);

    cout << "Press any key to Close the Application! ";
    cin >> y;

    /*nErrorCode = ImageCalibration(pImage, calRefPath, 3072, 3072, IMAGECAP_TRIGGER_MODE, rectMargin, FALSE);
    std::cout << dec <<"Calibration Status: " << nErrorCode << std::endl;*/

    imaqDispose(calibratedImage);
    imaqDispose(uncalibratedImage);
    delete[] pImage;
    nErrorCode = ImageCapDisConnectSensor(nSensorIP);
}
