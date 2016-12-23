AC_DEFUN([AM_ZLIB_VERSION_GT_1_2],
[

 AC_CHECK_HEADERS(zlib.h, ,$2)
 AC_CHECK_LIB(z, zlibVersion, , $2)
 AC_MSG_CHECKING([for zlib version >= 1.2])

 AC_TRY_RUN(
 [
  #include <stdio.h>
  #include <zlib.h>

  int main()
  {
   int maj, min ;
   sscanf(ZLIB_VERSION, "%d.%d", &maj, &min) ;
   if (maj < 1 && min < 2)
   {
    printf("*** Found Zlib headers version %d.%d\n", maj, min) ;
    printf("*** Need Zlib headers version >= 1.2\n") ;
    return(1) ;
   }
   return(0) ;
  }
], AC_MSG_RESULT([yes]), [AC_MSG_RESULT([no]); $2], echo)

])