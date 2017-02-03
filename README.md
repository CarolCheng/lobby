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
   * c++ PostgreSQL Client API : [libpqxx 4.0.2](http://pqxx.org/development/libpqxx/)
      * C client API : libpq
6. 測試客戶端: [RESTClient] (http://restclient.net/)

## PostgreSQL
請先在本機安裝好PostgreSQL, 啟動此服務段將自動建立表格players, 資料表欄位如以下所示:
(id是PostgreSQL自動產生)

![database schema](https://github.com/CarolCheng/lobby/blob/master/docs/databaseschema.png?raw=true?raw=true)

## Players JSON 
以下為Players JSON的範例: 
```json
{ 
	"id" 		: 1,
	"username" 	: "carol", 
	"password"	: "123456", 
	"userdata"	: "123456" 
}
```
新增使用者時, 請不要填id, 新增成功服務端將返回id, 如下圖所示:
![RESTClient_addPlayers](https://github.com/CarolCheng/lobby/blob/master/docs/RESTClient_addPlayers.png?raw=true?raw=true)

## RESTful API規格
|      功能      	|       URL      	| HTTP Method 	|    Request   	|     Response(status)     	| Response Body 	|
|:--------------:	|:--------------:	|:-----------:	|:------------:	|:------------------------:	|:-------------:	|
|   新增使用者   	|   /api/users   	|     POST    	| Players JSON 	| 201 (已新增), 404 (失敗) 	|  Players JSON 	|
| 取得使用者資訊 	| /api/users/:id 	|     GET     	|      --      	| 200 (成功), 404 (無效ID) 	|  Players JSON 	|
| 更新使用者資訊 	| /api/users/:id 	|     PUT     	| Players JSON 	| 200 (成功), 404 (無效ID) 	|       --      	|
|   刪除使用者   	| /api/users/:id 	|    DELETE   	|      --      	| 200 (成功), 404 (無效ID) 	|       --      	|
| 取得使用者列表 	|   /api/users/  	|     GET     	|      --      	| 200 (成功), 404 (無效ID) 	|  Players JSON 	|

## 參考資料
1. [Using HTTP Methods for RESTful Services] (http://www.restapitutorial.com/lessons/httpmethods.html)
2. [使用 Flask-RESTful 设计 RESTful API] (http://www.pythondoc.com/flask-restful/second.html#id1)
3. [Designing a RESTful API with Python and Flask] (https://blog.miguelgrinberg.com/post/designing-a-restful-api-with-python-and-flask)
4. [libpqxx 4.0.2 - pqxx::result Class Reference] (http://pqxx.org/devprojects/libpqxx/doc/4.0/html/Reference/a00082.html)
5. [postgreSQL function for last inserted ID] (http://stackoverflow.com/questions/2944297/postgresql-function-for-last-inserted-id)
6. [Crow is C++ microframework for web. (inspired by Python Flask)] (https://github.com/ipkn/crow)


