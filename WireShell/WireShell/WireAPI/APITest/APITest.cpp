//
// 调用示例子代码
//

#include "stdafx.h"
#include <windows.h>
#include "../WireAPI.h"

int RunProject(int argc, _TCHAR* argv[])
{
	char * ip = "192.168.0.143";      // 本机(localhost)/或者其它WireTest主机IP地址
	char * prj_name = "DIN30";        // 工程名或者产品编号/序列名称
	TestInfo info;
	memset(&info,0,sizeof(TestInfo));

	if(argc == 3)
	{
		ip = argv[1];
		prj_name = argv[2];
	}

	// 传递参数
	strcpy(info.pri_name,prj_name);   // 工程名
	strcpy(info.spareparts,"SP001");  // 规格号
	strcpy(info.batch,"20190815");    // 批号
	info.counter = 100;               // 流水  

	if(wire_Open(ip) == WIRE_NO_ERROR)  // 连上测试机
	{
		if(wire_LoadProject(prj_name))  // 加载工程或者序列
		{
			int r = wire_RunProject(&info);  // 运行当前工程或者序列
			if(r == 1)                      // 运行通过
				printf("Run %s PASSED\n",prj_name);
			else if(r == 2)                 // 运行被取消
				printf("Project %s Cancelled\n",prj_name);
			else                            // 运行失败  
				printf("Run %s FAILED\n",prj_name);

			// 打印返回信息
			printf("Operator:%s\n",info.the_operator);
			printf("DateTime:%s\n",info.date_time);
			printf("TestResult:%d\n",info.operat_flag);
			// ...
		}
		else
		{
			printf("Load %s FAILED\n",prj_name);
		}

		wire_Close();
	}

	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	for(int i=0;i<3;i++)
	{
		printf("--->Try %d...",i+1);
		RunProject(argc,argv);

		Sleep(1000);
	}

	return 0;
}