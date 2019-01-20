// regex_match.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <stdio.h>
#include <unordered_map>    /* std::unordered_map */
#include <tchar.h>
#include "strip.h"

using namespace std;

int _tmain( int argc, TCHAR* argv[] )
{
    /* use strip */
    //strip url = make_strip( argv[1] );
    /* or std::string */
    string url = _T( R"(https://github.com/ppp-it/match_regex)" );// argv[1];

    string shema, host, object;

    // получим протокол, хост и объект
    {
        printf( "\n!!!   example use match   !!!\nparsing string:\n" );
        _tprintf( url.c_str() );
        printf( "\n\n" );

        strip full, shm, hst, obj;
        auto rgx = regex{ _T( R"(([a-z][a-z0-9+\-.]*)://([a-z0-9\-._~%]+|\[[a-f0-9:.]+\]|\[v[a-f0-9][a-z0-9\-._~%!$&'()*+,;=:]+\])/(.*$))" ) };  // паттерн на парсинг url

        /* из строки - url, по паттерну - rgx получим:
         * имя хоста в переменную - hst
         * имя объекта в переменную - obj */
        match( url, rgx, hst, obj );
        _tprintf( _T( "'%.*s' '%.*s'\n" ),
            (int)size( hst ), begin( hst ),
                  (int)size( obj ), begin( obj )
        );  //  'github.com' 'ppp-it/match_regex'

        /* из строки - url, по паттерну - rgx получим:
         * протокол в переменную - shm
         * имя хоста в переменную - hst
         * имя объекта в переменную - obj */
        match( url, rgx, shm, hst, obj );
        _tprintf( _T( "'%.*s' '%.*s' '%.*s'\n" ),
            (int)size( shm ), begin( shm ),
                  (int)size( hst ), begin( hst ),
                  (int)size( obj ), begin( obj )
        );  //  'https' 'github.com' 'ppp-it/match_regex'

        /* из строки - url, по паттерну - rgx получим:
         * оригинальную строку в переменную - full
         * протокол в переменную - shm
         * имя хоста в переменную - hst
         * имя объекта в переменную - obj */
        match( url, rgx, full, shm, hst, obj );
        _tprintf( _T( "'%.*s' '%.*s' '%.*s' '%.*s'\n" ),
            (int)size( full ), begin( full ),
                  (int)size( shm ), begin( shm ),
                  (int)size( hst ), begin( hst ),
                  (int)size( obj ), begin( obj )
        );  //  'https://github.com/ppp-it/match_regex' 'https' 'github.com' 'ppp-it/match_regex'

        shema = to_string( shm );
        host = to_string( hst );
        object = to_string( obj );

    }


    //---------------------------------------------------------------------------------------------
    string param_list = _T( R"(--url=<https://github.com> --header=<match_regex> --DestAddress=<teshcha@zhizni.net> --MessageTitle=<unpak match regex> --MessageText=<download here: https://github.com/ppp-it/match_regex>)" );
    auto const s = make_strip( param_list.c_str() );
    auto const r = regex{ _T( R"(\-\-(.*?)=\<(.*?)\>)" ) };  // паттерн на парсинг ключа и значения

    std::unordered_map<string, string> param_map;


    printf( "\n!!!   example use for_each   !!!\nparsing string:\n" );
    _tprintf( param_list.c_str() );
    printf( "\n\nparsing key & value\n" );
    for (auto const & m : for_each( s, r ))
    {
        strip key, value;
        unpack( m, key, value );

        param_map.emplace( to_string( key ), to_string( value ) );

        _tprintf( _T( "'%.*s' '%.*s'\n" ),
            (int)size( key ), begin( key ),
                  (int)size( value ), begin( value )
        );  /*
            'url' 'https://github.com'
            'header' 'match_regex'
            'DestAddress' 'teshcha@zhizni.net'
            'MessageTitle' 'unpak match regex'
            'MessageText' 'download here: https://github.com/ppp-it/match_regex'
            */
    }
    printf( "\n" );
    //---------------------------------------------------------------------------------------------


    return 0;
}


// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
