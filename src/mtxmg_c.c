/*

-Procedure  mtxmg_c ( Matrix transpose times matrix, general dimension )

-Abstract

   Multiply the transpose of a matrix with another matrix,
   both of arbitrary size. (The dimensions of the matrices must be
   compatible with this multiplication.)

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
   #include <stdlib.h>
   #include <math.h>
   #include "SpiceUsr.h"
   #include "SpiceZmc.h"
   #include "SpiceZim.h"
   #undef    mtxmg_c


   void mtxmg_c ( const void  * m1,
                  const void  * m2,
                  SpiceInt      ncol1,
                  SpiceInt      nr1r2,
                  SpiceInt      ncol2,
                  void        * mout  )
/*

-Brief_I/O

   VARIABLE  I/O  DESCRIPTION
   --------  ---  --------------------------------------------------
   m1         I   nr1r2 X ncol1 double precision matrix.
   m2         I   nr1r2 X ncol2 double precision matrix.
   ncol1      I   Column dimension of m1 and row dimension of mout.
   nr1r2      I   Row dimension of m1 and m2.
   ncol2      I   Column dimension of m2 (and also mout).
   mout       O   Transpose of m1 times m2.
 
-Detailed_Input

   m1         is any double precision matrix of arbitrary size.

   m2         is any double precision matrix of arbitrary size.
              The number of rows in m2 must match the number of
              rows in m1.

   ncol1      is the number of columns in m1 and the number of rows of
              mout.

   nr1r2      is the number of rows in both m1 and (by necessity) m2.

   ncol2      is the number of columns in both m2 and mout.
 
-Detailed_Output

   mout       mout is the product matrix defined as the transpose of 
              m1 times m2, that is
              
                            t
                 mout = (m1)  x (m2)
                 
              where the superscript t denotes the transpose of m1.
              
              mout is a double precision matrix of dimension ncol1 x 
              ncol2.
              
              mout may overwrite m1 or m2.  Note that this capability 
              does not exist in the Fortran version of SPICELIB; in the
              Fortran version, the output must not overwrite either 
              input.
-Parameters

   None.

-Exceptions

   1) If dynamic allocation of memory fails, the error 
      SPICE(MEMALLOCFAILED) is signalled.

-Files

   None.

-Particulars

   The code reflects precisely the following mathematical expression

   For each value of the subscript i from 1 to ncol1, and j from 1
   to ncol2:

      mout(i,j) = Summation from k=1 to nr1r2 of  m1(k,i) * m2(k,j)


-Examples
 
   1)  Suppose that m1 = | 1  2  3  0 |
                         | 1  1  1  1 |
 
       and that     m2 = | 1  2  3 |
                         | 0  0  0 |
 
       Then calling mtxmg_c as shown
 
          mtxmg_c ( m1, m2, 4, 2, 3, mout )
 
       will yield the following value for mout:
 
                 | 1  2  3 |
          mout = | 2  4  6 |
                 | 3  6  9 |
                 | 0  0  0 |
       
-Restrictions

   1) No error checking is performed to prevent numeric overflow or
      underflow.

   2) No error checking performed to determine if the input and
      output matrices have, in fact, been correctly dimensioned.
   
-Literature_References

   None.

-Author_and_Institution

   N.J. Bachman       (JPL)
   W.M. Owen          (JPL)

-Version

   -CSPICE Version 1.2.2, 16-JAN-2008   (EDW)

      Corrected typos in header titles:
      
      Detailed Input to Detailed_Input
      Detailed Output to Detailed_Output
      
   -CSPICE Version 1.2.1, 10-NOV-2006   (EDW)

      Added Parameters section header. 

   -CSPICE Version 1.2.0, 28-AUG-2001 (NJB)

      Const-qualified input arrays.

   -CSPICE Version 1.0.0, 16-APR-1999 (NJB)
   
-Index_Entries

   matrix transpose times matrix n-dimensional_case

-&
*/

{  /* Begin mtxmg_c */
 
 
   /*
   Local macros

   We'd like to be able to refer to the elements of the input and output
   matrices using normal subscripts, for example, m1[2][3].  Since the
   compiler doesn't know how to compute index offsets for the array
   arguments, which have user-adjustable size, we must compute the
   offsets ourselves.  To make syntax a little easier to read (we hope),
   we'll use macros to do the computations.  
   
   The macro INDEX(width, i,j) computes the index offset from the array 
   base of the element at position [i][j] in a 2-dimensional matrix
   having the number of columns indicated by width.  For example, if
   the input matrix m1 has 2 rows and 3 columns, the element at position
   [0][1] would be indicated by 
   
      m1[ INDEX(3,0,1) ] 
      
   */

   #define INDEX( width, row, col )     ( (row)*(width) + (col) )  
     
     
   /*
   Local variables
   */
   SpiceDouble             innerProduct;
   SpiceDouble            *tmpmat;
   SpiceDouble            *loc_m1;
   SpiceDouble            *loc_m2;

   SpiceInt                col;
   SpiceInt                i;
   SpiceInt                nelt;
   SpiceInt                row;

   size_t                  size;

   
   /*
   Allocate space for a temporary copy of the output matrix, which
   has ncol1 rows and ncol2 columns.  
   */
   nelt   =  ncol1 * ncol2;
   size   =  (size_t) ( nelt * sizeof(SpiceDouble) );
   
   tmpmat =  (SpiceDouble *) malloc ( size );

   if ( tmpmat == (SpiceDouble *)0 )
   {
      chkin_c  ( "mtxmg_c"                                         );
      setmsg_c ( "An attempt to create a temporary matrix failed." );
      sigerr_c ( "SPICE(MEMALLOCFAILED)"                           );
      chkout_c ( "mtxmg_c"                                         );
      return;
   }  

   /*
   Cast the input pointers to pointers to SpiceDoubles.  Note:  the
   original variables are pointers to void so that callers may
   supply the array names as arguments without casting them to
   SpiceDoubles.  The naked array name is considered by the compiler
   to be an incompatible pointer type with (SpiceDouble *), so we
   can't simply declare the arguments to be (SpiceDouble *).  On the
   other hand, every pointer type can be cast to (void *).
   */
   
   loc_m1 = (SpiceDouble *) m1;
   loc_m2 = (SpiceDouble *) m2;
   
   
   /*
   Compute the product.  The matrix element at position (row,col) is
   the inner product of the column of m1 having index row and the
   column of m2 having index col.  We compute index offsets using
   the macro INDEX.
   */

   for ( row = 0;  row < ncol1;  row++ )
   {
   
      for ( col = 0;  col < ncol2;  col++ )
      {
         innerProduct = 0.0;
         
         for ( i = 0;  i < nr1r2;  i++ )   
         {
            innerProduct +=   loc_m1[ INDEX(ncol1, i, row) ]
                            * loc_m2[ INDEX(ncol2, i, col) ];
         }
         
         tmpmat [ INDEX( ncol2, row, col ) ]  =  innerProduct;
      }
   }

   /*
   Move the result from tmpmat into mout.
   */ 
   MOVED ( tmpmat, nelt, mout );
   
   /*
   Free the temporary matrix.
   */ 
   free ( tmpmat );
   
   
} /* End mtxmg_c */

