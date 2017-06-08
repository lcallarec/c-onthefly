#include "fcgi_stdio.h"
#include "magic.h"
#include <stdlib.h>
#include <curl/curl.h>
#include <string.h>

typedef struct ImageResource {
    char* payload;
    size_t size;
    char* url;
    char* mime_type;
} ImageResource;

ImageResource* get_image(char* url, ImageResource* image);
static size_t write_callback(char *ptr, size_t size, size_t nmemb, ImageResource *image);
void compute_mime_type(ImageResource *image);

int main()
{
    while(FCGI_Accept() >= 0)
    {
        char* url = getenv("REQUEST_URI");
        if (url[0] == '/') url++;

        ImageResource *image = (ImageResource*) malloc(sizeof(ImageResource));

        image->payload    = (char*) malloc(sizeof(char));
        image->mime_type  = (char*) malloc(sizeof(char));
        image->url        = (char*) malloc(sizeof(char));
        *image->payload = '\0';
        image->size = 0;

        image = get_image(url, image);

        printf("Content-Length: %d\r\n", (int) image->size);
        printf("X-Via-Onthefly: true\r\n");
        printf("Content-type: %s\r\nStatus: 200 OK\r\n\r\n", image->mime_type);

        fwrite(image->payload, image->size, 1, stdout);
    }

    return 0;
}

static size_t write_callback(char *ptr, size_t size, size_t nmemb, ImageResource *image)
{
    size_t buffer_size = size * nmemb;

    size_t prev_size = image->size;

    image->size += buffer_size;

    image->payload = realloc(image->payload, image->size + 1);

    memcpy(image->payload + prev_size, ptr, buffer_size + 1);

    return buffer_size;
}

ImageResource* get_image(char* url, ImageResource *image)
{
    CURLcode res;

    curl_global_init(CURL_GLOBAL_ALL);

    CURL* curl = curl_easy_init();

    image->url = curl_easy_unescape(curl, url, 0, NULL);

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, image);
    curl_easy_setopt(curl, CURLOPT_URL, image->url);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

    res = curl_easy_perform(curl);
    if(res != CURLE_OK)
    {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    }

    curl_easy_cleanup(curl);

    compute_mime_type(image);

    return image;
}

void compute_mime_type(ImageResource *image)
{
    magic_t magic_cookie;
    magic_cookie = magic_open(MAGIC_MIME);

    if (magic_cookie == NULL) {
        image->mime_type = NULL;
        magic_close(magic_cookie);
        return;
    }

    if (magic_load(magic_cookie, NULL) != 0) {
        magic_close(magic_cookie);
        image->mime_type = NULL;
        return;
    }

    image->mime_type = (char*) magic_buffer(magic_cookie, image->payload, image->size);

    magic_close(magic_cookie);
}
