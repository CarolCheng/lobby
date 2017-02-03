# Design RESTful API with modern C++
## 總結
使用 C++ [crow](https://github.com/ipkn/crow), RESTful API和PostgreSQL實作一個使用者管理Web應用程式。

## 環境
1. 作業系統: Ubnutu 16.04.1 LTS 
2. 資料庫: PostgreSQL 9.5.5
3. 編譯器 IDE:
   * Visual Studio Code 1.8.1
   * CMake 3.5.3
4. C++11    
5. 依賴庫:
   * C++ Restful API: [crow](https://github.com/ipkn/crow)
      * Boost 1.59.0
      * libtcmalloc-minimal4
   * c++ PostgreSQL Client API : [libpqxx 4.0.0](http://pqxx.org/development/libpqxx/)
      * C client API : libpq
6. 測試客戶端: [RESTClient] (http://restclient.net/)

## RESTful API規格
|    功能    	|     URL    	| HTTP Method 	|   Request  	|     Response(status)    	|
|:----------:	|:----------:	|:-----------:	|:----------:	|:-----------------------:	|
| 新增使用者 	| /api/users 	|     POST    	| Users JSON 	| 201 (已新增) 404 (失敗) 	|
|            	|            	|             	|            	|                         	|
|            	|            	|             	|            	|                         	|

## 參考資料
1. [Using HTTP Methods for RESTful Services] (http://www.restapitutorial.com/lessons/httpmethods.html)
2. [使用 Flask-RESTful 设计 RESTful API] (http://www.pythondoc.com/flask-restful/second.html#id1)
3. [Designing a RESTful API with Python and Flask] (https://blog.miguelgrinberg.com/post/designing-a-restful-api-with-python-and-flask)
4. [pqxx::result Class Reference] (http://pages.cs.wisc.edu/~ajkunen/libpqxx-2.4.2/Reference/classpqxx_1_1result.html)
