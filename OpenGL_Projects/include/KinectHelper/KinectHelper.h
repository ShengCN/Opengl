#pragma once
#include <Windows.h>
#include <Ole2.h>
#include <Kinect.h>
#include <unordered_map>

/// To use kinect helper
// Step 1. InitKinect() before using it
// Step 2. By using GetKinectData(), you can get current joints information
bool InitKinect();
void GetKinectData(Joint*&, int& jointSize, bool& isTracked);

void GetBodyData(IMultiSourceFrame* frame);