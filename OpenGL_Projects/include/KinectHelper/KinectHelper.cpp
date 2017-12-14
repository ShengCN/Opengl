#include "KinectHelper.h"

// Kinect variables
IKinectSensor* sensor;
IColorFrameReader* color_reader;
IDepthFrameReader* depth_reader;
IMultiSourceFrameReader* reader;
ICoordinateMapper* mapper;

// Body informaiton
BOOLEAN istracked;
Joint joints[JointType_Count];


bool InitKinect()
{
	if (FAILED(GetDefaultKinectSensor(&sensor)))
		return false;
	
	if (sensor)
	{
		
		sensor->get_CoordinateMapper(&mapper);

		sensor->Open();
		sensor->OpenMultiSourceFrameReader(
			FrameSourceTypes::FrameSourceTypes_Depth |
			FrameSourceTypes::FrameSourceTypes_Color |
			FrameSourceTypes::FrameSourceTypes_Body,
			&reader);
		return reader;
	}
	else
		return false;
}

void GetKinectData(Joint*& js, int& jointSize, bool& isT)
{
	IMultiSourceFrame* frame = nullptr;
	if (SUCCEEDED(reader->AcquireLatestFrame(&frame)))
	{
		GetBodyData(frame);
	}

	js = joints;
	isT = istracked;
	jointSize = JointType_Count;

	if (frame)
		frame->Release();

}

void GetBodyData(IMultiSourceFrame* frame)
{
	IBodyFrame* bodyFrame;
	IBodyFrameReference* frameref = nullptr;
	frame->get_BodyFrameReference(&frameref);
	frameref->AcquireFrame(&bodyFrame);
	if (frameref) frameref->Release();

	if (!bodyFrame) return;

	IBody* body[BODY_COUNT] = { 0 };
	bodyFrame->GetAndRefreshBodyData(BODY_COUNT, body);
	for (int i = 0; i < BODY_COUNT; ++i)
	{
		body[i]->get_IsTracked(&istracked);
		if (istracked)
		{
			body[i]->GetJoints(JointType_Count, joints);
			break;
		}
	}

	if (bodyFrame) bodyFrame->Release();
}