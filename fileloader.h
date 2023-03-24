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

    std::vector<std::vector<double>> data;
    Uint16 width;
    Uint16 height;

//    Uint8 *pixelData;

private:
    void getColumns(DcmFileFormat fileFormat);
    void getRows(DcmFileFormat fileFormat);
    void getRescaleIntercept(DcmFileFormat fileFormat);
    void getRescaleSlope(DcmFileFormat fileFormat);
    void processPixelData();

    void getPixelData();

    std::vector<std::string> files;

    int numPixels;
    double rescaleIntercept;
    double rescaleSlope;
};

#endif // FILELOADER_H
