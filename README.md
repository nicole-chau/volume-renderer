# Volumetric Renderer for Medical Imaging

This project is an implementation of a volumetric renderer that can be used to view medical images (CT scans). 
The goal is to provide a tool for visualizing 2D medical images in 3D space and provide interactive features for transforming
the data or highlighting certain parts of the data using different colors. 

## Features
- Load in DICOM dataset (directory containing series of `.dcm` files)
- Volumetric rendering of loaded dataset (grayscale by default)
- Ability to translate and rotate the dataset to view it from various angles
- Option to apply a RGB transfer function to a custom-defined range of Hounsfield unit values

<img width="700" alt="main-ui" src="https://user-images.githubusercontent.com/74137085/234706856-ad9ac68d-c2e3-4d50-a74c-39f0ee7b8109.png">
<img width="700" alt="rgb-map" src="https://user-images.githubusercontent.com/74137085/234707733-823087b2-3f05-4ecd-b5b1-cbc95d8b6ed8.png">

The dataset loaded in the above screenshots was taken from [FigShare](https://figshare.com/articles/dataset/Patient34/6265679?backTo=/collections/FUMPE/4107803)
and was uploaded by Mojtaba Masoudi.

## Implementation Overview

This was implemented using Qt and C++.

### Data Loading and Processing
The application allows a user to load in a series of DICOM images. 
Once the directory is selected, the file tags are then parsed to identify the width, 
height, rescale intercept and rescale slope values. For each file, the raw pixel values are read and 
converted to Hounsfield units (HU) with the following formula :
$$\text{HU} = \text{pixelValue} * \text{rescaleSlope} + \text{rescaleIntercept}$$
Note that Hounsfield units are a relative quantitative measurement of radio density used in CT scans. 

### Volume Rendering
The volume rendering is implemented through a ray-cast approach. For each pixel on the screen, a ray is cast from the
camera origin to the pixel and through the voxel data. If the ray intersects the data volume, then the data is
sampled along the ray with grid marching. For each sample, the transmittance $T$ and color $C$ is updated based on the
density (calculated from Hounsfield unit) and color at the current voxel. If the data is to be rendered
using a RGB color map, then voxels with Hounsfield unit values falling within the selected range will be linearly 
interpolated between five predefined colors (yellow to red) to obtain the voxel color. 

## Third Party Libraries Used
- DCMTK: for loading and processing of DICOM files
- ctkRangeSlider: for implementing a range slider in Qt
