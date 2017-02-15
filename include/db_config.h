#ifndef _DB_CONFIG_
#define _DB_CONFIG_
namespace db_config {
    static std::string hostname = "127.0.0.1";
    static std::string user = "postgres";
    static std::string password =  "db2017tw";
    static std::string dbname = "postgres";
    static std::string port = "5432";
    

}
/**
*  @brief
*	HTTP status codes 
*	- 2xx Success 
*	- 4xx Client Error
*	- 5xx Server Error
*	RQD: Required INFO: Information
*/
 enum class HTTPStatus: std::uint32_t{	OK						= 200,
										Created 				= 201,
										Accepted 				= 202,
										NonAuthoritativeINFO	= 203,
										NoContent 				= 204,
										ResetContent 			= 205,
										PartialContent			= 206,
										BadRequest				= 400,
										Unauthorized			= 401,
										PaymentRQD				= 402,
										Forbidden				= 403,
										NotFound				= 404,
										MethodNotAllowed		= 405,
										NotAcceptable 			= 406,
										ProxyAuthenticationRQD	= 407,
										RequestTimeout 			= 408,
										Conflict				= 409,
										Gone					= 410,
										LengthRQD				= 411,
										InternalServerError		= 500
									};
class HTTP 
{
public:
    static uint32_t to_uint(HTTPStatus status) 
    {
        return static_cast<std::size_t>(status);
    }
};
#endif //_DB_CONFIG_