CC = arm-linux-gcc								#���빤����  ARM 
TRG = bin/main							#Ŀ���ļ� --> ��ִ�г���
SRC = $(wildcard ./src/*.c)				#srcĿ¼�µ�����Դ�ļ� (xx.c)
CFLAGS = -I ./include -L ./lib -lArmfont -lm 		#��������������в�����ָ��ͷ�ļ�·�� ./include , ���ļ�·�� ./lib 
OBJ = $(patsubst %.c, %.o, $(SRC))		#��������е��ļ�  xx.o

$(TRG):$(SRC)							#��.o�ļ����ɿ�ִ���ļ�
	$(CC) $^ -o $@ $(CFLAGS)

	
clean:									#ɾ����������в������ļ���
	$(RM) $(OBJ)
	$(RM) $(TRG)				
	
	
	