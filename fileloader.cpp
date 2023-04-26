#include "fileloader.h"
#include "dcmtk/dcmdata/dcdeftag.h"
#include <dcmtk/ofstd/offile.h>

#include <QDebug>

FileLoader::FileLoader(std::string directory)
{
    // Get all files in directory
    for (const auto &file : std::filesystem::directory_iterator(directory))
    {

        files.push_back(file.path());
    }

    // Sort files since directory_iterator operates in arbitrary order
    sort(files.begin(), files.end());

    // Get tags
    std::string file = files[0];

    DcmFileFormat fileFormat;
    OFCondition status = fileFormat.loadFile(file.c_str());

    if (status.good())
    {
        getColumns(fileFormat);
        getRows(fileFormat);
        getRescaleIntercept(fileFormat);
        getRescaleSlope(fileFormat);

        numPixels = width * height;
    }

    // Process pixel data
    processPixelData();
}

FileLoader::~FileLoader() {}

void FileLoader::getColumns(DcmFileFormat fileFormat)
{
    DcmTagKey key = DCM_Columns;
    if (fileFormat.getDataset()->findAndGetUint16(key, width).good())
    {
        return;
    } else
    {
        width = 0;
    }
}

void FileLoader::getRows(DcmFileFormat fileFormat)
{
    DcmTagKey key = DCM_Rows;
    if (fileFormat.getDataset()->findAndGetUint16(key, height).good())
    {
        return;
    } else
    {
        height = 0;
    }
}

void FileLoader::getRescaleIntercept(DcmFileFormat fileFormat)
{
    DcmTagKey key = DCM_RescaleIntercept;
    if (fileFormat.getDataset()->findAndGetFloat64(key, rescaleIntercept).good())
    {
        return;
    } else
    {
        rescaleIntercept = -1024.0;
    }
}

void FileLoader::getRescaleSlope(DcmFileFormat fileFormat)
{
    DcmTagKey key = DCM_RescaleSlope;
    if (fileFormat.getDataset()->findAndGetFloat64(key, rescaleSlope).good())
    {
//        qDebug() << "Rescale Slope: " << rescaleSlope;
        return;
    } else
    {
        rescaleSlope = 1.0;
    }
}

void FileLoader::processPixelData()
{
    int count = 1;

    // Iterate through all files
    for (const std::string &file : files) {
        DicomImage *image = new DicomImage(file.c_str());

        if (image != NULL)
        {
            if (image->getStatus() == EIS_Normal)
            {
                // Initialize vector for current slice
                std::vector<double> slice;

                // Get pixel data
                // FOR DEBUGGING: change to Uint8 to get grayscale RGB value
                short *pixelData = (short *)(image->getOutputData(16 /* bits per sample */));

//                const DiPixel *interData = image->getInterData();
//                EP_Representation rep = interData->getRepresentation();
//                Sint16 *pixelData = (Sint16 *)interData->getData();

                if (pixelData != NULL)
                {
                    for (int i = 0; i < numPixels; ++i)
                    {
                        // FOR DEBUGGING: get RGB value
                        // pixelData[i] = pixelData[i] * 32 ;

                        // Convert each pixel to Hounsfield unit: hu = pixel_value * slope + intercept
                        double hu = pixelData[i] * rescaleSlope + rescaleIntercept;
                        slice.push_back(hu);
                    }
                }

//                double min = *min_element(std::begin(slice), std::end(slice));
//                double max = *max_element(std::begin(slice), std::end(slice));

                data.push_back(slice);

            } else
            {
                qDebug() << "Cannot load file " << count;
            }
        }

        qDebug() << "File " << count << "/" << files.size() << " done processing.";
        count++;
    }
}
