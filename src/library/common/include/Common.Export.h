
#ifndef Common_EXPORT_H
#define Common_EXPORT_H

#ifdef Common_STATIC_DEFINE
#  define Common_EXPORT
#  define Common_NO_EXPORT
#else
#  ifndef Common_EXPORT
#    ifdef Common_EXPORTS
        /* We are building this library */
#      define Common_EXPORT 
#    else
        /* We are using this library */
#      define Common_EXPORT 
#    endif
#  endif

#  ifndef Common_NO_EXPORT
#    define Common_NO_EXPORT 
#  endif
#endif

#ifndef Common_DEPRECATED
#  define Common_DEPRECATED __declspec(deprecated)
#endif

#ifndef Common_DEPRECATED_EXPORT
#  define Common_DEPRECATED_EXPORT Common_EXPORT Common_DEPRECATED
#endif

#ifndef Common_DEPRECATED_NO_EXPORT
#  define Common_DEPRECATED_NO_EXPORT Common_NO_EXPORT Common_DEPRECATED
#endif

#if 1 /* DEFINE_NO_DEPRECATED */
#  ifndef Common_NO_DEPRECATED
#    define Common_NO_DEPRECATED
#  endif
#endif

#endif /* Common_EXPORT_H */
