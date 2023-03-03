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



private:
    void getColumns(DcmFileFormat fileFormat);
    void getRows(DcmFileFormat fileFormat);
    void getRescaleIntercept(DcmFileFormat fileFormat);
    void getRescaleSlope(DcmFileFormat fileFormat);

//    std::string directory; // directory storing series of DICOM files
    std::vector<std::string> files;

    Uint16 width;
    Uint16 height;
    int numPixels;
    double rescaleIntercept;
    double rescaleSlope;
};

#endif // FILELOADER_H
