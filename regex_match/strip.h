#pragma once

#include <regex>
#include <string>

#if defined (_UNICODE) || defined (UNICODE)
#    if  !defined (TCHAR)
#        define TCHAR wchar_t
#    endif
#    if  !defined (_tcslen)
#        define _tcslen wcslen
#    endif
#    define regex std::wregex
#    define string std::wstring
#    define cmatch std::wcmatch
#    define cregex_iterator std::wcregex_iterator
#else
#    if  !defined (TCHAR)
#        define TCHAR char
#    endif
#    if  !defined (_tcslen)
#        define _tcslen strlen
#    endif
#    define regex std::regex
#    define string std::string
#    define cmatch std::cmatch
#    define cregex_iterator std::cregex_iterator
#endif



///////////////////////////////////////////////////////////////////////////////
/// \brief тип, представляющий последовательность символов,
/// которые не обязательно заканчиваются null
///
struct strip
{
    TCHAR const * first;
    TCHAR const * last;
    strip( TCHAR const * const begin,
           TCHAR const * const end ) :
        first{ begin },
        last{ end }
    {}
    strip() : strip{ nullptr, nullptr } {}
};  //  struct strip



///////////////////////////////////////////////////////////////////////////////
/// \brief функции определения диапазона
/// \param s - strip объект
///
auto begin( strip const & s ) -> TCHAR const *
{
    return s.first;
}   //  auto begin(...
auto end( strip const & s ) -> TCHAR const *
{
    return s.last;
}   //  auto end(...



///////////////////////////////////////////////////////////////////////////////
/// \brief создание объекта
///
/// @code
/// auto s = make_strip( "Kerr, Kenny" );
/// @endcode
template <size_t Count>
auto make_strip( TCHAR const (&text)[Count] ) -> strip
{
    return strip{ text, text + Count - 1 };
}   //  auto make_strip(...
auto make_strip( TCHAR const *text ) -> strip
{
    return strip{ text, text + _tcslen(text) };
}   //  auto make_strip(...


///////////////////////////////////////////////////////////////////////////////
/// \brief определяет длинну и размер
/// \param s
///
auto size( strip const & s ) -> size_t
{
    return end( s ) - begin( s );
}   //  auto size(...



///////////////////////////////////////////////////////////////////////////////
/// \brief преобразовывает объект strip в строку
/// \param s- const strip
/// \return std::string - в зависимости от определения константы _UNICODE
///
#if defined (_UNICODE) || defined (UNICODE)
auto to_string(strip const & s) -> std::wstring
{
  return std::wstring { begin(s), end(s) };
}
#else
auto to_string(strip const & s) -> std::string
{
  return std::string { begin(s), end(s) };
}
#endif  //  auto to_string(...



///////////////////////////////////////////////////////////////////////////////
/// \brief распаковывает объект match в последовательные аргументы,
/// распаковка начинается с последих элементов объекта match, т.е.
/// если в объекта match 3 элемента, а передали 2 элемента для распаковки,
/// то 2-ой элемент объекта match распакуется в первую переменную,
/// 3-ий элемент объекта match распакуется во вторую переменную,
/// а 1-ый элемент объекта match не распакуется.
/// \param mtch - объект match
/// \param args - пакет функциональных параметров (function parameter pack)
///
template <typename... Args>
auto unpack( cmatch const & mtch,
             Args & ... args ) -> void
{
    if ( sizeof...(Args) > mtch.size() )
        return;
    unpack( mtch.size()-1, mtch, args... );
}



///////////////////////////////////////////////////////////////////////////////
/// \brief эта функция unpack отделяет первый аргумент, следующий за объектом
/// совпадения, от остальных аргументов
/// \param mtch - объект match - массив совпадений, согластно паттерну
/// \param str  - объект strip - первый элемент и списка параметров
/// \param args - пакет функциональных параметров (function parameter pack)
///
template <typename... Args>
auto unpack( size_t Total,
             cmatch const & mtch,
             strip & str,
             Args & ... args ) -> void
{
    auto const & elem = mtch[Total - sizeof...(Args)];
    str = { elem.first, elem.second };
    unpack( Total, mtch, args... );
}



///////////////////////////////////////////////////////////////////////////////
/// \brief распаковывает объект match в последовательные аргументы
/// \param mtch - объект match
/// \param args - пакет функциональных параметров (function parameter pack)
///
auto unpack( size_t Total,
             cmatch const & mtch,
             strip & str ) -> void
{
    auto const & v = mtch[Total];
    str = { v.first, v.second };
}



///////////////////////////////////////////////////////////////////////////////
/// \brief - загушкаб когда пакетные параметры законцились
///
//template <size_t>
auto unpack( size_t, cmatch const & ) -> void {}


///////////////////////////////////////////////////////////////////////////////
/// \brief распаковать строку str, по паттерну rgx, в параметры
/// \param str  - строка (strip), в которой ищем совпадения
/// \param rgx  - условие поиска
/// \param args - параметры, в которые копируем результат поимска
/// \return true - есть совпадения, false - совпадений нет
///
template <typename... Args>
auto match( strip const & str,
            regex const & rgx,
            Args & ... args ) -> bool
{
    auto mtch = cmatch {};
    if ( std::regex_match( begin(str), end(str), mtch, rgx ) )
    {
        unpack( mtch, args... );
    }
    return !mtch.empty();
}
template <typename... Args>
auto match( string const & str,
            regex const & rgx,
            Args & ... args ) -> bool
{
    auto mtch = cmatch {};
    if ( std::regex_match( str.c_str(), mtch, rgx ) )
    {
        unpack( mtch, args... );
    }
    return !mtch.empty();
}
template <typename... Args>
auto match( TCHAR const * str,
            regex const & rgx,
            Args & ... args ) -> bool
{
    auto mtch = cmatch {};
    if ( std::regex_match( str, mtch, rgx ) )
    {
        unpack( mtch, args... );
    }
    return !mtch.empty();
}



///////////////////////////////////////////////////////////////////////////////
/// \brief определение диапазона итератора для реализации for_each
///
template <typename T>
struct iterator_range
{
  T first, last;
  auto begin() const -> T { return first; }
  auto end()   const -> T { return last; }
};



///////////////////////////////////////////////////////////////////////////////
/// \brief for_each - перебор диапазона контенера
/// \param str  - строка (strip), в которой ищем совпадения
/// \param rgx  - условие поиска
/// \return     - объект match
///
auto for_each( strip const & str,
               regex const & rgx ) -> iterator_range<cregex_iterator>
{
  return
  {
      cregex_iterator{ begin( str ), end( str ), rgx },
      cregex_iterator{}
  };
}
