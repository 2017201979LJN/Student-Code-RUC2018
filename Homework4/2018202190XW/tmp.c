long calc(long y1, long y2, long y3, long y4, long y5) {
	long t1 = y3 * y5;
	y3 = y4 * t1;
	y4 = y4 + t1;
	long t2 = t1 * 4893625699003107610L;
	y5 = 7304112905112566613L + t2;
	y3 = y4 + t2;
	t2 = y4 + t2;
	y5 = y4 * t2;
	long t3 = y4 + t2;
	long t4 = y5 + t3;
	y1 = t4 * -4614163731635181374L;
	long t5 = y5 * t4;
	t4 = t1 * t5;
	long t6 = t1 + t5;
	t1 = t1 + t6;
	y1 = t1 + t6;
	t1 = t1 + t6;
	long t7 = 5547782747169491983L + t6;
	t3 = t2 + t7;
	t3 = t2 + t7;
	long t8 = t2 * t7;
	t3 = t2 * t8;
	y4 = -4033694248675870685L + t8;
	t6 = -4543997605084096850L + t8;
	long rval = -733248801270961520L + t8;
	return rval;
}
