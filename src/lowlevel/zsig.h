/** zsparsell zsig.h
    (C) 2018 Erik Zscheile
    License: MIT
 **/
#pragma once
#ifdef __cplusplus
extern "C" {
#endif
  typedef void (*sighandler_t)(int);
  void my_signal(int sig_nr, sighandler_t signalhandler);
#ifdef __cplusplus
}
#endif
