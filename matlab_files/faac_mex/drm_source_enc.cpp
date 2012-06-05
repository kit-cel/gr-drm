/*********************************************************************
 * Demo.cpp
 *
 * This file shows the basics of setting up a mex file to work with
 * Matlab.  This example shows how to use 2D matricies.  This may
 * 
 * Keep in mind:
 * <> Use 0-based indexing as always in C or C++
 * <> Indexing is column-based as in Matlab (not row-based as in C)
 * <> Use linear indexing.  [x*dimy+y] instead of [x][y]
 *
 * For more information, see my site: www.shawnlankton.com
 * by: Shawn Lankton
 *
 ********************************************************************/
#include <matrix.h>
#include <mex.h>   

#include "faac.h"
#include "drm/Vector.h"
#include <iostream>
#include <fstream>

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

//declare variables
    mxArray *pcm_in, *aac_out;
    const mwSize *dims;
    double *pcm_ptr, *aac_ptr;
    int dimx, dimy, numdims;
    int i,j;

//associate inputs
    pcm_in = mxDuplicateArray(prhs[0]);

//figure out dimensions
    dims = mxGetDimensions(prhs[0]);
    numdims = mxGetNumberOfDimensions(prhs[0]);
    dimy = (int)dims[0]; dimx = (int)dims[1];

//associate outputs
    aac_out = plhs[0] = mxCreateDoubleMatrix(dimy,dimx,mxREAL);

//associate pointers
    pcm_ptr = mxGetPr(pcm_in);
    aac_ptr = mxGetPr(aac_out);

//do something
//     for(i=0;i<dimx;i++)
//     {
//         for(j=0;j<dimy;j++)
//         {
//             mexPrintf("element[%d][%d] = %f\n",j,i,pcm_ptr[i*dimy+j]);
//             aac_ptr[i*dimy+j] = pcm_ptr[i*dimy+j]+5; //adds 5 to every element in pcm_ptr
//         }
//     }
    
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

	/* init input and output buffers */
	int iNumAACFrames = 10; // we use 24 kHz sampling NOTE: Dream would have used 12 kHz -> 5 AAC frames
	int iInputBlockSize = lNumSampEncIn * iNumAACFrames * 5; // should be enough samples for 1 super transmission frame
	int iOutputBlockSize = 5826; // see AudioSourceDecoder InitInteralTx()
	std::cout << "iInputBlockSize / iOutputBlockSize:\t" << iInputBlockSize << " / " << iOutputBlockSize << std::endl;
	pvecInputData->Init(iInputBlockSize);
	pvecOutputData->Init(iOutputBlockSize);

    return;
}
