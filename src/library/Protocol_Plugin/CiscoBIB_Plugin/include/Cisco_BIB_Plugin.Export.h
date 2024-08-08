
#ifndef Cisco_BIB_Plugin_EXPORT_H
#define Cisco_BIB_Plugin_EXPORT_H

#ifdef Cisco_BIB_Plugin_STATIC_DEFINE
#  define Cisco_BIB_Plugin_EXPORT
#  define Cisco_BIB_Plugin_NO_EXPORT
#else
#  ifndef Cisco_BIB_Plugin_EXPORT
#    ifdef Cisco_BIB_Plugin_EXPORTS
        /* We are building this library */
#      define Cisco_BIB_Plugin_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define Cisco_BIB_Plugin_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef Cisco_BIB_Plugin_NO_EXPORT
#    define Cisco_BIB_Plugin_NO_EXPORT 
#  endif
#endif

#ifndef Cisco_BIB_Plugin_DEPRECATED
#  define Cisco_BIB_Plugin_DEPRECATED __declspec(deprecated)
#endif

#ifndef Cisco_BIB_Plugin_DEPRECATED_EXPORT
#  define Cisco_BIB_Plugin_DEPRECATED_EXPORT Cisco_BIB_Plugin_EXPORT Cisco_BIB_Plugin_DEPRECATED
#endif

#ifndef Cisco_BIB_Plugin_DEPRECATED_NO_EXPORT
#  define Cisco_BIB_Plugin_DEPRECATED_NO_EXPORT Cisco_BIB_Plugin_NO_EXPORT Cisco_BIB_Plugin_DEPRECATED
#endif

#if 1 /* DEFINE_NO_DEPRECATED */
#  ifndef Cisco_BIB_Plugin_NO_DEPRECATED
#    define Cisco_BIB_Plugin_NO_DEPRECATED
#  endif
#endif

#endif /* Cisco_BIB_Plugin_EXPORT_H */
