#ifdef WIREAPI_EXPORTS
#define WIREAPI_API __declspec(dllexport)
#else
#define WIREAPI_API __declspec(dllimport)
#endif

#define WIRE_NO_ERROR 0
#define WIRE_OPEN_FAIL -1

enum
{
	WIRE_FAILED = 0,       // 测试失败
	WIRE_PASSED = 1,       // 测试通过
	WIRE_CANCELLED = 2,    // 测试被取消
	WIRE_BUSY = 3          // 测试未执行系统忙
};

enum
{
	NG_OPEN = 1,       // 开路 
	NG_SHORT,          // 短路
	NG_DIODE_INVERSE   // 二极管反向
};

//
// 传递参数对象
//
typedef struct 
{
	char pri_name[64];     // 工程名或者产品编号
	char spareparts[64];   // 规格号
	char reference[64];    // 品番
	char batch[64];        // 批次 
	int counter;           // 流水
	char date_time[64];    // 时间
	char the_operator[64]; // 操作工
	char team[64];         // 班组
	int operat_flag;       // 信号   
	char label_no[64];     // 电测签码
	char error_coding[64]; // NG代号
	char reverved[512];    // 保留空间
} TestInfo;

/**
 打开连接上测试机
 @ip : 测试机IP地址,默认不填写将视为本机
 @return : 返回0表示连接成功,返回1表示连接失败
 */
WIREAPI_API int wire_Open(const char * ip = NULL);

/**
查看当前是否与测试机时连接状态
@return : 1 (连接)  0 (断开)
*/
WIREAPI_API int wire_IsOpened();

/**
 关闭连接
 @param 无
 */
WIREAPI_API void wire_Close();

/**
加载测试工程
@key_word : 工程名称/或者产品编号
@return : 返回1表示加载成功,返回0表示加载失败
*/
WIREAPI_API int wire_LoadProject(const char * key_word);

/**
 运行测试工程
 @test_info : 参数对象,用于传递交互信息
 @return : 返回1表示测试通过,返回0表示测试失败,返回2表示测试取消
 */
WIREAPI_API int wire_RunProject(TestInfo * test_info); 


/**
 获取当前的测试报告路径
 @buff : 路径字符串填充缓存
 @buff_len : 缓存字节数
 @return : 返回0表示获取成功,返回1表示获取失败
 */
WIREAPI_API int wire_GetTestReport(char * buff,int buff_len); 