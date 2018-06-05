#include <config.h>

#ifdef WIN32
/* windows */
# include "lowlevel/win32_mman.c"
#endif

#ifdef WITH_ZSIG
# include "lowlevel/zsig.h"
# ifndef WIN32
#  include <signal.h>
# endif
  void my_signal(const int sig_nr, const sighandler_t signalhandler) {
# ifndef WIN32
  /* normal unix --
    source: http://openbook.rheinwerk-verlag.de/linux_unix_programmierung/Kap08-004.htm */
    struct sigaction sig;
    sig.sa_handler = signalhandler;
    sig.sa_flags   = SA_RESTART;
    sigemptyset(&sig.sa_mask);
    sigaction(sig_nr, &sig, 0);
# endif
  }
#endif
