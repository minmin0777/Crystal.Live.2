
#ifndef Codec_EXPORT_H
#define Codec_EXPORT_H

#ifdef Codec_STATIC_DEFINE
#  define Codec_EXPORT
#  define Codec_NO_EXPORT
#else
#  ifndef Codec_EXPORT
#    ifdef Codec_EXPORTS
        /* We are building this library */
#      define Codec_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define Codec_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef Codec_NO_EXPORT
#    define Codec_NO_EXPORT 
#  endif
#endif

#ifndef Codec_DEPRECATED
#  define Codec_DEPRECATED __declspec(deprecated)
#endif

#ifndef Codec_DEPRECATED_EXPORT
#  define Codec_DEPRECATED_EXPORT Codec_EXPORT Codec_DEPRECATED
#endif

#ifndef Codec_DEPRECATED_NO_EXPORT
#  define Codec_DEPRECATED_NO_EXPORT Codec_NO_EXPORT Codec_DEPRECATED
#endif

#if 1 /* DEFINE_NO_DEPRECATED */
#  ifndef Codec_NO_DEPRECATED
#    define Codec_NO_DEPRECATED
#  endif
#endif

#endif /* Codec_EXPORT_H */
