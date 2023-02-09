#include "fileloader.h"

FileLoader::FileLoader(std::string directory)
    : directory(directory)
{
    // Get all files in directory
    for (const auto &file : std::filesystem::directory_iterator(directory))
    {
        files.push_back(file.path());
    }

}

FileLoader::~FileLoader()
{

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
                const DiPixel *pixelData = image->getInterData();

                if (pixelData != NULL)
                {
                    // Process pixel data
                    std::cout << "number of pixels: " << pixelData->getCount() << std::endl;
                }
            } else
            {
                std::cout << "cannot load file: " << std::endl;
            }
        }
    }
}
