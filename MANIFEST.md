title: gr-drm
brief: DRM/DRM+ transmitter
tags: # Tags are arbitrary, but look at CGRAN what other authors are using
  - sdr
  - digital radio
  - short wave
author:
  - Felix Wunsch <wunsch.felix@googlemail.com>
copyright_owner:
  - Felix Wunsch
license: GPLv3
#repo: # Put the URL of the repository here, or leave blank for default
#website: <module_website> # If you have a separate project website, put it here
#icon: <icon_url> # Put a URL to a square image here that will be used as an icon on CGRAN
---

After successful installation of gr-drm, open the MLC flow graphs in `gr-drm/hier_blocks` 
with GNU Radio Companion and execute them in order to generate the appropriate 
XML and Python files that are used by the transmitter flow graphs.
 
You are now ready to transmit!
Usage
-----

There are various, fully configured flow graphs under `gr-drm/flow_graphs` 
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
