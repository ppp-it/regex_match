MATCH REGEX
===========

getting matches from one row to individual variables  
for each match in the line by the condition  
  
  
Example of use.
---------------
```c++
    /* use strip */
    //strip url = make_strip( argv[1] );
    /* or std::string */
    string url = _T( R"(https://github.com/ppp-it/match_regex)" );// argv[1];
	
    string host, object;
	strip hst, obj;
	auto rgx = regex{ _T( R"(([a-z][a-z0-9+\-.]*)://([a-z0-9\-._~%]+|\[[a-f0-9:.]+\]|\[v[a-f0-9][a-z0-9\-._~%!$&'()*+,;=:]+\])/(.*$))" ) };  // паттерн на парсинг url

	/* из строки - url, по паттерну - rgx получим:
	 * имя хоста в переменную - hst
	 * имя объекта в переменную - obj */
	match( url, rgx, hst, obj );
	_tprintf( _T( "'%.*s' '%.*s'\n" ),
			  size( hst ), begin( hst ),
			  size( obj ), begin( obj ) 
	);  //  'github.com' 'ppp-it/match_regex'
	
	host = to_string( hst );
	object = to_string( obj );


    //---------------------------------------------------------------------------------------------
    string param_list = _T( R"(--url=<https://github.com> --header=<match_regex> --DestAddress=<teshcha@zhizni.net> --MessageTitle=<unpak match regex> --MessageText=<download here: https://github.com/ppp-it/match_regex>)" );
    auto const s = make_strip( param_list.c_str() );
    auto const r = regex{ _T( R"(\-\-(.*?)=\<(.*?)\>)" ) };  // паттерн на парсинг ключа и значения

    std::unordered_map<string, string> param_map;

    for ( 	auto const & m : for_each( s, r ) )
    {
        strip key, value;
        unpack( m, key, value );

        param_map.emplace( std::piecewise_construct,
                           std::forward_as_tuple( to_string( key ) ),
                           std::forward_as_tuple( to_string( value ) )
        );

        _tprintf( _T( "'%.*s' '%.*s'\n" ),
                  size( key ), begin( key ),
                  size( value ), begin( value ) 
        );  /*
            'url' 'https://github.com'
            'header' 'match_regex'
            'DestAddress' 'teshcha@zhizni.net'
            'MessageTitle' 'unpak match regex'
            'MessageText' 'download here: https://github.com/ppp-it/match_regex'
            */
    }
    //---------------------------------------------------------------------------------------------

 ```  
