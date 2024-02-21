#include <iostream>
#include <string>
#include <curl/curl.h>

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
	    size_t total_size = size * nmemb;
	        output->append(static_cast<char*>(contents), total_size);
		    return total_size;
}

int main() {
	    CURL* curl;
	        CURLcode res;
		    std::string ip;

		        curl_global_init(CURL_GLOBAL_DEFAULT);
			    curl = curl_easy_init();
			        
			        if (curl) {
					        curl_easy_setopt(curl, CURLOPT_URL, "https://ipinfo.io/ip");
						        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
							        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ip);

								        res = curl_easy_perform(curl);
									        
									        if (res == CURLE_OK) {
											            std::cout << "Tu dirección IP es: " << ip << std::endl;
												            } else {
														                std::cerr << "Error al obtener la dirección IP: " << curl_easy_strerror(res) << std::endl;
																        }

										        curl_easy_cleanup(curl);
											    }

				    curl_global_cleanup();

				        return 0;
}

