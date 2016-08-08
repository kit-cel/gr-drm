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
icon: http://upload.wikimedia.org/wikipedia/commons/thumb/2/21/Digital_radio_mondiale-Logo.svg/200px-Digital_radio_mondiale-Logo.svg.png
---
This project features a DRM/DRM+ software transmitter fully integrated into GNU Radio
Companion.

After successful installation of gr-drm, open the MLC flow graphs in `grc/hier_blocks` 
with GNU Radio Companion and execute them in order to generate the appropriate 
XML and Python files that are used by the transmitter flow graphs. After generating,
you need to restart GRC or to click the "reload" button.
 
Of course you have to set the path to your USRP (or leave it blank for 
autodetection) and a source wav-file (either 12 or 24 kHz). There are also 
several other parameters you can change (see section Features). It is also 
possible to record a wav-file that can be decoded with DREAM.

As wav files usually aren't sampled with 12 or 24 kHz, I use sox for convenient
resampling.
Syntax: `sox <mysong.wav> -r <new_sample_rate> <mysongresampled.wav> resample`

There are various, fully configured flow graphs under `apps/drm_tx_grc` 
that can be used pretty much as-is. Robustness Mode B with Spectrum Occupancy 3 (10 kHz)
is the most popular mode and is also tested against the Newstar DR111 DRM Receiver. 
Other modes may work, too. 

The DRM+ flow graph is completely untested due to the lack of a receiver.

You are also free to play around with several robustness modes (RM) and spectrum 
occupancies (SO, signal bandwidth) ranging from 4.5 to 20 kHz. The corresponding
bit rates vary from below 5 kbps to about 55 kbps. A configuration that is widely
used is RM B (==1) and 10 kHz bandwidth (SO 3). Among other parameters, the
station label and a text message can also be set by simply adapting the 
correspondant blocks' values. Please note that not every combination of Robustness 
mode and Spectrum Occupancy is valid. 

As this project is still under development, there are some features of the DRM
standard that are not (yet) available. The most important are:

- Unequal Error Protection (UEP)
- Hierarchical mapping (HMsym and HMmix)
- multiple audio/data streams (currently only one audio stream in AAC Mono (no 
  SBR) is possible)

Known Bugs:
- text message corrupted for SO 5 / RM A (and possibly other modes) 

If you find any bugs or have ideas you think the project could benefit 
from, just write me: felix.wunsch[at]student.kit.edu

last updated on: 2016/08/08
