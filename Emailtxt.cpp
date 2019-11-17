#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <curl/curl.h>

/* 
 * https://www.phidgets.com/?view=articles&article=LIBCURLHowTo
 * Requires libcurl 7.20.0 or above.
 */

#define FROM    "watchdogsnoreply@gmail.com"

int sendMessage(struct curl_slist *recipients, FILE *f) {
  CURL *curl;
  CURLcode res = CURLE_OK;
  char errbuf[CURL_ERROR_SIZE];

  curl = curl_easy_init();
  if(curl) {
    /* This is the URL for your mailserver */
    curl_easy_setopt(curl, CURLOPT_URL, "smtp://smtp.gmail.com:587");

    curl_easy_setopt(curl, CURLOPT_MAIL_FROM, FROM);

    curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

    curl_easy_setopt(curl, CURLOPT_USERNAME, "watchdogsnoreply@gmail.com");
    curl_easy_setopt(curl, CURLOPT_PASSWORD, "watchdogs123");

    curl_easy_setopt(curl, CURLOPT_READDATA, f);
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
	
    curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf);
    /* Send the message */
    res = curl_easy_perform(curl);
    /* Check for errors */
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %d - %s\n%s\n",
              res, curl_easy_strerror(res), errbuf);

    curl_easy_cleanup(curl);
  }

  return (int)res;
}

void parseRecipients(char* emailAddrFileName, 
  struct curl_slist **recipientsPtr) {
    FILE *f = fopen(emailAddrFileName, "r");
    char emailAddress[65];

	if (f == NULL) {
		fprintf(stderr, "Error opening file!\n");
		exit(1);
	}

	//load email addresses
	for (int i = 0; ; i++) {
		if (fgets(emailAddress, 65, f) == NULL)
			break;
        emailAddress[strlen(emailAddress) - 1] = 0; //null terminate string
		if (strlen(emailAddress) == 0)
			break;
		*recipientsPtr = curl_slist_append(*recipientsPtr, emailAddress);
	}

	fclose(f);
}

int sendEmail(char* emailAddrFileName, char *emailFileName) {
	struct curl_slist *recipients = NULL;
	int ret;
    FILE *f;

    parseRecipients(emailAddrFileName, &recipients);

	//read in the email contents and send the message
	f = fopen(emailFile, "rb");
	ret = sendMessage(recipients, f);
	fclose(f);

	/* Free the list of recipients */
    curl_slist_free_all(recipients);
	
	return ret;
}
         
