//
////
//// Created by William.Hua on 2021/2/27.
////
//#include "libaa/fileio/aa_mp3_audio_format_reader.h"
//#include <iostream>
//using namespace std;
//int main(int argc, char* argv[])
//{
//    if(argc < 2){
//        cerr << "Usage: " << argv[0] << "/path/to/audio/file" << endl;
//        return -1;
//    }
//
//
//
//}

/** @file paex_sine.c
    @ingroup examples_src
    @brief Play a sine wave for several seconds.
    @author Ross Bencina <rossb@audiomulch.com>
    @author Phil Burk <philburk@softsynth.com>
*/
/*
 * $Id$
 *
 * This program uses the PortAudio Portable Audio Library.
 * For more information see: http://www.portaudio.com/
 * Copyright (c) 1999-2000 Ross Bencina and Phil Burk
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/*
 * The text above constitutes the entire PortAudio license; however,
 * the PortAudio community also makes the following non-binding requests:
 *
 * Any person wishing to distribute modifications to the Software is
 * requested to send the modifications to the original developer so that
 * they can be incorporated into the canonical version. It is also
 * requested that these non-binding requests be included along with the
 * license above.
 */
#include "libaa/fileio/aa_mp3_audio_format_reader.h"
#include "portaudio.h"
#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#include <thread>
using namespace std;
using namespace libaa;

#define FRAMES_PER_BUFFER  (64)

/* This routine will be called by the PortAudio engine when audio is needed.
** It may called at interrupt level on some machines so don't do anything
** that could mess up the system like calling malloc() or free().
*/
static int patestCallback( const void *inputBuffer, void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData )
{
    auto *reader = (Mp3AudioFormatReader*)userData;
    float *out = (float*)outputBuffer;

    (void) timeInfo; /* Prevent unused variable warnings. */
    (void) statusFlags;
    (void) inputBuffer;

    vector<float> left_buffer(framesPerBuffer, 0);
    vector<float> right_buffer(framesPerBuffer, 0);
    float* data_refer_to[2] = {left_buffer.data(), right_buffer.data()};
    auto read_ok = reader->readSamples(data_refer_to, 2, 0, 0, framesPerBuffer);


    for(auto i=0; i < framesPerBuffer; i++ )
    {
        *out++ = left_buffer[i];   /* left */
        *out++ = right_buffer[i];  /* right */
    }

//    if(!read_ok){
//        return paComplete;
//    }

    return paContinue;
}

/*
 * This routine is called by portaudio when playback is done.
 */
static void StreamFinished( void* userData )
{
    printf( "Stream Completed: %s\n", "data->message" );
}

/*******************************************************************/

int main(int argc, char* argv[])
{
    if(argc < 2){
        cerr << "Usage: " << argv[0] << "/path/to/audio/file" << endl;
        return -1;
    }

    const auto input_file_path = string(argv[1]);

    ifstream in_stream(input_file_path);
    Mp3AudioFormatReader reader(in_stream);
    if(!reader.isOpenOk()){
        cerr << "open file failed\n";
        return -1;
    }

    const auto sample_rate = reader.sample_rate;
    const auto num_channels = reader.num_channels;
    const auto num_frames = reader.length_in_samples;
    const auto duration = double(num_frames) / sample_rate;
    assert(num_channels == 2);


    PaStreamParameters outputParameters;
    PaStream *stream;
    PaError err;

    err = Pa_Initialize();
    if( err != paNoError ) goto error;

    outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
    if (outputParameters.device == paNoDevice) {
        fprintf(stderr,"Error: No default output device.\n");
        goto error;
    }
    outputParameters.channelCount = 2;       /* stereo output */
    outputParameters.sampleFormat = paFloat32; /* 32 bit floating point output */
    outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    err = Pa_OpenStream(
        &stream,
        NULL, /* no input */
        &outputParameters,
        sample_rate,
        FRAMES_PER_BUFFER,
        paClipOff,      /* we won't output out of range samples so don't bother clipping them */
        patestCallback,
        &reader );
    if( err != paNoError ) goto error;

    err = Pa_SetStreamFinishedCallback( stream, &StreamFinished );
    if( err != paNoError ) goto error;

    err = Pa_StartStream( stream );
    if( err != paNoError ) goto error;

    printf("Play for %lf seconds.\n", duration );
    Pa_Sleep( duration * 1000 );

    err = Pa_StopStream( stream );
    if( err != paNoError ) goto error;

    err = Pa_CloseStream( stream );
    if( err != paNoError ) goto error;

    Pa_Terminate();
    printf("Test finished.\n");

    return err;
    error:
    Pa_Terminate();
    fprintf( stderr, "An error occurred while using the portaudio stream\n" );
    fprintf( stderr, "Error number: %d\n", err );
    fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
    return err;
}
