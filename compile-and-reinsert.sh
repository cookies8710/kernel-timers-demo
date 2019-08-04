clear
make || exit
echo "Removing module"
sudo rmmod timers-demo
echo "Inserting module"
sudo insmod timers-demo.ko
date
echo -e "Done\n"
