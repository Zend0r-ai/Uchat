#include "server.h"

static int mx_proc_log_in(json_object *jobj);

// char *mx_strnew(const int size) {
//     char *str = NULL;

//     if (size < 0)
//         return str;
//     if (size >= 0 && (str = (char *)malloc(size + 1)) != NULL)
//         for (int i = 0; i < size + 1; i++)
//             str[i] = '\0';
//     return str;
// }

// char *mx_itoa(int number) {
// 	int count = 0;
// 	int num = number;
// 	bool flag = false;
// 	char *str = NULL;

// 	if(number == -2147483648)
// 		return "-2147483648";
// 	if(number < 0){
// 		flag = true;
// 		number = -number;
// 	}
// 	while(num != 0) {
// 		count++;
// 		num /= 10;
// 	}
// 	str = (flag) ? str = mx_strnew(count + 1) : mx_strnew(count);
// 	for(int i = 0; number != 0; i++) {
// 		str[i] = number % 10 + '0';
// 		number /= 10;
// 	}
// 	if(flag)
// 		str[count] = '-';
// 	mx_str_reverse(str);
// 	return str;
// }

int mx_socket_handler(int client_sock) {
    char buffer[1024];
    json_object *jobj = NULL;
    const char *type;
    int ret = 0;
    
  	ssize_t res = read(client_sock, buffer, 1024);
   	if (res <= 0)
        printf("%s\n", strerror(errno));
    jobj = json_tokener_parse(buffer);
    type = json_object_get_string(json_object_object_get(jobj, "type"));
    printf("log in:%s", type);
    if (strcmp(type, "log_in") == 0)
    	ret = mx_proc_log_in(jobj);
    
    
    if (res > 0)
        res = write(client_sock, &ret, sizeof(ret));


//        printf("Send %s\n", buffer);
//    json_object_put(jobj);

    return (int) res;
}




//int interact(int client_sock) {
//    int buffer = 0;
//
//    ssize_t size = read(client_sock, &buffer, sizeof(buffer));
//    if (size <= 0)
//        return (int) size;
//    printf("Received %d\n", buffer);
//    buffer++;
//    size = write(client_sock, &buffer, sizeof(buffer));
//    if (size > 0)
//        printf("Sent %d\n", buffer);
//    return (int) size;
//}



// static int mx_reg(json_object *jobj) {



// 	return 0;
// }

char *l = "admin";
char *p = "pass";

static int mx_proc_log_in(json_object *jobj) {
	const char *log = json_object_get_string(json_object_object_get(jobj, "login"));
	const char *pass = json_object_get_string(json_object_object_get(jobj, "password"));

	if (strcmp(log, l) == 0 && strcmp(pass, p) == 0)
		return 0;
	else
		return 1;
}


// static int *mx_proc_masseg(json_object *jobj) {
// 	char buf;

// 	return 0;
// }
