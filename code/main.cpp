#include <iostream>

const static unsigned int b = 2;  // base
const static unsigned int p = sizeof(long long) - 1; // number of digits

struct FLOAT
{
	unsigned long long f; // fraction
	int e; // exponent
	short s; // sign
};

FLOAT add(FLOAT v, FLOAT u);
void OutputDecimal(FLOAT w);

int main()
{
	FLOAT v, u;

	//Set u and v
	u.f = 0x6c00000000000000;
	u.e = 0x2;
	u.s = -1;
	v.f = 0x4000000000000000;
	v.s = 1;
	v.e = -0x1;

	FLOAT w = add(v, u);

	// Output the result in hex
	if(w.s == -1)
		std::cout << '-';
	std::cout << std::hex << w.f << " " << w.e << std::endl;

	OutputDecimal(w);

#if 0
	// Test 10 + 0.1 (in decimal this is u = 2.0, v = 0.5)
	// Output should be 10.1 or f = 0x50000000, e = 0x1 
	FloatingPoint(
		0x40000000,
		0x1,
		0x40000000,
		-0x1
	);
#endif


	return 0;
}

FLOAT add(FLOAT v, FLOAT u)
{
	FLOAT w;

	// Make u >= v
	if(u.e < v.e || (u.e == v.e && u.f < v.f))
	{
		FLOAT temp = u;
		u = v;
		v = temp;
	}

	w.e = u.e;
	w.s = u.s;

	//Perform addition 
	//(when difference in exponent does not exceed number of digits)
	if(u.e - v.e < p + 2)
	{
		if(u.s == v.s)
		{
			w.f = u.f + (v.f >> u.e - v.e);
		}
		else
		{
			w.f = u.f - (v.f >> u.e - v.e);
		}
	}
	else
	{
		w.f = u.f;
	}


#if 0
	if(w.f == 0)
	{
		return w;
	}
#endif

	// Below this line is the normalization process:

	// Shift right once if most significant digit is 1
	if((w.f & 0x80000000) == 0x80000000)
	{
		w.f = w.f >> 1;
		++w.e;
	}
	
	// Shift left until second to most significant bit is not 0
	while(w.f & 0x40000000 != 0x40000000)
	{
		w.f <<= 1;
		--w.e;
	}

	return w;
}

void OutputDecimal(FLOAT w)
{
	// Output the result in decimal
	unsigned long long cF = w.f;
	int cE = w.e;

	unsigned long long leftResult = 0; // result left to of the decimal
	unsigned long long rightResult = 0; // result right to of the decimal

	unsigned long long mask = 0x4000000000000000;
	int factor = cE;
	int rightFactor = 5;

	// Convert from binary/hex to decimal
	while(mask != 0x0)
	{
		if(cE >= 0)
		{
			if((cF & mask) == mask)
			{
				leftResult = leftResult + (1 << factor);
			}

			--factor; // 32, 16, 8, 4, etc.
			--cE;

			if(factor < 0)
				factor = 0;
		}
		else
		{
			if((cF & mask) == mask)
			{
				rightResult = rightResult + (rightFactor / (1 << factor));
			}

			rightFactor = rightFactor * 10; // 50, 500, 5000 etc.
			++factor; // 1, 2, 4, 8, etc.
		}

		mask = mask >> 1;
	}

	if(w.s == -1)
		std::cout << '-';
	std::cout << std::dec << leftResult << "." << rightResult << std::endl;
}

//TODO: ascii to float
void atof(char* input)
{
	int left_count = 0;
	int right_count = 0;
	bool counting = false;

	int i = 0;

	while(input[i] == '0')
	{
		++i;
	}

	while(input[i] != '.' && input[i] != '\0')
	{
		++i;
		++left_count;
	}

}

