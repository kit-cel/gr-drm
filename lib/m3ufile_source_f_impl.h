/* -*- c++ -*- */
/*
 * Copyright 2014 Felix Wunsch, Communications Engineering Lab (CEL) / Karlsruhe Institute of Technology (KIT).
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef INCLUDED_DRM_M3UFILE_SOURCE_F_IMPL_H
#define INCLUDED_DRM_M3UFILE_SOURCE_F_IMPL_H

#include <drm/m3ufile_source_f.h>
#include <wavfile_drm.h>
#include <fstream>
#include <cstdio>

typedef struct{
    std::string filename;
    int runlength; // length of the track in seconds
    std::string info; // track info, e.g. 'artist name - song title'
} TRACK_INFO;

typedef struct{
    unsigned int sample_rate;
	int nchans;
	int bytes_per_sample;
	int first_sample_pos;
	unsigned int samples_per_chan;
} WAV_HDR;

namespace gr {
  namespace drm {

    class m3ufile_source_f_impl : public m3ufile_source_f
    {
     private:
      transm_params *d_tp; // pointer to the transmit parameter instance
      bool d_playlist_done; // true if the last track in the list is over

      std::string d_m3u_filename; // M3U playlist file name
      std::ifstream d_m3u_file; // M3U playlist file handle
      bool d_extended_format; // indicates whether extended format is used or not
      TRACK_INFO d_track_info; // info about the current track

      FILE *d_wav_file; // current WAV file handle
      int d_num_samples_read; // number of samples that have been read so far
      WAV_HDR d_wav_hdr; // header of the current WAV file

      int d_normalize_fac; // normalization factor for short->float conversion
      int d_normalize_shift; // normalization shift for short->float conversion

      bool check_for_ext_header(); // check whether the playlist uses extended format
      bool get_next_track_info(); // get the next track of the playlist, return false if EOF
      void init_wav_file(); // opens the wav file indicated by d_track_info and parses its header

      float convert_to_float(short int sample); // converts the short value into a float normalized to [-1,1]

     public:
      m3ufile_source_f_impl(std::string filename, transm_params *tp);
      ~m3ufile_source_f_impl();

      // Where all the action really happens
      int work(int noutput_items,
	       gr_vector_const_void_star &input_items,
	       gr_vector_void_star &output_items);
    };

  } // namespace drm
} // namespace gr

#endif /* INCLUDED_DRM_M3UFILE_SOURCE_F_IMPL_H */

