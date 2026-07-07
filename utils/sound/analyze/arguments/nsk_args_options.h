#ifndef NSK_ARGS_OPTIONS
#define NSK_ARGS_OPTIONS

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

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

                /*!
                 * Lowest available frequency.
                 *
                 * A1  ->   55.0000 Hz
                 */
                double frequencylowest;

                /*!
                 * Highest available frequency.
                 *
                 * D#9 -> 9956.0634 Hz
                 */
                double frequencyhighest;
            } edgetrain;

            /*! Grid-locked boundary evidence settings */
            struct {
                /*!
                 * FPS of the target platform, eg. NTSC is 60 fps,
                 * PAL is 50 fps
                 *
                 * TODO: move to global profile settings
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

        struct {
            double windowerror;
        } quantization;

        struct {
            /*!
             * Minimum number of grid-close energy candidates required for
             * energy-boundary selection.  Zero disables this selector.
             */
            size_t energycandidatesthreshold;

            /*!
             * Maximum grid timing error for energy candidates to accept a mark.
             */
            double energygriderror;

            /*!
             * Minimum number of strong period candidates required for
             * strong-real selection.  Zero disables this selector.
             */
            size_t strongcandidatesthreshold;

            /*!
             * Minimum strength for strong-real selection.
             */
            double strengththreshold;

            /*!
             * Minimum number of real period candidates required for real-count
             * selection.  Zero disables this selector.
             */
            size_t periodcandidatesthreshold;

            /*!
             * Minimum absolute pitch distance, in semitones, for real-count
             * selection.
             */
            double deltastthreshold;

            /*!
             * Minimum number of real period candidates required for transition
             * rescue selection.  Zero disables this selector.
             */
            size_t transitionperiodcandidatesthreshold;

            /*!
             * Minimum number of transition probes required for transition
             * rescue selection.  Zero disables this selector.
             */
            size_t transitionprobecandidatesthreshold;

            /*!
             * Minimum mark strength for transition rescue selection.
             */
            double transitionstrengththreshold;

            /*!
             * Minimum absolute pitch distance, in semitones, for transition
             * rescue selection.
             */
            double transitiondeltastthreshold;

            /*!
             * Minimum number of mixed-period candidates required for
             * single-mixed selection.  Zero disables this selector.
             */
            size_t singlemixedcandidatesthreshold;

            /*!
             * Minimum mixed-period strength for single-mixed selection.
             */
            double singlemixedstrengththreshold;

            /*!
             * Minimum absolute mixed-period pitch distance, in semitones, for
             * single-mixed selection.
             */
            double singlemixeddeltastthreshold;

            /*!
             * Maximum grid timing error for single-mixed selection.
             */
            double singlemixedgriderror;

            /*!
             * Number of frames on each side used to test local dense context.
             */
            size_t denseradius;

            /*!
             * Minimum accepted marks count inside a dense-context window.
             */
            size_t densecountthreshold;

            /*!
             * Minimum accepted marks ratio inside a dense-context window.
             */
            double denseratiothreshold;

            /*!
             * Minimum transition-probe candidates required for dense probe
             * rescue.
             */
            size_t densetransitionsthreshold;

            /*!
             * Minimum real period candidates required for dense period rescue.
             */
            size_t denseperiodsthreshold;

            /*!
             * Minimum absolute pitch distance, in semitones, for dense period
             * rescue.
             */
            double densedeltastthreshold;

            /*!
             * Minimum candidate strength for dense period rescue.
             */
            double densestrengththreshold;
        } select;

        struct {
            /*!
             * Minimum per-frame peak-to-peak amplitude treated as nonzero
             * while estimating the global silence threshold.
             */
            double silencep2pfloor;

            /*!
             * Quantile of nonzero per-frame peak-to-peak amplitudes used
             * as the base for the global silence threshold.
             */
            double silencequantile;

            /*!
             * Multiplier applied to the selected peak-to-peak percentile.
             */
            double silencescale;

            /*!
             * Minimum global silence threshold.
             */
            double silencemin;

            /*!
             * Maximum global silence threshold.
             */
            double silencemax;

            /*!
             * Global silence threshold used when no nonzero frame amplitudes
             * are available.
             */
            double silencefallback;

            /*!
             * Minimum Hann window value used before square-root shaping.
             */
            double hannwindowlimit;

            /*!
             * Minimum segment sample count required to apply Hann windowing.
             */
            size_t hannwindowthreshold;

            /*!
             * Base FFT size used for segment pitch scoring.
             */
            size_t fftsize;

            /*!
             * Minimum accepted-mark ratio that enables dense-direct segment
             * analysis behavior.
             */
            double densedirectminacceptedratio;

            /*!
             * Maximum base FFT size used in dense-direct segment analysis.
             */
            size_t densedirectfftsize;

            /*!
             * Maximum segment length, in frames, for candidate-mark split.
             */
            size_t candidatesplitmaxframes;

            /*!
             * Maximum segment-list size for candidate-mark split.
             */
            size_t candidatesplitmaxsegments;

            /*!
             * Minimum absolute pitch delta, in semitones, for non-dense
             * candidate-mark split.
             */
            double candidatesplitmindeltast;

            /*!
             * FFT size used for short segments at high sample rates.
             */
            size_t shortfftsize;

            /*!
             * Maximum segment length, in frames, for short FFT override.
             */
            size_t shortfftmaxframes;

            /*!
             * Minimum sample rate required for short FFT override.
             */
            uint32_t shortfftminsamplerate;

            /*!
             * Number of frames trimmed from both sides before pitch scoring
             * when the segment is long enough.
             */
            size_t marginframes;

            /*!
             * Maximum number of seconds trimmed from both sides before
             * pitch scoring.
             */
            double trim;

            /*!
             * Maximum fraction of segment duration trimmed from both sides.
             */
            double trimratio;

            /*!
             * Minimum remaining core duration required to apply trimming.
             */
            double mincore;

            /*!
             * Maximum segment length, in frames, that disables trim.
             */
            size_t notrimshortframes;

            /*!
             * Maximum number of harmonics used for segment pitch scoring.
             */
            size_t maxharmonics;

            /*!
             * Minimum number of harmonics required for segment pitch scoring.
             */
            size_t minharmonics;

            /*!
             * Minimum harmonic-vector magnitude required for pitch scoring.
             */
            double spectralfloor;

            /*!
             * Base multiplier for harmonic template score shaping.
             */
            double scorefundamentalbase;

            /*!
             * Additional multiplier controlled by fundamental strength.
             */
            double scorefundamentalweight;

            /*!
             * Base multiplier for segment pitch confidence.
             */
            double confidencebase;

            /*!
             * Harmonic-ratio value that reaches full confidence weight.
             */
            double confidenceratioscale;

            /*!
             * Additional confidence multiplier controlled by harmonic ratio.
             */
            double confidenceratioweight;

            /*!
             * Minimum confidence required to accept segment pitch.
             */
            double minconfidence;

            /*!
             * Minimum harmonic ratio required to accept segment pitch.
             */
            double minharmonicratio;

            /*!
             * Maximum volume difference for merging equal neighboring
             * segments.
             */
            double mergevolumeequalitythreshold;

            /*!
             * Maximum duty difference for merging equal neighboring segments.
             */
            double mergedutyequalitythreshold;

            /*!
             * Minimum number of samples required for edge-period scoring.
             */
            size_t edgeperiodminsamples;

            /*!
             * Minimum peak-to-peak amplitude required for edge-period scoring.
             */
            double edgeperiodp2pfloor;

            /*!
             * Maximum relative distance from the median period for stable
             * edge-period intervals.
             */
            double edgeperiodcloseperiodratio;

            /*!
             * Multiplier that maps edge-period variation to confidence loss.
             */
            double edgeperiodcvscale;

            /*!
             * Cents error that maps edge-period pitch confidence to zero.
             */
            double edgeperiodcentsscale;

            /*!
             * Minimum edge-period confidence required for regular override.
             */
            double edgeperiodminconfidence;

            /*!
             * Minimum edge-period confidence required for short segments.
             */
            double edgeperiodshortminconfidence;

            /*!
             * Maximum segment length, in frames, that uses short edge-period
             * confidence threshold.
             */
            size_t edgeperiodshortmaxframes;

            /*!
             * Minimum edge-period confidence required for octave-down guard.
             */
            double edgeperiodoctaveminconfidence;

            /*!
             * Minimum edge-period support required for octave-down guard.
             */
            size_t edgeperiodoctaveminsupport;

            /*!
             * Maximum segment length, in frames, for cycle-edge override.
             */
            size_t cycleedgeoverridemaxframes;

            /*!
             * Maximum pitch distance, in semitones, for cycle-edge override.
             */
            size_t cycleedgeoverridemaxdeltast;

            /*!
             * Minimum edge-period confidence for cycle-edge override.
             */
            double cycleedgeoverrideminconfidence;

            /*!
             * Minimum edge-period support for cycle-edge override.
             */
            size_t cycleedgeoverrideminsupport;

            /*!
             * FFT confidence threshold that blocks weaker one-semitone edge
             * nudges.
             */
            double semitoneguardminconfidence;

            /*!
             * Minimum MIDI note for the high 12.5 percent duty guard.
             */
            int high125guardminmidi;

            /*!
             * Duty value used by the high 12.5 percent duty guard.
             */
            double high125guardduty;

            /*!
             * Minimum FFT confidence for the high 12.5 percent duty guard.
             */
            double high125guardminconfidence;

            /*!
             * Maximum score loss allowed when preferring a lower octave.
             */
            double octaveguardscoremargin;

            /*!
             * Minimum harmonic-ratio gain required to prefer a lower octave.
             */
            double octaveguardratiomultiplier;

            /*! Minimum autocorrelation score for period note evidence. */
            double periodminscore;

            /*! Enable residual pulse-template scorer. */
            bool templateresidualenabled;

            /*! Maximum template phase steps. */
            size_t templateresidualmaxphasesteps;

            /*! Minimum residual-template score. */
            double templateresidualminscore;

            /*! Minimum template improvement for generic override. */
            double templateresidualminimprovement;

            /*! Minimum template improvement for octave override. */
            double templateresidualoctaveminimprovement;

            /*! Minimum template improvement for semitone override. */
            double templateresidualsemitoneminimprovement;

            /*! Minimum template improvement for short low-note override. */
            double templateresiduallowshortminimprovement;

            /*! Maximum MIDI note for short low-note template override. */
            int templateresiduallowshortmaxmidi;

            /*! Maximum frame count that enables full template MIDI search. */
            size_t templateresidualfullsearchmaxframes;

            /*!
             * Minimum confidence that lets long segments skip template-risk
             * probing before edge/period evidence is computed.
             */
            double templateresidualprefilterminconfidence;

            /*! Maximum segment-list size for global template override pass. */
            size_t templateresidualmaxsegments;

            /*! Minimum long neighbor length for zero-delta merge. */
            size_t zerodeltaneighborminframes;

            /*! Maximum p2p delta at a zero-delta boundary. */
            double zerodeltaneighbormaxp2pdelta;

            /*! Minimum confidence for zero-delta neighbor merge. */
            double zerodeltaneighborminconfidence;

            /*! Minimum edge confidence for two-frame split. */
            double edgeconfirmedsplitminconfidence;

            /*! Minimum edge support for two-frame split. */
            size_t edgeconfirmedsplitminsupport;

            /*! Minimum state confidence for two-frame split. */
            double edgeconfirmedsplitminstateconfidence;

            /*! Maximum segment length for pulse-width split. */
            size_t pulsewidthsplitmaxframes;

            /*! Maximum MIDI note for pulse-width low-note split. */
            int pulsewidthsplitmaxmidi;

            /*! Minimum template score for pulse-width split. */
            double pulsewidthsplitmintemplatescore;

            /*! Minimum template margin for pulse-width split. */
            double pulsewidthsplitmintemplatemargin;

            /*! Minimum confidence for the remaining split segment. */
            double pulsewidthsplitremainingminconfidence;

            /*! Cents error that maps pulse-width edge score to zero. */
            double pulsewidthcentsscale;

            /*! Maximum segment length for confident frame-grid split. */
            size_t framegridsplitmaxframes;

            /*! Minimum frame confidence for frame-grid split. */
            double framegridsplitminconfidence;

            /*! Minimum pulse score for frame-grid split. */
            double framegridsplitpulseminscore;

            /*! Minimum pulse margin for frame-grid split. */
            double framegridsplitpulsemargin;

            /*! Maximum MIDI note for frame-grid pulse evidence. */
            int framegridsplitpulsemaxmidi;

            /*! Minimum pulse-to-FFT confidence ratio for frame-grid pulse evidence. */
            double framegridsplitpulseconfidenceratio;

            /*! Maximum pitch delta for frame-grid pulse evidence. */
            size_t framegridsplitpulsemaxdelta;

            /*! Minimum dense mark ratio for frame-grid split. */
            double framegridsplitmindensesratio;

            /*! Confidence that blocks frame-grid split if original state is absent. */
            double framegridsplitstrongconfidence;

            /*! Maximum outer segment length for pulse-width reclassify. */
            size_t outerpulsemaxframes;

            /*! Maximum MIDI note for outer pulse-width reclassify. */
            int outerpulsemaxmidi;

            /*! Minimum template score for outer pulse-width reclassify. */
            double outerpulsemintemplatescore;

            /*! Minimum template margin for outer pulse-width reclassify. */
            double outerpulsemintemplatemargin;

            /*! Minimum template improvement for outer pulse-width reclassify. */
            double outerpulseminimprovement;

            /*! Minimum dense mark ratio for dense single-frame reclassify. */
            double denseframemindensesratio;

            /*! Minimum pulse confidence for dense one-frame reclassify. */
            double denseframepulseminconfidence;

            /*! Minimum template score for dense one-frame pulse reclassify. */
            double denseframepulsemintemplatescore;

            /*! Minimum template margin for dense one-frame pulse reclassify. */
            double denseframepulsemargin;

            /*! Maximum MIDI note for dense one-frame pulse reclassify. */
            int denseframepulsemaxmidi;

            /*! Minimum MIDI note for dense one-frame octave-up reclassify. */
            int denseframehighoctaveminmidi;

            /*! Minimum template score for dense one-frame octave-up reclassify. */
            double denseframehighoctaveminscore;

            /*! Minimum template improvement for dense one-frame octave-up reclassify. */
            double denseframehighoctaveminimprovement;

            /*! Minimum segment count for dense chromatic sweep repair. */
            size_t densechromaticsweepminsegments;

            /*! Minimum active-segment ratio for dense chromatic sweep repair. */
            double densechromaticsweepminactiveratio;

            /*! Required endpoint duration, in frames, for lowest sweep notes. */
            size_t densechromaticsweependpointframes;

            /*! Duty assigned by dense chromatic sweep repair. */
            double densechromaticsweepduty;

            /*! Confidence assigned by dense chromatic sweep repair. */
            double densechromaticsweepconfidence;

            /*! Folded duty upper threshold for 12.5 percent duty. */
            double foldedduty12max;

            /*! Folded duty lower threshold for 25 percent duty. */
            double foldedduty25min;

            /*! Folded duty upper threshold for 25 percent duty. */
            double foldedduty25max;

            /*! Folded duty lower threshold for 50 percent duty. */
            double foldedduty50min;

            /*! Maximum MIDI note for low 12.5 percent duty guard. */
            int foldedduty125guardmaxmidi;
        } segments;
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
