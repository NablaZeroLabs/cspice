/* vcrss.f -- translated by f2c (version 19980913).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#include "f2c.h"

/* $Procedure      VCRSS ( Vector cross product, 3 dimensions ) */
/* Subroutine */ int vcrss_(doublereal *v1, doublereal *v2, doublereal *vout)
{
    doublereal vtemp[3];

/* $ Abstract */

/*     Compute the cross product of two 3-dimensional vectors. */

/* $ Disclaimer */

/*     THIS SOFTWARE AND ANY RELATED MATERIALS WERE CREATED BY THE */
/*     CALIFORNIA INSTITUTE OF TECHNOLOGY (CALTECH) UNDER A U.S. */
/*     GOVERNMENT CONTRACT WITH THE NATIONAL AERONAUTICS AND SPACE */
/*     ADMINISTRATION (NASA). THE SOFTWARE IS TECHNOLOGY AND SOFTWARE */
/*     PUBLICLY AVAILABLE UNDER U.S. EXPORT LAWS AND IS PROVIDED "AS-IS" */
/*     TO THE RECIPIENT WITHOUT WARRANTY OF ANY KIND, INCLUDING ANY */
/*     WARRANTIES OF PERFORMANCE OR MERCHANTABILITY OR FITNESS FOR A */
/*     PARTICULAR USE OR PURPOSE (AS SET FORTH IN UNITED STATES UCC */
/*     SECTIONS 2312-2313) OR FOR ANY PURPOSE WHATSOEVER, FOR THE */
/*     SOFTWARE AND RELATED MATERIALS, HOWEVER USED. */

/*     IN NO EVENT SHALL CALTECH, ITS JET PROPULSION LABORATORY, OR NASA */
/*     BE LIABLE FOR ANY DAMAGES AND/OR COSTS, INCLUDING, BUT NOT */
/*     LIMITED TO, INCIDENTAL OR CONSEQUENTIAL DAMAGES OF ANY KIND, */
/*     INCLUDING ECONOMIC DAMAGE OR INJURY TO PROPERTY AND LOST PROFITS, */
/*     REGARDLESS OF WHETHER CALTECH, JPL, OR NASA BE ADVISED, HAVE */
/*     REASON TO KNOW, OR, IN FACT, SHALL KNOW OF THE POSSIBILITY. */

/*     RECIPIENT BEARS ALL RISK RELATING TO QUALITY AND PERFORMANCE OF */
/*     THE SOFTWARE AND ANY RELATED MATERIALS, AND AGREES TO INDEMNIFY */
/*     CALTECH AND NASA FOR ALL THIRD-PARTY CLAIMS RESULTING FROM THE */
/*     ACTIONS OF RECIPIENT IN THE USE OF THE SOFTWARE. */

/* $ Required_Reading */

/*     None. */

/* $ Keywords */

/*     VECTOR */

/* $ Declarations */
/* $ Brief_I/O */

/*     VARIABLE  I/O  DESCRIPTION */
/*     --------  ---  -------------------------------------------------- */
/*     V1         I     Left hand vector for cross product. */
/*     V2         I     Right hand vector for cross product. */
/*     VOUT       O     Cross product V1xV2. */

/* $ Detailed_Input */

/*     V1       This may be any 3-dimensional vector.  Typically, this */
/*              might represent the (possibly unit) vector to a planet, */
/*              sun, or a star which defines the orientation of axes of */
/*              some coordinate system. */

/*     V2       Ditto. */

/* $ Detailed_Output */

/*     VOUT     This variable represents the cross product of V1 and V2. */

/* $ Parameters */

/*     None. */

/* $ Exceptions */

/*     Error free. */

/* $ Files */

/*     None. */

/* $ Particulars */

/*     VCRSS calculates the three dimensional cross product of two */
/*     vectors according to the definition. */

/*     If V1 and V2 are large in magnitude (taken together, their */
/*     magnitude surpasses the limit allow by the computer) then it may */
/*     be possible to generate a floating point overflow from an */
/*     intermediate computation even though the actual cross product may */
/*     be well within the range of double precision numbers. VCRSS does */
/*     NOT check the magnitude of V1 or V2 to insure that overflow will */
/*     not occur. */
/* $ Examples */

/*     V1                  V2                  VOUT (=V1XV2) */
/*     ----------------------------------------------------------------- */
/*     (0, 1, 0)           (1, 0, 0)           (0, 0, -1) */
/*     (5, 5, 5)           (-1, -1, -1)        (0, 0, 0) */

/* $ Restrictions */

/*     No checking of V1 or V2 is done to prevent floating point */
/*     overflow. The user is required to determine that the magnitude of */
/*     each component of the vectors is within an appropriate range so */
/*     as not to cause floating point overflow. In almost every case */
/*     there will be no problem and no checking actually needs to be */
/*     done. */

/* $ Literature_References */

/*     None. */

/* $ Author_and_Institution */

/*     W.M. Owen       (JPL) */

/* $ Version */

/* -    SPICELIB Version 1.0.2, 22-APR-2010 (NJB) */

/*        Header correction: assertions that the output */
/*        can overwrite the input have been removed. */

/* -    SPICELIB Version 1.0.1, 10-MAR-1992 (WLT) */

/*        Comment section for permuted index source lines was added */
/*        following the header. */

/* -    SPICELIB Version 1.0.0, 31-JAN-1990 (WMO) */

/* -& */
/* $ Index_Entries */

/*     vector cross product */

/* -& */

/*  Calculate the cross product of V1 and V2, store in VTEMP */

    vtemp[0] = v1[1] * v2[2] - v1[2] * v2[1];
    vtemp[1] = v1[2] * v2[0] - v1[0] * v2[2];
    vtemp[2] = v1[0] * v2[1] - v1[1] * v2[0];

/*  Now move the result into VOUT */

    vout[0] = vtemp[0];
    vout[1] = vtemp[1];
    vout[2] = vtemp[2];

    return 0;
} /* vcrss_ */

