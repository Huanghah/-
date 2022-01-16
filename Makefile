CC = arm-linux-gcc								#编译工具链  ARM 
TRG = bin/main							#目标文件 --> 可执行程序
SRC = $(wildcard ./src/*.c)				#src目录下的所有源文件 (xx.c)
CFLAGS = -I ./include -L ./lib -lArmfont -lm 		#编译过程中命令行参数：指定头文件路径 ./include , 库文件路径 ./lib 
OBJ = $(patsubst %.c, %.o, $(SRC))		#编译过程中的文件  xx.o

$(TRG):$(SRC)							#把.o文件生成可执行文件
	$(CC) $^ -o $@ $(CFLAGS)

	
clean:									#删除编译过程中产生的文件！
	$(RM) $(OBJ)
	$(RM) $(TRG)				
	
	
	