CC       = gcc

OBJ_FILE = copy.o
EXE_FILE = copy

${EXE_FILE}: ${OBJ_FILE}
	${CC} -o ${EXE_FILE}  ${OBJ_FILE} 

copy.o: copy.c
	${CC} -c copy.c
					
clean:
	rm -f ${OBJ_FILE} ${EXE_FILE}
