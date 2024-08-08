
#ifndef NetCapture_EXPORT_H
#define NetCapture_EXPORT_H

#ifdef NetCapture_STATIC_DEFINE
#  define NetCapture_EXPORT
#  define NetCapture_NO_EXPORT
#else
#  ifndef NetCapture_EXPORT
#    ifdef NetCapture_EXPORTS
        /* We are building this library */
#      define NetCapture_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define NetCapture_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef NetCapture_NO_EXPORT
#    define NetCapture_NO_EXPORT 
#  endif
#endif

#ifndef NetCapture_DEPRECATED
#  define NetCapture_DEPRECATED __declspec(deprecated)
#endif

#ifndef NetCapture_DEPRECATED_EXPORT
#  define NetCapture_DEPRECATED_EXPORT NetCapture_EXPORT NetCapture_DEPRECATED
#endif

#ifndef NetCapture_DEPRECATED_NO_EXPORT
#  define NetCapture_DEPRECATED_NO_EXPORT NetCapture_NO_EXPORT NetCapture_DEPRECATED
#endif

#if 1 /* DEFINE_NO_DEPRECATED */
#  ifndef NetCapture_NO_DEPRECATED
#    define NetCapture_NO_DEPRECATED
#  endif
#endif

#endif /* NetCapture_EXPORT_H */
