// Main file

#include "stdafx.h"
#include "VideoEncoder.h"
#include <windows.h>
#include "Settings.h"

// Create test video frame
void CreateFrame(char * buffer, int w, int h, int bytespan)
{
	int wxh = w * h;
	static float seed = 1.0;
	for (int i = 0; i < h; i ++)
	{
		char* line = buffer + i * bytespan;
		for (int j = 0; j < w; j ++)
		{
			// RGB
			line[0] = 255 * sin(((float)i / wxh * seed) * 3.14);
			line[1] = 255 * cos(((float)j / wxh * seed) * 3.14);
			line[2] = 255 * sin(((float)(i + j) / wxh * seed) * 3.14);
			line += 3;
		}
	}
	seed = seed + 2.2;
}

// Create sample
void CreateSample(short * buffer, int sampleCount)
{
  static float shift   = 0.0;
  static float seconds = 0.0;

  const float  minNu   = 3.14 / (44100.0f) * 700.0f;
  const float  maxNu   = 3.14 / (44100.0f) * 1500.0f;
  const float  speedNu = 3.14 / (44100.0f) * 10.0f;

  static float varNu = minNu;

  if (varNu > maxNu)
  {
	varNu = minNu;
  }

  varNu += speedNu;

  for (int i = 0; i < sampleCount; i ++)
  {
	seconds       += 1.0f / 44100.0f;

	buffer [i] = sin(i * varNu + shift) * 0x4FFF;
  }
  shift = shift + varNu * sampleCount;
}

int _tmain(int argc, _TCHAR* argv[])
{
	VideoEncoder encoder;

	if (encoder.InitFile(std::string(FILE_NAME), std::string(CONTAINER)))
	{
		int w = W_VIDEO;
		int h = H_VIDEO;
		AVFrame* frame = avcodec_alloc_frame();
		int nSampleSize = 2 * 44100.0f / 25.0f; // 1 / 25 sec * FORMAT SIZE(S16)
		char* sample = new char[nSampleSize];
		// Create frame
		int bufferImgSize = avpicture_get_size(PIX_FMT_BGR24, w, h);
		uint8_t * buffer = (uint8_t*)av_mallocz(bufferImgSize);
		avpicture_fill((AVPicture*)frame, buffer, PIX_FMT_BGR24, w, h);

		for (int i = 0; i < FRAME_COUNT; i++)
		{      
			CreateFrame((char *)frame->data[0], w, h, frame->linesize[0]);
			CreateSample((short *)sample, nSampleSize / 2);
			if (!encoder.AddFrame(frame, sample, nSampleSize))
			{
				printf("Cannot write frame\n");
			} 
		}

		encoder.Finish();
		av_free(frame->data[0]);
		av_free(frame);
		delete[] sample;
		sample = NULL;
	}
	else
	{
		printf ("Cannot open file " FILE_NAME "\n");
	}

	return 0;
}

