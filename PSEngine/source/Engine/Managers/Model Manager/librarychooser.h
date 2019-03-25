/**************************************************************************************

 Copyright (C) 2001 - 2009 Autodesk, Inc. and/or its licensors.
 All Rights Reserved.

 The coded instructions, statements, computer programs, and/or related material 
 (collectively the "Data") in these files contain unpublished information 
 proprietary to Autodesk, Inc. and/or its licensors, which is protected by 
 Canada and United States of America federal copyright law and by international 
 treaties. 
 
 The Data may not be disclosed or distributed to third parties, in whole or in
 part, without the prior written consent of Autodesk, Inc. ("Autodesk").

 THE DATA IS PROVIDED "AS IS" AND WITHOUT WARRANTY.
 ALL WARRANTIES ARE EXPRESSLY EXCLUDED AND DISCLAIMED. AUTODESK MAKES NO
 WARRANTY OF ANY KIND WITH RESPECT TO THE DATA, EXPRESS, IMPLIED OR ARISING
 BY CUSTOM OR TRADE USAGE, AND DISCLAIMS ANY IMPLIED WARRANTIES OF TITLE, 
 NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE OR USE. 
 WITHOUT LIMITING THE FOREGOING, AUTODESK DOES NOT WARRANT THAT THE OPERATION
 OF THE DATA WILL BE UNINTERRUPTED OR ERROR FREE. 
 
 IN NO EVENT SHALL AUTODESK, ITS AFFILIATES, PARENT COMPANIES, LICENSORS
 OR SUPPLIERS ("AUTODESK GROUP") BE LIABLE FOR ANY LOSSES, DAMAGES OR EXPENSES
 OF ANY KIND (INCLUDING WITHOUT LIMITATION PUNITIVE OR MULTIPLE DAMAGES OR OTHER
 SPECIAL, DIRECT, INDIRECT, EXEMPLARY, INCIDENTAL, LOSS OF PROFITS, REVENUE
 OR DATA, COST OF COVER OR CONSEQUENTIAL LOSSES OR DAMAGES OF ANY KIND),
 HOWEVER CAUSED, AND REGARDLESS OF THE THEORY OF LIABILITY, WHETHER DERIVED
 FROM CONTRACT, TORT (INCLUDING, BUT NOT LIMITED TO, NEGLIGENCE), OR OTHERWISE,
 ARISING OUT OF OR RELATING TO THE DATA OR ITS USE OR ANY OTHER PERFORMANCE,
 WHETHER OR NOT AUTODESK HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH LOSS
 OR DAMAGE. 

**************************************************************************************/

//
// Specify the fbxsdk lib of the FBX SDK package we must link with
// and lib we must exclude
//

// Dont forget to set the path to the FBX SDK Lib in 
// <Additional Library Directories> of your project
// Ex: "C:\Program Files\Autodesk\FBX\FbxSdk\2006.11.1\lib"
#if (_MSC_VER == 1500)
    #if defined(WIN32)

        #if defined( _MT) && !defined(_DLL) // if this is a Multi-threaded /MT or MTd project

            #ifdef _DEBUG
                #pragma comment(linker, "/nodefaultlib:libcmt.lib") // exclude a lib
                #pragma comment(lib, "fbxsdk_mt2008d.lib")
            #else
                #pragma comment(lib, "fbxsdk_mt2008.lib")
            #endif

        #elif defined( _MT) && defined(_DLL) // if this is a Multi-threaded DLL /MD or /MDd project

            #ifdef _DEBUG
                #pragma comment(linker, "/nodefaultlib:libcmt.lib") // exclude a lib
                #pragma comment(lib, "fbxsdk_md2008d.lib")
            #else
                #pragma comment(lib, "fbxsdk_md2008.lib" )
            #endif

        #else

            #error No FbxSdk library available for this compilation.

        #endif
    #elif defined (WIN64)
        #if defined( _MT) && !defined(_DLL) // if this is a Multi-threaded /MT or MTd project
            #ifdef _DEBUG
                #pragma comment(linker, "/nodefaultlib:libcmt.lib") // exclude a lib
                #pragma comment(lib, "fbxsdk_mt2008_amd64d.lib")
            #else
                #pragma comment(lib, "fbxsdk_mt2008_amd64.lib")
            #endif

        #elif defined( _MT) && defined(_DLL) // if this is a Multi-threaded DLL /MD or /MDd project

            #ifdef _DEBUG
                #pragma comment(linker, "/nodefaultlib:libcmt.lib") // exclude a lib
                #pragma comment(lib, "fbxsdk_md2008_amd64d.lib")
            #else
                #pragma comment(lib, "fbxsdk_md2008_amd64.lib" )
            #endif

        #else
            #error No FbxSdk library available for this compilation.
        #endif
    #endif //if win32 or win64
#elif (_MSC_VER == 1400)
    #if defined(WIN32)

        #if defined( _MT) && !defined(_DLL) // if this is a Multi-threaded /MT or MTd project

            #ifdef _DEBUG
                #pragma comment(linker, "/nodefaultlib:libcmt.lib") // exclude a lib
                #pragma comment(lib, "fbxsdk_mt2005d.lib")
            #else
                #pragma comment(lib, "fbxsdk_mt2005.lib")
            #endif

        #elif defined( _MT) && defined(_DLL) // if this is a Multi-threaded DLL /MD or /MDd project

            #ifdef _DEBUG
                #pragma comment(linker, "/nodefaultlib:libcmt.lib") // exclude a lib
                #pragma comment(lib, "fbxsdk_md2005d.lib")
            #else
                #pragma comment(lib, "fbxsdk_md2005.lib" )
            #endif

        #else

            #error No FbxSdk library available for this compilation.

        #endif
    #elif defined (WIN64)
        #if defined( _MT) && !defined(_DLL) // if this is a Multi-threaded /MT or MTd project
            #ifdef _DEBUG
                #pragma comment(linker, "/nodefaultlib:libcmt.lib") // exclude a lib
                #pragma comment(lib, "fbxsdk_mt2005_amd64d.lib")
            #else
                #pragma comment(lib, "fbxsdk_mt2005_amd64.lib")
            #endif

        #elif defined( _MT) && defined(_DLL) // if this is a Multi-threaded DLL /MD or /MDd project

            #ifdef _DEBUG
                #pragma comment(linker, "/nodefaultlib:libcmt.lib") // exclude a lib
                #pragma comment(lib, "fbxsdk_md2005_amd64d.lib")
            #else
                #pragma comment(lib, "fbxsdk_md2005_amd64.lib" )
            #endif

        #else
            #error No FbxSdk library available for this compilation.
        #endif
    #endif //if win32 or win64
#elif (_MSC_VER == 1310 )

    #if defined( _MT) && !defined(_DLL) // if this is a Multi-threaded /MT or MTd project

        #ifdef _DEBUG
            #pragma comment(linker, "/nodefaultlib:libcmt.lib") // exclude a lib
            #pragma comment(lib, "fbxsdk_mt2003d.lib")
        #else
            #pragma comment(lib, "fbxsdk_mt2003.lib")
        #endif

    #elif defined( _MT) && defined(_DLL) // if this is a Multi-threaded DLL /MD or /MDd project

        #ifdef _DEBUG
            #pragma comment(linker, "/nodefaultlib:libcmt.lib") // exclude a lib
            #pragma comment(lib, "fbxsdk_md2003d.lib")
        #else
            #pragma comment(lib, "fbxsdk_md2003.lib" )
        #endif
	#elif !defined( _MT) && !defined(_DLL) // if this is a Single Thread ML

        #ifdef _DEBUG
            #pragma comment(linker, "/nodefaultlib:libc.lib") // exclude a lib
            #pragma comment(lib, "fbxsdk_ml2003d.lib")
        #else
            #pragma comment(lib, "fbxsdk_ml2003.lib" )
        #endif
    #else
        #error No FbxSdk library available for this compilation.

    #endif

#endif //VS2005

//-----------------------------------------------------------------------------------