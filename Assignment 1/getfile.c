#include <curl/curl.h>
#include <stdint.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void latlonCurl(float lat, float lon);

typedef char *string;

typedef struct latlon
{
    int no;
    float lat;
    float lon;
    string place[1000]; 
}
latlon;

int main(void)
{
    char *token;
    const char sep[2] = ",";

    FILE *input = fopen("test.csv", "r");
    if (input == NULL)
    {
        printf("FILE DOES NOT EXIST");
        return 1;
    }

    int lineCount = 0;

    char content[1000];
    fgets(content, 1000, input);
    while (fgets(content, 1000, input))
    {
        lineCount++;
    }

    fseek(input, 0, SEEK_SET);

    latlon s[lineCount];

    FILE *output = fopen("output.csv", "w");
    if (input == NULL)
    {
        printf("Error creating file.");
        return 1;
    }    

    fgets(content, 1000, input);
    while (fgets(content, 1000, input))
    {
        // Gets the number
        token = strtok(content, sep);
        int currentNum = atoi(token);
        s[currentNum - 1].no = currentNum;

        // Gets Latitude
        token = strtok(NULL, sep);
        s[currentNum - 1].lat = atof(token);

        // Gets Longtitude
        token = strtok(NULL, sep);
        s[currentNum - 1].lon = atof(token);

        // Ignore places
        // uncomment below after code works only
        //latlonCurl(s[currentNum - 1].lat, s[currentNum - 1].lon);
        token = strtok(NULL, sep);
    }

    fprintf(output, "no,lat,lon,place\n");
    for (int i = 0; i < lineCount; i++)
    {
        printf("Lat: %f Lon: %f\n", s[i].lat, s[i].lon);
        fprintf(output, "%d,%f,%f,place\n", i + 1, s[i].lat, s[i].lon);
    }

    fclose(input);
    fclose(output);

    latlonCurl(s[0].lat, s[1].lon);
    return 0;
}

void latlonCurl(float lat, float lon)
{
    float latitude = lat;
    float longtitude = lon;
    char latstr[20];
    char lonstr[20];
    char urlname[250]; 

    sprintf(latstr, "%.6f", latitude);
    sprintf(lonstr, "%.6f", longtitude);
    sprintf(urlname, "https://nominatim.openstreetmap.org/reverse?format=json&lat=%s&lon=%s", latstr, lonstr);

    printf("%s\n", urlname);
    CURL *curl;
    CURLcode result;

    curl = curl_easy_init();
    if (curl == NULL)
    {
        printf("Request Failed\n");
        return;
    }

    curl_easy_setopt(curl, CURLOPT_URL, urlname); // Link to CURL

    // curl_easy_setopt(curl, CURLOPT_CAINFO, "C:/path_to/cacert.pem");

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, fwrite);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, stdout);

    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

    curl_easy_setopt(curl, CURLOPT_USERAGENT, "curl/8.10.1");
    curl_easy_setopt(curl, CURLOPT_REFERER, "https://myappwebsite.com");

    result = curl_easy_perform(curl);
    if (result != CURLE_OK)
    {
        printf("Error\n");
        printf("CURL Error: %s\n", curl_easy_strerror(result));
        return;
    }
    
    curl_easy_cleanup(curl);
}