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

FileLoader::~FileLoader()
{

}

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
            return rescaleIntercept;
        }
    }

    return 0.0;
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

    return 0;
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
//                Uint8 *pixelData = (Uint8 *)(image->getOutputData(8 /* bits per sample */));

                // getInterData() returns original pixel data which includes Houndsfield units
                const DiPixel *interData = image->getInterData();

                if (interData != NULL)
                {
                    // Process pixel data
//                    qInfo() << "number of pixels: " << interData->getCount();

                    int samplesPerPixel = interData->getPlanes(); // returns 1 --> monochromatic

                    // Get pointer to internal raw representation of image data
                    const void *data = interData->getData();

                    // Each pixel is a short (signed 16 bit integer)
                    int rep = interData->getRepresentation();
                    if (rep == EPR_Sint16)
                    {
                        qDebug() << ("EPR_Sint16");
                    }

                    std::vector<short> pixelData;

//                    DiMonoPixelTemplate<Sint16> pixelTemplate = new DiMonoPixelTemplate<Sint16>


//                    qDebug() << rep;


                }
            } else
            {
                std::cout << "cannot load file: " << std::endl;
            }
        }
    }
}
