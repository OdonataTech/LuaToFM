/*
 Copyright 2023 OdonataTech LLS. All rights reserved.
 */


#ifndef utilites_h
#define utilites_h

#include "FMXBinaryData.h"
#include "FMXCalcEngine.h"
#include "FMXClient.h"
#include "FMXData.h"
#include "FMXDateTime.h"
#include "FMXExtern.h"
#include "FMXFixPt.h"
#include "FMXText.h"
#include "FMXTextStyle.h"
#include "FMXTypes.h"

#include <map>
#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <list>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <iterator>
#include <memory>
#include <map>
#include <cmath>
#include <limits>
#include <random>
#include <stdio.h>
#include <stdarg.h>


using namespace std;

#define MAIN_CONTAINER_TYPE "MAIN"
#define SOUND_CONTAINER_TYPE "snd "
#define JPEG_CONTAINER_TYPE "JPEG"
#define GIF_CONTAINER_TYPE "GIFf"
#define EPS_CONTAINER_TYPE "EPS "
#define META_CONTAINER_TYPE "META"
#define PNG_CONTAINER_TYPE "PNGf"
#define BMP_CONTAINER_TYPE "BMPf"
#define PDF_CONTAINER_TYPE "PDF "


std::string TextAsUTF8String ( const fmx::Text& fmx_text );

unsigned long stringToVector(const string &str, vector<string> &dest, string delimetr );

long substitute( string &str, const string &sub, const string &value );

long sub_escape( string &str  );


//string simpleFormat( string format, double value );

int mersenne_rand_int();

fmx::QuadCharUniquePtr GetFileQuadType ( std::string filetype );

bool isFileTypeImage ( std::string filetype );

#endif /* utilites_h */

