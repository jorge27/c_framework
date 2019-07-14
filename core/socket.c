int push_route(struct route_struct *route){

	struct route_struct *pointer, *node;

	pointer = route;

	while(1){
		if (pointer->next == NULL){
			break;
		}
		pointer = pointer->next;
	}
	printf("\n");

	node = (struct route_struct *)malloc(sizeof(struct route_struct));

	if (node == NULL)
	{
		printf("Error al asignar memoria\n");
		return -1;
	}

	node->next = NULL;
	node->id = 1+(pointer->id);

	pointer->next = node;

	return 0;
}

int pop_route(struct route_struct *route){
	struct route_struct *pointer, *last;

	pointer = route;
	while(1){
		if (pointer->next == NULL){
			break;
		}
		last = pointer;
		pointer = pointer->next;
	}
	last->next = NULL;
	free(pointer);

	return 0;
}

void Route(struct route_struct *root, char *route,char* request_method, jag_function callback){

	struct route_struct *stack;

	stack = root;

	push_route(root);

	while(1){
		if (stack->next == NULL){
			break;
		}
		stack = stack->next;
	}

	stack->route = route;
	stack->request_method = request_method;
	stack->callback_function = callback;
}

int response(const char* dir, const char* name, const char* value, char *data){
	if (dir != ""){
		FILE *handle;
		char catch[120];
		int counter;

		handle = fopen(dir,"r");
         
		if( !handle ) return 1;
	
	
		while(1){
			memset(catch, 0, 120);
			if (!fgets(catch, 120, handle)){
				break;
			}
			counter = 0;
	
			while(catch[counter] != '\0'){
				counter++;
			}
			counter --;
	
			if(!(strlen(name) - counter)){
				if (!(strncmp(catch, name, counter))){
					strcat(data,value);
				}else{
					strcat(data,catch);
				}
			}else{
				strcat(data,catch);
			}
		}
	
		fclose(handle);
	}else{
		char *data_begin, *data_end, token[80];
		int counter, pointer = 0, token_pointer, flag, i, j, int_data = strlen(data), int_value = strlen(value), int_name = strlen(name);


		while(data[pointer] != '\0' && pointer < int_data){
			counter = pointer;

			token_pointer = 0;

			memset(token, 0, 80);
			while ((data[pointer] != 10 || data[pointer] != 13) && token_pointer < 80){
				if (name[token_pointer] == data[pointer]){
					token[token_pointer] = data[pointer];
					token_pointer++; pointer++;
					
				}else{
					pointer++;
					break;
				}
			}

			if (!strncmp(name, token, int_name) && !(int_name - token_pointer)){
				if (!(data_begin = (char *)malloc((int_data + int_value)*sizeof(char)))){
					return 1;
				}
				memset(data_begin, 0, (int_data + int_value));
			
				for (i = 0; i < pointer - token_pointer - 1; i++){
					data_begin[i] = data[i];
				}

				for(i = 0, j = pointer - token_pointer - 1; i < int_value; i++, j++){
					data_begin[j] = value[i];
				}

				for (i = pointer + int_value - token_pointer - 1, j = pointer; j < int_data; i++ , j++)	{
					data_begin[i] = data[j];
				}
				
				memset(data, 0, strlen(data));
				strcpy(data, data_begin);

				free(data_begin);
				break;
			}

		}
	}
	
	return 0;
}
