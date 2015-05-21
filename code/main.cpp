#include <iostream>

const static unsigned int b = 2;  // base
const static unsigned int p = sizeof(long long) - 1; // number of digits

struct FLOAT
{
	unsigned long long f; // fraction
	int e; // exponent
	int s; // sign
};

FLOAT mul(FLOAT v, FLOAT u);
FLOAT add(FLOAT v, FLOAT u);
void OutputDecimal(FLOAT w);

int main()
{
	FLOAT v, u;

	// Test add
	//Set u and v
	u.f = 0x6c00000000000000; // 6.75
	u.e = 0x2;
	u.s = -1;
	v.f = 0x4000000000000000; // 0.5
	v.e = -0x1;
	v.s = 1;

	FLOAT w = add(v, u);

	std::cout << "Test add" << std::endl;
	// Output the result in hex
	if(w.s == -1)
		std::cout << '-';
	std::cout << std::hex << w.f << " " << w.e << std::endl;

	// Output the result in dec
	OutputDecimal(u);
	OutputDecimal(v);
	OutputDecimal(w);

	// Test mul
	u.f = 0x6c00000000000000; // 6.75
	u.e = 0x2;
	u.s = 1;
	v.f = 0x4000000000000000; // 0.5
	v.e = 0x1;
	v.s = 1;

	w = mul(u, v);

	std::cout << std::endl << "Test mul" << std::endl;
	OutputDecimal(u);
	OutputDecimal(v);
	OutputDecimal(w);

	return 0;
}

FLOAT mul(FLOAT v, FLOAT u)
{
	FLOAT w;

	// Shift out 0's to avoid overflow or precision loss
	while((u.f & 1) == 0)
	{
		u.f = u.f >> 1;
	}

	while((v.f & 1) == 0)
	{
		v.f = v.f >> 1;
	}

	w.f = v.f * u.f;
	w.e = (v.e + u.e);
	w.s = v.s * u.s;

	// Normalize
	while((w.f & 0x4000000000000000) != 0x4000000000000000)
	{
		w.f <<= 1;
	}

	return w;
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

#if 0 //TODO(Ashkan): Not sure if this is necessary
	if(w.f == 0)
	{
		return w;
	}
#endif

	// Below this line is the normalization process:

	// Shift right once if most significant digit is 1
	if((w.f & 0x8000000000000000) == 0x8000000000000000)
	{
		w.f = w.f >> 1;
		++w.e;
	}
	
	// Shift left until second to most significant bit is not 0
	while((w.f & 0x4000000000000000) != 0x4000000000000000)
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

	if(factor < 0)
		factor = 0;

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
				rightResult = rightResult * 10;
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

//TODO: ascii to FLOAT
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

