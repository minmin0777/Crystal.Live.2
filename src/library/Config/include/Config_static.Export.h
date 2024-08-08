
#ifndef Config_static_EXPORT_H
#define Config_static_EXPORT_H

#ifdef Config_static_STATIC_DEFINE
#  define Config_static_EXPORT
#  define Config_static_NO_EXPORT
#else
#  ifndef Config_static_EXPORT
#    ifdef Config_static_EXPORTS
        /* We are building this library */
#      define Config_static_EXPORT 
#    else
        /* We are using this library */
#      define Config_static_EXPORT 
#    endif
#  endif

#  ifndef Config_static_NO_EXPORT
#    define Config_static_NO_EXPORT 
#  endif
#endif

#ifndef Config_static_DEPRECATED
#  define Config_static_DEPRECATED __declspec(deprecated)
#endif

#ifndef Config_static_DEPRECATED_EXPORT
#  define Config_static_DEPRECATED_EXPORT Config_static_EXPORT Config_static_DEPRECATED
#endif

#ifndef Config_static_DEPRECATED_NO_EXPORT
#  define Config_static_DEPRECATED_NO_EXPORT Config_static_NO_EXPORT Config_static_DEPRECATED
#endif

#if 1 /* DEFINE_NO_DEPRECATED */
#  ifndef Config_static_NO_DEPRECATED
#    define Config_static_NO_DEPRECATED
#  endif
#endif

#endif /* Config_static_EXPORT_H */
