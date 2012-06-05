/*
 * main.cpp
 *
 *  Created on: 01.06.2012
 *      Author: felixwunsch
 */

#include "faac.h"
#include "drm/Vector.h"
#include <iostream>
#include <fstream>


int main()
{
	/* declare input and output buffers */
	CVectorEx < _SAMPLE > vecInputData;
	CVectorEx < _SAMPLE >* pvecInputData = &vecInputData;
	CVectorEx < _BINARY > vecOutputData;
	CVectorEx < _BINARY >* pvecOutputData = &vecOutputData;
	CVectorEx < _BINARY > vecOutputTmp;
	CVector<_BYTE>			aac_crc_bits;
	CVector<_SAMPLE>		vecsEncInData;
	CMatrix<_BYTE>			audio_frame;
	CVector<int>			veciFrameLength;

	/* open encoder instance */
	unsigned long lEncSampRate = 24000;
	unsigned int numChannels = 1;
	unsigned long lNumSampEncIn;

	unsigned long lMaxBytesEncOut;

	faacEncHandle encHandle;
	encHandle = faacEncOpen(lEncSampRate, numChannels, &lNumSampEncIn, &lMaxBytesEncOut);
	std::cout << "lNumSampEncIn:\t" << lNumSampEncIn << std::endl;
	std::cout << "lMaxBytesEncOut:\t" << lMaxBytesEncOut << std::endl;
	if(encHandle == NULL)
	{
		std::cout << "Failed to open FAAC encoder instance!" << std::endl;
	}

	/* read PCM stream from file */
	ifstream pcm_in;
	pcm_in.open("raw_pcm.dat", ios::in | ios::binary);
	if(!pcm_in.is_open())
	{
        std::cout << "failed to open file!" << std::endl;
        return 1;
	}
	else
	{
        std::cout << "PCM file is opened." << std::endl;
	}

	// get length of file for preallocation
	pcm_in.seekg (0, ios::end);
    int length = pcm_in.tellg();
    int transform_length = 960; // FAAC transform length
    int blocks = length/960; // number of input blocks for FAAC encoder
    int superframes = blocks/10; // number of superframes (400ms) TODO: declare iNumAACFrames earlier!
    std::cout << "length:\t" << length << " blocks:\t" << blocks << " superframes:\t" << superframes << std::endl;
    pcm_in.seekg (0, ios::beg);

    // declare buffer

    uint8_t* buffer;
    buffer = new uint8_t[superframes*10*transform_length];

    // read data as a block:
    //pcm_in.read ((char*)buffer,length);
    pcm_in.read ((char*)buffer,superframes*10*transform_length);
    pcm_in.close();

	/* init input and output buffers */
	int iNumAACFrames = 10; // we use 24 kHz sampling NOTE: Dream would have used 12 kHz -> 5 AAC frames
	int iInputBlockSize = superframes*iNumAACFrames*transform_length; // integer number of audio super frames
	//int iInputBlockSize = length;
	//int iOutputBlockSize = 5826;
	int iOutputBlockSize = 5826 * superframes; // see AudioSourceDecoder InitInteralTx()
	std::cout << "iInputBlockSize / iOutputBlockSize:\t" << iInputBlockSize << " / " << iOutputBlockSize << std::endl;
	pvecInputData->Init(iInputBlockSize);
	pvecOutputData->Init(iOutputBlockSize);
	std::cout << "Output length at init:\t" << (*pvecOutputData).size() << std::endl;


	/* fill buffers with data */
	std::cout << "Filling buffers with data...\n";
	for(int i = 0; i<iInputBlockSize; i++)
	{
		(*pvecInputData)[i] = (_SAMPLE)buffer[i];
	}

	for(int i = 0; i<iOutputBlockSize; i++)
	{
		(*pvecOutputData)[i] = 0; // unused bytes have to be set to zero
	}

    /* Reset bit extraction access */
    (*pvecOutputData).ResetBitAccess();

	/* calculate bit rate */
	int sizeof_byte = 8;
	int iNumDecodedBitsMSC = 5826;
	int iTotNumBitsForUsage = (iNumDecodedBitsMSC / sizeof_byte) * sizeof_byte;
	int iTotNumBytesForUsage = iTotNumBitsForUsage / sizeof_byte;
	std::cout << "iTotNumBytesForUsage:\t" << iTotNumBytesForUsage << std::endl;
	int iTotAudFraSizeBits = iTotNumBitsForUsage; // no text message included
	int iTimeEachAudBloMS = 40;
	int iNumHeaderBytes = 14;


	int iAudioPayloadLen = iTotAudFraSizeBits / sizeof_byte - iNumHeaderBytes - iNumAACFrames /* for CRCs */ ;
	const int iActEncOutBytes = (int) (iAudioPayloadLen / iNumAACFrames);
	std::cout << "iActEncOutBytes:\t" << iActEncOutBytes << std::endl;
	int iBitRate = (int) (( iActEncOutBytes * sizeof_byte) / iTimeEachAudBloMS * 1000);
	std::cout << "iBitRate:\t" << iBitRate << std::endl;
	std::cout << "iAudioPayloadLen:\t" << iAudioPayloadLen << std::endl;

	/* set encoder configuration */
	faacEncConfigurationPtr CurEncFormat;
	CurEncFormat = faacEncGetCurrentConfiguration(encHandle);
	CurEncFormat->inputFormat = FAAC_INPUT_16BIT;
	CurEncFormat->useTns = 1;
	CurEncFormat->aacObjectType = LOW;
	CurEncFormat->mpegVersion = MPEG4;
	CurEncFormat->outputFormat = 0;	/* (0 = Raw; 1 = ADTS) */
	CurEncFormat->bitRate = iBitRate;
	CurEncFormat->bandWidth = 0;	/* Let the encoder choose the bandwidth */
	faacEncSetConfiguration(encHandle, CurEncFormat);

	/* Init storage for actual data, CRCs and frame lengths */
	audio_frame.Init(iNumAACFrames, lMaxBytesEncOut);
	vecsEncInData.Init(lNumSampEncIn);
	aac_crc_bits.Init(iNumAACFrames);
	veciFrameLength.Init(iNumAACFrames);

	/* actual encoding */
	for(int n = 0; n<superframes; n++) // first run is mainly init...
	{
		std::cout << "start of super audio frame" << std::endl;
		for (int j = 0; j < iNumAACFrames; j++)
		{
			/* copy the part of the input data that is converted in this iteration */
			for (unsigned long k = 0; k < lNumSampEncIn; k++)
			{
				vecsEncInData[k] = (*pvecInputData)[n*lNumSampEncIn*iNumAACFrames + j*lNumSampEncIn + k];
			}

			CVector < unsigned char >vecsTmpData(lMaxBytesEncOut);
			std::cout << "encoding... ";
			int bytesEncoded = faacEncEncode(encHandle, (int32_t*) &vecsEncInData[0], lNumSampEncIn, &vecsTmpData[0], lMaxBytesEncOut);
			std::cout << "bytesEncoded:\t" << bytesEncoded << std::endl;

			if (bytesEncoded > 0)
			{
				/* Extract CRC */
				aac_crc_bits[j] = vecsTmpData[0];

				/* Extract actual data */
				for (int i = 0; i < bytesEncoded - 1 /* "-1" for CRC */ ; i++)
					audio_frame[j][i] = vecsTmpData[i + 1];

				/* Store block lengths for boarders in AAC super-frame-header */
				veciFrameLength[j] = bytesEncoded - 1;
			}
			else
			{
				/* Encoder is in initialization phase, reset CRC and length */
				aac_crc_bits[j] = 0;
				veciFrameLength[j] = 0;
			}
		}

        /* AAC super-frame-header */
		int bits_written = 0;
        int iAccFrameLength = 0;
        for (int j = 0; j < iNumAACFrames - 1; j++)
        {
            iAccFrameLength += veciFrameLength[j];

            /* Frame border in bytes (12 bits) */
            std::cout << "iAccFrameLength:\t" << iAccFrameLength << std::endl;
            (*pvecOutputData).Enqueue(iAccFrameLength, 12);
            bits_written += 12;
        }

        /* Byte-alignment (4 bits) in case of 10 audio frames */
        if (iNumAACFrames == 10)
        {
            (*pvecOutputData).Enqueue(0, 4);
            bits_written += 4;
        }

        /* Higher protected part */
        int iCurNumBytes = 0;
        int iNumHigherProtectedBytes = 0; // EEP
        for (int j = 0; j < iNumAACFrames; j++)
        {
            /* Data */
            for (int i = 0; i < iNumHigherProtectedBytes; i++)
            {
                /* Check if enough data is mpavailable, set data to 0 if not */
                if (i < veciFrameLength[j])
                    (*pvecOutputData).Enqueue(audio_frame[j][i], 8);
                else
                    (*pvecOutputData).Enqueue(0, 8);

                bits_written += 8;
                iCurNumBytes++;
            }

            /* CRCs */
            (*pvecOutputData).Enqueue(aac_crc_bits[j], 8);
            bits_written += 8;
        }

        /* Lower protected part */
        for (int j = 0; j < iNumAACFrames; j++)
        {
            for (int i = iNumHigherProtectedBytes; i < veciFrameLength[j]; i++)
            {
                /* If encoder produced too many bits, we have to drop them */
                if (iCurNumBytes < iAudioPayloadLen){
                    (*pvecOutputData).Enqueue(audio_frame[j][i], 8);
                    bits_written += 8;
                }
                iCurNumBytes++;
            }
        }
        std::cout << "bits_written:\t" << bits_written << std::endl;

        /* append zeros until the end of the audio super frame */
        (*pvecOutputData).Enqueue(0, 5826-bits_written);
	}

	/* write output to file */
	std::cout << "output size before writing to file:\t" << (*pvecOutputData).size() << std::endl;
	std::cout << "write output to file" << std::endl;
	ofstream file;
	file.open("audiosuperframe.dat", ios::out | ios::binary);
	if(file.is_open())
	{
		for(int l = 0; l<iOutputBlockSize; l++)
		{
			file.write((const char*) &((*pvecOutputData)[l]), 1);
		}
		file.close();
		std::cout << "super audio frames have been written to file" << std::endl;
	}
	else
	{
		std::cout << "Failed to open file!" << std::endl;
	}

	return 0;
}

