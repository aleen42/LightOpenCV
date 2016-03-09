/***********************************************************************
 *                                                                   _
 *       _____  _                           ____  _                 |_|
 *      |  _  |/ \   ____  ____ __ ___     / ___\/ \   __   _  ____  _
 *      | |_| || |  / __ \/ __ \\ '_  \ _ / /    | |___\ \ | |/ __ \| |
 *      |  _  || |__. ___/. ___/| | | ||_|\ \___ |  _  | |_| |. ___/| |
 *      |_/ \_|\___/\____|\____||_| |_|    \____/|_| |_|_____|\____||_| 
 *                                                                      
 *      ================================================================
 *                 More than a coder, More than a designer              
 *      ================================================================
 *
 *
 *      - Document: common.cpp
 *      - Author: aleen42
 *      - Description: include the function of common transfering like
 *      			   converting type int to type string, etc.
 *      - Create Time: Dec 4th, 2015
 *      - Update Time: Feb 3rd, 2016 
 *
 **********************************************************************/

#include "opencv_main.h"

class Common
{
public:
	/* convert int to string */
	static string intToStr(int num) {
		/* return "0" when the number is 0 */
		if (num == 0) {
			return "0";
		}
		/* define the str which is used to return */
		string str = "";
		/* get the absolute value */
		int num_ = num > 0 ? num : -1 * num;
		/* loop to concat the str */
		while (num_) {
			str = (char)(num_ % 10 + 48) + str;
			num_ /= 10;
		}
		/* when the num is minus, then add "-" to the head */
		if (num < 0) {
			str = "-" + str;
		}
		/* return the str */
		return str;
	}

	/* convert double to string */
	static string doubleToStr(double num) {
		ostringstream os;

		if (os << num) {			
			return os.str();
		}

		return "invalid conversion"; 
	}

	/* error info print in json */
	static void errorPrint(const char* errMsg) {
		/* error parse */
		cJSON* result = cJSON_CreateObject();
		cJSON_AddStringToObject(result, "success", "false");
		cJSON_AddStringToObject(result, "errMsg", errMsg);
		cout << cJSON_Print(result) << endl;
		cJSON_Delete(result);
	}

	/* successful info print in json */
	static void successPrint(cJSON* msg) {
		/* success parse */
		cJSON* result = cJSON_CreateObject();
		cJSON_AddStringToObject(result, "success", "true");
		cJSON_AddItemToObject(result, "data", msg);
		cout << cJSON_Print(result) << endl;
		cJSON_Delete(result);
	}
};
