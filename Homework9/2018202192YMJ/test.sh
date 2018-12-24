make prog prog_intc prog_intl prog_intr check

echo "prog:"
./prog <data/data0.in >data.out
if diff data.out ./data/data0.out; then
	echo "Accepted"
else
	echo "Wrong Answer"
fi

echo "intc:"
./prog_intc <data/data0.in >data_intc.out
if diff data.out ./data/data0.out; then
	echo "Accepted"
else
	echo "Wrong Answer"
fi
./check <data_intc.out

echo "intl:"
./prog_intl <data/data0.in >data_intl.out
if diff data.out ./data/data0.out; then
	echo "Accepted"
else
	echo "Wrong Answer"
fi
./check <data_intl.out

echo "intr:"
LD_PRELOAD="./my_stdlib_intr.so" ./prog_intr <data/data0.in >data_intr.out
