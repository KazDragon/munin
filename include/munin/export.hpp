
#ifndef MUNIN_EXPORT_H
#define MUNIN_EXPORT_H

#ifdef MUNIN_STATIC_DEFINE
#  define MUNIN_EXPORT
#  define MUNIN_NO_EXPORT
#else
#  ifndef MUNIN_EXPORT
#    ifdef munin_EXPORTS
        /* We are building this library */
#      define MUNIN_EXPORT 
#    else
        /* We are using this library */
#      define MUNIN_EXPORT 
#    endif
#  endif

#  ifndef MUNIN_NO_EXPORT
#    define MUNIN_NO_EXPORT 
#  endif
#endif

#ifndef MUNIN_DEPRECATED
#  define MUNIN_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef MUNIN_DEPRECATED_EXPORT
#  define MUNIN_DEPRECATED_EXPORT MUNIN_EXPORT MUNIN_DEPRECATED
#endif

#ifndef MUNIN_DEPRECATED_NO_EXPORT
#  define MUNIN_DEPRECATED_NO_EXPORT MUNIN_NO_EXPORT MUNIN_DEPRECATED
#endif

#define DEFINE_NO_DEPRECATED 0
#if DEFINE_NO_DEPRECATED
# define MUNIN_NO_DEPRECATED
#endif

#endif
