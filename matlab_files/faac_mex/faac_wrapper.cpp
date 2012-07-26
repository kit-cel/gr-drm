/*********************************************************************
 * faac_wrapper.cpp
 *
 * syntax = [aac_data n_stf] = faac_wrapper(fs, n_samples, pcm_data)
 *
 * The input .wav file has to match the sample rate given and must be 16 bit signed.
 *
 * It is assumed that there is no text message included and EEP is used.
 * 3 transmission frames per super transmission frame are assumed.
 * Output is a matrix with n_stf rows and 3*iNumDecodedBitsMSC columns.
 * 
 * Keep in mind:
 * <> Use 0-based indexing as always in c or c++
 * <> Indexing is column-based as in Matlab (not row-based as in C)
 * <> Use linear indexing.  [x*dimy+y] instead of [x][y]
 *
 ********************************************************************/
#include <matrix.h>
#include <mex.h>   

#include "libs/faac.h"
#include "drm/Vector.h"

/* Definitions to keep compatibility with earlier versions of ML */
#ifndef MWSIZE_MAX
typedef int mwSize;
typedef int mwIndex;
typedef int mwSignedIndex;

#if (defined(_LP64) || defined(_WIN64)) && !defined(MX_COMPAT_32)
/* Currently 2^48 based on hardware limitations */
# define MWSIZE_MAX    281474976710655UL
# define MWINDEX_MAX   281474976710655UL
# define MWSINDEX_MAX  281474976710655L
# define MWSINDEX_MIN -281474976710655L
#else
# define MWSIZE_MAX    2147483647UL
# define MWINDEX_MAX   2147483647UL
# define MWSINDEX_MAX  2147483647L
# define MWSINDEX_MIN -2147483647L
#endif
#define MWSIZE_MIN    0UL
#define MWINDEX_MIN   0UL
#endif

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    /* check  arguments ***************************************************/
    if(nrhs!=3) 
      mexErrMsgIdAndTxt( "MATLAB:faac_wrapper:invalidNumInputs",
              "Three inputs required.");
    
    else if(nlhs != 2) 
      mexErrMsgIdAndTxt( "MATLAB:faac_wrapper:invalidNumOutputs",
              "Two outputs required.");

    /* declare variables *************************************************/
    // input variables
    unsigned long lEncSampRate = mxGetScalar(prhs[0]); // sampling frequency
    int n_samples;
    n_samples = mxGetScalar(prhs[1]); // number of input samples
    short* pcm;
    pcm = (short*) mxGetPr(prhs[2]);
    
    // output variables
    int n_stf; // number of super transmission frames
    double* aac;
    
    // other variables
    int iNumAACFrames;
    int iTimeEachAudBloMS;
    int iNumHeaderBytes;
    
    switch(lEncSampRate)
    {
            case 12000:
                iNumAACFrames = 5;
                iTimeEachAudBloMS = 80;
                iNumHeaderBytes = 6;
                break;
            case 24000:
                iNumAACFrames = 10;
                iTimeEachAudBloMS = 40;
                iNumHeaderBytes = 14;  
                break;
            default:
                mexErrMsgIdAndTxt( "MATLAB:faac_wrapper:invalid_fs",
              "fs must be either 12000 or 24000.");
    }
    
    unsigned int numChannels = 1;
	unsigned long lNumSampEncIn; // is set by FaacEncOpen
    unsigned long lMaxBytesEncOut; // is set by FaacEncOpen
    
    CVectorEx < _SAMPLE >   vecInputData;
	CVectorEx < _SAMPLE >*  pvecInputData = &vecInputData;
	CVectorEx < _BINARY >   vecOutputData;
	CVectorEx < _BINARY >*  pvecOutputData = &vecOutputData;
	CVectorEx < _BINARY >   vecOutputTmp;
	CVector<_BYTE>			aac_crc_bits;
	CVector<_SAMPLE>		vecsEncInData;
	CMatrix<_BYTE>			audio_frame;
	CVector<int>			veciFrameLength;
    
    /* open encoder instance *********************************************/
    faacEncHandle encHandle;
	encHandle = faacEncOpen(lEncSampRate, numChannels, &lNumSampEncIn, &lMaxBytesEncOut);     
    
    if(encHandle == NULL)
	{
        mexErrMsgIdAndTxt( "MATLAB:faac_wrapper:openEncoder",
              "Failed to open FAAC encoder instance.");
	}
    
    int transform_length = 960; // FAAC transform length
    int blocks = n_samples/transform_length; // number of input blocks for FAAC encoder
    int superframes = blocks/iNumAACFrames; // number of superframes (400ms)
    n_stf = (int) superframes/3;
    superframes = n_stf * 3; // truncate to whole super transmission frames

    
    /* init input and output buffers *************************************/
	int iInputBlockSize = superframes *iNumAACFrames*transform_length; // integer number of super transmission frames
	int iNumDecodedBitsMSC = 5826;
	int iOutputBlockSize = iNumDecodedBitsMSC * superframes;
	pvecInputData->Init(iInputBlockSize);
	pvecOutputData->Init(iOutputBlockSize);

	/* fill buffers with data */
	for(int i = 0; i<iInputBlockSize; i++)
	{
		(*pvecInputData)[i] = pcm[i];
	}

	for(int i = 0; i<iOutputBlockSize; i++)
	{
		(*pvecOutputData)[i] = 0; // unused bytes have to be set to zero
	}

    /* Reset bit extraction access */
    (*pvecOutputData).ResetBitAccess();

    /* configure encoder instance ****************************************/
	/* calculate bit rate */
	int sizeof_byte = 8;
	int iTotNumBitsForUsage = (iNumDecodedBitsMSC / sizeof_byte) * sizeof_byte;
	int iTotNumBytesForUsage = iTotNumBitsForUsage / sizeof_byte;
	int iTotAudFraSizeBits = iTotNumBitsForUsage; // no text message included!

	int iAudioPayloadLen = iTotAudFraSizeBits / sizeof_byte - iNumHeaderBytes - iNumAACFrames /* for CRCs */ ;
	const int iActEncOutBytes = (int) (iAudioPayloadLen / iNumAACFrames);
	int iBitRate = (int) (( iActEncOutBytes * sizeof_byte) / iTimeEachAudBloMS * 1000);
    
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
    
    /* actual encoding ***************************************************/
    
    // write raw encoded data to file for debugging
	FILE* aac_raw;
	aac_raw = fopen("aac_raw_ref.bin", "a");
	
	for(int n = 0; n<superframes; n++) // first run is mainly init (no output)
	{
		for (int j = 0; j < iNumAACFrames; j++)
		{
			/* copy the part of the input data that is converted in this iteration */
			for (unsigned long k = 0; k < lNumSampEncIn; k++)
			{
				vecsEncInData[k] = (*pvecInputData)[n*lNumSampEncIn*iNumAACFrames + j*lNumSampEncIn + k];
			}

			CVector < unsigned char >vecsTmpData(lMaxBytesEncOut);
			int bytesEncoded = faacEncEncode(encHandle, (int32_t*) &vecsEncInData[0], lNumSampEncIn, &vecsTmpData[0], lMaxBytesEncOut);
            
			if (bytesEncoded > 0)
			{
				/* Extract CRC */
				aac_crc_bits[j] = vecsTmpData[0];

				/* Extract actual data */
				for (int i = 0; i < bytesEncoded - 1 /* "-1" for CRC */ ; i++)
				{
					audio_frame[j][i] = vecsTmpData[i + 1];	
				}
				
				if(n<1)
			    {
			        fwrite((void*) &vecsTmpData[1], sizeof(char), bytesEncoded-1, aac_raw);
			    }
			    
				/* Store block lengths for borders in AAC super-frame-header */
				veciFrameLength[j] = bytesEncoded - 1;
			}
			else
			{
				/* Encoder is in initialization phase, reset CRC and length */
				aac_crc_bits[j] = 0;
				veciFrameLength[j] = 0;
			}
		}
		
		
        
    /* make AAC data DRM compliant ***************************************/
        
    /* AAC super-frame-header */
		int bits_written = 0; // counter to determine the net length of the super frame
        int iAccFrameLength = 0;
        for (int j = 0; j < iNumAACFrames - 1; j++)
        {
            iAccFrameLength += veciFrameLength[j];

            /* Frame border in bytes (12 bits) */
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

        /* append zeros until the end of the audio super frame */
        (*pvecOutputData).Enqueue(0, 5826 - bits_written);
	}
    
    /* reshape output to n_stf * [ bits_per_tranmission_superframe ] *****/
    int bits_stf = 3 * iNumDecodedBitsMSC;
    plhs[0] = mxCreateDoubleMatrix(bits_stf, n_stf, mxREAL);
    aac = mxGetPr(plhs[0]);
    int p = 0;
    
    /* assign output *****************************************************/
    // copy output to mex matrix (columnwise)
    for(int i = 0; i<n_stf*bits_stf; i++)
    {
        aac[i] = (*pvecOutputData)[i]; // Use linear indexing.  [x*dimy+y] instead of [x][y]
    }
    
    /* assign rest of output *********************************************/
    plhs[1] = mxCreateDoubleScalar(n_stf);
    
    return;
}
