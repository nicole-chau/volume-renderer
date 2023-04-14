#pragma once

#include <map>
#include "globals.h"

#define AIR_MIN -1024
#define AIR_MAX -996

#define LUNG_MIN -800
#define LUNG_MAX -500

#define BONE_MIN 300
#define BONE_MAX 3072

enum Substance {
   AIR, LUNG, FAT, WATER, BLOOD, BONE, UNKNOWN
};

static const std::map<int, std::tuple<float, float>> grayscale = {
    {AIR, std::tuple<float, float>(0.f, 0.1f)},
    {LUNG, std::tuple<float, float>(0.2f, 0.5f)},
    {BONE, std::tuple<float, float>(0.5f, 0.9f)},
};

//https://coolors.co/d00000-e24603-f58d05-faa307-ffcd50
// Yellow to red
static const std::vector<Vector3f> colorScale = {
//    Vector3f(1.f, 0.f, 0.f),
//    Vector3f(1.f, 0.f, 0.f),
//    Vector3f(0.f, 1.f, 0.f),
//    Vector3f(0.f, 1.f, 0.f),
//    Vector3f(0.f, 1.f, 0.f),
    Vector3f(255.f, 205.f, 80.f) / 255.f,
    Vector3f(250.f, 163.f, 7.f) / 255.f,
    Vector3f(245.f, 141.f, 5.f) / 255.f,
    Vector3f(226.f, 70.f, 3.f) / 255.f,
    Vector3f(208.f, 0.f, 0.f) / 255.f,
};


