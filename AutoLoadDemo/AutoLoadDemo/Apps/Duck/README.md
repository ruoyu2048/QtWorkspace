<div align=center>
<img src="https://gitee.com/uploads/images/2018/0226/204052_d18504c9_20764.png" width="100" height="100" />
</div>


 Duck 基于Qt的插件式开发框架，他是一个简易的插件框架，所有业务由插件处理，与主框架无关，也可以把他称为程序的集合。由于要使用此框架开发个人项目，所以贡献出来让大家参考学习。
 

### Duck




 **运行环境** 

- windows
- Qt 5.9
- python2.7 OR python3



 **构建主框架** 

- 使用Qt Creator 打开Duck.pro  构建->运行
- 将AutoGetDllforWin.bat 放置主框架根目录下，他将程序所用的DLL拷贝至程序根目录，第三方库需要自己拷贝



 **新建插件** 

- 使用NewProject.py 会自动生成一个插件模板，你可以添加或者删除相应的Ui或者程序逻辑

- :fa-windows: + R

```
CMD

python NewProject.py

Enter Project Name: "输入插件名称按后回车键"
```

- 在plugin文件夹找到相应的插件文件夹，使用Qt Creator 打开相应的.pro文件 构建项目




 **运行** 

- 由于个人项目需要，目前插件为自动加载
- 将编译好的插件放置程序根目录，插件将自动加载



 **调试** 

- 目前尚未解决与主框架联调，其实也并不需要，因为主框架本没有什么可用的:stuck_out_tongue_winking_eye: 
- 分享我的调试方法，将插件写成可执行程序，然后将其变为插件。虽然方法Low了点，但是既简单又好用 :sunglasses: 





 **插件通信** 

- Shared Memory， Socket， Qt Remote Objects
