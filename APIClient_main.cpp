// Author      : Meng Wang (mwang@orbe.us)
// Copyright   : Orbeus 2014 All right reserved
// Description : A test of C++ rekognition SDK.

#include "Base64Codec.h"
#include "RESTapi.h"
#include "json/json.h"

#include <fstream>
#include <sstream>
#include <streambuf>

using namespace std;

int main(int argc, char **argv) {
	Json::Value response;
	const string api_addr_base = "https://rekognition.com/func/api/?";
	map<string, string> query_config;

	// for testing only, please use client specific key and secret!
	query_config["api_key"] = "";
	query_config["api_secret"] = "";
	query_config["jobs"] = "face";
	//query_config["urls"] = "http://rekognition.com/static/img/people.jpg";

	std::ifstream t("YourImagePath/SomeImage.jpg");
	std::string buff_str((std::istreambuf_iterator<char>(t)),
			std::istreambuf_iterator<char>());

	rekognition_api::Base64Codec base64_codec;
	string encoded;
	base64_codec.Encode(buff_str, &encoded);
	query_config["base64"] = encoded;

	if (!rekognition_api::APICall(api_addr_base, query_config, &response)) {
		cerr << "API call failure!" << endl;
		return -1;
	}

	// For the format of the results, please refer to our doc:
	// http://rekognition.com/docs/
	const Json::Value face_detection = response["face_detection"];
	for (unsigned int i = 0; i < face_detection.size(); ++i) {
		double x, y, w, h;
		x = face_detection[i]["boundingbox"]["tl"]["x"].asDouble();
		y = face_detection[i]["boundingbox"]["tl"]["y"].asDouble();
		w = face_detection[i]["boundingbox"]["size"]["width"].asDouble();
		h = face_detection[i]["boundingbox"]["size"]["height"].asDouble();

		cout << "face " << i << ": [" << x << " " << y << " " << w << " " << h
				<< "]" << endl;
	}
	return 0;
}

