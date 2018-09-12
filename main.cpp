#include "opcodes.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <cmath>

#include <boost/multiprecision/cpp_int.hpp>


#define	OCT_DIGITS	"01234567"
#define	DEC_DIGITS	"0123456789"
#define	HEX_DIGITS	"0123456789ABCDEFabcdef"


using namespace std;
using namespace boost::multiprecision;


bool is_integer(const string& s)
{
	if (s.substr(0, 2) == "0x")
		return !s.empty() && s.substr(2).find_first_not_of(HEX_DIGITS) == string::npos;
	else if (s[0] == '0')
		return !s.empty() && s.find_first_not_of(OCT_DIGITS) == string::npos;
	else
		return !s.empty() && s.find_first_not_of(DEC_DIGITS) == string::npos;

}


int main()
{
	ifstream in("in", ios::binary);
	ofstream out("out");
	vector<string> t;

	cpp_int n;
	vector<unsigned char> nv;
	string ns;
	uint32_t nbytes;

	for (string ins; in >> ins;) {
		if (is_integer(ins)) {
			n = cpp_int(ins);
			export_bits(n, back_inserter(nv), 8);
			ns = string(nv.begin(), nv.end());
			nbytes = nv.size();

			if (n == 0)
				out << opcodes["OP_0"];
			else if (n <= 16)
				out << nv[0] + 0x50;
			else if (nbytes <= 0x4b)
				out << (char*) &nbytes << ns;
			else if (nbytes <= 0xff)
				out << opcodes["OP_PUSHDATA1"] << (char*) &nbytes << ns;
			else if (nbytes <= 0xffff)
				out << opcodes["OP_PUSHDATA2"] << (char*) &nbytes << ns;
			else if (nbytes <= 0xffffffff)
				out << opcodes["OP_PUSHDATA4"] << (char*) &nbytes << ns;
		} else {
			out << opcodes[ins];
		}
	}

	return (0);
}
