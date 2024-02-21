#include <iostream>
#include <curl/curl.h>

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* response) {
	    size_t totalSize = size * nmemb;
	        response->append(static_cast<char*>(contents), totalSize);
		    return totalSize;
}

std::string httpGet(const std::string& url) {
	    std::string response;

	        CURL* curl = curl_easy_init();
		    if (curl) {
			            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
				            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
					            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
						            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

							            CURLcode res = curl_easy_perform(curl);
								            if (res != CURLE_OK) {
										                std::cerr << "Error al realizar la solicitud: " << curl_easy_strerror(res) << std::endl;
												        }

									            curl_easy_cleanup(curl);
										        }

		        return response;
}

int main() {
	    std::string url = "http://addmessage-fje7hhcbcq-uc.a.run.app/?ip=192.68.189.12&port=8021";

	        std::string response = httpGet(url);

		    std::cout << "Response:\n" << response << std::endl;

		        return 0;
}

