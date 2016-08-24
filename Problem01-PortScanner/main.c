#include "stdio.h"
#include "my_string.c"

const char DOT = '.';

typedef struct data
{
	String* base_ip;
	uint ip_start;
	uint ip_end;
	uint port_start;
	uint port_end;
} Data;

void get_base_ip_and_ip_range(Data* data, char* ip){
	uint number_of_dots_in_a_base_ip = 3;
	uint dots_found = 0;
	uint index = 0;
	String* ip_start = String_new();
	String* ip_end = String_new();
	// Find the base ip, in 192.134.122.52
	// the base ip is 192.134.122.
	while(dots_found < number_of_dots_in_a_base_ip){
		if (ip[index] == DOT) dots_found++;
		String_add_element(data->base_ip, ip[index]);
		index++;
	}
	// Find the start ip range
	while(ip[index] != '-' && ip[index] != '\0'){
		String_add_element(ip_start, ip[index]);
		index++;
	}
	data->ip_start = String_to_int(ip_start);
	// Find the end ip range, if there is any
	if (ip[index] == '\0') data->ip_end = data->ip_start;
	else if(ip[index] == '-')
	{
		while(ip[index] != '\0'){
			index++;
			String_add_element(ip_end, ip[index]);
		}
		data->ip_end = String_to_int(ip_end);
	}
}

void get_port_range(Data* data, char* ports){
	uint index = 0;
	String* port_start = String_new();
	String* port_end = String_new();
	// Find start port
	while(ports[index] != '-' && ports[index] != '\0'){
		String_add_element(port_start, ports[index]);
		index++;
	}
	data->port_start = String_to_int(port_start);
	// Find end port if the is one
	if (ports[index] == '\0')
		data->port_end = data->port_start;
	else if(ports[index] == '-'){
		while(ports[index] != '\0'){
			index++;
			String_add_element(port_end, ports[index]);
		}
		data->port_end = String_to_int(port_end);
	}
}

Data* get_data(int argc, char *argv[]){
	Data* data = malloc(sizeof(Data));
	data->base_ip = String_new();
	get_base_ip_and_ip_range(data, argv[1]);
	if (argc > 2)
		get_port_range(data, argv[2]);
	else{
		data->port_start = 0;
		data->port_end = 65535;
	}
	return data;
}

void print_data(Data* data){
	printf("%s\n", data->base_ip->characters);
	printf("%d\n", data->ip_start);
	printf("%d\n", data->ip_end);
	printf("%d\n", data->port_start);
	printf("%d\n", data->port_end);
}

int main(int argc, char *argv[]) {
		Data* data = get_data(argc, argv);
        return 0;
}
