
#ifndef Audio_EXPORT_H
#define Audio_EXPORT_H

#ifdef Audio_STATIC_DEFINE
#  define Audio_EXPORT
#  define Audio_NO_EXPORT
#else
#  ifndef Audio_EXPORT
#    ifdef Audio_EXPORTS
        /* We are building this library */
#      define Audio_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define Audio_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef Audio_NO_EXPORT
#    define Audio_NO_EXPORT 
#  endif
#endif

#ifndef Audio_DEPRECATED
#  define Audio_DEPRECATED __declspec(deprecated)
#endif

#ifndef Audio_DEPRECATED_EXPORT
#  define Audio_DEPRECATED_EXPORT Audio_EXPORT Audio_DEPRECATED
#endif

#ifndef Audio_DEPRECATED_NO_EXPORT
#  define Audio_DEPRECATED_NO_EXPORT Audio_NO_EXPORT Audio_DEPRECATED
#endif

#if 1 /* DEFINE_NO_DEPRECATED */
#  ifndef Audio_NO_DEPRECATED
#    define Audio_NO_DEPRECATED
#  endif
#endif

#endif /* Audio_EXPORT_H */
