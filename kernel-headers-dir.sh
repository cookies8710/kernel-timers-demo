for dir in /usr/src/kernels/ /usr/src/linux-headers-; do
	path="$dir$(uname -r)"
        [ -d $path ] && echo $path && exit 0
done
exit 1
