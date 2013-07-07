CC=gcc
OBJS=mail_server.o mail_client.o
EXECUTABLE=mail_server mail_client
RM=rm
all: $(EXECUTABLE)
mail_server: mail_server.o  
  @echo 'Building target $@'
	@echo 'Invoking: GCC C Linker'
	$(CC)  mail_server.o  -o $@
	@echo 'Finished building target: $@'
	@echo ' '
mail_server.o: mail_server.c
	@echo 'Building target $@'
	@echo 'Invoking: GCC C Linker'
	$(CC)  -c mail_server.c -o $@
	@echo 'Finished building target: $@'
	@echo ' '
mail_client: mail_client.o  
	@echo 'Building target $@'
	@echo 'Invoking: GCC C Linker'
	$(CC)  mail_client.o  -o $@
	@echo 'Finished building target: $@'
	@echo ' '
mail_client.o: mail_client.c
	@echo 'Building target $@'
	@echo 'Invoking: GCC C Linker'
	$(CC)  -c mail_client.c -o $@
	@echo 'Finished building target: $@'
	@echo ' '

clean:
	$(RM) $(OBJS) $(EXECUTABLE)
