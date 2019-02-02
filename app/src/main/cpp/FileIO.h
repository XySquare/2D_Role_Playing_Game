//
// Created by Xyy on 2018/12/31.
//

#ifndef RPG2D_FILEIO_H
#define RPG2D_FILEIO_H


#include <stdio.h>
#include <string>

class FileIO {

private:

    const std::string externalFilesDir;

    AAssetManager *const aAssetManager;

public:

    FileIO(const char *externalFilesDir, AAssetManager *aAssetManager) : externalFilesDir(
            externalFilesDir), aAssetManager(aAssetManager) {}

    std::string getExternalFilePath(const char *fileName) {

        return externalFilesDir + fileName;
    }

    /**
    * Read file from Asset folder
    * Note: The object returned here should be freed by calling AAsset_close().
    */
    AAsset *readAssetFile(const char *fileName) {

        return AAssetManager_open(aAssetManager, fileName, AASSET_MODE_BUFFER);
    }

};


#endif //RPG2D_FILEIO_H
