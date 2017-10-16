#include "OS.hpp"

#include <iostream>
#include <dirent.h>

#ifdef __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#include <CoreFoundation/CFBundle.h>
#endif




std::string OS::mPwd="";

OS::OS() {
}

OS::~OS() {
}

std::string OS::getPwd() {
	//if it's already set, just return it
		if(mPwd.size() > 0){
			return mPwd;
		}

		//otherwise determine the pwd
		#ifdef _WIN64
		    mPwd = "unknown";
		    assert(false); //Needs implementation
		#elif _WIN32
		    char* temp = _getcwd( NULL, 1022 );
		    std::string path(temp);
		    path = path + "\\";
		    free( temp );
		#elif __APPLE__
			#if TARGET_OS_MAC
                char path[1024];
                sprintf( path, "%s", getenv("PWD") );
    
//			   char path[1024];
//                for (int i=0; i<1024; i++) {
//                    path[i] = '\0';
//                }
//    
//			   uint32_t size = sizeof(path);
//			   if (_NSGetExecutablePath(path, &size) == 0){
////				   std::strrchr(path, '/')[0] = '\0';
////				   std::strcat(path,"/\0");
//				   mPwd = std::string(path);
//			   } else {
//				   mPwd = "";
//			   }

                mPwd = std::string(path);
    
			#endif
		#endif
    
        // ----------------------------------------------------------------------------
        // This makes relative paths work in C++ in Xcode by changing directory to the Resources folder inside the .app bundle
//        #elif __APPLE__
//            CFBundleRef mainBundle = CFBundleGetMainBundle();
//            CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
//            char path[PATH_MAX];
//            if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, PATH_MAX))
//            {
//                // error!
//
//            }
//            CFRelease(resourcesURL);
//            
//            chdir(path);
//            std::cout << "Current Path: " << path << std::endl;
//            mPwd = std::string(_NSGetExecutablePath);
//            #endif
        // ----------------------------------------------------------------------------


	   DIR* dirExists = opendir((mPwd + "Resources/").c_str());
		if(!dirExists){
			mPwd = "";
		}

	    return mPwd;
}
