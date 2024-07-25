/*
 Copyright 2023 OdonataTech LLS. All rights reserved.
 */

#include <utilites.h>

// ********************************************s********************************
// TextAsUTF8String()  convert fmx::Text to std::string
// ****************************************************************************
std::string TextAsUTF8String ( const fmx::Text& fmx_text )
{
    
    std::string result;
    
    FMX_UInt32 text_size = ( 4 * ( fmx_text.GetSize() ) ) + 1;
    char * text = new char [ text_size ]();
    fmx_text.GetBytes ( text, text_size, 0, (FMX_UInt32)fmx::Text::kSize_End, fmx::Text::kEncoding_UTF8 );
    result.assign ( text );
    delete [] text;
    
    return result;
    
}; // TextAsString


// ****************************************************************************
// stringToVector() Split string to vector by delimeter
// ****************************************************************************
unsigned long stringToVector(const std::string &str, vector<string> &dest, std::string delimeter){
    if (str.empty()) return 0;
    
    if (delimeter.empty()) {
        dest.push_back(str);
        
    }else{
        size_t pos1 = 0, pos2 = 0;
        std::string el;
        while (pos2 != std::string::npos) {
            pos2 = str.find(delimeter, pos1);
            el = str.substr(pos1, pos2 - pos1);
            dest.push_back(el);
            pos1 = pos2+ delimeter.length();
        }
    }
    
    return dest.size();
}; // stringToVector



// ********************************************************************************************
// substitute() replce all subsrings in string by value
// ********************************************************************************************
long substitute( string &str, const string &sub, const string &value ){
    
    if (str.empty()) return 0;
    
    size_t pos = str.find(sub);
    while (pos != string::npos) {
        str.replace(pos, sub.length() , value);
        pos = str.find(sub, pos+value.length() );
    }
    return 1;
};

// ********************************************************************************************
// sub_escape() substitute ' "" \n \r
// ********************************************************************************************
long sub_escape( string &str  ){
	substitute ( str , "'", " " );
	substitute ( str , "\"", " " );
	substitute ( str , "\n", " " );
	substitute ( str , "\n", " " );
	substitute ( str , "\\", " " );
	
	return 1;
}



// ********************************************************************************************
// simpleFormat() get string by format like printf
// ********************************************************************************************
//string simpleFormat( string format, double value ){
//    
//    int bufSize = snprintf(NULL, 0, format.c_str(), value);
//    char buf[bufSize+1];
//    snprintf(buf, sizeof(buf), format.c_str(), value);
//    std::string str = "";
//    
//    return buf;
//}


std::random_device rd; // random device engine, usually based on /dev/random on UNIX-like systems
// initialize Mersennes' twister using rd to generate the seed
std::mt19937 rng{rd()};

int mersenne_rand_int()
{
    static std::uniform_int_distribution<int> uid(1,32767); // random dice
    return uid(rng); // use rng as a generator
}


/*****  ***********************************************************************
 GetFileQuadType() UTILITY FUNCTION.   return quadchar filemaker file type
 **************************************************************************** */
fmx::QuadCharUniquePtr GetFileQuadType ( std::string filetype )
{
    std::string typeName;
    
    if (filetype == "png") {
        typeName = PNG_CONTAINER_TYPE;
    }else if (filetype == "jpg"){
        typeName = JPEG_CONTAINER_TYPE;
    }else if (filetype == "jpeg"){
        typeName = JPEG_CONTAINER_TYPE;
    }else if (filetype == "eps"){
        typeName = EPS_CONTAINER_TYPE;
    }else if (filetype == "gif"){
        typeName = GIF_CONTAINER_TYPE;
    }else if (filetype == "bmp"){
        typeName = BMP_CONTAINER_TYPE;
    }else if (filetype == "pdf"){
        typeName = PDF_CONTAINER_TYPE;
    }else{
        typeName = META_CONTAINER_TYPE;
    }

    fmx::QuadCharUniquePtr    type ( typeName[0], typeName[1], typeName[2], typeName[3] );
    
    return type;
    
} // GetFileQuadType


/*****  ***********************************************************************
 isFileTypeImage() UTILITY FUNCTION.   return TRUE if filetype can be process as image by FileMaker
 **************************************************************************** */
bool isFileTypeImage ( std::string filetype )
{
    
    if (filetype == "png" || filetype == "jpg" || filetype == "eps" || filetype == "gif" || filetype == "bmp" || filetype == "pdf" ){
        return 1;
    }else{
        return 0;
    }
    
} // isFileTypeImage