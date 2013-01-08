/* This class holds the actual DRM receiver configuration */

class drm_rx_conf
{
		unsigned short d_RM; // Robustness Mode; 0-4 corresponds to A-E
		unsigned short d_SO; // Spectrum Occupancy; 0-5


		public:
		// constructor, destructor
		drm_rx_conf{};
		~drm_rx_conf{}_();

		// get methods
		unsigned short RM(){ return d_RM; }
		unsigned short SO(){ return d_SO; }

		// set methods
		void set_RM(unsigned short RM){ d_RM = RM; }
		void set_SO(unsigned short SO){ d_SO = SO; }
};
