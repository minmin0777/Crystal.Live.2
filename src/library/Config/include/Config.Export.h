
#ifndef Config_EXPORT_H
#define Config_EXPORT_H

#ifdef Config_STATIC_DEFINE
#  define Config_EXPORT
#  define Config_NO_EXPORT
#else
#  ifndef Config_EXPORT
#    ifdef Config_EXPORTS
        /* We are building this library */
#      define Config_EXPORT 
#    else
        /* We are using this library */
#      define Config_EXPORT 
#    endif
#  endif

#  ifndef Config_NO_EXPORT
#    define Config_NO_EXPORT 
#  endif
#endif

#ifndef Config_DEPRECATED
#  define Config_DEPRECATED __declspec(deprecated)
#endif

#ifndef Config_DEPRECATED_EXPORT
#  define Config_DEPRECATED_EXPORT Config_EXPORT Config_DEPRECATED
#endif

#ifndef Config_DEPRECATED_NO_EXPORT
#  define Config_DEPRECATED_NO_EXPORT Config_NO_EXPORT Config_DEPRECATED
#endif

#if 1 /* DEFINE_NO_DEPRECATED */
#  ifndef Config_NO_DEPRECATED
#    define Config_NO_DEPRECATED
#  endif
#endif

#endif /* Config_EXPORT_H */
