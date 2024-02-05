inline int clamp(int min, int val, int max)
{
	val = (val < min) ? min : val;
	val =(val > max) ? max : val;

	return val;
}