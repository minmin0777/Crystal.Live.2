
#ifndef Sip_Plugin_EXPORT_H
#define Sip_Plugin_EXPORT_H

#ifdef Sip_Plugin_STATIC_DEFINE
#  define Sip_Plugin_EXPORT
#  define Sip_Plugin_NO_EXPORT
#else
#  ifndef Sip_Plugin_EXPORT
#    ifdef Sip_Plugin_EXPORTS
        /* We are building this library */
#      define Sip_Plugin_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define Sip_Plugin_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef Sip_Plugin_NO_EXPORT
#    define Sip_Plugin_NO_EXPORT 
#  endif
#endif

#ifndef Sip_Plugin_DEPRECATED
#  define Sip_Plugin_DEPRECATED __declspec(deprecated)
#endif

#ifndef Sip_Plugin_DEPRECATED_EXPORT
#  define Sip_Plugin_DEPRECATED_EXPORT Sip_Plugin_EXPORT Sip_Plugin_DEPRECATED
#endif

#ifndef Sip_Plugin_DEPRECATED_NO_EXPORT
#  define Sip_Plugin_DEPRECATED_NO_EXPORT Sip_Plugin_NO_EXPORT Sip_Plugin_DEPRECATED
#endif

#if 1 /* DEFINE_NO_DEPRECATED */
#  ifndef Sip_Plugin_NO_DEPRECATED
#    define Sip_Plugin_NO_DEPRECATED
#  endif
#endif

#endif /* Sip_Plugin_EXPORT_H */
