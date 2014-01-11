GR-DRM
======

A SOFTWARE DRM/DRM+ TRANSMITTER FOR GNU RADIO

Contents
--------

1: Installation

2: Usage

3: Features

4: (Current) Constraints

5: Known Bugs


Installation
------------

Dependencies: GNU Radio (min. v3.7), UHD, FAAC, FAAD2

For installation instructions for GNU Radio and UHD please visit 
GNUradio http://www.gnuradio.org (there is a build script making things really easy!).

- FAAC	

		wget http://downloads.sourceforge.net/faac/faac-1.28.tar.gz
		tar zxf faac-1.28.tar.gz
		cd faac-1.28
		. bootstrap
		./configure --with-pic --disable-shared --without-mp4v2 --enable-drm
		make
		sudo cp include/faaccfg.h  include/faac.h /usr/include
		sudo cp libfaac/.libs/libfaac.a /usr/local/lib
		cd ..
		
- FAAD2	(only needed for receiver)

		wget http://downloads.sourceforge.net/faac/faad2-2.7.tar.gz
		tar zxf faad2-2.7.tar.gz
		cd faad2-2.7
		. bootstrap
		./configure --disable-shared --without-xmms --without-bmp --with-drm --without-mpeg4ip --with-pic
		make
		sudo cp include/faad.h include/neaacdec.h /usr/include
		sudo cp libfaad/.libs/libfaad.a /usr/local/lib
		cd ..
		
- gr-drm	

		git clone git://github.com/kit-cel/gr-drm.git
		cd gr-drm
		git checkout stable
		mkdir build
		cd build
		cmake ../
		make
		make test # optional
		sudo make install
		sudo ldconfig

After successful installation of gr-drm, open the MLC flow graphs in hier_blocks/ 
with GNU Radio Companion and execute them in order to generate the appropriate 
XML and Python files that are used by the transmitter flow graphs.
 
You are now ready to transmit!
				
		
Usage
-----

There are various, fully configured flow graphs under `gnuradio_drm/flow_graphs` 
that can be used pretty much as-is. The DRM+ flow graph does not crash but is
completely untested.

Of course you have to set the path to your USRP (or leave it blank for 
autodetection) and a source wav-file (either 12 or 24 kHz). There are also 
several other parameters you can change (see section Features). It is also 
possible to record a wav-file that can be decoded with DREAM.

As wav files usually aren't sampled with 12 or 24 kHz, I use sox for convenient
resampling.
Syntax: `sox <mysong.wav> -r <new_sample_rate> <mysongresampled.wav> resample`


Features
--------

This project features a DRM/DRM+ software transmitter fully integrated into GNU Radio
Companion.

You are also free to play around with several robustness modes (RM) and spectrum 
occupancies (SO, signal bandwidth) ranging from 4.5 to 20 kHz. The corresponding
bit rates vary from below 5 kbps to about 55 kbps. A configuration that is widely
used is RM B (==1) and 10 kHz bandwidth (SO 3). Among other parameters, the
station label and a text message can also be set by simply adapting the 
correspondant blocks' values.


(Current) Constraints
---------------------

As this project is still under development, there are some features of the DRM
standard that are not (yet) available. The most important are:

- Unequal Error Protection (UEP)
- Hierarchical mapping (HMsym and HMmix)
- multiple audio/data streams (currently only one audio stream in AAC Mono (no 
  SBR) is possible)
- Reconfiguration


Known Bugs
----------

- text message corrupted for SO 5 / RM A (and possibly other modes) 

If you find any bugs or have great ideas you think the project could benefit 
from, just write me: uncnr[at]student.kit.edu

last updated on: 2014/01/11
