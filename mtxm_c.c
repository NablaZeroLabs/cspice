/*

-Procedure mtxm_c  ( Matrix transpose times matrix, 3x3 )

-Abstract

   Multiply the transpose of a 3x3 matrix and a 3x3 matrix.

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

   None.

-Keywords

   MATRIX

*/

   #include "SpiceUsr.h"
   #include "SpiceZmc.h"
   #undef    mtxm_c
   

   void mtxm_c ( ConstSpiceDouble    m1  [3][3],
                 ConstSpiceDouble    m2  [3][3],
                 SpiceDouble         mout[3][3] )

/*

-Brief_I/O

   VARIABLE  I/O  DESCRIPTION
   --------  ---  --------------------------------------------------
   m1         I   3x3 double precision matrix.
   m2         I   3x3 double precision matrix.
   mout       O   The produce m1 transpose times m2.

-Detailed_Input

   m1         is any 3x3 double precision matrix. Typically,
              m1 will be a rotation matrix since then its
              transpose is its inverse (but this is not a
              requirement).

   m2         is any 3x3 double precision matrix.

-Detailed_Output

   mout       is a 3x3 double precision matrix. mout is the
              product

                          t
                 mout = m1  m2

              mout may overwrite either m1 or m2.

-Parameters

   None.

-Particulars

   The code reflects precisely the following mathematical expression

   For each value of the subscripts i and j from 0 to 2:

                     2
                    __
                    \
      mout[i][j] =  /_  m1[k][i] * m2[k][j]
                    k=0

   Note that the reversal of the k and i subscripts in the left-hand
   matrix m1 is what makes mout the product of the TRANSPOSE of M1
   and not simply of m1 itself.  Also, the intermediate results of
   the operation above are buffered in a temporary matrix which is
   later moved to the output matrix.  Thus mout can be actually be
   m1 or m2 if desired without interfering with the computations.

-Examples

   Let m1 = | 1.  2.  3. |
            |            |
            | 4.  5.  6. |
            |            |
            | 7.  8.  9. |


       m2 = |  1.   1.  0. |
            |              |
            | -1.   1.  0. |
            |              |
            |  0.   0.  1. |

   then the call

   mtxm_c ( m1, m2, mout );

   produces the matrix

   mout = | -3.   5.  7. |
          |              |
          | -3.   7.  8. |
          |              |
          | -3.   9.  9. |


-Restrictions

   The user is responsible for checking the magnitudes of the
   elements of m1 and m2 so that a floating point overflow does
   not occur.  (In the typical use where m1 and m2 are rotation
   matrices, this not a risk at all.)

-Exceptions

   Error free.

-Files

   None

-Author_and_Institution

   W.M. Owen       (JPL)
   E.D  Wright     (JPL)

-Literature_References

   None.

-Version

   -CSPICE Version 1.0.0, 16-APR-1999 (EDW)

-Index_Entries

   matrix_transpose times matrix 3x3_case

-&
*/


{ /* Begin mtxm_c */

   /*
   Local variables
   */

   SpiceInt                i;
   SpiceInt                j;

   SpiceDouble             mtemp[3][3];


   for ( i = 0; i < 3;  i++ )
      {

      for ( j = 0; j < 3;  j++ )
         {
         mtemp[i][j]  =     m1[0][i] * m2[0][j]
                         +  m1[1][i] * m2[1][j]
                         +  m1[2][i] * m2[2][j];
         }
      }

   /*
   Copy the results from the temporary matrix to the return matrix.
   */

   MOVED ( mtemp, 9, mout );


} /* End mtxm_c */

