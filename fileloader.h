#ifndef FILELOADER_H
#define FILELOADER_H

#include <vector>
#include <filesystem>
#include <dcmtk/dcmimgle/dcmimage.h>
#include <dcmtk/dcmdata/dcfilefo.h> // DcmFileFormat
#include <dcmtk/ofstd/offile.h>



class FileLoader
{
public:
    FileLoader(std::string directory);
    ~FileLoader();
    void processPixelData();

    void getTags();

private:
    std::string directory; // directory storing series of DICOM files
    std::vector<std::string> files;
};

#endif // FILELOADER_H
