#include <getopt.h>
#include <string.h>
#include <assert.h>
#include <nsk_util_meta.h>

#include "../arguments/nsk_args_options.h"
#include "../arguments/processors/nsk_option_help.h"
#include "../arguments/processors/nsk_option_input.h"
#include "../arguments/processors/nsk_option_quiet.h"
#include "../arguments/processors/nsk_option_version.h"

/*!
 * Available options table
 */
struct nsk_options_entry nsk_options_table[] = {
    /* Input options */
    {
        "input", 'i', required_argument,
        nsk_option_input,
        "Name of the input file to analyze\n"
        "\n"
    },

    /* Common options */
    {
        "quiet", 'q', no_argument,
        nsk_option_quiet,
        "Suppress any output but errors\n"
        "\n"
    },
    {
        "help", 'h', no_argument,
        nsk_option_help,
        "Print help and exit\n"
        "\n"
        "Shows usage and option reference.\n"
        "\n"
    },
    {
        "version", 'v', no_argument,
        nsk_option_version,
        "Print program version and exit\n"
        "\n"
    }
};

/*!
 * Available options table size
 */
size_t nsk_options_count = NSK_SIZE(nsk_options_table);

/*!
 * \brief  Provided program options
 */
struct nsk_options_program nsk_options_program = {
    .profile = {
        .boundary = {
            .rmsenv = {
                /*! <TODO> reassign as the whole profile */
                .energywindow     = 0.0015,
                .minactive        = 0.001,
                .energytreshold   = 0.06,
                .strengthonset    = 1.0,
                .strengthoffset   = 1.0,
                /*
                 * Energy onset is fairly reliable: a clear rise from silence usually means a
                 * note starts here.  The confidence is intentionally below 1.0 because the RMS
                 * window can shift the measured onset, attacks can be smeared, and short noise
                 * bursts may still cross the threshold.
                 */
                .confidenceonset  = 0.75,
                /*
                 * Energy offset is slightly less reliable than onset.  A drop in energy may
                 * mark a real note-off, but it can also be affected by envelope tails, exporter
                 * artifacts, or the smoothing delay of the RMS active-span detector.
                 */
                .confidenceoffset = 0.70
            },
            .schmitt = {
                /*
                 * Generous default settings
                 *
                 * 44.1 kHz -> round(44100 * 0.020) = 882 samples
                 * 48 kHz   -> round(48000 * 0.020) = 960 samples
                 * 96 kHz   -> round(96000 * 0.020) = 1920 samples
                 * 192 kHz  -> round(192000 * 0.020) = 3840 samples
                 */
                .window           = 0.02,

                .hysteresis       = 0.25,

                .strength         = 0.01,

                .confidence       = 0.05
            }
        }
    }
};


