#ifndef LAB3_API_H
#define LAB3_API_H

/**
 * @brief Экспорт и импорт функций библиотеки в Windows
 *
 * LAB3_SHARED нужен для DLL, LAB3_BUILD задается при сборке самой DLL.
 * При статической сборке объявление остается обычным.
 */
#if defined(_WIN32) && defined(LAB3_SHARED)
    #if defined(LAB3_BUILD)
        #define LAB3_API __declspec(dllexport)
    #else
        #define LAB3_API __declspec(dllimport)
    #endif
#else
    #define LAB3_API
#endif

#endif
