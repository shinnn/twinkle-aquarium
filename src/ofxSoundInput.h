#pragma once

#include "ofMain.h"

// SoundInput
// from audioInputExample
class ofxSoundInput : ofBaseSoundInput
{
	ofSoundStream soundStream;
	
	vector <float> left;
	vector <float> right;
	float smoothedVol;
	
public:
	void setup()
	{
		int bufferSize = 256;
		
		left.assign(bufferSize, 0.0);
		right.assign(bufferSize, 0.0);
		
		smoothedVol = 0.0;
		soundStream.setup( 0, 2, 44100, bufferSize, 4 );
		soundStream.setInput( this );
	}
public:
	float getVolume()
	{
		float ret = smoothedVol * 3.0f;
		if ( ret > 1.0f ) ret = 1.0f;
		return ret;
	}
	float getVolumeLog()
	{
		float ret = smoothedVol * 3.0f;
		if ( ret > 1.0f ) ret = 1.0f;
		ret = log( ret );
		ret /= 5.0f;
		ret += 1.0f;
		if ( ret < 0.0f ) ret = 0.0f;
		
		return ret;
	}
  float getVolumeLog(bool b)
	{
    b = !b;
		return getVolumeLog();
	}
	
	void audioIn( float * input, int bufferSize, int nChannels )
	{
		float curVol = 0.0;
		int numCounted = 0;	
		
		for ( int i=0; i<bufferSize; i++ )
		{
			left[i]		= input[i*2]*0.5;
			right[i]	= input[i*2+1]*0.5;
			
			curVol += left[i] * left[i];
			curVol += right[i] * right[i];
			numCounted+=2;
			
		}
		curVol /= (float)numCounted;
		curVol = sqrt( curVol );
		smoothedVol *= 0.93;
		smoothedVol += 0.07 * curVol;
	}
	
};
