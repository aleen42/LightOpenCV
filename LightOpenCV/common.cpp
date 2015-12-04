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
};