make dmk encode decode
./dmk
./encode
./decode
if diff data.out data.ans; then
	printf "AC\n"
else
	printf "WA\n"
	exit 0
fi
make clean