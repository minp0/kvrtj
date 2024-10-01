#include<stdio.h>
#include<string.h>
#include<stdlib.h>

typedef struct location{
    int no;
    char lat[15];
    char lon[15];
    char place[1000];
}location;

void extract_description(const char* json_data, char* description) {
	const char* key = "\"display_name\":";
	// Find the "description" key in the JSON data
	const char* pos = strstr(json_data, key);
	if (pos) {
		// Move the pointer to the beginning of the description value
		pos += strlen(key);
		// Skip any spaces or quotes
		while (*pos == ' ' || *pos == '"') pos++;
		// Copy the description content until the closing quote
		int i= 0;
		while (*pos != '"' && *pos != '\0') {
			description[i++] = *pos++;
		}
		description[i] = '\0'; // Null-terminate the string
	}
	else {
		strcpy(description, "Description not found");
	}
}

int main() {

    FILE* csv = fopen("latlonfile.csv","r");
    FILE* csv2 = fopen("67010534.csv","w");
    char str[100];
    location ans ;

    fgets(str,100,csv); // ไม่เอาบรรทัดเเรก
    fprintf(csv2,"no,lat,lon,place\n"); // set หัวตาราง

    while(!feof(csv)){

        fscanf(csv,"%d,%15[^,],%15s",&ans.no,ans.lat,ans.lon);    
        if(feof(csv)) break; //  บรรทัดท้ายละ

        for(int i=0;i<strlen(ans.lon);i++){ // เอา comma ตัวท้ายออก
            if(ans.lon[i]==',') ans.lon[i] = '\0';
        }
        //printf("%d %s %s\n", ans.no, ans.lat, ans.lon);

        char url[256];
        sprintf(url,"curl \"https://nominatim.openstreetmap.org/reverse?format=json&lat=%s&lon=%s\" > data.txt", ans.lat, ans.lon); // คำสั่งเปิดเว็บผ่าน system + save  ข้อมูล json ลง txt

        int result = system(url);
    // Check if the curl command was successful
        if (result != 0)  printf("curl command failed\n");
        
        FILE *fp = fopen("data.txt","r");
        char data[10001];
        fread(data,1,1000,fp); // ดึงข้อมูล
        data[1000] = '\0';
        fclose(fp);

        extract_description(data, ans.place);
        printf("%s\n",ans.place);

        fprintf(csv2,"%d,%s,%s,%s\n",ans.no,ans.lat,ans.lon,ans.place);

    }    

    fclose(csv);
    fclose(csv2);
	return 0;
}