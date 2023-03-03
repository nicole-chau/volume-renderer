#include "fileloader.h"
#include "dcmtk/dcmdata/dcdeftag.h"
#include <dcmtk/ofstd/offile.h>

#include <QDebug>

FileLoader::FileLoader(std::string directory)
    : directory(directory)
{
    // Get all files in directory
    for (const auto &file : std::filesystem::directory_iterator(directory))
    {

        files.push_back(file.path());
    }

    // Sort files since directory_iterator operates in arbitrary order
    sort(files.begin(), files.end());
}

FileLoader::~FileLoader() {}

double FileLoader::getRescaleIntercept()
{
    std::string file = files[0];

    DcmFileFormat fileformat;
    OFCondition status = fileformat.loadFile(file.c_str());

    double rescaleIntercept = 0.0;

    if (status.good())
    {
        DcmTagKey key = DCM_RescaleIntercept;
        if (fileformat.getDataset()->findAndGetFloat64(key, rescaleIntercept).good())
        {
            qDebug() << "Rescale Intercept: " << rescaleIntercept;
            return rescaleIntercept;
        }
    }

    return -1024.0;
}

double FileLoader::getRescaleSlope()
{
    std::string file = files[0];

    DcmFileFormat fileformat;
    OFCondition status = fileformat.loadFile(file.c_str());

    double rescaleSlope = 0.0;

    if (status.good())
    {
        DcmTagKey key = DCM_RescaleSlope;
        if (fileformat.getDataset()->findAndGetFloat64(key, rescaleSlope).good())
        {
            qDebug() << "Rescale Slope: " << rescaleSlope;
            return rescaleSlope;
        }
    }

    return 1.0;
}

void FileLoader::processPixelData()
{
    // Iterate through all files
    for (const std::string &file : files) {
        DicomImage *image = new DicomImage(file.c_str());

        if (image != NULL)
        {
            if (image->getStatus() == EIS_Normal)
            {
                // Get image size aka number of pixels
                DcmFileFormat fileFormat;
                OFCondition status = fileFormat.loadFile(file.c_str());
                Uint16 height = 0;
                Uint16 width = 0;

                fileFormat.getDataset()->findAndGetUint16(DCM_Rows, height).good();
                fileFormat.getDataset()->findAndGetUint16(DCM_Columns, width).good();

                int numPixels = height * width;

                // Get pixel data
                short *pixelData = (short *)(image->getOutputData(16 /* bits per sample */));

                if (pixelData != NULL)
                {
                    for (int i = 0; i < numPixels; ++i)
                    {
                        // Convert each pixel to Hounsfield unit
                        // hu = pixel_value * slope + intercept
                        double slope = getRescaleSlope();
                        double intercept = getRescaleIntercept();
                        double hu = pixelData[i] * slope + intercept;
                    }

                        qDebug() << pixelData[0];


                }
            } else
            {
                std::cout << "cannot load file " << std::endl;
            }
        }
    }
}
