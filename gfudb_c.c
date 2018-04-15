/*
-Procedure gfudb_c ( GF, user defined boolean )

-Abstract

   Perform a GF search on a user defined boolean quantity.

-Disclaimer

   THIS SOFTWARE AND ANY RELATED MATERIALS WERE CREATED BY THE
   CALIFORNIA INSTITUTE OF TECHNOLOGY (CALTECH) UNDER A U.S.
   GOVERNMENT CONTRACT WITH THE NATIONAL AERONAUTICS AND SPACE
   ADMINISTRATION (NASA). THE SOFTWARE IS TECHNOLOGY AND SOFTWARE
   PUBLICLY AVAILABLE UNDER U.S. EXPORT LAWS AND IS PROVIDED "AS-IS"
   TO THE RECIPIENT WITHOUT WARRANTY OF ANY KIND, INCLUDING ANY
   WARRANTIES OF PERFORMANCE OR MERCHANTABILITY OR FITNESS FOR A
   PARTICULAR USE OR PURPOSE (AS SET FORTH IN UNITED STATES UCC
   SECTIONS 2312-2313) OR FOR ANY PURPOSE WHATSOEVER, FOR THE
   SOFTWARE AND RELATED MATERIALS, HOWEVER USED.

   IN NO EVENT SHALL CALTECH, ITS JET PROPULSION LABORATORY, OR NASA
   BE LIABLE FOR ANY DAMAGES AND/OR COSTS, INCLUDING, BUT NOT
   LIMITED TO, INCIDENTAL OR CONSEQUENTIAL DAMAGES OF ANY KIND,
   INCLUDING ECONOMIC DAMAGE OR INJURY TO PROPERTY AND LOST PROFITS,
   REGARDLESS OF WHETHER CALTECH, JPL, OR NASA BE ADVISED, HAVE
   REASON TO KNOW, OR, IN FACT, SHALL KNOW OF THE POSSIBILITY.

   RECIPIENT BEARS ALL RISK RELATING TO QUALITY AND PERFORMANCE OF
   THE SOFTWARE AND ANY RELATED MATERIALS, AND AGREES TO INDEMNIFY
   CALTECH AND NASA FOR ALL THIRD-PARTY CLAIMS RESULTING FROM THE
   ACTIONS OF RECIPIENT IN THE USE OF THE SOFTWARE.

-Required_Reading

   GF
   WINDOWS

-Keywords

   EVENT
   GEOMETRY
   SEARCH
   WINDOW
*/


   #include <signal.h>
   #include "SpiceUsr.h"
   #include "SpiceZmc.h"
   #include "SpiceZfc.h"
   #include "SpiceZad.h"
   #undef   gfudb_c

   void gfudb_c (  void             ( * udfuns ) ( SpiceDouble       et,
                                                   SpiceDouble     * value ),

                   void             ( * udfunb ) ( void ( * udfuns )
                                                        ( SpiceDouble   et,
                                                          SpiceDouble * value ),

                                                   SpiceDouble       et,
                                                   SpiceBoolean    * xbool ),

                   SpiceDouble          step,
                   SpiceCell          * cnfine,
                   SpiceCell          * result )

/*
-Brief_I/O

   VARIABLE  I/O  DESCRIPTION
   --------  ---  --------------------------------------------------
   udfuns     I   Name of the routine that computes a scalar quantity of
                  interest corresponding to an `et'.
   udfunb     I   Name of the routine returning the boolean value
                  corresponding to an `et'.
   step       I   Step size used for locating extrema and roots.
   cnfine    I-O  SPICE window to which the search is restricted.
   result     O   SPICE window containing results.

-Detailed_Input

   udfuns     the name of the external routine that returns the
              value of the scalar quantity of interest at time `et'.
              The calling sequence for "udfuns" is:

                 udfuns ( et, &value )

              where:

                 et      a double precision value representing
                         ephemeris time, expressed as seconds past
                         J2000 TDB at which to evaluate "udfuns."

                 value   is the value of the geometric quantity
                         at `et'.

   udfunb     the user defined routine returning a boolean value
              for an epoch ET. The calling sequence for "udfunb" is:

                 udfunb ( udfuns, et, xbool )

              where:

                 udfuns   the name of the scalar function as defined above.

                 et       a double precision value representing
                          ephemeris time, expressed as seconds past
                          J2000 TDB, at which to evaluate "udfunb."

                 xbool     the boolean value at `et'.

              gfudb_c will correctly operate only for boolean
              functions with true conditions defining non zero
              measure time intervals.

              Note, "udfunb" need not call "udfuns." The use of "udfuns"
              is determined by the needs of the calculation and
              the user's design.

   step       the double precision time step size to use in
              the search.

              `step' must be shorter than any interval, within the
              confinement window, over which the user defined boolean
              function is met. In other words, `step' must be shorter
              than the shortest time interval for which the boolean
              function is true; `step' must also be shorter than the
              shortest time interval between two boolean function true events
              occurring within the confinement window (see below).
              However, `step' must not be *too* short, or the search
              will take an unreasonable amount of time.

              The choice of `step' affects the completeness but not
              the precision of solutions found by this routine; the
              precision is controlled by the convergence tolerance.
              See the discussion of the parameter SPICE_GF_CNVTOL for
              details.

              `step' has units of TDB seconds.

   cnfine     a double precision SPICE window that confines the time
              period over which the specified search is conducted.
              cnfine may consist of a single interval or a collection
              of intervals.

              In some cases the confinement window can be used to
              greatly reduce the time period that must be searched
              for the desired solution. See the Particulars section
              below for further discussion.

              See the Examples section below for a code example
              that shows how to create a confinement window.

-Detailed_Output

   cnfine     is the input confinement window, updated if necessary
              so the control area of its data array indicates the
              window's size and cardinality. The window data are
              unchanged.

   result     is a SPICE window representing the set of time
              intervals, within the confinement period, when the
              specified geometric event occurs.

              If `result' is non-empty on input, its contents
              will be discarded before gfuds_c conducts its
              search.

-Parameters

   SPICE_GF_CNVTOL

               is the convergence tolerance used for finding endpoints
               of the intervals comprising the result window.
               SPICE_GF_CNVTOL is used to determine when binary
               searches for roots should terminate: when a root is
               bracketed within an interval of length SPICE_GF_CNVTOL,
               the root is considered to have been found.

               The accuracy, as opposed to precision, of roots found by
               this routine depends on the accuracy of the input data.
               In most cases, the accuracy of solutions will be
               inferior to their precision.

               SPICE_GF_CNVTOL is declared in the header file
               SpiceGF.h.

-Exceptions

   1)  In order for this routine to produce correct results,
       the step size must be appropriate for the problem at hand.
       Step sizes that are too large may cause this routine to miss
       roots; step sizes that are too small may cause this routine
       to run unacceptably slowly and in some cases, find spurious
       roots.

       This routine does not diagnose invalid step sizes, except
       that if the step size is non-positive, an error is signaled
       by a routine in the call tree of this routine.

   2)  Due to numerical errors, in particular,

          - Truncation error in time values
          - Finite tolerance value
          - Errors in computed geometric quantities

       it is *normal* for the condition of interest to not always be
       satisfied near the endpoints of the intervals comprising the
       result window.

       The result window may need to be contracted slightly by the
       caller to achieve desired results. The SPICE window routine
       wncond_c can be used to contract the result window.

   3)  If an error (typically cell overflow) occurs while performing
       window arithmetic, the error will be diagnosed by a routine
       in the call tree of this routine.

   4)  If required ephemerides or other kernel data are not
       available, an error is signaled by a routine in the call tree
       of this routine.

   5) If the output SPICE window `result' has insufficient capacity to
      contain the number of intervals on which the specified geometric
      condition is met, the error will be diagnosed by a routine in
      the call tree of this routine. If the result window has size
      less than 2, the error SPICE(INVALIDDIMENSION) will signal.

   6) If either input cell has type other than SpiceDouble,
      the error SPICE(TYPEMISMATCH) will signaled from a routine
      in the call tree of this routine.

-Files

   Appropriate kernels must be loaded by the calling program before
   this routine is called.

   If the user defined function requires access to ephemeris data:

      - SPK data: ephemeris data for any body over the
        time period defined by the confinement window must be
        loaded. If aberration corrections are used, the states of
        target and observer relative to the solar system barycenter
        must be calculable from the available ephemeris data.
        Typically ephemeris data are made available by loading one
        or more SPK files via furnsh_c.

      - If non-inertial reference frames are used, then PCK
        files, frame kernels, C-kernels, and SCLK kernels may be
        needed.

   In all cases, kernel data are normally loaded once per program
   run, NOT every time this routine is called.

-Particulars

   This routine determines a set of one or more time intervals
   within the confinement window when the boolean function
   evaluates to true. The resulting set of intervals is returned
   as a SPICE window.

   Below we discuss in greater detail aspects of this routine's
   solution process that are relevant to correct and efficient
   use of this routine in user applications.


   udfuns Default Template
   =======================

   The boolean function includes an argument for an input scalar
   function. Use of a scalar function during the evaluation of
   the boolean function is not required. SPICE provides a no-op
   scalar routine, udf_c, as a dummy argument for instances when
   the boolean function does not need to call the scalar function.


   The Search Process
   ==================

   The search for boolean events is treated as a search for state
   transitions: times are sought when the boolean function value
   changes from true to false or vice versa.


   Step Size
   =========

   Each interval of the confinement window is searched as follows:
   first, the input step size is used to determine the time
   separation at which the boolean function will be sampled.
   Starting at the left endpoint of the interval, samples of the
   boolean function will be taken at each step. If a state change
   is detected, a root has been bracketed; at that point, the
   "root"--the time at which the state change occurs---is found by a
   refinement process, for example, via binary search.

   Note that the optimal choice of step size depends on the lengths
   of the intervals over which the boolean function is constant:
   the step size should be shorter than the shortest such interval
   and the shortest separation between the intervals, within
   the confinement window.

   Having some knowledge of the relative geometry of the targets and
   observer can be a valuable aid in picking a reasonable step size.
   In general, the user can compensate for lack of such knowledge by
   picking a very short step size; the cost is increased computation
   time.

   Note that the step size is not related to the precision with which
   the endpoints of the intervals of the result window are computed.
   That precision level is controlled by the convergence tolerance.


   Convergence Tolerance
   =====================

   Once a root has been bracketed, a refinement process is used to
   narrow down the time interval within which the root must lie.
   This refinement process terminates when the location of the root
   has been determined to within an error margin called the
   "convergence tolerance." The convergence tolerance used by this
   routine is set via the parameter SPICE_GF_CNVTOL.

   The value of SPICE_GF_CNVTOL is set to a "tight" value so that the
   tolerance doesn't limit the accuracy of solutions found by this
   routine. In general the accuracy of input data will be the limiting
   factor.

   The user may change the convergence tolerance from the default
   SPICE_GF_CNVTOL value by calling the routine gfstol_c, e.g.

      gfstol_c( tolerance value )

   Call gfstol_c prior to calling this routine. All subsequent
   searches will use the updated reference value.

   Setting the tolerance tighter than SPICE_GF_CNVTOL is unlikely to be
   useful, since the results are unlikely to be more accurate.
   Making the tolerance looser will speed up searches somewhat,
   since a few convergence steps will be omitted. However, in most
   cases, the step size is likely to have a much greater effect
   on processing time than would the convergence tolerance.


   The Confinement Window
   ======================

   The simplest use of the confinement window is to specify a time
   interval within which a solution is sought.

   The confinement window also can be used to restrict a search to
   a time window over which required data are known to be
   available.

   In some cases, the confinement window can be used to make searches
   more efficient. Sometimes it's possible to do an efficient search
   to reduce the size of the time period over which a relatively
   slow search of interest must be performed. See the "CASCADE"
   example program in gf.req for a demonstration.

-Examples

   The numerical results shown for these examples may differ across
   platforms. The results depend on the SPICE kernels used as
   input, the compiler and supporting libraries, and the machine
   specific arithmetic implementation.

   Use the meta-kernel shown below to load the required SPICE
   kernels.

         KPL/MK

         File name: standard.tm

         This meta-kernel is intended to support operation of SPICE
         example programs. The kernels shown here should not be
         assumed to contain adequate or correct versions of data
         required by SPICE-based user applications.

         In order for an application to use this meta-kernel, the
         kernels referenced here must be present in the user's
         current working directory.


         \begindata

            KERNELS_TO_LOAD = ( 'de418.bsp',
                                'pck00008.tpc',
                                'naif0009.tls'  )

         \begintext

   Example(1):

   Calculate the time intervals when the position of the moon relative
   to the earth in the IAU_EARTH frame has a positive value in for
   the Z position component, with also a positive value for the Vz
   velocity component.

      #include <stdio.h>
      #include <stdlib.h>
      #include <string.h>

      #include "SpiceUsr.h"
      #include "SpiceZfc.h"
      #include "SpiceZad.h"


      #define       MAXWIN    20000
      #define       TIMFMT    "YYYY-MON-DD HR:MN:SC.###"
      #define       TIMLEN    41
      #define       NLOOPS    7

      void gfq ( void ( * udfunc ) ( SpiceDouble    et,
                                     SpiceDouble  * value ),
                 SpiceDouble et,
                 SpiceBoolean * xbool );

      int main( int argc, char **argv )
         {

         /.
         Create the needed windows. Note, one interval
         consists of two values, so the total number
         of cell values to allocate is twice
         the number of intervals.
         ./
         SPICEDOUBLE_CELL ( result, 2*MAXWIN );
         SPICEDOUBLE_CELL ( cnfine, 2        );

         SpiceDouble       begtim;
         SpiceDouble       endtim;
         SpiceDouble       left;
         SpiceDouble       right;
         SpiceDouble       step;
         SpiceDouble       ltime;
         SpiceDouble       state  [6];

         SpiceChar         begstr [ TIMLEN ];
         SpiceChar         endstr [ TIMLEN ];

         SpiceInt          count;
         SpiceInt          i;

         printf( "Compile date %s, %s\n\n", __DATE__, __TIME__ );

         /.
         Load kernels.
         ./
         furnsh_c( "standard.tm" );

         /.
         Store the time bounds of our search interval in the 'cnfine'
         confinement window.
         ./
         str2et_c ( "Jan 1 2011", &begtim );
         str2et_c ( "Jan 1 2012", &endtim );

         wninsd_c ( begtim, endtim, &cnfine );


         /.
         The moon orbit about the earth-moon barycenter is
         twenty-eight days. The event condition occurs
         during (very) approximately a quarter of the orbit. Use
         a step of five days.
         ./

         step = 5.0 * spd_c();

         gfudb_c ( udf_c,
                   gfq,
                   step,
                   &cnfine,
                   &result );

         count = wncard_c( &result );

         /.
         Display the results.
         ./
         if (count == 0 )
            {
            printf ( "Result window is empty.\n\n" );
            }
         else
            {
            for ( i = 0;  i < count;  i++ )
               {

               /.
               Fetch the endpoints of the Ith interval
               of the result window.
               ./
               wnfetd_c ( &result, i, &left, &right );

               printf ( "Interval %d\n", (int)i );

               timout_c ( left, TIMFMT, TIMLEN, begstr );
               printf   ( "   Interval start: %s \n", begstr );
               spkez_c  ( 301, left, "IAU_EARTH", "NONE", 399, state, &ltime);
               printf   ( "                Z= %.12g \n", state[2] );
               printf   ( "               Vz= %.12g \n", state[5] );

               timout_c ( right, TIMFMT, TIMLEN, endstr );
               printf   ( "   Interval end  : %s \n", endstr );
               spkez_c  ( 301, right, "IAU_EARTH", "NONE", 399, state, &ltime);
               printf   ( "                Z= %.12g \n",   state[2] );
               printf   ( "               Vz= %.12g \n\n", state[5] );
               }

            }

         kclear_c();
         return( 0 );
         }



      /.
      The user defined functions required by gfudb_c.

         udf_c   for udfuns
         gfq     for udfunb
      ./



      /.
      -Procedure Procedure gfq
      ./

      void gfq ( void ( * udfuns ) ( SpiceDouble    et,
                                     SpiceDouble  * value ),
                 SpiceDouble et,
                 SpiceBoolean * xbool )

      /.
      -Abstract

        User defined geometric boolean function:

           Z >= 0 with dZ/dt > 0.

      ./
         {

         /.
         Initialization. Retrieve the vector from the earth to
         the moon in the IAU_EARTH frame, without aberration
         correction.
         ./
         SpiceInt             targ   = 301;
         SpiceInt             obs    = 399;

         SpiceChar          * ref    = "IAU_EARTH";
         SpiceChar          * abcorr = "NONE";

         SpiceDouble          state [6];
         SpiceDouble          lt;

         /.
         Evaluate the state of TARG from OBS at ET with
         correction ABCORR.
         ./
         spkez_c ( targ, et, ref, abcorr, obs, state, &lt );

         /.
         Calculate the boolean value.
         ./

         *xbool = (state[2] >= 0.0) && (state[5] > 0.0);

         return;
         }

   The program outputs:

      Interval 0
         Interval start: 2011-JAN-09 15:24:23.415
                      Z= -3.67969050785e-08
                     Vz= 0.396984084929
         Interval end  : 2011-JAN-16 16:08:28.563
                      Z= 156247.488202
                     Vz= 3.76859567858e-13

      Interval 1
         Interval start: 2011-FEB-05 23:17:57.359
                      Z= -3.98442807636e-08
                     Vz= 0.396781283223
         Interval end  : 2011-FEB-13 01:38:28.425
                      Z= 157016.055162
                     Vz= 3.2238816651e-13

      Interval 2
         Interval start: 2011-MAR-05 06:08:17.667
                      Z= -1.16190221888e-08
                     Vz= 0.393990253999
         Interval end  : 2011-MAR-12 10:27:45.188
                      Z= 157503.773934
                     Vz= -3.41879302646e-13

                        ...

      Interval 11
         Interval start: 2011-NOV-05 18:43:39.742
                      Z= -1.80199890565e-08
                     Vz= 0.373937629543
         Interval end  : 2011-NOV-13 03:50:17.153
                      Z= 153172.086618
                     Vz= -3.62962481251e-13

      Interval 12
         Interval start: 2011-DEC-03 01:16:40.817
                      Z= 1.30391470066e-07
                     Vz= 0.374257845032
         Interval end  : 2011-DEC-10 09:51:07.718
                      Z= 152511.720377
                     Vz= 2.11386680729e-13

      Interval 13
         Interval start: 2011-DEC-30 09:48:57.409
                      Z= 9.79434844339e-09
                     Vz= 0.377333201453
         Interval end  : 2012-JAN-01 00:00:00.000
                      Z= 50793.0833127
                     Vz= 0.354549969268


   Example(2):

   Calculate the time intervals when the Z component of the earth
   to moon position vector in the IAU_EARTH frame has value
   between -1000 km and 1000 km (e.g. above and below the equatorial
   plane).


      Code:

      #include <stdio.h>
      #include <stdlib.h>
      #include <string.h>

      #include "SpiceUsr.h"


      #define       MAXWIN    20000
      #define       TIMFMT    "YYYY-MON-DD HR:MN:SC.###"
      #define       TIMLEN    41
      #define       NLOOPS    7


      void gfq ( SpiceDouble    et,
                 SpiceDouble  * value );

      void gfb ( void ( * udfuns ) ( SpiceDouble    et,
                                     SpiceDouble  * value ),
                 SpiceDouble    et,
                 SpiceBoolean * xbool );

      int main( int argc, char **argv )
         {

         /.
         Create the needed windows. Note, one interval
         consists of two values, so the total number
         of cell values to allocate is twice
         the number of intervals.
         ./
         SPICEDOUBLE_CELL ( result, 2*MAXWIN );
         SPICEDOUBLE_CELL ( cnfine, 2        );

         SpiceDouble       begtim;
         SpiceDouble       endtim;
         SpiceDouble       left;
         SpiceDouble       right;
         SpiceDouble       step;
         SpiceDouble       ltime;
         SpiceDouble       state  [6];

         SpiceChar         begstr [ TIMLEN ];
         SpiceChar         endstr [ TIMLEN ];

         SpiceInt          count;
         SpiceInt          i;

         printf( "Compile date %s, %s\n\n", __DATE__, __TIME__ );

         /.
         Load kernels.
         ./
         furnsh_c( "standard.tm" );

         /.
         Store the time bounds of our search interval in the 'cnfine'
         confinement window.
         ./
         str2et_c ( "Jan 1 2011", &begtim );
         str2et_c ( "Jan 1 2012", &endtim );

         wninsd_c ( begtim, endtim, &cnfine );


         /.
         The duration of the event is approximately ninety minutes.
         Use a step of one hour.
         ./

         step = 60.*60.;

         gfudb_c ( gfq,
                   gfb,
                   step,
                   &cnfine,
                   &result );

         count = wncard_c( &result );

         /.
         Display the results.
         ./
         if (count == 0 )
            {
            printf ( "Result window is empty.\n\n" );
            }
         else
            {
            for ( i = 0;  i < count;  i++ )
               {

               /.
               Fetch the endpoints of the Ith interval
               of the result window.
               ./
               wnfetd_c ( &result, i, &left, &right );

               printf ( "Interval %d\n", (int)i );

               timout_c ( left, TIMFMT, TIMLEN, begstr );
               printf   ( "   Interval start: %s \n", begstr );
               spkez_c  ( 301, left, "IAU_EARTH", "NONE", 399, state, &ltime);
               printf   ( "                Z= %.12g \n", state[2] );

               timout_c ( right, TIMFMT, TIMLEN, endstr );
               printf   ( "   Interval end  : %s \n", endstr );
               spkez_c  ( 301, right, "IAU_EARTH", "NONE", 399, state, &ltime);
               printf   ( "                Z= %.12g \n",   state[2] );
               }

            }

         kclear_c();
         return( 0 );
         }



      /.
      The user defined functions required by gfudb_c.

         gfq     for udfuns
         gfb     for udfunb
      ./



      /.
      -Procedure Procedure gfq
      ./

      void gfq ( SpiceDouble    et,
                 SpiceDouble  * value )

      /.
      -Abstract

        User defined scalar function:

      ./
         {

         /.
         Initialization. Retrieve the vector from the earth to
         the moon in the IAU_EARTH frame, without aberration
         correction.
         ./
         SpiceInt             targ   = 301;
         SpiceInt             obs    = 399;

         SpiceChar          * ref    = "IAU_EARTH";
         SpiceChar          * abcorr = "NONE";

         SpiceDouble          pos [3];
         SpiceDouble          lt;

         /.
         Evaluate the state of TARG from OBS at ET with
         correction ABCORR.
         ./
         spkezp_c ( targ, et, ref, abcorr, obs, pos, &lt );

         *value = pos[2];

         return;
         }


      /.
      -Procedure Procedure gfb
      ./

      void gfb ( void ( * udfuns ) ( SpiceDouble    et,
                                     SpiceDouble  * value ),
                 SpiceDouble    et,
                 SpiceBoolean * xbool )

      /.
      -Abstract

        User defined boolean function:


      ./
         {
         SpiceDouble          value;
         SpiceDouble          lim1   = -1000.;
         SpiceDouble          lim2   = 1000.;

         udfuns( et, &value );

         /.
         Calculate the boolean value.
         ./

         *xbool = (value >= lim1) && (value <= lim2);

         return;
         }

   The program outputs:

      Interval 0
         Interval start: 2011-JAN-09 14:42:24.484
                      Z= -999.999999903
         Interval end  : 2011-JAN-09 16:06:22.502
                      Z= 1000.00000009
      Interval 1
         Interval start: 2011-JAN-23 04:07:44.455
                      Z= 1000.00000012
         Interval end  : 2011-JAN-23 05:23:06.243
                      Z= -1000.00000011
      Interval 2
         Interval start: 2011-FEB-05 22:35:57.156
                      Z= -999.999999975
         Interval end  : 2011-FEB-05 23:59:57.748
                      Z= 999.999999891

                        ...

      Interval 24
         Interval start: 2011-DEC-03 00:32:08.820
                      Z= -999.99999988
         Interval end  : 2011-DEC-03 02:01:12.769
                      Z= 999.999999876
      Interval 25
         Interval start: 2011-DEC-17 10:17:24.039
                      Z= 1000.00000008
         Interval end  : 2011-DEC-17 11:40:37.223
                      Z= -999.999999975
      Interval 26
         Interval start: 2011-DEC-30 09:04:47.275
                      Z= -1000.00000005
         Interval end  : 2011-DEC-30 10:33:07.670
                      Z= 999.999999868

   Recall the default convergence tolerance for the GF system has
   value 10^-6 seconds.

-Restrictions

   1) Any kernel files required by this routine must be loaded
      before this routine is called.

-Literature_References

   None.

-Author_and_Institution

   N.J. Bachman   (JPL)
   E.D. Wright    (JPL)

-Version

   -CSPICE Version 1.0.1, 28-JUN-2016 (EDW)

      Edit to Example code, SpiceInts output as ints using 
      explicit casting.
      
   -CSPICE Version 1.0.0, 23-OCT-2013 (EDW) (NJB)

-Index_Entries

   GF user defined boolean function search

-&
*/
   { /* Begin gfudb_c */

   /*
   Participate in error tracing.
   */
   if ( return_c() )
     {
      return;
      }
   chkin_c ( "gfudb_c" );


   /*
   Make sure cell data types are d.p.
   */
   CELLTYPECHK2 ( CHK_STANDARD, "gfudb_c", SPICE_DP, cnfine, result );

   /*
   Initialize the input cells if necessary.
   */
   CELLINIT2 ( cnfine, result );

   /*
   Store the input function pointers so these functions can be
   called by the GF adapters.
   */
   zzadsave_c ( UDFUNC,  (void *)(udfuns)  );
   zzadsave_c ( UDQDEC,  (void *)(udfunb)  );


   /*
   Let the f2c'd routine do the work.

   We pass the adapter functions, not those provided as inputs,
   to the f2c'd routine:

      zzadfunc_c  adapter for  udfuns
      zzadqdec_c     ''        udfunb

   */

   (void) gfudb_( ( U_fp            ) zzadfunc_c,
                  ( U_fp            ) zzadqdec_c,
                  ( doublereal    * ) &step,
                  ( doublereal    * ) (cnfine->base),
                  ( doublereal    * ) (result->base)  );


   /*
   Sync the output cell.
   */
   if ( !failed_c() )
     {
     zzsynccl_c ( F2C, result );
     }

   chkout_c ( "gfudb_c" );

   } /* End gfudb_c */
