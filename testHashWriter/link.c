#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CHUNK_SIZE 1024
int main(int argc, char * argv[]){

	FILE * testDB;
	testDB = fopen("prop","w");
	if(testDB == NULL){
		printf("Error opening prop\n");
		exit(0);
	}
	if(argc < 3){
		printf("error enter the world you want to say\n");
		exit(0);
	}
	char emptyBUFF[CHUNK_SIZE];
	memset(emptyBUFF,1,CHUNK_SIZE);
//	for(int i = 0; i < 1024;i++){
	//	fprintf(testDB,"%s",emptyBUFF);
//	}
	int newLocOffset = atoi(argv[1]);
	printf("New loc offset  is %d \n",newLocOffset);	
char info[4];
	// first two bytes are location next two are length of string
	// im guessing the first will be 2^16
	// I will only use first 4 bits
	// aka strings can only be 16 long
	fread(info,2,2,testDB);
	info[2] &=0x0F;
	unsigned int seekLocation = 0;
	seekLocation =  info[0];
	printf("Fread the first byte which is %c \n",info[0]);
	seekLocation <<= 8;
	seekLocation |= info[1];
	printf("The seek location is %u \n",seekLocation);
	fseek(testDB,seekLocation,SEEK_SET);
	char word[info[2]];
	fread(word,sizeof(word),1,testDB);
	printf("The word was %s\n",word);
	// get the integer location as of now it will probably be
	// i will onnly read the first two bytes because the file isnt
	// big enough 2^32 is too big
	fseek(testDB,0,SEEK_SET);
	fprintf(testDB,"%u",newLocOffset);
	fseek(testDB,newLocOffset,SEEK_SET);	
	fprintf(testDB,"%s",argv[2]);
	
	
	return 0;
}
