
#ifndef XMIDICTRL_EXPORT_H
#define XMIDICTRL_EXPORT_H

#ifdef XMIDICTRL_STATIC_DEFINE
#  define XMIDICTRL_EXPORT
#  define XMIDICTRL_NO_EXPORT
#else
#  ifndef XMIDICTRL_EXPORT
#    ifdef XMidiCtrl_EXPORTS
        /* We are building this library */
#      define XMIDICTRL_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define XMIDICTRL_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef XMIDICTRL_NO_EXPORT
#    define XMIDICTRL_NO_EXPORT 
#  endif
#endif

#ifndef XMIDICTRL_DEPRECATED
#  define XMIDICTRL_DEPRECATED __declspec(deprecated)
#endif

#ifndef XMIDICTRL_DEPRECATED_EXPORT
#  define XMIDICTRL_DEPRECATED_EXPORT XMIDICTRL_EXPORT XMIDICTRL_DEPRECATED
#endif

#ifndef XMIDICTRL_DEPRECATED_NO_EXPORT
#  define XMIDICTRL_DEPRECATED_NO_EXPORT XMIDICTRL_NO_EXPORT XMIDICTRL_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef XMIDICTRL_NO_DEPRECATED
#    define XMIDICTRL_NO_DEPRECATED
#  endif
#endif

#endif /* XMIDICTRL_EXPORT_H */
