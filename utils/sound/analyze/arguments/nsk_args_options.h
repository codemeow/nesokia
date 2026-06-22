#ifndef NSK_ARGS_OPTIONS
#define NSK_ARGS_OPTIONS

#include <stdlib.h>
#include <stdbool.h>

/*!
 * \brief  The starting point of the short-option replacing value
 *
 * Use this value when the entry lacks of the short option. Increment the value
 * for each entry of this kind
 */
#define NSK_OPTION_SHORTLIMIT (1000)

/*!
 * \brief  Argument processing result
 */
enum nsk_args_result {
    NSK_ARGS_CONTINUE,      /*!< Continue normal program execution */
    NSK_ARGS_EXIT_SUCCESS,  /*!< Stop argument processing successfully */
    NSK_ARGS_EXIT_FAILURE   /*!< Stop argument processing with error */
};

/*!
 * \brief  Options table entry
 */
struct nsk_options_entry {
    const char   *option_long;             /*!< Long argument option or NULL  */
    const int     option_short;            /*!< Short argument option or 1000+*/
    const int     option_arg;              /*!< Argument presence flag        */
    enum nsk_args_result (*option_processor)(void); /*!< Argument processor */
    const char   *option_desc;             /*!< Argument description          */
};


/*!
 * \brief  Provided program options
 */
struct nsk_options_program {
    /*! Input program data */
    struct {
        const char *file; /*!< Input filename */
    } input;

    /*! Profile settings */
    struct {

        /*! Boundary detectors settings */
        struct {

            /*! RMS envelope detector settings */
            struct {
                /*! Detector window in seconds */
                double energywindow;

                /*! Minimum active energy window in seconds */
                double minactive;

                /*! Energy treshold, 0..1 */
                double energytreshold;

                /*! Candidate strength for onset candidate */
                double strengthonset;

                /*! Candidate strength for offset candidate */
                double strengthoffset;

                /*! Candidate confidence for onset candidate */
                double confidenceonset;

                /*! Candidate confidence for offset candidate */
                double confidenceoffset;
            } rmsenv;

            /*! Schmitt trigger settings */
            struct {
                /*! Schmitt window in seconds */
                double window;

                /*! Hysteresis */
                double hysteresis;

                /*! Schmitt candidates strength */
                double strength;

                /*! Schmitt candidates confidence */
                double confidence;
            } schmitt;

            /*! Same polarity edge train settings */
            struct {
                /*!
                 * Number of stable same-polarity periods required on each
                 * side of a suspected mixed period.  With 1, the detector can
                 * react to very short notes while still comparing a left and a
                 * right full waveform period.
                 */
                size_t periodcontext;

                /*!
                 * Minimum pitch distance, in semitones, required between the
                 * robust left and right periods.  Smaller changes are treated
                 * as the same note or harmless period jitter.
                 */
                double mindeltast;

                /*!
                 * Maximum robust coefficient of variation allowed for the
                 * left and right period windows.  Larger values mean the local
                 * edge train is too unstable for reliable period evidence.
                 */
                double maxperiodcv;

                /*!
                 * Confidence scale for weak period-edge-change candidates.
                 * The raw confidence already reflects local period stability;
                 * this factor keeps observed-edge fallback evidence below
                 * stronger candidates that can estimate the boundary inside
                 * the mixed period.
                 */
                double confidencescalepec;

                /*!
                 * Confidence scale for weak transition probe candidates.
                 * These probes mark possible positions inside the mixed
                 * interval and should stay weaker than observed-edge fallback
                 * evidence.
                 */
                double confidencescaletp;

                /*!
                 * Maximum ratio between the mixed period and the stable local
                 * period for reset-like edge detection.  Smaller mixed periods
                 * indicate that a phase reset may have produced the next
                 * same-polarity edge early.
                 */
                double shortresetratio;

                /*!
                 * Strength for weak probes extrapolated from the first
                 * same-polarity period inside an active span.
                 */
                double strengthspanstartprobe;

                /*!
                 * Confidence for weak probes extrapolated from the first
                 * same-polarity period inside an active span.
                 */
                double confidencespanstartprobe;

                /*!
                 * Strength for weak probes extrapolated from the last
                 * same-polarity period inside an active span.
                 */
                double strengthspanendprobe;

                /*!
                 * Confidence for weak probes extrapolated from the last
                 * same-polarity period inside an active span.
                 */
                double confidencespanendprobe;
            } edgetrain;

            /*! Grid-locked boundary evidence settings */
            struct {
                /*!
                 * FPS of the target platform, eg. NTSC is 60 fps,
                 * PAL is 50 fps
                 */
                size_t fps;

                /*!
                 * Minimum positive RMS increase across a grid frame required
                 * to emit an energy-rise onset candidate.
                 */
                double energyrisethreshold;

                /*!
                 * Multiplier that maps RMS rise magnitude to candidate
                 * strength.
                 */
                double strengthscale;
            } grid;
        } boundary;
    } profile;

    char *const *files; /*!< Compability list of files */
};

/*!
 * Available options table
 */
extern struct nsk_options_entry nsk_options_table[];

/*!
 * Available options table size
 */
extern size_t nsk_options_count;

/*!
 * \brief  Provided program options
 */
extern struct nsk_options_program nsk_options_program;

#endif
