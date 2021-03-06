/*
-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2014 Torus Knot Software Ltd

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/
#ifndef __gLTFImportConstants_H__
#define __gLTFImportConstants_H__

#include <string>
#include <sstream>
#include <fstream>
#include <ctime>

//---------------------------------------------------------------------
struct Devnull : std::ostream, std::streambuf
{
	Devnull() : std::ostream(this) {}
};

// (un)comment first/second line to enable/disable debug to screen
#define OUT std::cout
//#define OUT Devnull()

//---------------------------------------------------------------------
// Disable the line below when using gLTFTEST, because otherwise Ogre::Image (used for texture transformation) does not work
#define USE_OGRE_IN_PLUGIN 1

//---------------------------------------------------------------------
static const std::string TAB = "  ";
static const std::string TABx2 = TAB + TAB;
static const std::string TABx3 = TAB + TABx2;
static const std::string TABx4 = TAB + TABx3;
static const std::string TABx5 = TAB + TABx4;
static const std::string TABx6 = TAB + TABx5;
static const std::string TABx7 = TAB + TABx6;
static const std::string TABx8 = TAB + TABx7;
static const std::string TABx9 = TAB + TABx8;
static const std::string TABx10 = TAB + TABx9;

static std::string gFileExtension = "";
static std::string gJsonString = "";
static const unsigned int MAX_JOINTS_PER_SKIN = 1000;

//---------------------------------------------------------------------
static const std::string& getFileExtension (const std::string& fileName)
{
	std::string::size_type idx;
	idx = fileName.rfind('.');

	if (idx != std::string::npos)
	{
		gFileExtension = fileName.substr(idx + 1);
	}

	return gFileExtension;
}

//---------------------------------------------------------------------
static void copyFile (const std::string& fileNameSource, std::string& fileNameDestination)
{
	if (fileNameSource == fileNameDestination)
		return;

	std::ifstream src(fileNameSource.c_str(), std::ios::binary);
	std::ofstream dst(fileNameDestination.c_str(), std::ios::binary);
	dst << src.rdbuf();
	dst.close();
	src.close();
}

//---------------------------------------------------------------------
static const std::string& getJsonAsString (const std::string& jsonFileName)
{
	std::ostringstream sstream;
	std::ifstream fs(jsonFileName);
	sstream << fs.rdbuf();
	gJsonString = sstream.str();
	return gJsonString;
}

//---------------------------------------------------------------------
static bool fileExists (const std::string& fileName)
{
	struct stat fileInfo;
	return stat(fileName.c_str(), &fileInfo) == 0;
}

//---------------------------------------------------------------------
static bool isUriEmbeddedBase64 (std::string uri)
{
	std::size_t found = uri.find(";base64,");
	return found != std::string::npos;
}

//---------------------------------------------------------------------
static std::string getEmbeddedBase64FromUri (std::string uri)
{
	std::size_t found = uri.find(";base64,");
	if (found != std::string::npos)
	{
		std::size_t found = uri.find(";base64,");
		return uri.substr(found + 8);
	}

	return "";
}

//---------------------------------------------------------------------
static std::string getMimeTypeFromBase64Uri (std::string uri)
{
	std::size_t start = uri.find("data:") + 5;
	std::size_t end = uri.find(";base64,");
	return uri.substr(start, end - start);
}

//---------------------------------------------------------------------
static bool isFilePathAbsolute (std::string fileName)
{
	if (fileName == "")
		return false;

#if defined(_WIN32) || defined(_WIN64)
	if (fileName.substr(1, 1) == ":")
		return true;
	else
		return false;

#elif defined(LINUX )
	if (fileName.substr(0, 1) == "/")
		return true;
	else
		return false;

#endif 

	return false;
}

//---------------------------------------------------------------------
static std::string getBaseFileNameWithExtension (std::string fileName)
{
	std::size_t found = fileName.find_last_of("/\\");
	return fileName.substr(found + 1);
}


//---------------------------------------------------------------------
static std::string generateRandomString (void)
{
	// Generate a name if not provided (name is optional in gLTF)
	std::stringstream ss;
	std::time_t result = std::time(nullptr);
	ss << rand() << rand();
	return ss.str();
}

//---------------------------------------------------------------------
static std::string getExtensionFromMimeType(std::string mimeType)
{
	if (mimeType == "image/jpeg")
		return ".jpg";
	if (mimeType == "image/jpg")
		return ".jpg";
	if (mimeType == "image/bmp")
		return ".bmp";
	if (mimeType == "image/tiff")
		return ".tiff";
	return ".png";
}

#endif