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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "m3ufile_source_f_impl.h"

namespace gr {
  namespace drm {

    m3ufile_source_f::sptr
    m3ufile_source_f::make(std::string filename, transm_params *tp)
    {
      return gnuradio::get_initial_sptr
        (new m3ufile_source_f_impl(filename, tp));
    }

    /*
     * The private constructor
     */
    m3ufile_source_f_impl::m3ufile_source_f_impl(std::string filename, transm_params *tp)
      : gr::sync_block("m3ufile_source_f",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(1, 1, sizeof(float)))
    {
        // save given parameters
        d_tp = tp;
        d_m3u_filename = filename;

        // open playlist file and check whether it's in extended M3U format
        d_m3u_file.open(d_m3u_filename.c_str(), std::ios::in);
        if(!d_m3u_file.is_open())
            throw std::runtime_error(str(boost::format("can't open file '%s'") % d_m3u_filename));
        d_extended_format = check_for_ext_header();

        // get first track info
        d_playlist_done = false;
        if(get_next_track_info())
            init_wav_file();
        else
             d_playlist_done = true;
   }

    /*
     * Our virtual destructor.
     */
    m3ufile_source_f_impl::~m3ufile_source_f_impl()
    {
    }

    int
    m3ufile_source_f_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
        float *out = (float *) output_items[0];

        // terminate block if the playlist is done
        if(d_playlist_done)
            return WORK_DONE;





        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

    bool
    m3ufile_source_f_impl::check_for_ext_header()
    {
        char ext_header[] = "#EXTM3U";
        const int len_ext_header = strlen(ext_header);
        char linebuf[len_ext_header];
        d_m3u_file.getline(linebuf, len_ext_header);
        if(strcmp(linebuf, ext_header) == 0) // return 0 if equal
            d_extended_format = true;
        else
        {
            d_extended_format = false;
            d_m3u_file.seekg(0); // rewind the stream to the beginning or else the first track is lost
        }
    }

    bool
    m3ufile_source_f_impl::get_next_track_info()
    {
        // the info line looks like: #EXTINF:<runlength>,<artist/track info>
        char extinf_str[] = "#EXTINF";
        const int len_linebuf = 128;
        char linebuf[len_linebuf];
        if(d_extended_format) // parse #EXTINF string
        {
            // gets the #EXTINF part
            d_m3u_file.getline(linebuf, len_linebuf,':');
            if(strcmp(extinf_str,linebuf) != 0)
                throw std::runtime_error("M3U playlist uses extended format, but #EXTINF was not found");

            // get runlength
            d_m3u_file.getline(linebuf, len_linebuf, ',');
            d_track_info.runlength = atoi(linebuf);

            // get info string
            d_m3u_file.getline(linebuf, len_linebuf);
            d_track_info.info = std::string(linebuf);
        }
        else // there is no exended information, fill in default values
        {
            d_track_info.info = "unknown artist";
            d_track_info.runlength = -1;
        }

        // get file name
        d_m3u_file.getline(linebuf, len_linebuf);
        d_track_info.filename = std::string(linebuf);

        // check if EOF is reached
        if(d_m3u_file.eof())
            return false;

        // print results
        printf("next track:\n\t'%s'\n\trunlength: %d\n\tfile: %s\n", d_track_info.info.c_str(), d_track_info.runlength, d_track_info.filename.c_str()); fflush(stdout);

        return true;
    }

    void
    m3ufile_source_f_impl::init_wav_file()
    {
        // open the file
        d_wav_file = fopen(d_track_info.filename.c_str(), "rb");
        if(d_wav_file != NULL)
            throw std::runtime_error(str(boost::format("can't open file '%s'") % d_track_info.filename.c_str()));

        // parse its header
        bool is_valid = blocks::wavheader_parse(d_wav_file,
                                                d_wav_hdr.sample_rate,
                                                d_wav_hdr.nchans,
                                                d_wav_hdr.bytes_per_sample,
                                                d_wav_hdr.first_sample_pos,
                                                d_wav_hdr.samples_per_chan);
        if(!is_valid)
            throw std::runtime_error("not a valid WAV file");
    }

  } /* namespace drm */
} /* namespace gr */

