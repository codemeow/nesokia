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
                 * 44.1 kHz -> 44100  * 0.020 ~ 882 samples
                 * 48 kHz   -> 48000  * 0.020 ~ 960 samples
                 * 96 kHz   -> 96000  * 0.020 ~ 1920 samples
                 * 192 kHz  -> 192000 * 0.020 ~ 3840 samples
                 */
                .window           = 0.02,
                .hysteresis       = 0.25,
                .strength         = 0.01,
                .confidence       = 0.05
            },
            .edgetrain = {
                .periodcontext      = 1,
                .mindeltast         = 0.5,
                .maxperiodcv        = 0.08,
                .confidencescalepec = 0.5,
                .confidencescaletp  = 0.1,
                .shortresetratio    = 0.55,
                .strengthspanstartprobe = 0.02,
                .confidencespanstartprobe = 0.05,
                .strengthspanendprobe = 0.02,
                .confidencespanendprobe = 0.05,
                .frequencylowest        = 55.0000,
                .frequencyhighest       = 9956.063
            },
            .grid = {
                .fps                    = 60,
                .energyrisethreshold    = 0.08,
                .strengthscale          = 10.0
            }
        },
        .quantization = {
            .windowerror = 0.001667
        },
        .select = {
            .energycandidatesthreshold     = 1,
            .energygriderror               = 0.00120,
            .strongcandidatesthreshold     = 1,
            .strengththreshold             = 0.90,
            .periodcandidatesthreshold     = 2,
            .deltastthreshold              = 0.75,
            .transitionperiodcandidatesthreshold = 1,
            .transitionprobecandidatesthreshold  = 1,
            .transitionstrengththreshold   = 0.13,
            .transitiondeltastthreshold    = 1.30,
            .singlemixedcandidatesthreshold = 1,
            .singlemixedstrengththreshold  = 0.90,
            .singlemixeddeltastthreshold   = 0.90,
            .singlemixedgriderror          = 0.00150,
            .denseradius                   = 16,
            .densecountthreshold           = 10,
            .denseratiothreshold           = 0.55,
            .densetransitionsthreshold     = 2,
            .denseperiodsthreshold         = 2,
            .densedeltastthreshold         = 0.50,
            .densestrengththreshold        = 0.50
        },
        .segments = {
            .silencep2pfloor     = 1e-6,
            .silencequantile     = 0.10,
            .silencescale        = 0.35,
            .silencemin          = 0.015,
            .silencemax          = 0.12,
            .silencefallback     = 0.02,
            .hannwindowlimit     = 0.15,
            .hannwindowthreshold = 4,
            .fftsize             = 32768,
            .densedirectminacceptedratio = 0.65,
            .densedirectfftsize   = 8192,
            .candidatesplitmaxframes = 64,
            .candidatesplitmaxsegments = 512,
            .candidatesplitmindeltast = 3.0,
            .shortfftsize        = 32768,
            .shortfftmaxframes   = 1,
            .shortfftminsamplerate = 96000,
            .marginframes        = 0,
            .trim                = 0.003,
            .trimratio           = 0.20,
            .mincore             = 0.008,
            .notrimshortframes   = 1,
            .maxharmonics        = 18,
            .minharmonics        = 2,
            .spectralfloor       = 1e-12,
            .scorefundamentalbase   = 0.65,
            .scorefundamentalweight = 0.35,
            .confidencebase         = 0.55,
            .confidenceratioscale   = 0.20,
            .confidenceratioweight  = 0.45,
            .minconfidence          = 0.10,
            .minharmonicratio       = 0.025,
            .mergevolumeequalitythreshold = 3.0,
            .mergedutyequalitythreshold = 0.51,
            .edgeperiodminsamples   = 8,
            .edgeperiodp2pfloor     = 1e-6,
            .edgeperiodcloseperiodratio = 0.25,
            .edgeperiodcvscale      = 6.0,
            .edgeperiodcentsscale   = 55.0,
            .edgeperiodminconfidence = 0.75,
            .edgeperiodshortminconfidence = 0.75,
            .edgeperiodshortmaxframes = 4,
            .edgeperiodoctaveminconfidence = 0.20,
            .edgeperiodoctaveminsupport = 20,
            .cycleedgeoverridemaxframes = 1,
            .cycleedgeoverridemaxdeltast = 2,
            .cycleedgeoverrideminconfidence = 0.95,
            .cycleedgeoverrideminsupport = 1,
            .semitoneguardminconfidence = 0.98,
            .high125guardminmidi = 122,
            .high125guardduty = 12.5,
            .high125guardminconfidence = 0.75,
            .octaveguardscoremargin = 0.006,
            .octaveguardratiomultiplier = 1.18,
            .periodminscore = 0.72,
            .templateresidualenabled = true,
            .templateresidualmaxphasesteps = 12,
            .templateresidualminscore = 0.52,
            .templateresidualminimprovement = 0.045,
            .templateresidualoctaveminimprovement = 0.010,
            .templateresidualsemitoneminimprovement = 0.030,
            .templateresiduallowshortminimprovement = 0.018,
            .templateresiduallowshortmaxmidi = 45,
            .templateresidualfullsearchmaxframes = 0,
            .templateresidualprefilterminconfidence = 0.95,
            .templateresidualmaxsegments = 4096,
            .zerodeltaneighborminframes = 2,
            .zerodeltaneighbormaxp2pdelta = 1e-6,
            .zerodeltaneighborminconfidence = 0.94,
            .edgeconfirmedsplitminconfidence = 0.80,
            .edgeconfirmedsplitminsupport = 2,
            .edgeconfirmedsplitminstateconfidence = 0.75,
            .pulsewidthsplitmaxframes = 4,
            .pulsewidthsplitmaxmidi = 48,
            .pulsewidthsplitmintemplatescore = 0.40,
            .pulsewidthsplitmintemplatemargin = 0.10,
            .pulsewidthsplitremainingminconfidence = 0.75,
            .pulsewidthcentsscale = 35.0,
            .framegridsplitmaxframes = 6,
            .framegridsplitminconfidence = 0.76,
            .framegridsplitpulseminscore = 0.45,
            .framegridsplitpulsemargin = 0.12,
            .framegridsplitpulsemaxmidi = 48,
            .framegridsplitpulseconfidenceratio = 0.45,
            .framegridsplitpulsemaxdelta = 2,
            .framegridsplitmindensesratio = 0.75,
            .framegridsplitstrongconfidence = 0.99,
            .outerpulsemaxframes = 2,
            .outerpulsemaxmidi = 48,
            .outerpulsemintemplatescore = 0.55,
            .outerpulsemintemplatemargin = 0.30,
            .outerpulseminimprovement = 0.20,
            .denseframemindensesratio = 0.75,
            .denseframepulseminconfidence = 0.48,
            .denseframepulsemintemplatescore = 0.60,
            .denseframepulsemargin = 0.30,
            .denseframepulsemaxmidi = 48,
            .denseframehighoctaveminmidi = 108,
            .denseframehighoctaveminscore = 0.50,
            .denseframehighoctaveminimprovement = 0.20,
            .densechromaticsweepminsegments = 120,
            .densechromaticsweepminactiveratio = 0.90,
            .densechromaticsweependpointframes = 2,
            .densechromaticsweepduty = 12.5,
            .densechromaticsweepconfidence = 1.0,
            .foldedduty12max = 16.0,
            .foldedduty25min = 23.0,
            .foldedduty25max = 31.0,
            .foldedduty50min = 39.0,
            .foldedduty125guardmaxmidi = 48
        }
    }
};
