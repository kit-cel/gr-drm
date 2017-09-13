title: gr-drm
brief: DRM/DRM+ transmitter
tags: 
  - sdr
  - drm
  - digital radio
  - short wave
author:
  - Felix Wunsch <felix.wunsch@kit.edu>
copyright_owner:
  - Felix Wunsch
license: GPLv3
repo: https://github.com/kit-cel/gr-drm.git
stable_release: HEAD
icon: http://upload.wikimedia.org/wikipedia/commons/thumb/2/21/Digital_radio_mondiale-Logo.svg/200px-Digital_radio_mondiale-Logo.svg.png
---
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

Dependencies: GNU Radio (min. v3.7), UHD, FAAC

From Source (automated)
=======================

If you're using a recent version of the popular linux distributions, [pybombs](http://pybombs.info) is probably the way to go:

```shell
git clone --recursive https://github.com/pybombs/pybombs.git
cd pybombs
./pybombs install gr-drm
```

and answering all the questions to best knowledge (the defaults are sane); you might need to later call

```shell
./pybombs env
```

to generate a file that you can `source <filename>` in your .bashrc or .zshenv.

From Source (manual)
====================

For installation instructions for GNU Radio and UHD please visit 
[GNU Radio](http://www.gnuradio.org) (there is a build script making things really easy!).
The instructions assume that you don't want to install into your home folder. If that's the
case, adjust the prefixes accordingly.

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
		
- gr-drm	

		git clone git://github.com/kit-cel/gr-drm.git
		cd gr-drm
		mkdir build
		cd build
		cmake ../
		make
		make test # optional, all tests should pass!
		sudo make install # sudo is only needed if you don't install to your home folder
		sudo ldconfig
		
Usage
-----

After successful installation of gr-drm, you can either use the flow graph
in `apps/grc_flowgraph` or the GUI in `apps/gui`. The DRM+ flow graph is 
completely untested due to the lack of a receiver.

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
station label and a text message can also be set. Please note that not every combination of Robustness 
mode and Spectrum Occupancy is valid. 


(Current) Constraints
---------------------

As this project is still under development, there are some features of the DRM
standard that are not (yet) available. The most important are:

- Unequal Error Protection (UEP)
- Hierarchical mapping (HMsym and HMmix)
- multiple audio/data streams (currently only one audio stream in AAC Mono (no 
  SBR) is possible)


Known Bugs
----------

- text message corrupted for SO 5 / RM A (and possibly other modes) 

If you find any bugs or have ideas you think the project could benefit 
from, just write me: felix.wunsch[at]kit.edu

last updated on: 2016/10/08
