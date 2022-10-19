#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include <inttypes.h>
void print(void *arr, int len) {
	if(arr != NULL) {
		int datalen = 0, s1 = 0, s2 = 0, ok = 0;
		int i;
		for(i = 0; i < len; i++) {
			printf("Tipul %c\n", *((unsigned char*)arr));
			// in datalen retin lungimea datelor
			datalen = *(unsigned int*)(arr + sizeof(unsigned char));
			// verific tipul pentru a stii ce tip de int folosesc
			if(*((unsigned char*)arr) == '1') {
				ok = 1;
				s1 = sizeof(int8_t);
				s2 = s1;
			}
			if(*((unsigned char*)arr) == '2') {
				ok = 2;
				s1 = sizeof(int16_t);
				s2 = sizeof(int32_t);
			}
			if(*((unsigned char*)arr) == '3') {
				ok = 3;
				s1 = sizeof(int32_t);
				s2 = s1;
			}
			arr = arr + sizeof(head);
			char *p = strtok(arr, "\0");
			printf("%s pentru %s\n", (char*)arr, (char*)arr + strlen(p) + 1 + s1 + s2);
			if(ok == 1) {
				printf("%"PRId8"\n", *(int8_t*)(arr + strlen(p) + 1));
				printf("%"PRId8"\n\n", *(int8_t*)(arr + strlen(p) + 1 + s1));
			}
			if(ok == 2) {
				printf("%"PRId16"\n",*(int16_t*)(arr + strlen(p) + 1));
				printf("%"PRId32"\n\n",*(int32_t*)(arr + strlen(p) + 1 + s1));
			}
			if(ok == 3) {
				printf("%"PRId32"\n",*(int32_t*)(arr + strlen(p) + 1));
				printf("%"PRId32"\n\n",*(int32_t*)(arr + strlen(p) + 1 + s1));
			}
			// trecem la urmatorul element
			arr = arr + datalen;
		}
	}
}

int add_last(void **arr, int *len, data_structure *data)
{	
	if(*len == 0) { // caz in care nu am niciun element in vector
		(*arr) = malloc(data->header->len + sizeof(head)); // in data->header->len am retinut lungimea lui data->data
		memcpy((*arr), &data->header->type, sizeof(unsigned char));
		memcpy((*arr) + sizeof(unsigned char), &data->header->len, sizeof(unsigned int));
		memcpy((*arr) + sizeof(head), data->data, data->header->len);
		*len += 1;
		return 0;	
	}
	else { // caz in care sunt elemente in vector
		int total = 0;
		int i;
		for(i = 0; i < *len; i++) {
			//aflu lungimea totala a vectorului
			total += sizeof(head) + *(unsigned int*)(*arr + total + sizeof(unsigned char));
		}
		(*arr) = realloc(*arr, total + sizeof(head) + data->header->len);
		memcpy((*arr) + total, &data->header->type, sizeof(unsigned char));
		memcpy((*arr) + total + sizeof(unsigned char), &data->header->len, sizeof(unsigned int));
		memcpy((*arr) + total + sizeof(head), data->data, data->header->len);
		*len += 1;
		return 0;
	}
	
	return -1;
}

int add_at(void **arr, int *len, data_structure *data, int index)
{
	if(index < 0)
		return -1;
	if(index > *len) { 
		// daca indexul e mai mare decat lungimea vectorului pun element la sfarsit
		add_last(arr, len, data);
		return 0;
	}
	int total1 = 0, total2 = 0, i, datalen = 0;
	void *aux1 = *arr;
	for(i = 0; i < index; i++) { 
		// retin lungimea pana la index
		datalen = *(unsigned int*)(aux1 + sizeof(unsigned char));
		total1 += sizeof(head) + datalen;
		aux1 = aux1 + total1;
	}
	void *aux2 = aux1;
	for(i = index; i < *len; i++) { 
		// retin lungimeade la index pana la sfarsitul vectorului 
		datalen = *(unsigned int*)(aux2 + sizeof(unsigned char));
		total2 += sizeof(head) + datalen;
		aux2 = aux2 + total2;
	} 
	*arr = realloc(*arr, total1 + total2 + data->header->len + sizeof(head));
	// retin la adresa indexului+lungimea elementului introdus ce urma dupa index
	memcpy((*arr) + total1 + data->header->len + sizeof(head), (*arr) + total1, total2); 
	memcpy((*arr) + total1, &data->header->type, sizeof(unsigned char));
	memcpy((*arr) + total1 + sizeof(unsigned char), &data->header->len, sizeof(unsigned int));
	memcpy((*arr) + total1 + sizeof(head), data->data, data->header->len);
	*len += 1;
	return 0;
}

void find(void *data_block, int len, int index) 
{
	int total1 = 0, i;
	if(index < len && index >= 0) {
		for(i = 0; i < index; i++) {
			// retin lungimea pana la index
			total1 += sizeof(head) + *(unsigned int*)(data_block + total1 + sizeof(unsigned char));
		}
		// printez doar elementul de la index
		print(data_block + total1, 1); 
	}
}

int delete_at(void **arr, int *len, int index)
{
	int total1 = 0, total2 = 0, total = 0;
	int i;
	for(i = 0; i < index; i++) {
		// retin lungimea pana la index
		total1 += sizeof(head) + *(unsigned int*)(*arr + total1 + sizeof(unsigned char));
	}
	// retin lungimea elementului eliminat
	int lenelim = *(unsigned int*)(*arr + total1 + sizeof(unsigned char)) + sizeof(head);
	total = total1 + sizeof(head) + *(unsigned int*)(*arr + total1 + sizeof(unsigned char));
	for(i = index + 1; i < *len; i++) {
		// retin lungimea de la elementul eliminat pana la sfarsitul vectorului
		total2 += sizeof(head) + *(unsigned int*)(*arr + total + total2 + sizeof(unsigned char));
	}
	total += total2; // lungimea totala
	int l = 0;
	// creez un vector nou in care introduc vectorul dupa eliminare
	void *new_array = malloc(total1 + total2);
	for(i = 0; i < index; i++) {
		memcpy(new_array + l, *arr + l, sizeof(unsigned char));
		memcpy(new_array + l + sizeof(unsigned char), *arr + l + sizeof(unsigned char), sizeof(unsigned int));
		memcpy(new_array + l + sizeof(head), *arr + l + sizeof(head), *(unsigned int*)(*arr + l + sizeof(unsigned char)));
		l += sizeof(head) + *(unsigned int*)(*arr + l + sizeof(unsigned char));
	}
	for(i = index + 1; i < *len; i++) {
		memcpy(new_array + l, *arr + l + lenelim, sizeof(unsigned char));
		memcpy(new_array + l + sizeof(unsigned char), *arr + l + lenelim + sizeof(unsigned char), sizeof(unsigned int));
		memcpy(new_array + l + sizeof(head), *arr + l + lenelim + sizeof(head), *(unsigned int*)(*arr + l + lenelim + sizeof(unsigned char)));
		l += sizeof(head) + *(unsigned int*)(*arr + l + lenelim + sizeof(unsigned char));
	}
	// eliberez memoria ocupata de vectorul initial si il introduc pe cel nou
	free(*arr);
	*arr = malloc(l);
	memcpy(*arr, new_array, l);
	free(new_array);
	*len = *len - 1;
	return 0; 
}


int main() {
	// the vector of bytes u have to work with
	// good luck :)
	void *arr = NULL;
	int len = 0;
	char command[256];
	while(strcmp(fgets(command, 256, stdin), "exit\n") != 0) {
		char *p = strtok(command, " ");
		// verific ce comanda a fost citita
		if(strcmp(p, "insert") == 0) {
			p = strtok(NULL, " ");
			data_structure *datele = malloc(sizeof(data_structure));
			datele->header = malloc(sizeof(datele->header));
			datele->header->type = *p;
			p = strtok(NULL, " ");
			if(datele->header->type == '1') {
				char* name1 = malloc((strlen(p) + 1)*sizeof(char));
				memcpy(name1, p, strlen(p) + 1);
				p = strtok(NULL, " ");
				int8_t sum1 = atoi(p);
				p = strtok(NULL, " ");
				int8_t sum2 = atoi(p);
				p = strtok(NULL, " \n");
				char* name2 = malloc((strlen(p) + 1)*sizeof(char));
				memcpy(name2, p, strlen(p) + 1);
				datele->header->len = sizeof(char)*(1 + strlen(name1)) + 2*sizeof(int8_t) + sizeof(char)*(1 + strlen(name2));
				// aloc data cu lungimea totala formata din name1, sum1, sum2, name2
				datele->data = malloc(datele->header->len);
				memcpy(((char*)datele->data), name1, 1 + strlen(name1));
				int ofs = (1 + strlen(name1))*sizeof(char);
				memcpy((datele->data + ofs), &sum1, sizeof(int8_t));
				ofs += sizeof(int8_t);
				memcpy(datele->data + ofs, &sum2, sizeof(int8_t));
				ofs += sizeof(int8_t);
				memcpy(datele->data + ofs, name2, 1 + strlen(name2));
				free(name1);
				free(name2);
			}
			if(datele->header->type == '2') {
				char* name1 = malloc((strlen(p) + 1)*sizeof(char));
				memcpy(name1, p, strlen(p) + 1);
				p = strtok(NULL, " ");
				int16_t sum1 = atoi(p);
				p = strtok(NULL, " ");
				int32_t sum2 = atoi(p);
				p = strtok(NULL, " \n");
				char* name2 = malloc((strlen(p) + 1)*sizeof(char));
				memcpy(name2, p, strlen(p) + 1);
				datele->header->len = sizeof(char)*(1 + strlen(name1)) + sizeof(int16_t) + sizeof(int32_t) + sizeof(char)*(1 + strlen(name2));
				// aloc data cu lungimea totala formata din name1, sum1, sum2, name2
				datele->data = malloc(datele->header->len);
				memcpy(((char*)datele->data), name1, 1 + strlen(name1));
				int ofs = (1 + strlen(name1))*sizeof(char);
				memcpy((datele->data + ofs), &sum1, sizeof(int16_t));
				ofs += sizeof(int16_t);
				memcpy(datele->data + ofs, &sum2, sizeof(int32_t));
				ofs += sizeof(int32_t);
				memcpy(datele->data + ofs, name2, 1 + strlen(name2));
				ofs += 1 + strlen(name2);
				free(name1);
				free(name2);
			}
			if(datele->header->type == '3') {
				char *name1 = malloc(sizeof(char)*(strlen(p) + 1));
				memcpy(name1, p, strlen(p) + 1);
				p = strtok(NULL, " ");
				int32_t sum1 = atoi(p);
				p = strtok(NULL, " ");
				int32_t sum2 = atoi(p);
				p = strtok(NULL, " \n");
				char *name2 = malloc((strlen(p) + 1)*sizeof(char));
				memcpy(name2, p, strlen(p) + 1);
				datele->header->len = sizeof(char)*(2 + strlen(name1) + strlen(name2)) + 2*sizeof(int32_t);
				// aloc data cu lungimea totala formata din name1, sum1, sum2, name2
				datele->data = malloc(datele->header->len);
				memcpy(datele->data, name1, 1 + strlen(name1));
				int ofs = (1 + strlen(name1))*sizeof(char);
				memcpy(datele->data + ofs, &sum1, sizeof(int32_t));
				ofs += sizeof(int32_t);
				memcpy(datele->data + ofs, &sum2, sizeof(int32_t));
				ofs += sizeof(int32_t);
				memcpy(datele->data + ofs, name2, 1 + strlen(name2));
				free(name1);
				free(name2);
			}
			add_last(&arr, &len, datele);
			free(datele->header);
			free(datele->data);
			free(datele);
		}
		if(strcmp(p, "insert_at") == 0) {
			p = strtok(NULL, " ");
			int index = atoi(p);
			p = strtok(NULL, " ");
			data_structure *datele = malloc(sizeof(data_structure));
			datele->header = malloc(sizeof(datele->header));
			datele->header->type = *p;
			p = strtok(NULL, " ");
			if(datele->header->type == '1') {
				char* name1 = malloc((strlen(p) + 1)*sizeof(char));
				memcpy(name1, p, strlen(p) + 1);
				p = strtok(NULL, " ");
				int8_t sum1 = atoi(p);
				p = strtok(NULL, " ");
				int8_t sum2 = atoi(p);
				p = strtok(NULL, " \n");
				char* name2 = malloc((strlen(p) + 1)*sizeof(char));
				memcpy(name2, p, strlen(p) + 1);
				datele->header->len = sizeof(char)*(1 + strlen(name1)) + 2*sizeof(int8_t) + sizeof(char)*(1 + strlen(name2));
				// aloc data cu lungimea totala formata din 
				datele->data = malloc(datele->header->len);
				memcpy(((char*)datele->data), name1, 1 + strlen(name1));
				int ofs = (1 + strlen(name1))*sizeof(char);
				memcpy((datele->data + ofs), &sum1, sizeof(int8_t));
				ofs += sizeof(int8_t);
				memcpy(datele->data + ofs, &sum2, sizeof(int8_t));
				ofs += sizeof(int8_t);
				memcpy(datele->data + ofs, name2, 1 + strlen(name2));
				free(name1);
				free(name2);
			}
			if(datele->header->type == '2') {
				char* name1 = malloc((strlen(p) + 1)*sizeof(char));
				memcpy(name1, p, strlen(p) + 1);
				p = strtok(NULL, " ");
				int16_t sum1 = atoi(p);
				p = strtok(NULL, " ");
				int32_t sum2 = atoi(p);
				p = strtok(NULL, " \n");
				char* name2 = malloc((strlen(p) + 1)*sizeof(char));
				memcpy(name2, p, strlen(p) + 1);
				datele->header->len = sizeof(char)*(1 + strlen(name1)) + sizeof(int16_t) + sizeof(int32_t) + sizeof(char)*(1 + strlen(name2));
				// aloc data cu lungimea totala formata din name1, sum1, sum2, name2
				datele->data = malloc(datele->header->len);
				memcpy(((char*)datele->data), name1, 1 + strlen(name1));
				int ofs = (1 + strlen(name1))*sizeof(char);
				memcpy((datele->data + ofs), &sum1, sizeof(int16_t));
				ofs += sizeof(int16_t);
				memcpy(datele->data + ofs, &sum2, sizeof(int32_t));
				ofs += sizeof(int32_t);
				memcpy(datele->data + ofs, name2, 1 + strlen(name2));
				ofs += 1 + strlen(name2);
				free(name1);
				free(name2);
			}
			if(datele->header->type == '3') {
				char *name1 = malloc(sizeof(char)*(strlen(p) + 1));
				memcpy(name1, p, strlen(p) + 1);
				p = strtok(NULL, " ");
				int32_t sum1 = atoi(p);
				p = strtok(NULL, " ");
				int32_t sum2 = atoi(p);
				p = strtok(NULL, " \n");
				char *name2 = malloc((strlen(p) + 1)*sizeof(char));
				memcpy(name2, p, strlen(p) + 1);
				datele->header->len = sizeof(char)*(2 + strlen(name1) + strlen(name2)) + 2*sizeof(int32_t);
				// aloc data cu lungimea totala formata din name1, sum1, sum2, name2
				datele->data = malloc(datele->header->len);
				memcpy(datele->data, name1, 1 + strlen(name1));
				int ofs = (1 + strlen(name1))*sizeof(char);
				memcpy(datele->data + ofs, &sum1, sizeof(int32_t));
				ofs += sizeof(int32_t);
				memcpy(datele->data + ofs, &sum2, sizeof(int32_t));
				ofs += sizeof(int32_t);
				memcpy(datele->data + ofs, name2, 1 + strlen(name2));
				free(name1);
				free(name2);
			}
			add_at(&arr, &len, datele, index);
			free(datele->header);
			free(datele->data);
			free(datele);
		}
		if(strcmp(p, "print\n") == 0) {
			print(arr, len);
		}
		if(strcmp(p, "delete_at") == 0) {
			p = strtok(NULL, " \n");
			int index = atoi(p);
			delete_at(&arr, &len, index);
		}
		if(strcmp(p, "find") == 0) {
			p = strtok(NULL, " \n");
			int index = atoi(p);
			find(arr, len, index);
		} 
	}
	if(arr != NULL)
		free(arr);
	return 0;
}
