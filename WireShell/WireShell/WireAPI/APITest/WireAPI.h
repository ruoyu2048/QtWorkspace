#ifdef WIREAPI_EXPORTS
#define WIREAPI_API __declspec(dllexport)
#else
#define WIREAPI_API __declspec(dllimport)
#endif

#define WIRE_NO_ERROR 0
#define WIRE_OPEN_FAIL -1

#define WIRE_PASSED 1
#define WIRE_FAILED 0

/**
 打开连接上测试机
 @ip : 测试机IP地址,默认不填写将视为本机
 @return : 返回0表示连接成功,返回1表示连接失败
 */
WIREAPI_API int wire_Open(const char * ip = NULL);


/**
 关闭连接
 @param 无
 */
WIREAPI_API void wire_Close();


/**
 运行测试工程
 @pri_name : 工程名称
 @return : 返回1表示测试通过,返回0表示测试失败
 */
WIREAPI_API int wire_RunProject(const char * pri_name); 


/**
 获取当前的测试报告路径
 @buff : 路径字符串填充缓存
 @buff_len : 缓存字节数
 @return : 返回0表示获取成功,返回1表示获取失败
 */
WIREAPI_API int wire_GetTestReport(char * buff,int buff_len); 