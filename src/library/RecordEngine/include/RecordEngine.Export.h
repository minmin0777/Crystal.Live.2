
#ifndef RecordEngine_EXPORT_H
#define RecordEngine_EXPORT_H

#ifdef RecordEngine_STATIC_DEFINE
#  define RecordEngine_EXPORT
#  define RecordEngine_NO_EXPORT
#else
#  ifndef RecordEngine_EXPORT
#    ifdef RecordEngine_EXPORTS
        /* We are building this library */
#      define RecordEngine_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define RecordEngine_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef RecordEngine_NO_EXPORT
#    define RecordEngine_NO_EXPORT 
#  endif
#endif

#ifndef RecordEngine_DEPRECATED
#  define RecordEngine_DEPRECATED __declspec(deprecated)
#endif

#ifndef RecordEngine_DEPRECATED_EXPORT
#  define RecordEngine_DEPRECATED_EXPORT RecordEngine_EXPORT RecordEngine_DEPRECATED
#endif

#ifndef RecordEngine_DEPRECATED_NO_EXPORT
#  define RecordEngine_DEPRECATED_NO_EXPORT RecordEngine_NO_EXPORT RecordEngine_DEPRECATED
#endif

#if 1 /* DEFINE_NO_DEPRECATED */
#  ifndef RecordEngine_NO_DEPRECATED
#    define RecordEngine_NO_DEPRECATED
#  endif
#endif

#endif /* RecordEngine_EXPORT_H */
