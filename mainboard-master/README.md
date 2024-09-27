## project settings
not sure if project settings reacts well with git. but just in case here is all the project settings you should change:
1. Clone [microros](https://github.com/micro-ROS/micro_ros_stm32cubemx_utils.git) repository in your STM32CubeIDE project folder. Make sure it is the humble branch
1. Go to `Project -> Settings -> C/C++ Build -> Settings -> Build Steps Tab` and in `Pre-build steps` add:
	```bash
	docker pull microros/micro_ros_static_library_builder:humble && docker run --rm -v ${workspace_loc:/${ProjName}}:/project --env MICROROS_LIBRARY_FOLDER=micro_ros_stm32cubemx_utils/microros_static_library_ide microros/micro_ros_static_library_builder:humble
	```
1. In `Project -> Settings -> C/C++ Build -> Settings -> Tool Settings Tab -> MCU GCC Compiler -> Include paths` add
	```bash
	../micro_ros_stm32cubemx_utils/microros_static_library_ide/libmicroros/include
 	```
 	```bash
 	"${workspace_loc:/${ProjName}/Core/Src/MAIN}"
	```
	```bash
 	../Middlewares/ST/STM32_USB_Device_Library/Core/Inc
	```
	```bash
 	../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc
	```
1. In `Project -> Settings -> C/C++ Build -> Settings -> MCU GCC Linker -> Libraries`:
	+ in `Library search path (-L)`         
	```bash
	<ABSOLUTE_PATH_TO>/micro_ros_stm32cubemx_utils/microros_static_library_ide/libmicroros
	``` 
	+ in `Libraries (-l)`: add `microros`
1. Add the following source code files to your project, dragging them to source folder:
	- `extra_sources/microros_time.c`
	- `extra_sources/microros_allocators.c`
	- `extra_sources/custom_memory_manager.c`
	- `extra_sources/microros_transports/dma_transport.c`
1. Make sure that if you are using FreeRTOS, the micro-ROS task **has more than 10 kB of stack**: [Detail](https://github.com/micro-ROS/micro_ros_stm32cubemx_utils/blob/iron/.images/Set_freertos_stack.jpg)
1. Build and run

## some tests:
+ tests were measured with `ros2 topic hz` with a sample size of 10k msgs for each test.
+ tests were measured with [`std_msgs/Int32MultiArray.msg`](http://docs.ros.org/en/melodic/api/std_msgs/html/msg/Int32MultiArray.html) which has some overhead so an array of 15 is not exactly 15 but more.

- array of 1 	~450 hz
- array of 2 	~440 hz

- array of 3 	~320 hz
- array of 4 	~315 hz

- array of 5 	~308 hz
- array of 7 	~305 hz	

- array of 14 	~234 hz
- array of 15 	~230 hz




